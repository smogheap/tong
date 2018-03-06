# TONG! makefile, tweak as you see fit

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)
MIXER_FLAGS := -lSDL_mixer -lpthread
IMG_FLAGS := -lSDL_image
GAME_DATA_DIR := .
CC = g++ -O3 -Wall #-ggdb3 
SOURCES = tetris.cpp pong.cpp text.cpp option.cpp media.cpp
# these sources need to be linked to SDL libs, so may have special flag settings
SDL_SOURCES = tong.cpp
OBJS = $(SOURCES:%.cpp=%.o) $(SDL_SOURCES:%.cpp=%.o)
TARGET = tong
RM=rm -f

# gp2x/wiz build
GP2XBASE = /home/penduin/projects/wiz/GPH_SDK
GP2XBIN = $(GP2XBASE)/tools/gcc-4.0.2-glibc-2.3.6/arm-linux/bin
GP2XLIB = $(GP2XBASE)/lib/target
GP2XDGE = $(GP2XBASE)/DGE/lib/target
GP2XSDL = -lSDLmain -lSDL_image -lm -lSDL_mixer -lSDL -lts -lsmpeg -lpng -lz -s
GP2XINC = $(GP2XBASE)/DGE/include
GP2XCC = $(GP2XBIN)/arm-linux-g++
GP2XLIBS = -L$(GP2XDGE) -L$(GP2XLIB) -lDGE $(GP2XSDL)
GP2XINCS = -I$(GP2XINC) -I$(GP2XBASE)/include -I$(GP2XINC)/SDL
GP2XFLAGS = $(GP2XINCS) -DGP2X -D_REENTRANT -D_GNU_SOURCE=1 -O3
GP2XOBJS = $(SOURCES:%.cpp=%.o.gp2x) $(SDL_SOURCES:%.cpp=%.o.gp2x)
GP2XTARGET = tong.gpe

# builds all, builds the target
all:	$(OBJS)
	$(CC) $(SDL_LDFLAGS) $(MIXER_FLAGS) $(IMG_FLAGS) $(OBJS) -o $(TARGET)
%.o: %.cpp
	$(CC) $(SDL_CFLAGS) -c -o $@ $<
tong.o: $(SDL_SOURCES)
	$(CC) -DGAME_DATA_DIR=\"$(GAME_DATA_DIR)\" $(SDL_CFLAGS) -c -o $@ $<

# clean out the mess
clean:
	-$(RM) $(TARGET) $(GP2XTARGET) *.o *.o.gp2x

# gp2x/wiz build
gp2x:	$(GP2XOBJS)
	$(GP2XCC) $(GP2XLIBS) $(GP2XOBJS) $(GP2XFLAGS) -o $(GP2XTARGET)
%.o.gp2x: %.cpp
	$(GP2XCC) -DGAME_DATA_DIR=\"$(GAME_DATA_DIR)\" $(GP2XFLAGS) -c -o $@ $<
