#pragma once

/*
    ###################################################################################
    PureManagedImpl.h
    This file is part of PURE.
    Internal header.
    PureManagedImpl class.
    Made by PR00F88
    ###################################################################################
*/

#include "../external/PureAllHeaders.h"
#include "../external/PureManager.h"

class PureManaged::PureManagedImpl
{

public:
    virtual ~PureManagedImpl();            

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

    explicit PureManagedImpl(PureManaged* owner);

    PureManagedImpl(const PureManagedImpl& other);
    PureManagedImpl& operator=(const PureManagedImpl& other);

private:
    static TPureInt nManagedsTotal;

    // ---------------------------------------------------------------------------

    PureManaged* _pOwner;           /**< Public instance of which private part is this. */
    PureManager* pParentMgr;        /**< Who manages this; descendants can read it through getManager(). */
    PureManaged* pUtiliser;         /**< Who utilizes this managed. */
    std::string  sName;             /**< Name. */

    friend class PureManaged;
    friend class PureManager;
    friend class PureManagerImpl;   /* Manager can fully access it. */

}; // class PureManaged::PureManagedImpl