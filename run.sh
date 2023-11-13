#!/usr/bin/env bash

if [ -z "$1" ]
then
    echo "Usage: ./run.sh <linux kernel> <initramfs>"
    exit 1
fi

if [ -z "$2" ]
then
    echo "Usage: ./run.sh <linux kernel> <initramfs>"
    exit 1
fi

qemu-system-x86_64 -no-reboot \
    -m 256M\
    -kernel $1 \
    -initrd $2  \
    -cpu kvm64,+smep,+smap \
    -append "console=ttyS0 oops=panic panic=1 kpti=1 nokaslr quiet" \
    -monitor /dev/null \
    -serial mon:stdio \
    -virtfs local,path=/tmp,mount_tag=host0,security_model=passthrough,id=foobar \
    -nographic
