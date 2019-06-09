CROSS_COMPILE = arm-linux-gnueabihf-
#CROSS_COMPILE = 
AS			= $(CROSS_COMPILE)as
LD			= $(CROSS_COMPILE)ld
CC			= $(CROSS_COMPILE)gcc
CXX			= $(CROSS_COMPILE)g++
AR			= $(CROSS_COMPILE)ar
NM			= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CXX AR NM STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -O2 -g -Werror -Wno-psabi
CPPFLAGS := -Wall -O2 -g -Werror -Wno-psabi

# arm
CFLAGS += -I$(shell pwd)/include -I/work/build/arm-linux-gnueabihf/include
CPPFLAGS += -I$(shell pwd)/include -I/work/build/arm-linux-gnueabihf/include

# pc
#CFLAGS += -I$(shell pwd)/include
#CPPFLAGS += -I$(shell pwd)/include

# arm
LDFLAGS += -L/work/build/arm-linux-gnueabihf/lib


LDFLAGS += -lpthread -ldl -lrt -lcurl

export CFLAGS CPPFLAGS LDFLAGS

TOPDIR := $(shell pwd)

export TOPDIR

TARGET := app

STATIC_LIB := libprinter.a

#obj-y += Main.o

obj-y += log/
obj-y += lib/
obj-y += print/
obj-y += net/

all:
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(AR) -crsv $(STATIC_LIB) built-in.o

#all:
#	make -C ./ -f $(TOPDIR)/Makefile.build
#	$(CXX) -o $(TARGET) built-in.o $(LDFLAGS)
	
install:
	cp -rf *.a _install/lib/
	cp -rf include/* _install/include/

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
