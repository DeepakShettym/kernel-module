
🚀 What happens when you load a char driver?

You load the module

Kernel allocates a major number

Kernel creates /dev/mydevice

When a user reads/writes to that file → your code runs

When module unloads device file is removed

-----------------------------------------------------------------------------------------------------------------------------------------------
✔️ Step 1: Build
make

✔️ Step 2: Insert module
sudo insmod char_driver.ko


Check logs:

dmesg | tail


You will see:

mychardev: major=240 minor=0

✔️ Step 3: Create the device node
sudo mknod /dev/mychardev c <major> 0
sudo chmod 666 /dev/mychardev


Replace <major> from dmesg.

✔️ Step 4: Test
Write
echo "i need food" > /dev/mychardev

Read
cat /dev/mychardev

✔️ Step 5: Remove
sudo rmmod char_driver
