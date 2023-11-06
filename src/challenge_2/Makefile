MOD = chall2
KPATH :=/home/dvorhack/Documents/ws_hackutt/linux-6.5.10/
PWD :=$(shell pwd)
obj-m = $(MOD).o

all:
	$(MAKE) -C $(KPATH) M=$(PWD) modules

clean:
	$(MAKE) -C $(KPATH) M=$(PWD) clean