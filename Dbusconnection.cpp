#include "sdbus.h"
#include <boost/asio/io_context.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <boost/asio/io_service.hpp>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>


//D-Bus interfaces 
std::string chassisIntrusionName = "xyz.openbmc_project.Chassis.myownbus";

//shared pointers to sdbusplus::asio::dbus_interface objects. These represent D-Bus interfaces that your application will expose on the system bus using sdbusplus
std::shared_ptr<sdbusplus::asio::dbus_interface> intrusionIFace;

// multiple chassis nodes (e.g., node 0, node 1) — especially useful in multi-chassis or multi-board systems.
std::string node = "0";


// Creates a new D-Bus connection object associated with the ASIO io_context (io).
//conn is a shared pointer to manage the lifetime of the connection.
conn = std::make_shared<sdbusplus::asio::connection>(io);

// Appends `"0"` to the base service name
// std::string chassisIntrusionName = "xyz.openbmc_project.Chassis.myownbus";
// becomes: "xyz.openbmc_project.Chassis.myownbus0"
chassisIntrusionName += node;
// Registers the D-Bus service name (like `xyz.openbmc_project.Chassis.myownbus0`) on the system bus.
// This name must be unique — only one owner allowed at a time.
//Now your process "owns" this service on the D-Bus.
conn->request_name(chassisIntrusionName.c_str());

// Hardtamper DBus Intialization
// Creates an object server, which will manage D-Bus objects.
// This server handles incoming D-Bus method/property calls for interfaces registered on conn.
sdbusplus::asio::object_server intrusionServer = sdbusplus::asio::object_server(conn);

// Adds a new D-Bus **interface** to the object server.
// **Object path**: `/xyz/openbmc_project/Chassis/myownbus` like hieraical tree 
// **Interface name**: `xyz.openbmc_project.Chassis.myownbus`
// intrusionIFace` now refers to this interface and can register properties/methods on it.
intrusionIFace = intrusionServer.add_interface(
"/xyz/openbmc_project/Chassis/myownbus",
"xyz.openbmc_project.Chassis.myownbus");

// Registers a D-Bus property called "Status" of type string with a default value.
// getinstrusionstate(0) is a function that returns one of:
// "xyz.openbmc_project.Chassis.myownbus.HardIntrusion" or
// "xyz.openbmc_project.Chassis.myownbus.SoftwareIntrusion"
// depending on the integer input (0 here).
intrusionIFace->register_property(
"Status",
std::string(getinstrusionstate(0)));

// Finalizes the setup of the D-Bus interface.
// This step is necessary to **announce** the interface and its properties on the D-Bus so that other processes can see and interact with it.
/*
###  **What You End Up With**

| Component          | Value                                                 |
|-------------------|--------------------------------------------------------|
| D-Bus Service      | `xyz.openbmc_project.Chassis.myownbus0`               |
| D-Bus Object Path  | `/xyz/openbmc_project/Chassis/myownbus`               |
| D-Bus Interface    | `xyz.openbmc_project.Chassis.myownbus`                |
| Property Registered| `Status` = `"xyz.openbmc_project.Chassis.myownbus.SoftwareIntrusion"` |

This forms a **D-Bus service** that lets external tools (like WebUI, phosphor-health-monitor, or `busctl`) monitor or change the **intrusion detection state** of the chassis.

Let me know if you want a diagram or introspection XML for this!
*/
intrusionIFace->initialize();

// start the asynchronous I/O event loop for D-Bus communication using boost::asio::io_context, in a background thread
// io.run() — What is it? io.run() starts the boost::asio::io_context event loop.
std::thread ioThread([]() {
    io.run();
});



constexpr std::string_view getinstrusionstate(int state_value)
{
    if (state_value == 1)
    {
        return "xyz.openbmc_project.Chassis.myownbus.HardIntrusion";
    }
    else
    {
        return "xyz.openbmc_project.Chassis.myownbus.SoftwareIntrusion";
    }
}


















