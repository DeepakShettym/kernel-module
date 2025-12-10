# Kernel Module: Parameter Example

This repository contains a simple Linux kernel module that demonstrates how to use module parameters in the Linux kernel. The module exposes an integer parameter (`value`) that can be set during module insertion or modified later through sysfs.

## Overview

The module defines one integer parameter and prints its value when the module loads and unloads.  
It helps understand:
- Declaring module parameters
- How parameters show up under `/sys/module/<module>/parameters/`
- Reading and modifying parameters from user space

## Files
param/
├── param.c
└── Makefile


## Building the Module
```bash
make
This generates:

param.ko
Loading the Module
bash

sudo insmod param.ko value=123
Check logs:

bash

dmesg | tail
Check current parameter value:

bash

cat /sys/module/param/parameters/value
Changing Parameter Value at Runtime
bash

echo 555 | sudo tee /sys/module/param/parameters/value
Verify:

bash

cat /sys/module/param/parameters/value
Unloading the Module
bash

sudo rmmod param
