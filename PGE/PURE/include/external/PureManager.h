#pragma once

/*
    ###################################################################################
    PureManager.h
    This file is part of PURE.
    External header.
    Ancestor classes for manager and managed classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureAllHeaders.h"
#include "PureTypes.h"

class CConsole;
class PureManager;


/**
    Ancestor class for managed classes.
    Cannot be copied, because the copy should be inserted into the manager's array (and it's unnecessary to implement now).
*/
class PureManaged
{

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureManaged(); /* TODO: mark this as noexcept(false) when using newer compiler! */      
    virtual ~PureManaged();    

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    const PureManaged* getUtiliser() const;                  /**< Returns the managed that utilizes this managed. */
          PureManaged* getUtiliser();                        /**< Returns the managed that utilizes this managed. */
    void               SetUtiliser(PureManaged* pUtiliser);  /**< Sets the managed that utilizes this managed. */

    void               DetachFrom();                      /**< Removes the managed from its manager. */
    PureManager*       getManager() const;                /**< Gets the manager of the managed. */

    const std::string& getName() const;                   /**< Gets the name. */
    void               SetName(const std::string& name);  /**< Sets the name. */

    virtual void       FlushResources();                  /**< This can be used if the specialized managed object has some resources
                                                               that may be released from memory when not needed anymore. */

    virtual TPureUInt  getUsedSystemMemory() const;       /**< Gets the amount of allocated system memory for this managed. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------

    PureManaged(const PureManaged& other);
    PureManaged& operator=(const PureManaged& other);

private:
    class PureManagedImpl;
    PureManagedImpl* pImpl;

    friend class PureManager;
    friend class PureManagerImpl;   /* Manager can fully access it. */

}; // class PureManaged


/**
    Ancestor class for manager classes.
    Copying is not available yet.
*/
class PureManager
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureManager is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureManager(); /* TODO: mark this as noexcept(false) when using newer compiler! */ 
    virtual ~PureManager();

    CConsole&    getConsole() const;                      /**< Returns access to console preset with logger module name as this class. */

    TPureInt     getCount() const;                        /**< Gets the number of managed objects. */
    TPureBool    isEmpty() const;                         /**< Is the number of manageds 0? */
    TPureInt     getSize() const;                         /**< Gets the number of allocated slots for managed objects; getSize() >= getCount(). */
    PureManaged* getAttachedAt(TPureInt ind) const;       /**< Gets the pointer to the managed at the given index. */
    TPureInt     getAttachedIndex(
        const PureManaged& m) const;                      /**< Gets the index of the given managed. */
    void         PreAlloc(TPureInt count);                /**< Allocates count free slots for manageds. */
    TPureBool    hasAttached(const PureManaged& m) const; /**< Gets whether the given managed is managed by this manager. */
    virtual void Attach(PureManaged& m);                  /**< Adds the given managed to the manager, if the managed has no manager yet. */
    virtual void Detach(PureManaged& m);                  /**< Removes the given managed from the manager, so the managed will have no manager. */
    void         DeleteAttachedInstance(PureManaged& m);  /**< Removes the given managed from the manager and destructs it. */
    void         DeleteAll();                             /**< Removes and destructs every managed element in the manager. */
    virtual void HandleManagedPropertyChanged(
                 PureManaged& m);                         /**< Should be invoked when a managed's property got changed from a different kind of manager or managed. */
    virtual
    TPureUInt    getUsedSystemMemory() const;             /**< Gets the amount of allocated system memory for all manageds owner by this manager. */
    virtual void WriteList() const;                       /**< Writes a list of managed objects to the console. */

protected:

    // ---------------------------------------------------------------------------

    PureManager(const PureManager& other);
    PureManager& operator=(const PureManager& );

    virtual void WriteListCallback(const PureManaged& mngd) const;  /**< WriteList() calls this for every single managed object. */

private:
    class PureManagerImpl;
    PureManagerImpl* pImpl;

}; // class PureManager




