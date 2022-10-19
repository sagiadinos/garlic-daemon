#ifndef DBUSSERVER_HPP
#define DBUSSERVER_HPP

#include <dbus/dbus.h>
#include <string>

class DBusServer
{
    public:
        DBusServer();
       ~DBusServer();
        std::string getUniqueName();
        bool isReceiving();
        bool isRebootSignal();
        bool iśPlayerUpdateSignal();
    private:
        DBusConnection *dbus_conn = nullptr;
        DBusError dbus_error;
        DBusMessage * dbus_msg = nullptr;
        DBusMessageIter args;

        bool isPasswordMatching();
        void cleanQuery();
};

#endif // DBUSSERVER_HPP
