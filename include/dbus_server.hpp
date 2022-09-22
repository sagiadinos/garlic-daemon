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
        void addRuleForListening();
        bool isReceiving();
        void checkForSignal();
        void checkForMethod();
        void queryDBus();
        void cleanQuery();
        std::string parseMessage();
    private:
        DBusConnection *dbus_conn = nullptr;
        DBusError dbus_error;
        DBusMessage * dbus_msg = nullptr;
        DBusMessage * dbus_reply = nullptr;
        DBusMessageIter args;
        bool sigvalue;

        void replyToMethodCall();
};

#endif // DBUSSERVER_HPP
