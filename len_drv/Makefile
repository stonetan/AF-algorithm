SDK_PATH   := /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0
LINUX_ROOT := $(SDK_PATH)/osdrv/opensource/kernel/linux-3.4.y

ifeq ($(PARAM_FILE), )
		PARAM_FILE:=$(SDK_PATH)/mpp/Makefile.param
		include $(PARAM_FILE)
endif

obj-m := lens_drv.o
lens_drv-y += ls_lens_drv.o ls_lens_41908.o ls_lens_spi.o ls_lens_pwm.o ls_lens_timer.o

EXTRA_CFLAGS += -I$(PWD)/

default:
	$(CC) -g -Wall -o lens_test ls_lens_test.c
	@$(MAKE) -C $(LINUX_ROOT) M=$(PWD) modules
	@rm -f *.o modules.* *.symvers *.mod.c
clean:
	@make -C $(LINUX_ROOT) M=$(PWD) clean
	@rm -f lens_test *.bak

