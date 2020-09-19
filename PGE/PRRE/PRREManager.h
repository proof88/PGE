#pragma once

/*
    ###################################################################################
    PRREManager.h
    This file is part of PRRE.
    External header.
    Ancestor classes for manager and managed classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREBaseClass.h"


class PRREManager;


/**
    Ancestor class for managed classes.
    Cannot be copied, because the copy should be inserted into the manager's array (and it's unnecessary to implement now).

    This class doesn't use any API directly.
*/
class PRREManaged :
    public PRREBaseClass
{

public:
    PRREManaged();       
    virtual ~PRREManaged();            

    void               DetachFrom();                      /**< Removes the managed from its manager. */
    PRREManager*       getManager() const;                /**< Gets the manager of the managed. */

    const std::string& getName() const;                   /**< Gets the name. */
    void               SetName(const std::string& name);  /**< Sets the name. */

    virtual void       FlushResources();                  /**< This can be used if the specialized managed object has some resources
                                                               that may be released from memory when not needed anymore. */

    virtual TPRREuint  getUsedSystemMemory() const;       /**< Gets the amount of allocated system memory for this managed. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------

    PRREManaged(const PRREManaged& other);
    PRREManaged& operator=(const PRREManaged& other);

private:
    static TPRREint nManagedsTotal;

    // ---------------------------------------------------------------------------

    PRREManager* pParentMgr;        /**< Who manages this; descendants can read it through getManager(). */
    std::string  sName;             /**< Name. */

    friend class PRREManager;
    friend class PRREManagerImpl;   /* Manager can fully access it. */

}; // class PRREManaged




/**
    Ancestor class for manager classes.
    Copying is not available yet.

    This class doesn't use any API directly.
*/
class PRREManager :
    public PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREManager is included")
#endif

public:
    PRREManager();  
    virtual ~PRREManager(); 

    TPRREint     getCount() const;                        /**< Gets the number of managed objects. */
    TPRREbool    isEmpty() const;                         /**< Is the number of manageds 0? */
    TPRREint     getSize() const;                         /**< Gets the number of allocated slots for managed objects; getSize() >= getCount(). */
    PRREManaged* getAttachedAt(TPRREint ind) const;       /**< Gets the pointer to the managed at the given index. */
    TPRREint     getAttachedIndex(
        const PRREManaged& m) const;                      /**< Gets the index of the given managed. */
    void         PreAlloc(TPRREint count);                /**< Allocates count free slots for manageds. */
    TPRREbool    hasAttached(const PRREManaged& m) const; /**< Gets whether the given managed is managed by this manager. */
    void         Attach(PRREManaged& m);                  /**< Adds the given managed to the manager, if the managed has no manager yet. */
    void         Detach(PRREManaged& m);                  /**< Removes the given managed from the manager, so the managed will have no manager. */
    void         DeleteAttachedInstance(PRREManaged& m);  /**< Removes the given managed from the manager and destructs it. */
    void         DeleteAll();                             /**< Removes and destructs every managed element in the manager. */
    virtual void WriteList() const;                       /**< Writes a list of managed objects to the console. */

protected:

    // ---------------------------------------------------------------------------

    PRREManager(const PRREManager& other);
    PRREManager& operator=(const PRREManager& );

    virtual void WriteListCallback(const PRREManaged& mngd) const;  /**< WriteList() calls this for every single managed object. */

private:
    class PRREManagerImpl;
    PRREManagerImpl* pImpl;

}; // class PRREManager




