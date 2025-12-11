Step 1: Build the module
make


You should see:

myproc.ko

✅ Step 2: Load the module
sudo insmod myproc.ko

check for entry : /proc/myproc

Check dmesg:

dmesg | tail

✅ Step 3: Test it
Read:
cat /proc/myproc

Write:
echo "Deepak rocks" | sudo tee /proc/myproc


Check logs:

dmesg | tail

✅ Step 4: Unload
sudo rmmod myproc

---------------------------------------------------------------------------------------------------------------------------------------------------------Kernel Module: Custom /proc Directory With Multiple Entries

This module creates a set of custom entries inside /proc to demonstrate different ways of handling proc files, directory creation, read/write operations, and cleanup procedures in the Linux kernel.

Features

Creates a top-level directory:

/proc/myDriver


Creates a subdirectory inside it:

/proc/myDriver/DriverInfo


Adds three read-only files inside the subdirectory:

/proc/myDriver/DriverInfo/version

/proc/myDriver/DriverInfo/status

/proc/myDriver/DriverInfo/config

Each of these files returns a simple hardcoded string using copy_to_user().

Also creates an independent read/write entry:

/proc/myproc


Supports both reading (simple_read_from_buffer)

And writing (copy_from_user)

File Operations

/proc/myproc uses custom read + write operations.

version, status, config use a read-only handler, returning:

get a life bro!
