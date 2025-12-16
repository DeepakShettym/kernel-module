# Avengers Multi-Device Character Driver :

This project implements a **Linux character device driver** that exposes
**multiple devices under a single driver and class**, each with its **own
independent data buffer**.

The driver demonstrates how Linux differentiates devices using **minor numbers**
while sharing the same **major number, file operations, and sysfs class**.

---

## 🔧 What This Driver Does

- Registers **one character driver** with the kernel
- Creates **two devices**:
  - `/dev/IronMan`  (minor 0)
  - `/dev/SpiderMan` (minor 1)
- Uses a **single class**: `Avengers`
- Maintains **separate buffers per device**
- Supports:
  - `open`
  - `read`
  - `write`
  - `release`
- Automatically creates `/dev` nodes using **sysfs + udev**
- Cleans up safely on module removal

---

## 🧠 Core Concepts Demonstrated

- Character device drivers (`cdev`)
- Major / minor number handling
- Multi-device drivers in a single module
- Per-device state using minor numbers
- User ↔ kernel memory transfer
- sysfs (`class_create`, `device_create`)
- udev automatic `/dev` node creation
- Proper init / exit cleanup

---

## 📁 Devices Created

/dev/IronMan   → major 511, minor 0
/dev/SpiderMan → major 511, minor 1


/sys/class/Avengers/
 ├── IronMan
 └── SpiderMan

🔁 Verifying Independent Buffers

Each device maintains its own buffer.
Writing to one device does not affect the other.

Writing and reading from IronMan
echo "Yo Tony, it's the God of Thunder" > /dev/IronMan
cat /dev/IronMan

Output:

Yo Tony, it's the God of Thunder 

Writing and reading from SpiderMan
echo "Hey Peter, its Aunt May! you forgot your lunch " > /dev/SpiderMan
cat /dev/SpiderMan


Output:

Hey Peter, its Aunt May! you forgot your lunch


✅ The messages remain separate, proving that:

Each device has its own buffer

Data is not overwritten across devices

🧾 Kernel Log Output (dmesg)

The following logs show how the kernel routes operations
to the correct device using minor numbers.

Iron Man ready to fight
Message to Tony Stark received
Iron Man read the message
Tony Stark saved the world

Spider Man ready to fight
Message to Spider Man received
Spider Man read the message
Spider Man saved the world

Avengers disassembled


These logs reflect:

Device open

Write from user space

Read back to user space

Proper release

Clean module unload

🧪 How to Build and Test
make
sudo insmod multi_device.ko

ls -l /dev/IronMan /dev/SpiderMan

echo "Hello Iron Man" > /dev/IronMan
cat /dev/IronMan

echo "Hello Spider Man" > /dev/SpiderMan
cat /dev/SpiderMan

sudo rmmod multi_device
