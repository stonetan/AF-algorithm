#
# isp firmware Makefile
#

SDK_PATH   := /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.5.0
#SDK_PATH   := /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0

ifeq ($(PARAM_FILE), )
	 PARAM_FILE:=$(SDK_PATH)/mpp/Makefile.param
	include $(PARAM_FILE)
endif

ISP_PATH := $(SDK_PATH)/mpp/component/isp
LIBPATH = ./lib
OBJPATH = ./obj

3A_INC  := $(ISP_PATH)/3a/include
VREG_INC := $(ISP_PATH)/firmware/vreg

#tmp, delete in the future
SRC_INC := $(ISP_PATH)/firmware/src/main
ISP_INC := $(ISP_PATH)/include

ifeq ($(MPP_BUILD), y)
EXT_PATH := $(SDK_PATH)/mpp/$(EXTDRV)
else
EXT_PATH := $(SDK_PATH)/mpp/extdrv
endif

BUS_DIR := $(EXT_PATH)/pwm

ARFLAGS = rcv
CFLAGS  = -Wall -fPIC

ifeq ($(HIGDB),HI_GDB)
CFLAGS += -g
endif
ifeq ($(CONFIG_JPEGEDCF), y)
     CFLAGS += -D ENABLE_JPEGEDCF 
endif
CFLAGS  += -O2
CFLAGS  += $(LIBS_CFLAGS)
DFLAGS  := -DEXT_REG

INC := -I$(REL_INC) -I$(ISP_INC) -I$(BUS_DIR) -I$(3A_INC) -I$(SRC_INC) -I$(VREG_INC) -I$(VREG_INC)/arch/$(HIARCH)

COMPILE = $(CC) $(CFLAGS) $(DFLAGS) -lm

$(OBJPATH)/%.o: ./%.c
	@(echo "compiling $< ...")
	@[ -e $(LIBPATH) ] || mkdir $(LIBPATH)
	@[ -e $(OBJPATH) ] || mkdir $(OBJPATH)
	@($(COMPILE) -o $@ -c $< $(INC))

SRCS = $(wildcard ./*.c)
OBJS = $(SRCS:%.c=%.o)
OBJS := $(OBJS:./%=obj/%)

LIB_A := $(LIBPATH)/lib_hiaf.a
LIB_S := $(LIBPATH)/lib_hiaf.so

all:$(OBJS)
	@($(AR) $(ARFLAGS) $(LIB_A) $(OBJS))
	@($(CC) $(LIBS_LD_CFLAGS) -shared -fPIC -o $(LIB_S) $(OBJS))
	@(cp lib/lib_hiaf.so lib/lib_hiaf.a ../../hi3516_mpp/mpp/lib/18PTZ)

clean:
	@$(RM) -rf $(LIB_A) $(LIB_S) $(OBJS)
	@$(RM) -rf $(LIBPATH) $(OBJPATH)
