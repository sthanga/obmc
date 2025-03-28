### **OpenBMC Interview Questions**  

OpenBMC is an open-source firmware stack for managing server hardware, commonly used in data centers. If you're preparing for an OpenBMC interview, expect questions covering **Embedded Linux, Yocto, networking, security, and system management**.  
---

## **1. General OpenBMC Questions**  
🔹 **What is OpenBMC?**  
🔹 What are the key features of OpenBMC? 
	Open-Source and Modular Architecture
	Linux-Based Firmware
	D-Bus for Inter-Process Communication
	Redfish and IPMI Support
	Security Features
	Hardware Management Capabilities
	Web-Based Management Interface
	Multi-Platform & Vendor Support
	Remote and Automated Management
	Active Community and Ongoing Development
🔹 How does OpenBMC differ from traditional BMC firmware?  
	open source, Security & Transparency, Management Interface & Protocols, Hardware & Platform Support, Update & Maintenance
	Ecosystem & Industry Adoption
	
🔹 What are the main components of OpenBMC?  
	Yocto-Based Build System
		Bitbake
		Recipe (.bb files)
		Layers(meta-openbmc, meta-phosphor, etc.)
	Linux Kernel
		Device Tree Support, I2C, SPI, GPIO, PCIe Drivers
		Security Modules
	 System & Process Management
	 	Systemd
	 	uboot
	 D-Bus (Inter-Process Communication)
	 	Phosphor-Logging. -sensors, -leds
	 Web & API Management
	 	Redfish and IPMI
	 Security & Access Control
	 	Role-Based Access Control
	 	TLS, HTTPS
	 	SSH and Secure SHell, Secure Boot
	 Hardware Monitoring & Control
	 	Fan, sensor, power and Thermal managment
	 Firmware Update & Recovery
	 	Phosphor-Software Manager
	 	BMC Self-Recovery
	 	Redundant Image Support
	 9. Networking & Remote Management
 		Systemd-Networkd – Manages network settings.
		DHCP, Static IP Support – Configures network access.
		IPMI & Serial Console – Provides remote access for troubleshooting.
🔹 Can you explain the OpenBMC architecture? 
	OpenBMC consists of three main layers:
		Hardware Layer – Physical components (BMC chip, sensors, fans, power control, etc.).
		Firmware & OS Layer – Linux-based OS and essential system services.
		Application Layer – Interfaces for remote management (Redfish, IPMI, Web UI, SSH).
	Key Technologies Used:
		Yocto Project (Build System)
		D-Bus (Service Communication)
		Systemd (Service Management)
		Phosphor Project (Core OpenBMC services)
		Redfish API & IPMI (Remote management protocols) 
		
	
🔹 What hardware platforms support OpenBMC?  
	google, facebook,IBM, Intel, 
🔹 How do you build OpenBMC for a specific hardware platform?  
---

## **2. Yocto & Build System**  
🔹 What is the Yocto Project, and how is it used in OpenBMC?  
🔹 What are the key components of Yocto (BitBake, recipes, layers, etc.)?  
🔹 How do you add a new package to OpenBMC using Yocto?  
🔹 How do you customize the OpenBMC build for a specific board?  
🔹 How do you debug build failures in OpenBMC?  
🔹 What are layers in Yocto, and how are they structured?  
🔹 How do you create a new recipe in Yocto?  

---

## **3. Linux & System Programming**  
🔹 What Linux kernel version does OpenBMC use?  
🔹 How does OpenBMC interact with the Linux kernel?  
🔹 How do you debug kernel issues in OpenBMC?  
🔹 What is D-Bus, and how is it used in OpenBMC?  
🔹 What is systemd, and how does OpenBMC manage services with it?  
🔹 How do you configure network settings in OpenBMC?  
🔹 What is IPMI, and how does OpenBMC handle it?  

---

## **4. Networking & Security**  
🔹 How does OpenBMC handle remote management of servers?  
🔹 How is Redfish used in OpenBMC?  
🔹 What authentication mechanisms does OpenBMC support?  
🔹 How do you secure an OpenBMC system?  
🔹 What are some common security vulnerabilities in OpenBMC?  
🔹 How does OpenBMC handle firmware updates?  
🔹 What is TLS, and how is it used in OpenBMC?  

---

## **5. Debugging & Development**  
🔹 How do you log information in OpenBMC?  
🔹 What tools do you use for debugging OpenBMC issues?  
🔹 How do you access the OpenBMC shell?  
🔹 How do you reset an OpenBMC device?  
🔹 How do you update OpenBMC firmware on a device?  
🔹 What are phosphor-logging and phosphor-dbus?  

---

## **6. BMC-Specific Knowledge**  
🔹 What is a Baseboard Management Controller (BMC)?  
🔹 How does OpenBMC monitor hardware components like CPU temperature and fan speed?  
🔹 What is Redfish, and how is it implemented in OpenBMC?  
🔹 How do sensors work in OpenBMC?  
🔹 What is PLDM, and how does OpenBMC use it?  
🔹 How do you integrate a new hardware sensor into OpenBMC?  

---

### **🔹 Tips for Preparing:**
1. **Set up OpenBMC locally** – Try building it for a supported board.  
2. **Learn Yocto deeply** – Since OpenBMC is built using Yocto, understanding it is crucial.  
3. **Understand D-Bus** – Many OpenBMC services communicate via D-Bus.  
4. **Study Redfish & IPMI** – These protocols are key to server management.  
5. **Practice debugging** – Be familiar with journalctl, systemd logs, and Yocto debugging.  

Would you like help with any specific topic? 🚀


DBus
	Bus Types:
		system Bus
		Session Bus
		private Bus
		-- alsomost all are runs in system bus only
	
	D-Bus Objects:
		D-bus represents services as object with unique paths
		Ex :/xyz/openbmc_project/State/chassis0
		-- Each object has Method, Properties, signal
	D-Bus Services :
		A Dbus services is a running process that provide objects, Ex:
		xyz.openbmc_project.Logging (Logging Service)
		xyz.openbmc_project.State.Host (Host State Service)
		xyz.openbmc_project.Sensor.Temperature (Temperature Sensor)
	Use/test case of Dbus implementation
		To List:
			busctl list   -- > it will show all dbus Services
		
		Inspect a Specific Service :
			busctl tree xyz.openbmc_project.State.Chassis    -->This shows the object tree of the Chassis state service.
		
		Get Properties of a D-Bus Object :
			busctl introspect xyz.openbmc_project.State.Chassis /xyz/openbmc_project/State/Chassis0 
			|--->>>>shows methods, properties, and signals.
		
		Get a specific property value :
			busctl get-property xyz.openbmc_project.State.Chassis \
				/xyz/openbmc_project/State/Chassis0 \
				xyz.openbmc_project.State.Chassis \
				CurrentPowerState
		
		Call a D-Bus Method :
			busctl call xyz.openbmc_project.State.Host \
				/xyz/openbmc_project/State/Host0 \
				xyz.openbmc_project.State.Host \
				Transition s "On"
			--- >>>Turn on the system power
		
		Monitor D-Bus Signals :
			busctl monitor
	Steps to Create a D-Bus Service in OpenBMC
		Set up the environment
		Create a new service using C++
		Define a custom D-Bus interface
		Compile and install the service
		Test the D-Bus service


Smart Pointer
	Types of smart pointer week pointer/uniq ptr/shar ptr
	lambda function / diff b/w lambda and macro definition
	C++ pointer 
	RTOS 
	Multi level thread
	RMII in C++
	RTOS Queue
