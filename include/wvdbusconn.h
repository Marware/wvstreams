/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2004-2006 Net Integration Technologies, Inc.
 * 
 * Pathfinder Software:
 *   Copyright (C) 2007, Carillon Information Security Inc.
 *
 * This library is licensed under the LGPL, please read LICENSE for details.
 *
 * A WvDBusConn represents a connection to another application. Messages
 * can be sent and received via this connection. In most cases, the
 * other application is a message bus. 
 */ 
#ifndef __WVDBUSCONN_H
#define __WVDBUSCONN_H

#include "wvistreamlist.h"
#include "wvlog.h"
#include "wvdbusmsg.h"

class WvDBusConn;

struct DBusConnection;
struct DBusError;
struct DBusWatch;
struct DBusTimeout;
struct DBusPendingCall;

/**
 * The data type of callbacks used by WvDBusConn::add_callback().  The
 * return value should be true if the callback processes the message, false
 * otherwise.
 */
typedef WvCallback<bool, WvDBusConn&, WvDBusMsg&> WvDBusCallback;
    

class WvDBusConn : public WvIStreamList
{
    friend class WvDBusConnHelpers;
public:
    WvLog log;
    
    /**
     * Creates a new dbus connection on the given bus.
     * 
     * WvDBus uses special monikers for the "standard" DBus buses:
     * bus:system, bus:session, and bus:starter.  These correspond to 
     * DBUS_BUS_SYSTEM, DBUS_BUS_SESSION, and DBUS_BUS_STARTER, respectively.
     */
    WvDBusConn(WvStringParm dbus_moniker);
    
    /**
     * Initialize this object from an existing low-level DBusConnection object.
     */
    WvDBusConn(DBusConnection *_c, WvStringParm _uniquename);
    
    /**
     * Release this connection.  If this is the last owner of the associated
     * DBusConnection object, the connection itself closes.
     */
    virtual ~WvDBusConn();

    /**
     * Request the given service name on DBus.  There's no guarantee the
     * server will let us have the requested name, though.
     * 
     * The name will be released when this connection object is destroyed.
     */
    void request_name(WvStringParm name, WvDBusCallback onreply = 0);
    
    /**
     * Return this connection's unique name on the bus, assigned by the server
     * at connect time.
     */
    WvString uniquename() const;
    
    /**
     * Called by WvStreams when incoming data is ready.
     */
    virtual void execute();
    
    /**
     * False if the connection is nonfunctional for any reason.
     */
    virtual bool isok() const;
    
    /**
     * Close the underlying stream.  The object becomes unusable.  This is
     * also called whenever an error is set.
     */
    virtual void close();
    
    /**
     * Send a message on the bus, returning the serial number that was
     * assigned to it.
     */
    uint32_t send(WvDBusMsg &msg);
    
    /**
     * Send a message on the bus, calling onreply() when the answer comes
     * back.
     */
    void send(WvDBusMsg &msg, const WvDBusCallback &onreply);
    
    /**
     * The priority level of a callback registration.  This defines the order
     * in which callbacks are processed, from lowest to highest integer.
     */
    enum CallbackPri { 
	PriSystem    = 0,     // implemented by DBus or WvDBus.  Don't use.
	PriSpecific  = 5000,  // strictly limited by interface/method
	PriNormal    = 6000,  // a reasonably selective callback
	PriBridge    = 7000,  // proxy selectively to other listeners
	PriBroadcast = 8000,  // last-resort proxy to all listeners
    };
    
    /**
     * Adds a callback to the connection: all received messages will be
     * sent to all callbacks to look at and possibly process.  This method
     * is simpler and more flexible than add_listener()/add_method(),
     * but it can be slow if you have too many callbacks set.
     *
     * Your application is very unlikely to have "too many" callbacks.  If
     * for some reason you need to register lots of separate callbacks,
     * make your own data structure for them and register just a single
     * callback here that looks things up in your own structure.
     * 
     * 'pri' defines the callback sort order.  When calling callbacks, we
     * call them in priority order until the first callback returns 'true'.
     * If you just want to log certain messages and let other people handle
     * them, use a high priority but return 'false'.
     * 
     * 'cookie' is used to identify this callback for del_callback().  Your
     * 'this' pointer is a useful value here.
     */
    void add_callback(CallbackPri pri, WvDBusCallback cb, void *cookie = NULL);
    
    /**
     * Delete all callbacks that have the given cookie.
     */
    void del_callback(void *cookie);

    /**
     * Set the error code of this connection if 'e' is nonempty.
     */
    void maybe_seterr(DBusError &e);

    /**
     * Called by DBus for each incoming message.  Returns true if we handled
     * this message, false if not.
     */
    virtual bool filter_func(WvDBusConn &conn, WvDBusMsg &msg);
    
private:
    struct CallbackInfo
    {
	CallbackPri pri;
	WvDBusCallback cb;
	void *cookie;
	
	CallbackInfo(CallbackPri _pri,
		     const WvDBusCallback &_cb, void *_cookie)
	    : cb(_cb)
	    { pri = _pri; cookie = _cookie; }
    };
    static int priority_order(const CallbackInfo *a, const CallbackInfo *b);
	
    DeclareWvList(CallbackInfo);
    CallbackInfoList callbacks;
    
    void init(bool client);

    bool add_watch(DBusWatch *watch);
    void remove_watch(DBusWatch *watch);
    void watch_toggled(DBusWatch *watch);

    bool add_timeout(DBusTimeout *timeout);
    void remove_timeout(DBusTimeout *timeout);
    void timeout_toggled(DBusTimeout *timeout);

    static void pending_call_notify(DBusPendingCall *pending, void *user_data);

    DBusConnection *dbusconn;
    bool registered;
    WvString _uniquename;
};

#endif // __WVDBUSCONN_H
