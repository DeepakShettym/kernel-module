# IOCTL Character Device Driver (Linux Kernel)

This project demonstrates a **Linux character device driver** implementing **IOCTL (Input/Output Control)** for communication between **user space** and **kernel space**.

---

## Overview

The project includes:

- A kernel module (`.ko`) implementing `unlocked_ioctl`
- A shared IOCTL header used by both kernel and user space
- A user-space application to test IOCTL read/write operations

---

## Project Structure

```text
ioctl/
├── ioctl_deriver.c      # Kernel module source
├── ioctl_deriver.h      # Shared IOCTL definitions
├── ioctl_user.c         # User-space test application
├── Makefile             # Builds kernel module + user app
└── README.md            # Project documentation
Features
Dynamic major/minor number allocation

Character device implementation using cdev

IOCTL commands using _IOW and _IOR

Shared header between kernel and user space

Compatible with Linux kernel 6.x

Safe user ↔ kernel data transfer using
copy_from_user() / copy_to_user()

IOCTL Commands
Defined in ioctl_deriver.h:


#define AVNG_MAGIC 'a'

#define AVNG_WR_VALUE _IOW(AVNG_MAGIC, 1, int32_t)
#define AVNG_RD_VALUE _IOR(AVNG_MAGIC, 2, int32_t)
Command	Direction	Description
AVNG_WR_VALUE	User → Kernel	Write an integer value
AVNG_RD_VALUE	Kernel → User	Read the stored integer value

Build Instructions
Build kernel module and user application

make
This will generate:

ioctl_deriver.ko — kernel module

ioctl_user — user-space executable

Load the Kernel Module

sudo insmod ioctl_deriver.ko
Check kernel logs:


dmesg | tail
Verify device node:


ls -l /dev/avng_device
Run the User Application


./ioctl_user
Expected Behavior
Writes a value to the kernel via IOCTL

Reads the value back from the kernel

Prints the result to the console

Unload the Kernel Module

sudo rmmod ioctl_deriver
Verify unload:


dmesg | tail
Clean Build Artifacts


make clean
Kernel Compatibility Notes
Uses unlocked_ioctl

Uses class_create("name") (Linux kernel 6.x API)

Avoids deprecated .ioctl interface

Shared header uses conditional includes to support both kernel and user space

Common Issues
Permission denied when running user application
Ensure the binary is executable:


chmod +x ioctl_user
AppArmor warnings in dmesg
Unrelated to this driver. Common with Snap applications (e.g., Firefox).

Learning Outcomes

This project demonstrates:

Linux kernel module development basics

Character device registration

IOCTL design and implementation

Safe user ↔ kernel data exchange

Author
Deepak M

