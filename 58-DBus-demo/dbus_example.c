#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>

void check_dbus_error(DBusError *err, const char *msg) {
    if (dbus_error_is_set(err)) {
        fprintf(stderr, "%s: %s\n", msg, err->message);
        dbus_error_free(err);
        exit(1);
    }
}

int main() {
    DBusError err;
    DBusConnection* conn;
    DBusMessage* msg;
    DBusMessage* reply;
    DBusMessageIter args, array_iter;
    
    dbus_error_init(&err);

    // Connect to the system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    check_dbus_error(&err, "Failed to connect to the D-Bus system bus");

    // Create a new method call and check for errors
    msg = dbus_message_new_method_call(
        "org.freedesktop.DBus",        // Target service
        "/org/freedesktop/DBus",       // Object path
        "org.freedesktop.DBus",        // Interface
        "ListNames"                    // Method
    );
    if (!msg) {
        fprintf(stderr, "Message NULL\n");
        exit(1);
    }

    // Send message and wait for reply
    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
    check_dbus_error(&err, "Failed to send message");
    dbus_message_unref(msg);

    // Read the parameters
    if (!dbus_message_iter_init(reply, &args)) {
        fprintf(stderr, "Reply has no arguments\n");
    } else if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
        fprintf(stderr, "Argument is not an array!\n");
    } else {
        dbus_message_iter_recurse(&args, &array_iter);
        printf("List of service names:\n");
        while (dbus_message_iter_get_arg_type(&array_iter) != DBUS_TYPE_INVALID) {
            char* name;
            dbus_message_iter_get_basic(&array_iter, &name);
            printf("  %s\n", name);
            dbus_message_iter_next(&array_iter);
        }
    }

    // Clean up
    dbus_message_unref(reply);
    dbus_connection_unref(conn);

    return 0;
}

