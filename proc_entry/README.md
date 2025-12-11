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
