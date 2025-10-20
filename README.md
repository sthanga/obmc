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
``` bash
Here's a categorized and comprehensive overview of **sdbus C API** function calls used for **D-Bus** interaction, particularly relevant to OpenBMC and low-level system programming using **`libsystemd`'s `sd-bus` interface**:

---

## ✅ **Core sd-bus Function Categories**
### 1. **Connection Management**

These functions are for creating, connecting, and closing D-Bus sessions.

| Function               | Description                                   |
| ---------------------- | --------------------------------------------- |
| `sd_bus_open_system()` | Connect to the system bus                     |
| `sd_bus_open_user()`   | Connect to the user session bus               |
| `sd_bus_open()`        | Open the system/user bus based on context     |
| `sd_bus_default()`     | Get a shared default connection               |
| `sd_bus_unref()`       | Decrease the reference count and free the bus |
| `sd_bus_flush()`       | Flush all outgoing messages                   |
| `sd_bus_close()`       | Close connection                              |
| `sd_bus_close_unref()` | Combined close and unref                      |

### 2. **Message Creation & Sending**

| Function                           | Description                                          |
| ---------------------------------- | ---------------------------------------------------- |
| `sd_bus_message_new_method_call()` | Create a new method call message                     |
| `sd_bus_message_append()`          | Append arguments to a message                        |
| `sd_bus_message_append_basic()`    | Append a basic type (string, int, etc.)              |
| `sd_bus_call()`                    | Send a message and wait for reply (synchronous)      |
| `sd_bus_call_async()`              | Send a message and register callback for async reply |
| `sd_bus_send()`                    | Send a message without expecting a reply             |
| `sd_bus_message_new_signal()`      | Create a signal message                              |
| `sd_bus_emit_signal()`             | Emit a signal directly                               |
| `sd_bus_emit_properties_changed()` | Emit property change signal                          |

### 3. **Reading/Parsing Messages**

| Function                           | Description                                     |
| ---------------------------------- | ----------------------------------------------- |
| `sd_bus_message_read()`            | Read multiple values from a message             |
| `sd_bus_message_read_basic()`      | Read a single basic type                        |
| `sd_bus_message_enter_container()` | Begin reading a structured or array type        |
| `sd_bus_message_exit_container()`  | Exit container context                          |
| `sd_bus_message_peek_type()`       | Peek the type and signature of the next element |

### 4. **Reply/Response Handling**

| Function                            | Description                                 |
| ----------------------------------- | ------------------------------------------- |
| `sd_bus_reply_method_return()`      | Send a return message from a method handler |
| `sd_bus_reply_method_error()`       | Send an error reply                         |
| `sd_bus_reply_method_errorf()`      | Same as above but with format string        |
| `sd_bus_reply_method_return_strv()` | Return a string array                       |

### 5. **Object Registration (Server Side)**

| Function                       | Description                                        |
| ------------------------------ | -------------------------------------------------- |
| `sd_bus_add_object_vtable()`   | Register a D-Bus object and its methods/properties |
| `sd_bus_add_fallback_vtable()` | Register a fallback handler                        |
| `sd_bus_add_object()`          | Register a static object path (no vtable)          |
| `sd_bus_add_node_enumerator()` | Register a subtree node enumerator                 |
| `sd_bus_slot_set_userdata()`   | Store user data in slot handler                    |
| `sd_bus_slot_get_userdata()`   | Retrieve user data in slot handler                 |

### 6. **Object Introspection & Interfaces**

| Function                             | Description                                |
| ------------------------------------ | ------------------------------------------ |
| `sd_bus_message_new_method_return()` | Create a method return message             |
| `sd_bus_message_new_method_error()`  | Create a method error reply                |
| `sd_bus_get_unique_name()`           | Get the connection’s unique name           |
| `sd_bus_get_name_creds()`            | Get information about a peer’s credentials |
| `sd_bus_get_owner()`                 | Query bus for the owner of a bus name      |

### 7. **Service Management**

| Function                       | Description                               |
| ------------------------------ | ----------------------------------------- |
| `sd_bus_request_name()`        | Request a bus name                        |
| `sd_bus_release_name()`        | Release a previously requested name       |
| `sd_bus_list_names()`          | List all registered names                 |
| `sd_bus_get_name_machine_id()` | Get the machine ID of the owner of a name |

### 8. **Event Loop Integration**

| Function               | Description                          |
| ---------------------- | ------------------------------------ |
| `sd_bus_process()`     | Process one incoming message         |
| `sd_bus_wait()`        | Wait for input/output availability   |
| `sd_bus_get_fd()`      | Get the file descriptor for polling  |
| `sd_bus_get_events()`  | Get the event mask (POLLIN, POLLOUT) |
| `sd_bus_get_timeout()` | Get the timeout for the bus fd       |

## 🛠️ **Typical Workflow Example (Client Call)**

```c
sd_bus *bus = NULL;
sd_bus_message *msg = NULL;
sd_bus_error error = SD_BUS_ERROR_NULL;

// Connect to system bus
sd_bus_open_system(&bus);

// Create method call
sd_bus_message_new_method_call(bus, &msg,
    "xyz.openbmc_project.Logging",                  // destination
    "/xyz/openbmc_project/logging",                 // path
    "xyz.openbmc_project.Logging.Create",           // interface
    "CreateLog");                                   // method

// Append arguments
sd_bus_message_append(msg, "sss", "Error", "Critical", "Sensor Failed");

// Send message and wait for response
sd_bus_call(bus, msg, 0, &error, NULL);

// Cleanup
sd_bus_error_free(&error);
sd_bus_message_unref(msg);
sd_bus_unref(bus);
```

---

Would you like a **server-side example** (to expose a method or emit a signal), or help debugging `sd-bus` calls in OpenBMC specifically?





Write a device driver for a device (factorial engine) which computes factorial in hardware (think of it as an offload engine for cpu).
 
The driver function should have this signature: uint64_t find_factorial_driver(uint32_t num).
 
You should come up with your own register spec (aligned base addr & offsets) for such a device.
 
For example: one 32-bit reg for input num (uint32_t), two 32-bit regs for output factorial (uint64_t) etc.
 
Assume these device registers are memory-mapped and byte-addressable.
 
You are provided with read_reg(addr) and write_reg(addr, val) as 32-bit register access API's, if you need.


static int read8_reg(void *phy_id, u8 tpm_register, u8 *tpm_data, int tpm_size)
{
        struct st33zp24_i2c_phy *phy = phy_id;
        u8 status = 0;
        u8 data;

        data = TPM_DUMMY_BYTE;
        status = write8_reg(phy, tpm_register, &data, 1);
        if (status == 2) 
                status = i2c_master_recv(phy->client, tpm_data, tpm_size);
        return status;
}

static int write8_reg(void *phy_id, u8 tpm_register, u8 *tpm_data, int tpm_size)
{
        struct st33zp24_i2c_phy *phy = phy_id;

        phy->buf[0] = tpm_register;
        memcpy(phy->buf + 1, tpm_data, tpm_size);
        return i2c_master_send(phy->client, phy->buf, tpm_size + 1);
}


static int probe(uint32_t get_RegValue)
{
    uint64_t find_factorial_driver;
            
	return 0;
}

static int remove()
{

}

void *p = NULL;
p = 0x1000000000;
(*int)(*p)=100;


memory operation;
volatile uint8_t read;

uint8_t Write;
volat



union
{
	unsigned int value;
	struct
	{
		unsigned char a0;
		unsigned char a1;
		unsigned char a2;
		unsigned char a3;
	}X;
}Y;
 
Suppose Y.value = 0xaa bb cc dd;
then what will be the value of Y.X.a1  


32 bit ; 7 bit


int a ;
0000 1100 0011
0000 0100 0000
--------------
0000 1000 0011

 a= a ^ (1<<7);


```


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
uri=$(curl -k -u root:0penBmc https://192.168.1.42/redfish/v1/UpdateService | jq -r ' .HttpPushUri')
step = 2 :
curl -k -u root:0penBmc -H "Content-Type: application/octet-stream" -X POST -T /home/skthanga/PrimSecTest/test.static.mtd.all.tar https://192.168.1.42${uri}

curl -k -u root:0penBmc -H "Content-Type: application/octet-stream" "Selection-Type: Primary"-X POST -T /home/user/obmc-def.static.mtd.all.tar https://192.168.1.39${uri}
*******************************************************************************************************************************************

curl -k -u root:0penBmc -X POST -H "Content-Type: application/json" -d '{"ImageURI": "http://192.168.1.32:8080/obmc-phosphor-image-ast2600-default-20250228060956.static.mtd.all.tar"}' https://192.168.1.32/redfish/v1/UpdateService

*******************************************************************************************************************************************

}user@dell:~$ curl -k -u root:0penBmc https://192.168.1.32/redfish/v1/UpdateService/FirmwareInventory
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


