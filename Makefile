DEST_DIR=~/.local/bin
EXE=todo

help:
	@echo "Help:"
	@echo
	@echo "build"
	@echo "install"

build:
	g++ src/main.cpp -o ${EXE}

install: build
	mv ${EXE} ${DEST_DIR}
