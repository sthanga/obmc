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

static boost::asio::io_context io;
std::shared_ptr<sdbusplus::asio::connection> conn;

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

*************************************************************EXAMPLE************************************************************************************************


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

static boost::asio::io_context io;
std::shared_ptr<sdbusplus::asio::connection> conn;

//D-Bus interfaces 
std::string chassisIntrusionName = "xyz.openbmc_project.Chassis.myownbus";
std::string chassisSoftTamperName = "xyz.openbmc_project.Chassis.softtamper";
std::string chassiskeylock_l = "xyz.openbmc_project.Chassis.chassiskeylock_l";
std::string chassiskeylock_r = "xyz.openbmc_project.Chassis.chassiskeylock_r";

std::shared_ptr<sdbusplus::asio::dbus_interface> intrusionIFace;
std::shared_ptr<sdbusplus::asio::dbus_interface> softamperIFace;
std::shared_ptr<sdbusplus::asio::dbus_interface> keylockIFace_l;
std::shared_ptr<sdbusplus::asio::dbus_interface> keylockIFace_r;


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


void intialiaze_instrusiondbus()
{
    std::string node = "0";
    conn = std::make_shared<sdbusplus::asio::connection>(io);
    chassisIntrusionName += node;
    conn->request_name(chassisIntrusionName.c_str());

    // Hardtamper DBus Intialization
    sdbusplus::asio::object_server intrusionServer = sdbusplus::asio::object_server(conn);
    intrusionIFace = intrusionServer.add_interface(
        "/xyz/openbmc_project/Chassis/myownbus",
        "xyz.openbmc_project.Chassis.myownbus");

    intrusionIFace->register_property(
        "Status",
        std::string(getinstrusionstate(0)));

    intrusionIFace->initialize();
    chassisSoftTamperName += node;
    conn->request_name(chassisSoftTamperName.c_str());

    // Softtamper DBus intialization
    sdbusplus::asio::object_server softamperServer = sdbusplus::asio::object_server(conn);
    softamperIFace = softamperServer.add_interface(
        "/xyz/openbmc_project/Chassis/softtamper",
        "xyz.openbmc_project.Chassis.softtamper");

    softamperIFace->register_property(
        "Status",
        std::string(getsofttamperstate(0, 0)));

    softamperIFace->initialize();
    chassiskeylock_l += node;
    conn->request_name(chassiskeylock_l.c_str());

    // Chassis keylock_r DBus Intialization
    sdbusplus::asio::object_server chassiskeylockServer_l = sdbusplus::asio::object_server(conn);
    keylockIFace_l = chassiskeylockServer_l.add_interface(
        "/xyz/openbmc_project/Chassis/chassiskeylock_l",
        "xyz.openbmc_project.Chassis.chassiskeylock_l");
    
    // getchassiskeylockstate create same type function twice
    keylockIFace_l->register_property(
        "Status",
        std::string(getchassiskeylockstate_l(0, 0)));

    keylockIFace_l->initialize();

    // Chassis key end for both
    chassiskeylock_r += node;
    conn->request_name(chassiskeylock_r.c_str());

    // Chassis keylock_r DBus Intialization
    sdbusplus::asio::object_server chassiskeylockServer_r = sdbusplus::asio::object_server(conn);
    keylockIFace_r = chassiskeylockServer_r.add_interface(
        "/xyz/openbmc_project/Chassis/chassiskeylock_r",
        "xyz.openbmc_project.Chassis.chassiskeylock_r");

    // getchassiskeylockstate create same type function twice
    keylockIFace_r->register_property(
        "Status",
        std::string(getchassiskeylockstate_r(0, 0)));

    keylockIFace_r->initialize(); // end

    /*std::thread ioThread([]()
                         { io.run(); });
    ioThread.detach();*/
     
      try {
      	  std::thread ioThread([](){ io.run(); });
    	  ioThread.detach();
      }
     catch (const std::exception& e)
     {
		std::cerr << "Exception in initialize_intrusiondbus(): " << e.what() << std::endl;
     }
}


void setinstrusion(int state_instrusion)
{
    intrusionIFace->set_property("Status", std::string(getinstrusionstate(state_instrusion)));
}


constexpr std::string_view getsofttamperstate(int state_value, int pin_number)
{
    std::string_view tamper_state[] = {"xyz.openbmc_project.Chassis.softtamper.SFTTMP_12V_OV",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_3.3V_OV",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_PWRSTBY_OV",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_5V_OV",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_CRYPT1_OT",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_CRYPT2_OT",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_CPU_UT",
                                       "xyz.openbmc_project.Chassis.softtamper.SFTTMP_CPU_OT"};
    if (state_value == 1)
    {
        return tamper_state[pin_number - 1];
    }
    else
    {
        return "xyz.openbmc_project.Chassis.softtamper.NotDetected";
    }
}

void setsofttamper(int state_softtamper, int pin_number)
{
    softamperIFace->set_property("Status", std::string(getsofttamperstate(state_softtamper, pin_number)));
}


constexpr std::string_view getchassiskeylockstate_l(int state_value, int keyLockNumber)
{
    if (state_value == 1 && keyLockNumber == 1)
    {

        return "xyz.openbmc_project.Chassis.chassiskeylock_l.KEYLOCK_L_STS";
    }
    else
    {
        return "xyz.openbmc_project.Chassis.chassiskeylock_l.NotDetected";
    }
}


constexpr std::string_view getchassiskeylockstate_r(int state_value, int keyLockNumber)
{
    if (state_value == 1 && keyLockNumber == 2)
    {
        return "xyz.openbmc_project.Chassis.chassiskeylock_r.KEYLOCK_R_STS";
    }
    else
    {
        return "xyz.openbmc_project.Chassis.chassiskeylock_r.NotDetected";
    }
}


void setchassiskeylock_l(int state_value, int keyLockNumber)
{
    keylockIFace_l->set_property("Status", std::string(getchassiskeylockstate_l(state_value, keyLockNumber)));
}


void setchassiskeylock_r(int state_value, int keyLockNumber)
{
    keylockIFace_r->set_property("Status", std::string(getchassiskeylockstate_r(state_value, keyLockNumber)));
}

*****************************************************Sdbus header file********************************************************

#ifdef __cplusplus
#include <iostream>
#include <string_view>
#include <boost/asio/io_service.hpp>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>


extern "C" {
#endif

#ifdef __cplusplus
constexpr std::string_view getinstrusionstate(int state_value);
constexpr std::string_view getsofttamperstate(int state_value,int pin_number);
constexpr std::string_view getchassiskeylockstate_l(int state_value,int keyLockNumber);
constexpr std::string_view getchassiskeylockstate_r(int state_value,int keyLockNumber);

#endif
void setinstrusion(int state_instrusion);
void intialiaze_instrusiondbus();
void setsofttamper(int state_softtamper,int pin_number);
void setchassiskeylock_l(int state_value,int keyLockNumber);
void setchassiskeylock_r(int state_value,int keyLockNumber);

#ifdef __cplusplus
}
#endif















