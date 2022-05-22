
# 1. 使用不同的开发板内核时, 一定要修改KERN_DIR
# 2. KERN_DIR中的内核要事先配置、编译, 为了能编译内核, 要先设置下列环境变量:
# 2.1 ARCH,          比如: export ARCH=arm
# 2.2 CROSS_COMPILE, 比如: export CROSS_COMPILE=arm-linux-gnueabihf-
# 2.3 PATH,          比如: export PATH=$PATH:/home/long/imx6ull_sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin
#       请参考各开发板的高级用户使用手册

KERN_DIR = /home/long/imx6ull_sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o led_test led_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f led_test

# 参考内核源码drivers/char/ipmi/Makefile
# 要想把a.c b.c 编译成ab.ko，可以这样指定：
# ab-y := a.o b.o
# obj-m += ab.o

100ask_led-y := led_drv.o board_imx6ull.o
obj-m	+= 100ask_led.o
