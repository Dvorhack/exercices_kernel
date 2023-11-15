#!/usr/bin/env bash

mkdir initramfs
gunzip initramfs.cpio.gz
(cd initramfs && cpio -idv < ../initramfs.cpio)
