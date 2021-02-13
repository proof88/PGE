#pragma once

/*
    ###################################################################################
    PRREManagedImpl.h
    This file is part of PRRE.
    Internal header.
    PRREManagedImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../external/PRREallHeaders.h"
#include "../external/PRREManager.h"

class PRREManaged::PRREManagedImpl
{

public:
    virtual ~PRREManagedImpl();            

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

    explicit PRREManagedImpl(PRREManaged* owner);

    PRREManagedImpl(const PRREManagedImpl& other);
    PRREManagedImpl& operator=(const PRREManagedImpl& other);

private:
    static TPRREint nManagedsTotal;

    // ---------------------------------------------------------------------------

    PRREManaged* _pOwner;
    PRREManager* pParentMgr;        /**< Who manages this; descendants can read it through getManager(). */
    std::string  sName;             /**< Name. */

    friend class PRREManaged;
    friend class PRREManager;
    friend class PRREManagerImpl;   /* Manager can fully access it. */

}; // class PRREManaged::PRREManagedImpl