

CROSS = 
CC = $(CROSS)gcc
CXX = $(CROSS)g++

VERSION  	=  1.00
DEBUG = 
CFLAGS = $(DEBUG) -Wall -c -Wno-comment
LDFLAGS = -fPIC 

MV ?= mv -f
RM ?= rm -rf
LN ?= ln -sf

ifeq (V,1)
	V := @
else
	V :=
endif

TARGET = demo


TOP_PATH = $(shell pwd)
SRC_PATH += $(TOP_PATH)/
SRC_PATH += $(TOP_PATH)/src

INC_PATH += -I$(TOP_PATH)/
INC_PATH += -I$(TOP_PATH)/include 



MOD_PATH = $(TOP_PATH)/modules
MOD_LIB_PATH = $(MOD_PATH)/lib
MOD_INC_PATH = $(MOD_PATH)/include

DIRS = $(shell find $(SRC_PATH) -maxdepth 3 -type d)
FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))


##########################################################
# modules
##########################################################
modules = 
MODULES_PATH = $(foreach m, $(modules), $(MOD_PATH)/$(m))


##########################################################
# srcs
##########################################################
SRCS_CPP += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
SRCS_CC += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cc))
SRCS_C += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))


##########################################################
# objs
##########################################################
OBJS_CPP = $(patsubst %.cpp, %.o, $(SRCS_CPP))
OBJS_CC = $(patsubst %.cc, %.o, $(SRCS_CC))
OBJS_C = $(patsubst %.c, %.o, $(SRCS_C))

##########################################################
# paths
##########################################################
INC_PATH += -I$(MOD_INC_PATH)
INC_PATH += -I$(MOD_PATH)
LIB_PATH += -L$(TOP_PATH)/lib
LIB_PATH += -L$(MOD_LIB_PATH)

##########################################################
# libs
##########################################################

##########################################################
# building
##########################################################
all:$(TARGET) 

$(TARGET) : $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	$(V) for i in $(MODULES_PATH); \
	do \
	make -C $$i; \
	done

	$(V) $(CXX) $^ -o $@ $(LIB_PATH) $(LIBS) $(LDFLAGS)
	$(V) echo Create $(TARGET) ok...

$(OBJS_CPP):%.o : %.cpp
	$(V) $(CXX) $(CFLAGS) $< -o $@ $(INC_PATH)

$(OBJS_CC):%.o : %.cc
	$(V) $(CC) $(CFLAGS) $< -o $@ $(INC_PATH)

$(OBJS_C):%.o : %.c
	$(V) $(CC) $(CFLAGS) $< -o $@ $(INC_PATH)

.PHONY : clean
clean:
	$(V) $(RM) $(TARGET) $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	$(V) for i in $(MODULES_PATH); \
	do \
	make clean -C $$i; \
	done
	$(V) echo clean all...

clean_o:
	$(V) $(RM) $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	$(V) echo clean all...

rebuild :
	make clean
	make
