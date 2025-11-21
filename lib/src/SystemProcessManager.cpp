#include "SystemProcessManager.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <iostream>

namespace ds {
    bool SystemProcessManager::execute(const std::string& command, 
                                       std::function<void(const std::string&)> onOutput,
                                       std::function<bool()> shouldCancel,
                                       int* outPid) {
        int pipefd[2];
        if (pipe(pipefd) != 0) {
            std::cerr << "[SystemProcessManager] pipe() failed\n"; return false;
        }
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "[SystemProcessManager] Fork failed\n";
            close(pipefd[0]); close(pipefd[1]);
            return false;
        }
        if (pid == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            execl("/bin/sh", "sh", "-c", command.c_str(), (char *) nullptr);
            _exit(127);
        } else {
            close(pipefd[1]);
            if (outPid) *outPid = pid;
            int status;
            FILE* stream = fdopen(pipefd[0], "r");
            char buffer[256];
            while (true) {
                pid_t result = waitpid(pid, &status, WNOHANG);
                if (result == 0) {
                    if (shouldCancel && shouldCancel()) {
                        kill(pid, SIGTERM);
                        waitpid(pid, &status, 0);
                        std::cerr << "[SystemProcessManager] Process canceled\n";
                        if (stream) fclose(stream); else close(pipefd[0]);
                        return false;
                    }
                    if (stream) {
                        while (fgets(buffer, sizeof(buffer), stream)) {
                            if (onOutput) onOutput(std::string(buffer));
                        }
                        clearerr(stream);
                    }
                    sleep(1);
                } else if (result == pid) {
                    if (stream) {
                        while (fgets(buffer, sizeof(buffer), stream)) {
                            if (onOutput) onOutput(std::string(buffer));
                        }
                        fclose(stream);
                    } else {
                        close(pipefd[0]);
                    }
                    if (WIFEXITED(status)) {
                        int exitStatus = WEXITSTATUS(status);
                        return exitStatus == 0;
                    } else {
                        std::cerr << "[SystemProcessManager] Process terminated abnormally\n";
                        return false;
                    }
                } else {
                    std::cerr << "[SystemProcessManager] waitpid failed\n";
                    return false;
                }
            }
        }
    }

    bool SystemProcessManager::cancelProcess(int pid) {
        if (kill(pid, SIGTERM) == 0) {
            int status;
            waitpid(pid, &status, 0);
            return true;
        }
        return false;
    }
} // namespace ds