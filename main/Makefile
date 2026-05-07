obj-m += your_module.o

your_module-objs := \
    src/main.o \
    src/detectors/bt_kprobe.o \
    src/detectors/exam_alert.o 

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
