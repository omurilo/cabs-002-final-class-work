#include "VideoExporter.hpp"
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

namespace fs = std::filesystem;

namespace ds {

static std::string buildFFmpegCmd(const std::string& framesDir,
                                  const std::string& outputFile,
                                  const VideoConfig& cfg) {
    fs::path pattern = fs::path(framesDir) / "frame_%04d.png";
    std::ostringstream cmd;
    cmd << "ffmpeg -y -hide_banner -loglevel error -progress pipe:1";
    cmd << " -framerate " << cfg.fps;
    cmd << " -i \"" << pattern.string() << "\"";
    if (cfg.forcedSize) {
        cmd << " -s " << cfg.forcedSize->first << "x" << cfg.forcedSize->second;
    }
    if (!cfg.bitrate.empty()) {
        cmd << " -b:v " << cfg.bitrate;
    }
    cmd << " -c:v " << cfg.codec;
    cmd << " -pix_fmt " << cfg.pixFmt;
    cmd << " -preset " << cfg.preset;
    cmd << " -crf " << cfg.crf;
    cmd << " \"" << outputFile << "\"";
    return cmd.str();
}

bool VideoExporter::exportFromPNGs(const std::string& framesDir,
                                   const std::string& outputFile,
                                   const VideoConfig& cfg,
                                   EventFn onEvent,
                                   CancelFn shouldCancel,
                                   int* outPid) const {
    if (onEvent) onEvent({ExportEventType::Start,0,0,0.0,"Iniciando ffmpeg"});
    std::string cmd = buildFFmpegCmd(framesDir, outputFile, cfg);

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"Falha pipe"});
        return false;
    }
    pid_t pid = fork();
    if (pid < 0) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"Falha fork"});
        return false;
    }
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        execl("/bin/sh","sh","-c",cmd.c_str(),(char*)nullptr);
        _exit(127);
    }
    // parent
    close(pipefd[1]);
    if (outPid) *outPid = pid;
    FILE* stream = fdopen(pipefd[0], "r");
    if (!stream) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"fdopen falhou"});
        return false;
    }
    char buf[256];
    size_t frameCounter = 0;
    while (true) {
        if (shouldCancel && shouldCancel()) {
            kill(pid, SIGTERM);
            if (onEvent) onEvent({ExportEventType::Cancelled,frameCounter,0,0.0,"Cancelado"});
            break;
        }
        if (fgets(buf, sizeof(buf), stream) == nullptr) {
            if (feof(stream)) break; else continue;
        }
        std::string line(buf);
        while (!line.empty() && (line.back()=='\n' || line.back()=='\r')) line.pop_back();
        if (line.rfind("frame=",0)==0) {
            auto val = line.substr(6);
            try { frameCounter = (size_t)std::stoul(val); } catch(...) {}
            if (onEvent) onEvent({ExportEventType::Progress,frameCounter,0,0.0,line});
        } else if (line == "progress=end") {
            if (onEvent) onEvent({ExportEventType::Completed,frameCounter,0,100.0,"Finalizado"});
        } else {
            if (onEvent) onEvent({ExportEventType::Progress,frameCounter,0,0.0,line});
        }
    }
    fclose(stream);
    int status=0; waitpid(pid,&status,0);
    if (WIFEXITED(status) && WEXITSTATUS(status)==0) {
        if (onEvent) onEvent({ExportEventType::Completed,frameCounter,0,100.0,"OK"});
        return true;
    }
    if (onEvent) onEvent({ExportEventType::Error,frameCounter,0,0.0,"ffmpeg falhou"});
    return false;
}

bool VideoExporter::cancelProcess(int pid) {
    if (pid <= 0) return false;
    return kill(pid, SIGTERM) == 0;
}

} // namespace ds