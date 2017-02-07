cmd_/home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o := arm-hisiv300-linux-gcc -Wp,-MD,/home/tan/job/hi3516d/code/speed_dome/len_drv/.ls_lens_pwm.o.d  -nostdinc -isystem /opt/hisi-linux/x86-arm/arm-hisiv300-linux/bin/../lib/gcc/arm-hisiv300-linux-uclibcgnueabi/4.8.3/include -I/home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi3516a/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -marm -fno-dwarf2-cfi-asm -fno-aggressive-loop-optimizations -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -I/home/tan/job/hi3516d/code/speed_dome/len_drv/  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ls_lens_pwm)"  -D"KBUILD_MODNAME=KBUILD_STR(lens_drv)" -c -o /home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o /home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.c

source_/home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o := /home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.c

deps_/home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o := \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/sysinfo.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/posix_types.h \
  include/asm-generic/posix_types.h \
  /opt/hisi-linux/x86-arm/arm-hisiv300-linux/lib/gcc/arm-hisiv300-linux-uclibcgnueabi/4.8.3/include/stdarg.h \
  include/linux/linkage.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/irqflags.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/hwcap.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/div64.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/compiler.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/arm/lpae.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/delay.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/delay.h \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/io.h \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/need/mach/io/h.h) \
    $(wildcard include/config/pcmcia/soc/common.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/isa.h) \
    $(wildcard include/config/pccard.h) \
    $(wildcard include/config/mmu.h) \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/memory.h \
    $(wildcard include/config/need/mach/memory/h.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/phys/offset.h) \
    $(wildcard include/config/hisi/snapshot/boot.h) \
  include/linux/const.h \
  arch/arm/include/generated/asm/sizes.h \
  include/asm-generic/sizes.h \
  arch/arm/mach-hi3516a/include/mach/memory.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
  /home/tan/job/hi3516d/code/Hi3516A_SDK_V1.0.6.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  arch/arm/mach-hi3516a/include/mach/io.h \
  /home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.h \

/home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o: $(deps_/home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o)

$(deps_/home/tan/job/hi3516d/code/speed_dome/len_drv/ls_lens_pwm.o):
