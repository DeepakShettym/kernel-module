# Linux Character Device Driver (Iron Man 🦾)

This project implements a **Linux character device driver** using modern kernel mechanisms such as **cdev**, **sysfs**, and **udev**.  
The driver demonstrates end-to-end communication between **user space and kernel space**.

---

## 🔧 What This Driver Does

- Registers a **character device** in the kernel
- Automatically creates `/dev/Iron_Man` (no manual `mknod`)
- Exposes device information via **sysfs**
- Supports `open`, `read`, `write`, and `release`
- Allows user space to send and receive messages through the device file

---

## 🧠 Core Concepts Used

- Kernel module lifecycle (`module_init`, `module_exit`)
- Character devices (`cdev`)
- Major / Minor numbers
- File operations (`struct file_operations`)
- Safe user ↔ kernel memory transfer
  - `copy_from_user`
  - `simple_read_from_buffer`
- sysfs (`class_create`, `device_create`)
- udev automatic `/dev` node creation

---

## 📁 Files Created by the Driver

### `/dev`
/dev/Iron_Man


### `/sys/class`
/sys/class/Avengers/Iron_Man/
├── dev
├── power
├── subsystem
└── uevent

---

## 🔄 Verified User ↔ Kernel Communication

### Writing from user space to kernel:

echo "hey its me spider man i need help!" > /dev/Iron_Man
Reading back from kernel to user space:
bash
Copy code
cat /dev/Iron_Man
Output:
bash
Copy code
hey its me spider man i need help!
This confirms:

The write handler was invoked in kernel space

Data was safely stored in kernel memory

The read handler returned the same data to user space

🧪 How to Build and Test
make
sudo insmod avengers_driver.ko
ls /dev/Iron_Man
echo "hello kernel" > /dev/Iron_Man
cat /dev/Iron_Man
sudo rmmod avengers_driver

## 🧾 Kernel Log Verification (dmesg)

The following kernel log output confirms the complete lifecycle of the device and
successful user ↔ kernel interaction:

```text
[98665.792075] Tony Stark: major=511 minor=0
[98665.792336] Avengers assembled successfully

[99005.436092] Iron Man ready to fight
[99005.436135] Message to Tony Stark received: hey its me spider man i need help!
[99005.436144] Iron Man saved the world

[99014.068546] Iron Man ready to fight
[99014.068611] Iron Man saved the world

These logs are just debug messages — Iron Man did not actually save the world (yet).
