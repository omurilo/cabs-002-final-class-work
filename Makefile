CXX ?= g++

EXEC = visualizador

SRCS = $(wildcard app/*.cpp)
OBJS = $(SRCS:.cpp=.o)

PKGCONFIG_BIN := $(shell command -v pkg-config 2>/dev/null)
ifeq ($(PKGCONFIG_BIN),)
$(info [INFO] pkg-config nao encontrado; usando caminhos padrao para SFML.)
SFML_CFLAGS :=
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system
else
	SFML_CFLAGS := $(shell pkg-config --cflags sfml-all 2>/dev/null)
	SFML_LIBS := $(shell pkg-config --libs sfml-all 2>/dev/null)
endif

NIX_SFML_INCLUDE := $(shell find /nix/store -maxdepth 3 -type f -name Graphics.hpp 2>/dev/null | head -1 | xargs dirname | xargs dirname)
ifneq ($(NIX_SFML_INCLUDE),)
SFML_CFLAGS += -I$(NIX_SFML_INCLUDE)
endif

NIX_SFML_LIBDIR := $(shell find /nix/store -maxdepth 3 -type f -name libsfml-graphics.* 2>/dev/null | head -1 | xargs dirname)
ifneq ($(NIX_SFML_LIBDIR),)
SFML_LIBS += -L$(NIX_SFML_LIBDIR)
endif

CXXFLAGS = $(STD_FLAG) -Wall -Wextra -g $(SFML_CFLAGS) -Ilib/include

LDLIBS = $(SFML_LIBS)

LIB_SRCS := $(wildcard lib/src/*.cpp)
LIB_OBJS := $(LIB_SRCS:.cpp=.o)
LIB_STATIC := lib/libdatastructures.a

all: $(LIB_STATIC) $(EXEC)

library: $(LIB_STATIC)

$(LIB_STATIC): $(LIB_OBJS)
	@mkdir -p lib
	ar rcs $@ $(LIB_OBJS)
	@echo "Biblioteca estatica criada: $@"

$(EXEC): $(LIB_STATIC) $(OBJS)
	$(CXX) $(OBJS) -Llib -ldatastructures -o $(EXEC) $(LDLIBS)
	@echo "Executavel '$(EXEC)' criado com sucesso!"

app/%.o: app/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
lib/src/%.o: lib/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(EXEC)

clean:
	@echo "Limpando arquivos gerados..."
	rm -f $(OBJS) $(EXEC)
	rm -f $(LIB_OBJS) $(LIB_STATIC)

distclean: clean
	@echo "Removendo artefatos adicionais (frames, vídeos, logs, temporários)"
	@rm -f *.mp4
	@rm -f commands.*
	@find frames -type f -name 'frame_*.png' -delete 2>/dev/null || true
	@rm -f .DS_Store
	@echo "distclean concluído."

build-doc:
	@npx @marp-team/marp-cli@latest APRESENTACAO.md

.PHONY: all clean run library build-doc
