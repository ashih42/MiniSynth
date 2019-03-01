CC = clang++

TARGET = MiniSynth

SDL2_LOC := $(shell brew --prefix sdl2)
SDL2_INC := $(SDL2_LOC)/include/SDL2/
SDL2_LINK := -L $(SDL2_LOC)/lib/ -lSDL2

SDL2_IMAGE_LOC := $(shell brew --prefix sdl2_image)
SDL2_IMAGE_INC := $(SDL2_IMAGE_LOC)/include/SDL2/
SDL2_IMAGE_LINK := -L $(SDL2_IMAGE_LOC)/lib/ -lSDL2_image

SDL2_TTF_LOC := $(shell brew --prefix sdl2_ttf)
SDL2_TTF_INC := $(SDL2_TTF_LOC)/include/SDL2/
SDL2_TTF_LINK := -L $(SDL2_TTF_LOC)/lib/ -lSDL2_ttf

CFLAGS := -Wall -Werror -Wextra -Wfatal-errors -std=c++11

INCLUDES := includes/
HEADERS := -I $(INCLUDES) -I $(SDL2_INC) -I $(SDL2_IMAGE_INC) -I $(SDL2_TTF_INC)

SRCSDIR := srcs/
SRCS := \
AudioModule.cpp \
DisplayModule.cpp \
EventManager.cpp \
MiniSynth.cpp \
NoteManager.cpp \
ResourceManager.cpp \
SDLException.cpp \
main.cpp

OBJDIR := objs/
OBJS := $(addprefix $(OBJDIR), $(SRCS:.cpp=.o))

all: $(TARGET)

$(OBJDIR)%.o: $(SRCSDIR)%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) $(HEADERS) $< -o $@

$(TARGET): $(OBJS)
	@echo "\x1b[1mBuilding $(TARGET)...\x1b[0m"
	$(CC) -o $(TARGET) $(OBJS) $(SDL2_LINK) $(SDL2_IMAGE_LINK) $(SDL2_TTF_LINK) -pthread
	@echo "\x1b[1mBuild finished!!\x1b[0m"

clean:
	@echo "\x1b[1mCleaning...\x1b[0m"
	/bin/rm -rf $(OBJDIR)
	@echo

fclean: clean
	@echo "\x1b[1mFcleaning...\x1b[0m"
	/bin/rm -f $(TARGET)
	@echo

re: fclean all

.PHONY: all clean fclean re
