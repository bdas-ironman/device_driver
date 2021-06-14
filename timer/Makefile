#
# Copyright: Copyright (c) 2021 Bdas
# All rights reserved.
#

ifneq ($(KERNELPATH),)
	# Makefile supports different kernel path
	KDIR	:= $(KERNELPATH)
else
	# Default kernel path
	KDIR	:= /lib/modules/$(shell uname -r)/build
endif

PWD	:= $(shell pwd)

obj-m += ktimer.o

all: build

build:
	@export KBUILD_VERBOSE=1 && \
	make -C $(KDIR)  M=$(PWD) modules

clean:
	@export KBUILD_VERBOSE=1 && \
	make -C $(KDIR)  M=$(PWD) clean

