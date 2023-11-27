.EXPORT_ALL_VARIABLES:
# Files
S_FILES=main.c

# Output
EXEC=main.exe

# Build settings
CC=gcc
INCLUDE= -I${SDL2_INCLUDE_DIRS}
LINK= -L${SDL2_LIBRARIES}
OPTIONS= -g -Wall 
# SDL options
CC_SDL= -lmingw32 -lSDL2main -lSDL2


all:Build

Build:
	$(CC) $(OPTIONS) $(INCLUDE) $(LINK) $(S_FILES) -o $(EXEC) $(CC_SDL)

build_run:Build
	$(EXEC)

clean:
	-del ${EXEC}