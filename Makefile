MAIN_FILE 		:= src/application.cpp
DIRECTORIES		:= lib/headers bin obj assets
EXCL_LIBS		:= 

CXX_FLAGS		:= --std=c++14
DEFAULT_FLAGS	:= -lgdi32 -lwinmm
EXPORTED_FUNCS	:= _main,_SetScreen

ifdef project
	EXEC		:= bin/$(project)
else
	EXEC		:= bin/application.exe
endif

REMOVE_LIB		:= $(subst lib/$(excl),,$(ALL_LIBS))
ALL_LIBS		:= $(wildcard lib/*)
ifdef EXCL_LIBS
	INCLUDE		:= $(foreach excl,$(EXCL_LIBS),$(REMOVE_LIB))
else
	INCLUDE		:= $(ALL_LIBS)
endif
INCLUDE_FLAG	:= $(foreach l,$(INCLUDE),-I$(l))

STATIC_LIBS		:= $(foreach dir,$(INCLUDE),$(wildcard $(dir)/*.a))
SRCS			:= $(wildcard src/*.cpp)
OBJS			:= $(subst src/,obj/,$(subst .cpp,.o,$(SRCS)))

TARGET			:= WINDOWS
RESOURCES		:= FALSE
SHELL_FILE		:= ./lib/raylib/minshell.html

ifeq ($(RESOURCES),TRUE)
	PRELOAD_FILES	:= --preload-file assets
else
	PRELOAD_FILES	:=
endif

# .PHONY: build_web

run: $(EXEC)
ifeq ($(TARGET),WINDOWS)
	@echo ---------------------------------------------
	@echo Executing file: $<
	@echo
	@./$(EXEC)
	@echo ---------------------------------------------
endif

build_web: $(OBJS)
	

$(EXEC): $(OBJS)
	@echo ---------------------------------------------
	@echo Linking files...
	@echo
ifeq ($(TARGET),WINDOWS)
	g++ $(OBJS) $(INCLUDE_FLAG) $(STATIC_LIBS) $(DEFAULT_FLAGS) $(CXX_FLAGS) -o $(EXEC)
else ifeq ($(TARGET),WEB)
	C:/emsdk/upstream/emscripten/em++ -o $(subst .exe,.html,$(EXEC)) $(OBJS) -Os -Wall lib/raylib/rayweb.a -Ilib/raylib -s USE_GLFW=3 --shell-file $(SHELL_FILE) $(PRELOAD_FILES) -sEXPORTED_FUNCTIONS=$(EXPORTED_FUNCS) -D_DEFAULT_SOURCE -DPLATFORM_WEB $(CXX_FLAGS)
endif
	@echo ---------------------------------------------

$(OBJS): obj/%.o: src/%.cpp
	@echo ---------------------------------------------
	@echo Compiling source file: $<
	@echo
ifeq ($(TARGET),WINDOWS)
	g++ $(CXX_FLAGS) $(INCLUDE_FLAG) -c $< -o $@
else ifeq ($(TARGET),WEB)
	C:/emsdk/upstream/emscripten/em++ $(CXX_FLAGS) $(INCLUDE_FLAG) -D_DEFAULT_SOURCE -c $< -o $@
endif
	@echo ---------------------------------------------


UPPERCASE	:= $(shell echo '$(FILE)' | tr '[:lower:]' '[:upper:]')
newfile: src lib/headers
ifdef FILE
	@echo ---------------------------------------------
	@echo Generating file: lib/headers/$(FILE).h
	@echo '#ifndef $(UPPERCASE)_H' 	>  lib/headers/$(FILE).h
	@echo '#define $(UPPERCASE)_H' 	>> lib/headers/$(FILE).h
	@echo							>> lib/headers/$(FILE).h
	@echo							>> lib/headers/$(FILE).h
	@echo							>> lib/headers/$(FILE).h
	@echo '#endif'					>> lib/headers/$(FILE).h

	@echo Generating file: src/$(FILE).cpp
	@echo '#include "$(FILE).h"'		>> src/$(FILE).cpp
	@echo							>> src/$(FILE).cpp
	@echo ---------------------------------------------
else
	@echo FILENAME NEEDED. USAGE: FILE={filename}
	@echo NOTE: file extension is not required
endif

rmfile: src lib/headers
ifdef FILE
	@echo ---------------------------------------------
	@echo Removing file: src/$(FILE).cpp
	@$(shell rm src/$(FILE).cpp)

	@echo Removing file: lib/headers/$(FILE).h
	@$(shell rm lib/headers/$(FILE).h)
	@echo ---------------------------------------------
else
	@echo FILENAME NEEDED. USAGE: FILE={filename}
	@echo NOTE: file extension is not required
endif


setup: $(MAIN_FILE) | $(DIRECTORIES)

$(MAIN_FILE): src
	@echo ---------------------------------------------
	@echo Generating main file...
	@echo
	echo "#include <iostream>" > $(MAIN_FILE)
	echo>> $(MAIN_FILE)
	echo "int main()" >> $(MAIN_FILE)
	echo "{" >> $(MAIN_FILE)
	echo '    std::cout << "Fuck you" << std::endl;' >> $(MAIN_FILE)
	echo "    return 0;" >> $(MAIN_FILE)
	echo "}" >> $(MAIN_FILE)
	@echo ---------------------------------------------

$(DIRECTORIES):
	@echo ---------------------------------------------
	@echo Making directory: $@
	@echo
	mkdir $@
	@echo ---------------------------------------------

REMOVE := $(wildcard obj/*.o)
ifeq ($(TARGET),WINDOWS)
REMOVE += $(wildcard bin/*.exe)
else ifeq ($(TARGET),WEB)
REMOVE += $(wildcard bin/*.html) $(wildcard bin/*.js) $(wildcard bin/*.wasm) $(wildcard bin/*.data)
endif

clean:
	@echo ---------------------------------------------
	@echo Removing object and executable files...
	@echo
ifeq ($(strip $(REMOVE)),)
	@echo All object and executable files removed
else
	rm $(REMOVE)
endif
	@echo ---------------------------------------------

clean-objs:
	@echo ---------------------------------------------
	@echo Removing object files...
	@echo
ifeq ($(strip $(wildcard obj/*.o)),)
	@echo All object files removed
else
	rm $(wildcard obj/*.o)
endif
	@echo ---------------------------------------------