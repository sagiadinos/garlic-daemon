#include "dbus_server.hpp"

DBusServer::DBusServer()
{
    ::dbus_error_init(&dbus_error);
    dbus_conn = ::dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error);
    int ret = ::dbus_bus_request_name(dbus_conn, "com.sagiadinos.garlic.daemon", DBUS_NAME_FLAG_REPLACE_EXISTING, &dbus_error);
    ::dbus_bus_set_unique_name(dbus_conn, "com.sagiadinos.garlic.daemon");

    if (dbus_error_is_set(&dbus_error))
    {
          fprintf(stderr, "Name Error (%s)\n", dbus_error.message);
          dbus_error_free(&dbus_error);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
       fprintf(stderr, "Not Primary Owner (%d)\n", ret);
    }
}

DBusServer::~DBusServer()
{
    ::dbus_connection_unref(dbus_conn);
}

std::string DBusServer::getUniqueName()
{
    return ::dbus_bus_get_unique_name(dbus_conn);
}

bool DBusServer::isReceiving()
{
    dbus_connection_read_write(dbus_conn, 0);
    dbus_msg = dbus_connection_pop_message(dbus_conn);
    if (dbus_msg != NULL)
        return true;

    return false;
}

bool DBusServer::isRebootSignal()
{
    bool ret = false;
    if (dbus_message_is_signal(dbus_msg, "com.sagiadinos.garlic.daemon", "reboot"))
    {
        ret = isPasswordMatching();
        cleanQuery();
    }

    return ret;
}

bool DBusServer::iśPlayerUpdateSignal()
{
    bool ret = false;
    if (dbus_message_is_signal(dbus_msg, "com.sagiadinos.garlic.daemon", "updatePlayer"))
    {
        ret = isPasswordMatching();
        cleanQuery();
    }

    return ret;
}

void DBusServer::cleanQuery()
{
    ::dbus_message_unref(dbus_msg);
}

bool DBusServer::isPasswordMatching()
{
    const char *sigvalue;
    if (dbus_message_iter_init(dbus_msg, &args) && DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
    {
       dbus_message_iter_get_basic(&args, &sigvalue);
       fprintf(stderr, "Got reboot signal with value: %s\n", sigvalue);
       return true;
    }
    return false;
}

