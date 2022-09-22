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

void DBusServer::addRuleForListening()
{
   // dbus_bus_add_match(dbus_conn, "type='signal',interface='test.signal.Type'", &dbus_error);
  //  dbus_bus_add_match(dbus_conn, "type='signal',interface='com.sagiadinos.garlic.daemon'", &dbus_error);


     /* dbus-glib call */

   // dbus_connection_flush(dbus_conn);
    if (dbus_error_is_set(&dbus_error))
    {
       fprintf(stderr, "Match Error (%s)\n", dbus_error.message);
       exit(1);
    }
/*    dbus_bus_add_match(dbus_conn, "type='signal',interface='com.sagiadinos.garlic.daemon.close'", &dbus_error);
    dbus_connection_flush(dbus_conn);
    if (dbus_error_is_set(&dbus_error))
    {
       fprintf(stderr, "Match Error (%s)\n", dbus_error.message);
       exit(1);
    }*/
}

bool DBusServer::isReceiving()
{
    dbus_connection_read_write(dbus_conn, 0);
    dbus_msg = dbus_connection_pop_message(dbus_conn);
    if (dbus_msg != NULL)
        return true;

    return false;
}

void DBusServer::checkForSignal()
{

    // check if the message is a signal from the correct interface and with the correct name
    if (dbus_message_is_signal(dbus_msg, "test.signal.Type", "reboot"))
    {
       // read the parameters
       if (!dbus_message_iter_init(dbus_msg, &args))
          fprintf(stderr, "Message has no arguments!\n");
       else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
          fprintf(stderr, "Argument is not string!\n");
       else {
          dbus_message_iter_get_basic(&args, &sigvalue);
          printf("Got Signal with value %s\n", sigvalue);
       }
    }

    // free the message
    dbus_message_unref(dbus_msg);
}

void DBusServer::checkForMethod()
{
    // check this is a method call for the right interface and method
    if (dbus_message_is_method_call(dbus_msg, "com.sagiadinos.garlic.daemon.reboot", "reboot"))
       replyToMethodCall();

    // free the message
    dbus_message_unref(dbus_msg);
}


void DBusServer::queryDBus()
{
    // Compose remote procedure call
    dbus_msg = ::dbus_message_new_method_call("org.freedesktop.DBus", "/", "org.freedesktop.DBus.Introspectable", "Introspect");

    // Invoke remote procedure call, block for response
    dbus_reply = ::dbus_connection_send_with_reply_and_block(dbus_conn, dbus_msg, DBUS_TIMEOUT_USE_DEFAULT, &dbus_error);
}

void DBusServer::cleanQuery()
{
    ::dbus_message_unref(dbus_msg);
    ::dbus_message_unref(dbus_reply);
}

std::string DBusServer::parseMessage()
{
    const char * dbus_result = nullptr;

    // Parse response
    ::dbus_message_get_args(dbus_reply, &dbus_error, DBUS_TYPE_STRING, &dbus_result, DBUS_TYPE_INVALID);

    return dbus_result;
}

void DBusServer::replyToMethodCall()
{
   DBusMessage* reply;
   DBusMessageIter args;
   DBusConnection* conn;
   bool stat = true;
   dbus_uint32_t level = 21614;
   dbus_uint32_t serial = 0;
   char* param = NULL;

   // read the arguments
   if (!dbus_message_iter_init(dbus_msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not string!\n");
   else
      dbus_message_iter_get_basic(&args, &param);
   printf("Method called with %s\n", param);

   // create a reply from the message
   reply = dbus_message_new_method_return(dbus_msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }

   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}
