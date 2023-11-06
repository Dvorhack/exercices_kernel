#!/usr/bin/env bash

mkdir initramfs
(cd initramfs && cpio -idv < ../initramfs.cpio)