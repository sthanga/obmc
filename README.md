##  [Linux Commands] https://www.geeksforgeeks.org/linux-commands/
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

software
modprobe ipmi_ipmb bmcaddr=0x20 retry_time_ms=500 max_retries=3
sudo nano /etc/modprobe.d/ipmi_ipmb.conf 
sudo systemctl enable ipmi_ipmb.service
sudo systemctl start ipmi_ipmb.service


phosphor-host-state-manager
bmcweb.service
-org.openbmc.HostIpmi.service
-phosphor-ipmi-host.service
-xyz.openbmc_project.EntityManager.service
-xyz.openbmc_project.Inventory.Manager.service
-xyz.openbmc_project.Network.service
-xyz.openbmc_project.Settings.service
-xyz.openbmc_project.Software.BMC.Updater.service
-xyz.openbmc_project.ObjectMapper.service
-xyz.openbmc_project.Software.Download.service
-xyz.openbmc_project.Software.Version.service
-xyz.openbmc_project.State.BMC.service


# Override to enable ipmbbridged service
DISABLE_IPMBBRIDGED_SERVICE = "0"

# Re-add the service if it was removed
SYSTEMD_SERVICE:${PN} += " ipmb.service"

# Ensure the service is installed and enabled by default
SYSTEMD_AUTO_ENABLE:${PN} = "enable"
```

### root@ast2600-default:~# journalctl -f
``` bash

*******************************************************************************************************************************************
curl -k -u root:0penBmc https://192.168.1.58/redfish/v1/UpdateService

*******************************************************************************************************************************************
curl -k -u root:0penBmc -X POST -H "Content-Type: appication/json" -d '{"ResetType":"GracefulRestart"}' https://192.168.1.58/redfish/v1/Managers/bmc/Actions/Manager.Reset

*******************************************************************************************************************************************

curl -k -u root:0penBmc-X POST -H "Content-Type: application/json" -d '{"ResetType":"ForceRestart"}' https://192.168.1.58/redfish/v1/Managers/bmc/Actions/Manager.Reset
*******************************************************************************************************************************************

curl -k -u root:0penBmc https://192.168.1.58/redfish/v1/Managers/bmc/EthernetInterfaces/eth0
*******************************************************************************************************************************************

curl -k -u root:0penBmc https://192.168.1.58/redfish/v1/Managers/bmc/EthernetInterfaces/bond0
*******************************************************************************************************************************************

curl -k -u root:0penBmc -X PATCH -H "Content-Type: application/json" -d '{"Image": "/redfish/v1/UpdateService/FirmwareInventory/80ab32ad
"}' https://192.168.1.58/redfish/v1/Managers/bmc
*******************************************************************************************************************************************

curl -k -u root:0penBmc -X PATCH -H "Content-Type: application/json" -d '{"Image": "/redfish/v1/UpdateService/FirmwareInventory/80ab32ad"}' https://192.168.1.58/redfish/v1/Managers/bmc -v
*******************************************************************************************************************************************
Step-1:
uri=$(curl -k -u <BMC_USER:BMC_PASSWORD> https://<BMC_IP>/redfish/v1/UpdateService | jq -r ' .HttpPushUri')

Step-2:
curl -k -u <BMC_USER:BMC_PASSWORD> -H "Content-Type: application/octet-stream" -X POST -T <.mtd.all.tar file> https://<BMC_IP>${uri}


************************************************that is ***********************************************************************************

step = 1 :
uri=$(curl -k -u root:0penBmc https://192.168.1.53/redfish/v1/UpdateService | jq -r ' .HttpPushUri')
step = 2 :
curl -k -u root:0penBmc -H "Content-Type: application/octet-stream" -X POST -T /home/skthanga/PrimSecTest/test.static.mtd.all.tar https://192.168.1.53${uri}

curl -k -u root:0penBmc -H "Content-Type: application/octet-stream" "Selection-Type: Primary"-X POST -T /home/user/obmc-def.static.mtd.all.tar https://192.168.1.39${uri}
*******************************************************************************************************************************************

curl -k -u root:0penBmc -X POST -H "Content-Type: application/json" -d '{"ImageURI": "http://192.168.1.32:8080/obmc-phosphor-image-ast2600-default-20250228060956.static.mtd.all.tar"}' https://192.168.1.32/redfish/v1/UpdateService

*******************************************************************************************************************************************

}user@BLRTSL01069:~$ curl -k -u root:0penBmc https://192.168.1.32/redfish/v1/UpdateService/FirmwareInventory
{
  "@odata.id": "/redfish/v1/UpdateService/FirmwareInventory",
  "@odata.type": "#SoftwareInventoryCollection.SoftwareInventoryCollection",
  "Members": [
    {
      "@odata.id": "/redfish/v1/UpdateService/FirmwareInventory/32f4d2a6"
    }
  ],
  "Members@odata.count": 1,
  "Name": "Software Inventory Collection"

*******************************************************************************************************************************************

root@ast2600-default:~# busctl tree | grep xyz.openbmc_project.Software           
	xyz.openbmc_project.Software.BMC.Updater
	xyz.openbmc_project.Software.Download
	xyz.openbmc_project.Software.Version
	xyz.openbmc_project.Software.Sync

*******************************************************************************************************************************************

curl -k -X POST -H "Content-Type: application/json" -d '{ "ImageURI": "tftp://192.168.1.24/obmc.static.mtd.tar"}' -u root:0penBmc https://192.168.1.60/redfish/v1/UpdateService/Actions/UpdateService.SimpleUpdate


*******************************************************************************************************************************************
https://github.com/openbmc/docs/blob/master/architecture/code-update/code-update.md#steps-to-update


*******************************************************************************************************************************************
root@ast2600-default:~# journalctl -f

```
## Custom meta-folder for creating application
``` bash
meta-thanga





```

``` bash
#/usr/bin$ ls *ctl
bluetoothctl   
bootctl
coredumpctl
hostnamectl
localectl
networkctl
oomctl
powerprofilesctl
snapctl
systemctl   - used
udisksctl
boltctl
busctl   - used
grdctl
journalctl  - used
loginctl
obexctl
pactl
resolvectl
switcherooctl
timedatectl
wdctl

```


``` bash
*********** busctl concept *****************

root@ast2600-default-secure:~# busctl tree | grep softtamper
      `-/xyz/openbmc_project/Chassis/softtamper
      `-/xyz/openbmc_project/Chassis/softtamper
      `-/xyz/openbmc_project/Chassis/softtamper
Service xyz.openbmc_project.Chassis.softtamper0:
      `-/xyz/openbmc_project/Chassis/softtamper
root@ast2600-default-secure:~# busctl introspect xyz.openbmc_project.Chassis.softtamper0 /xyz/openbmc_project/Chassis/softtamper
NAME                                   TYPE      SIGNATURE RESULT/VALUE                             FLAGS
org.freedesktop.DBus.Introspectable    interface -         -                                        -
.Introspect                            method    -         s                                        -
org.freedesktop.DBus.Peer              interface -         -                                        -
.GetMachineId                          method    -         s                                        -
.Ping                                  method    -         -                                        -
org.freedesktop.DBus.Properties        interface -         -                                        -
.Get                                   method    ss        v                                        -
.GetAll                                method    s         a{sv}                                    -
.Set                                   method    ssv       -                                        -
.PropertiesChanged                     signal    sa{sv}as  -                                        -
xyz.openbmc_project.Chassis.softtamper interface -         -                                        -
.Status                                property  s         "xyz.openbmc_project.Chassis.softtamp... emits-change
root@ast2600-default-secure:~#
root@ast2600-default-secure:~# 
root@ast2600-default-secure:~# busctl get-property xyz.openbmc_project.Chassis.chassiskeylock_r0   /xyz/openbmc_project/Chassis/chassiskeylock_r   xyz.openbmc_project.Chassis.chassiskeylock_r   Status
s "xyz.openbmc_project.Chassis.chassiskeylock_r.KEYLOCK_R_STS"


```




