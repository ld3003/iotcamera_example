#
# (C) Copyleft 2011
# Late Lee from http://www.latelee.org
# 
# A simple Makefile for *ONE* project(c or/and cpp file) in *ONE*  directory
#
# note: 
# you can put head file(s) in 'include' directory, so it looks 
# a little neat.
#
# usage: $ make
#        $ make debug=y
###############################################################################

# cross compile...
CROSS_COMPILE = arm-himix100-linux-
#CROSS_COMPILE =


CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

ARFLAGS = cr
RM = -rm -rf
MAKE = make

CFLAGS = -Wall
#debug = y

CFLAGS += -O3

CFLAGS += 

DEFS = 

CFLAGS += $(DEFS)

LDFLAGS = $(LIBS)

INCDIRS = ./

CFLAGS += -I$(INCDIRS)
CFLAGS += -I./IOTCAMSDK/inc



LDFLAGS +=  -lcurl -L./IOTCAMSDK/lib -L./   -lpthread  -lopencv_core   -lopencv_imgproc -lturbojpeg -lm  -lfreetype  -liotcamera  -lpaho-mqtt3a -lpaho-mqtt3c  -lmpi -lsecurec



# source file(s), including c file(s) cpp file(s)
# you can also use $(wildcard *.c), etc.
SRC_C   := $(wildcard *.c)
SRC_C	+= $(wildcard ./JSON/*.c)
SRC_C	+= $(wildcard ./mqtt/*.c)


SRC_CPP := $(wildcard *.cpp)
SRC_CPP += $(wildcard ./PCM/*.cpp)


# object file(s)
#OBJ_C   := $(patsubst %.c,%.o,$(SRC_C))
#OBJ_CPP := $(patsubst %.cpp,%.o,$(SRC_CPP))


OBJ_C 		:= $(SRC_C:.c=.o)
OBJ_CPP 	:= $(SRC_CPP:.cpp=.o)



# executable file
target = iotcamera

###############################################################################

all: $(target)

$(target): $(OBJ_C) $(OBJ_CPP)
	@echo "Generating executable file..." $(notdir $(target))
	@$(CXX) $(CFLAGS) $^ -o $(target) $(LDFLAGS)

# make all .c or .cpp
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@echo "cleaning..."
	@$(RM) $(target)
	@$(RM) *.o *.back *~

.PHONY: all clean
