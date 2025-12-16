
# Linux Kernel Module Experiments

This repository contains a collection of **Linux kernel module examples**
built while exploring **kernel space programming**, device drivers, and
user ↔ kernel interaction.

Each directory focuses on a specific kernel concept and is kept
**self-contained** with its own source code and documentation.

---

## 📂 Repository Structure

```text
kernel-module/
├── hello/
│   └── Simple hello-world kernel module
│
├── param/
│   └── Kernel module parameters example
│
├── proc_entry/
│   └── /proc filesystem entry demonstration
│
└── sysfs_Avengers/
    ├── single_device/
    │   └── Single character device driver using sysfs
    │
    └── multidev/
        └── Multi-device character driver (IronMan & SpiderMan)

