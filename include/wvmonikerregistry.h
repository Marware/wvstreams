/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Support for moniker registries.  See wvmoniker.h.
 */
#ifndef __WVMONIKERREGISTRY_H
#define __WVMONIKERREGISTRY_H

#include "wvmoniker.h"
#include "wvhashtable.h"

/**
 * A dictionary for holding moniker-prefix to factory-function mappings.
 * 
 * This is used by WvMoniker and wvcreate().  See those for details.
 */
class WvMonikerRegistry : public IObject
{
    struct Registration
    {
	WvString id;
	WvMonikerCreateFunc *func;
	
	Registration(WvStringParm _id, WvMonikerCreateFunc *_func) 
	    : id(_id)
	    { func = _func; }
    };
    
    DeclareWvDict(Registration, WvString, id);

    unsigned refcount;
    
public:
    UUID reg_iid;
    RegistrationDict dict;
    
    WvMonikerRegistry(const UUID &iid);
    virtual ~WvMonikerRegistry();
    
    virtual void add(WvStringParm id, WvMonikerCreateFunc *func);
    virtual void del(WvStringParm id);
    
    virtual IObject *create(WvStringParm _s,
			    IObject *obj = NULL, void *userdata = NULL);
    
    // find a registry for objects of the given interface UUID
    static WvMonikerRegistry *find_reg(const UUID &iid);
    
    // IObject stuff
    virtual IObject *getInterface(const UUID &uuid);
    
    // we can't use GenericComponent, since we have to unregister ourselves
    // on the second-last release().
    virtual unsigned int addRef();
    virtual unsigned int release();
};


#endif // __WVMONIKERREGISTRY_H