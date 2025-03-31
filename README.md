#### bitbake-layers create-layer ../../layers/meta-custom
####  bitbake-layers add-layer ../../meta-custom

### Command for yocto based embedded linux.
#### cat /proc/config.gz | grep IPMI
``` bash
IPMITOOL:
/lib/modules/5.15.71+g95448dd0dc9b/kernel/drivers/char/ipmi/
CONFIG_IPMI_HANDLER=y
CONFIG_IPMI_PLAT_DATA=y 
CONFIG_IPMI_DEVICE_INTERFACE=y
CONFIG_IPMI_SI=y
CONFIG_IPMI_SSIF=y
CONFIG_IPMI_IPMB=y
CONFIG_IPMI_WATCHDOG=y
CONFIG_IPMI_POWEROFF=y
udhcpc -i eth4   //shared by ragav
```
#### to enable the printk in terminal
``` bash
cat /proc/sys/kernel/printk
echo 8 > /proc/sys/kernel/printk
rmmod ipmi_ipmb
depmod -a

ls -l /dev/ipmi0
udevadm trigger --subsystem-match=ipmi
udevadm control --reload
udevadm trigger
udevadm
udevadm trigger
dmesg | tail -n 50
modprobe ipmi_si type=i2c addrs=0x15

sudo mknod /dev/ipmi0 c 243 0
sudo chmod 600 /dev/ipmi0

modprobe ipmi_ipmb bmcaddr=0x20 retry_time_ms=500 max_retries=3
insmod ipmi_ipmb bmcaddr=0x20 retry_time_ms=500 max_retries=3
echo ipmi-ipmb 0x30 > /sys/class/i2c-dev/i2c-6/device/new_device
echo 'module ipmi_ipmb +p' | tee /sys/kernel/debug/dynamic_debug/control
```
