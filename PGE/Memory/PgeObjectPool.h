#pragma once

/*
    ###################################################################################
    PgeObjectPool.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine object pool for permanently allocating fixed number of objects.
    Made by PR00F88
    ###################################################################################
*/

#include <string>
#include <type_traits>

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"

// I've added _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING to preprocessor defines list in project settings,
// because defining here before including blIteratorAPI is NOT enough, as it needs to be defined before ANY stl include!
#include "blIteratorAPI/blIteratorAPI.hpp"


class PgePooledObject;


/**
* Base class for pool only to have a non-template base so we can use it as non-void pointer in PgePooledObject.
*/
class PgeObjectPoolBase
{
public:
    PgeObjectPoolBase() = default;
    virtual ~PgeObjectPoolBase() = default;

    PgeObjectPoolBase(const PgeObjectPoolBase&) = default;
    PgeObjectPoolBase& operator=(const PgeObjectPoolBase&) = default;
    PgeObjectPoolBase(PgeObjectPoolBase&&) = default;
    PgeObjectPoolBase& operator=(PgeObjectPoolBase&&) = default;

    // This one is declared so we can have convenience function remove() within PgePooledObject.
    virtual void remove(PgePooledObject& ptr) = 0;
};


/**
* Only classes derived from PgePooledObject can be stored in PgeObjectPool.
* Rules for making such derived class:
*  - must have an init() function that can be used anytime to re-initialize the object with proper values.
*    This init() is invoked by PgeObjectPool::create(), forwarding the given parameters to it.
*    Therefore, the init() function should not do performance-extensive operations as the primary aim of
*    this whole pooled approach is to have blazing-fast real-time create and remove operations.
* 
* See PgeObjectPoolTest or the PRooFPS-dd project for real examples.
*/
class PgePooledObject
{
public:

    virtual ~PgePooledObject() = default;

    PgePooledObject(const PgePooledObject&) = default;
    PgePooledObject& operator=(const PgePooledObject&) = default;
    PgePooledObject(PgePooledObject&&) = default;
    PgePooledObject& operator=(PgePooledObject&&) = default;

    /**
    * @return The object pool that created and manages this pooled object.
    */
    PgeObjectPoolBase& getParentPool()
    {
        return m_parentPool;
    }

    /**
    * @return The object pool that created and manages this pooled object.
    */
    const PgeObjectPoolBase& getParentPool() const
    {
        return m_parentPool;
    }

    /**
    * @return True if this pooled object is flagged as used, false if it is free and available to be reused.
    */
    const bool& used() const
    {
        return m_isUsed;
    }

    const PgePooledObject* next() const
    {
        return m_pNext;
    }

    PgePooledObject* next()
    {
        return m_pNext;
    }

    /**
    * Convenience function. Equivalent to: PgeObjectPool.remove(*this) .
    */
    void remove()
    {
        m_parentPool.remove(*this);
    }

protected:
    /**
    * Only PgeObjectPool should instantiate such pooled object, passing itself to this instance.
    * Obviously not private so we allow user to derive from this class.
    */
    template<typename T>
    friend class PgeObjectPool;

    PgePooledObject(PgeObjectPoolBase& parentPool) : m_parentPool(parentPool)
    {}

private:
    PgeObjectPoolBase& m_parentPool;
    bool m_isUsed{false};
    PgePooledObject* m_pNext{nullptr};

    /* Even derived classes SHALL NOT modify isUsed and pNext, it is only for PgeObjectPool! */

    void setUsed(const bool& state)
    {
        m_isUsed = state;
    }

    void setNext(PgePooledObject* ptr)
    {
        m_pNext = ptr;
    }

}; // class PgePooledObject


/**
    PR00F's Game Engine object pool for permanently allocating fixed number of game objects.
    The aim of this class is to have a fixed number of objects being kept in contiguous memory area, and
    to be able to distinguish between them as they are free for use or busy (being used).
    
    This way we just flag the preallocated objects, instead of actually creating or deleting them in memory
    which would be otherwise performance-expensive operation, especially when we are talking about resource-heavy
    objects such as PureObject3D or even higher-level game instances.

    Thus, I expect this pool to be used for storing objects created and deleted with high frequency,
    such as bullets fired from weapons, particles, etc.

    Since these virtual "create" and "delete" operations are expected to happen multiple times
    within 1 frame, I want them to have O(1) complexity.
    
    Also, since they are in contiguous memory area, and usually higher-level logic iterates over
    them from begin to end, it is more cache-friendly than iterating over occasionally allocated objects
    that are placed here and there in memory.

    The only downsides of having such preallocated object pool is that:
     - we need to have a good guess for the number of required objects to be allocated at the beginning;
     - these preallocated objects are always using memory capacity even when not all is needed.

    However, the advantages regarding performance are outweighing these small disadvantages.

    Basically this class is based on this "Object Pool Pattern": https://gameprogrammingpatterns.com/object-pool.html .

    Also, for iterators, I'm using Vincenzo Barbato's blIteratorAPI: https://github.com/navyenzo/blIteratorAPI .
    Note that you might need to define the _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING macro for
    the MSVC preprocessor, it comes from the way of how blIteratorAPI is implemented. It is safe to silence this
    warning.

    Reminder: this class is NOT a memory pool neither a memory manager. This is an object pool with
    fixed number of objects, and after construction, no more memory allocation happens!
    For memory pool management, I would rather use an already existing solution from these:
     - https://github.com/gperftools/gperftools
     - https://jemalloc.net/
     - https://github.com/electronicarts/EASTL
     - https://github.com/cacay/MemoryPool/tree/master
     - https://github.com/lenonk/memorypool
     - https://github.com/danielkrupinski/StringPool?tab=readme-ov-file
*/
template <typename T>
class PgeObjectPool : public PgeObjectPoolBase
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeObjectPool is included")   
#endif
// https://stackoverflow.com/questions/4984502/how-to-force-template-class-to-be-derived-from-baseclassa
static_assert(std::is_base_of<PgePooledObject, T>::value, "T must derive from PgePooledObject");

public:

    static const char* getLoggerModuleName()
    {
        return "PgeObjectPool";
    }

    // ---------------------------------------------------------------------------

    CConsole& getConsole() const
    {
        return CConsole::getConsoleInstance(getLoggerModuleName());
    }

    /**
    * This constructor should be used when parameters for initializing the pool is NOT available.
    * In this case, no memory is allocated, size remains 0, and later the pool needs to be initialized
    * using reserve(). The name of the pool will be "unnamed pool".
    */
    PgeObjectPool() :
        m_name("unnamed pool")
    {
        getConsole().SOLn("PgeObjectPool %s with empty size created successfully!", m_name.c_str());
    }

    /**
    * Not only the memory pool is allocated for all pooled objects, but their non-default constructor
    * is also called with the provided pooled object parameters, or their default constructor if that
    * is available.
    * 
    * It is decided by the user if the pooled object can be properly initialized when the pool is allocated, or
    * further initialization is needed later. In the latter case, either a default constructor must be available
    * or default parameters should be passed to this constructor to be forwarded to the pooled object's
    * non-default constructor to bring the pooled objects to a default state, and later the pooled objects
    * can be properly initialized using the pool's create() function which can forward arbitrary parameters
    * to the pooled objects init() function.
    * 
    * @param poolName       Name of this pool, for informative purpose.
    * @param capacity       Number of pooled objects to be stored in this pool.
    *                       Can be changed later with reserve() only if 0 is passed here.
    * @param pooledObjArgs  Arguments to be forwarded to the constructor of the pooled objects.
    */
    template<typename... Args>
    PgeObjectPool(
        const std::string& poolName, const size_t& capacity, Args&&... pooledObjArgs)
    {
        reserve(poolName, capacity, std::forward<Args>(pooledObjArgs)...);
    }

    ~PgeObjectPool()
    {
        deallocate();
    }

    PgeObjectPool(const PgeObjectPool&) = delete;
    PgeObjectPool& operator=(const PgeObjectPool&) = delete;
    PgeObjectPool(PgeObjectPool&&) = delete;
    PgeObjectPool&& operator=(PgeObjectPool&&) = delete;

    /**
    * Frees up the allocated contiguous memory pool, capacity becomes zero, name becomes "unnamed pool".
    * Capacity can be changed again to non-zero value using reserve().
    */
    void deallocate()
    {
        if (m_capacity == 0)
        {
            return;
        }

        // I have to manually call the dtors because I allocated them 1 by 1 with placement new, so "delete[] m_pool" is not enough.
        for (size_t i = 0; i < m_capacity; i++)
        {
            m_pool[i].~T();
        }

        // And then actually free up the memory pool.
        ::operator delete[](m_pool);

        m_name = "unnamed pool";
        m_capacity = 0;
        m_count = 0;
        m_pool = nullptr;
        m_firstAvailable = nullptr;
        m_rawArrayWrapper = blIteratorAPI::getRawArrayWrapper(m_pool, m_capacity);
    }

    /**
    * @return Number of used objects (PgePooledObject) in this pool, having their used() state true.
    *         Basically this is the number of elements in the container, if this pool was an std container.
    */
    const size_t& count() const
    {
        return m_count;
    }

    /**
    * Conventionally std containers have a size() member function returning the number of elements, NOT the capacity.
    * Thus, size() is equivalent to count().
    * @return Number of used objects (PgePooledObject) in this pool, having their used() state true.
    *         Basically this is the number of elements in the container, if this pool was an std container.
    */
    const size_t& size() const
    {
        return m_count;
    }

    /**
    * @return Total number of allocated objects (PgePooledObject) in this pool.
    */
    const size_t& capacity() const
    {
        return m_capacity;
    }

    /**
    * @return True if none of the pooled objects are currently in use, false otherwise.
    */
    bool empty() const
    {
        return m_count == 0;
    }

    /**
    * @return Name of this pool as it was specified in ctor.
    */
    const std::string& name() const
    {
        return m_name;
    }

    /**
    * Finds a free (usable) object in the pool, sets it flag as used and returns it.
    * Complexity is O(1) (constant).
    * Note: the returned object stays in the pool but marked as used, and the user can mark it as free by calling remove().
    * 
    * @return An object ready to be used by caller, or nullptr if all objects within the pool are used already.
    */
    template<typename... Args>
    T* create(Args&&... pooledObjArgs)
    {
        if (!m_firstAvailable)
        {
            // no more available
            return nullptr;
        }

        PgePooledObject* const ptr = m_firstAvailable;
        ptr->setUsed(true);
        m_firstAvailable = ptr->next();
        m_count++;

        T* const ptrAsT = static_cast<T*>(ptr);
        ptrAsT->init(std::forward<Args>(pooledObjArgs)...);

        return ptrAsT;
    }

    /**
    * Resets the free (usable) flag of this object, "returns it" into the pool so it can be reused again.
    * Complexity is O(1) (constant).
    * 
    * Note: the removed object stays in the pool's contiguous memory, just gets marked as free, so a future call to create() might
    * return it again to the user.
    * 
    * @param obj The pooled object to be returned to the pool as free-to-use object.
    */
    void remove(PgePooledObject& obj)
    {
        if (&obj.getParentPool() != this)
        {
            throw std::runtime_error("PgeObjectPool::remove(): PgePooledObject parent pool mismatch!");
            return;
        }

        if (!obj.used())
        {
            return;
        }

        obj.setNext(m_firstAvailable);
        obj.setUsed(false);
        m_firstAvailable = &obj;
        m_count--;
    }

    /**
    * Resets the free (usable) flag of the pooled object pointed by the given iterator, "returns" the object into the pool so it can be reused again.
    * Equivalent to: remove(*itPos).
    * This function is for convenience, to mimic the erase() function of std containers, so it is easier to migrate code using an std::container to
    * use PgeObjectPool.
    * Complexity is O(1) (constant).
    * 
    * Note: the erased object stays in the pool's contiguous memory, just gets marked as free, so a future call to create() might
    * return it again to the user.
    * 
    * @param itPos Iterator to the element to be "erased".
    * 
    * @return Iterator following the last removed element.
    *         If itPos refers to the last element, then the end() iterator is returned.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::iterator erase(typename blIteratorAPI::blRawArrayWrapper<T>::iterator itPos)
    {
        if (itPos == m_rawArrayWrapper.end())
        {
            return m_rawArrayWrapper.end();
        }

        remove(*itPos);
        return ++itPos;
    }

    /**
    * Not only the memory pool is allocated for all pooled objects, but their non-default constructor
    * is also called with the provided pooled object parameters, or their default constructor if that
    * is available.
    * 
    * Currently works only for zero-capacity pools.
    * 
    * Cannot be used to reduce the capacity of the container.
    * To reduce the capacity of a non-zero capacity container to non-zero, or to increase the capacity, first
    * use deallocate() to completely free up the used memory area, and then call reserve().
    *
    * It is decided by the user if the pooled object can be properly initialized when the pool is allocated, or
    * further initialization is needed later. In the latter case, either a default constructor must be available
    * or default parameters should be passed to this function to be forwarded to the pooled object's
    * non-default constructor to bring the pooled objects to a default state, and later the pooled objects
    * can be properly initialized using the pool's create() function which can forward arbitrary parameters
    * to the pooled objects init() function.
    *
    * @param poolName       Name of this pool, for informative purpose.
    * @param capacity       Number of pooled objects to be stored in this pool.
    *                       Can be changed later only with reserve() if 0 is passed here.
    * @param pooledObjArgs  Arguments to be forwarded to the constructor of the pooled objects.
    */
    template<typename... Args>
    void reserve(const std::string& poolName, const size_t& capacity, Args&&... pooledObjArgs)
    {
        if (m_capacity != 0)
        {
            throw std::runtime_error("PgeObjectPool::reserve(): already allocated pool " + m_name + "!");
            return;
        }

        m_name = poolName;
        m_capacity = capacity;

        // early return in case capacity is 0, this is because even with 0 capacity operator new[] would
        // return a non-null address down there which I definitely want to avoid!
        // https://stackoverflow.com/questions/1087042/c-new-int0-will-it-allocate-memory
        if (m_capacity == 0)
        {
            getConsole().SOLn("PgeObjectPool %s with empty size created successfully!", m_name.c_str());
            return;
        }

        /* I cannot simply forward arguments like this:
           new T[capacity]{ std::forward<Args>(args)... }
           because aggregate initialization would use the default ctor of T, which might not be available!
           So we are forwarding arguments in the loop below. */
        m_pool = static_cast<T*>(::operator new[](capacity * sizeof(T)));
        m_firstAvailable = m_pool;

        for (size_t i = 0; i < m_capacity; i++)
        {
            /* Perfect forwarding : https://devblogs.microsoft.com/oldnewthing/20230727-00/?p=108494 .
               I use placement new here so the already allocated mem pool is used.
               Reason explained above. */
            new (&m_pool[i]) T(*this, std::forward<Args>(pooledObjArgs)...);
        }

        for (size_t i = 0; i < (m_capacity - 1); i++)
        {
            m_pool[i].setNext(&(m_pool[i + 1]));
        }
        // no need for last elem's setNext(nullptr) since its ctor sets it by default

        m_rawArrayWrapper = blIteratorAPI::getRawArrayWrapper(m_pool, m_capacity);

        getConsole().SOLn("PgeObjectPool %s with size of %u Bytes created successfully!", poolName.c_str(), capacity * sizeof(T));
    }

    /**
    * Resets the free (usable) flag of all objects in the pool, "returns them" into the pool so they can be reused again.
    * Complexity is O(n) (linear).
    */
    void clear()
    {
        for (size_t i = 0; i < m_capacity; i++)
        {
            remove(m_pool[i]);
        }
    }

    /**
    * @return All pooled instances, derived from PgePooledObject, both free and used.
    *         Nullptr if the pool is not yet initialized with non-zero size.
    */
    T* elems()
    {
        return m_pool;
    }

    /**
    * @return All pooled instances, derived from PgePooledObject, both free and used.
    *         Nullptr if the pool is not yet initialized with non-zero size.
    */
    const T* elems() const
    {
        return m_pool;
    }

    /**
    * @return The underlying wrapper object giving the iterator functionality.
    */
    typename blIteratorAPI::blRawArrayWrapper<T> rawArrayWrapper()
    {
        return m_rawArrayWrapper;
    }

    /**
    * Gives iterator for beginning iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Iterator to the first element of the object pool, to begin iterating over elements in order as they are placed in the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::iterator begin()
    {
        return m_rawArrayWrapper.begin();
    }

    /**
    * Gives iterator for ending iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Iterator to after the last element of the object pool, to detect finished iterating over elements in order as they are placed in the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::iterator end()
    {
        return m_rawArrayWrapper.end();
    }

    /**
    * Gives const iterator for beginning iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Constant iterator to the first element of the object pool, to begin iterating over elements in order as they are placed in the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::const_iterator cbegin() const
    {
        return m_rawArrayWrapper.cbegin();
    }

    /**
    * Gives const iterator for ending iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Constant iterator to after the last element of the object pool, to detect finished iterating over elements in order as they are placed in the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::const_iterator cend() const
    {
        return m_rawArrayWrapper.cend();
    }

    /**
    * Gives iterator for beginning reverse iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Reverse iterator to the last element of the object pool, to begin iterating over elements in reverse order, from the end of the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::reverse_iterator rbegin()
    {
        return m_rawArrayWrapper.rbegin();
    }

    /**
    * Gives iterator for ending reverse iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Reverse iterator to after the first element of the object pool, to detect finished iterating over elements in reversed order, from the end of the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::reverse_iterator rend()
    {
        return m_rawArrayWrapper.rend();
    }

    /**
    * Gives const iterator for beginning reverse iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Constant reverse iterator to the last element of the object pool, to begin iterating over elements in reverse order, from the end of the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::const_reverse_iterator crbegin() const
    {
        return m_rawArrayWrapper.crbegin();
    }

    /**
    * Gives const iterator for ending reverse iterating over the contiguous memory area of pooled objects, the same area that elems() returns.
    * Note that iterating over this area iterating over both used and non-used pooled objects, thus you should filter for used pooled objects manually.
    * 
    * @return Constant reverse iterator to after the first element of the object pool, to detect finished iterating over elements in reversed order, from the end of the contiguous memory pool.
    */
    typename blIteratorAPI::blRawArrayWrapper<T>::const_reverse_iterator crend() const
    {
        return m_rawArrayWrapper.crend();
    }

private:
    std::string m_name{};
    size_t m_count{0}, m_capacity{0};
    T* m_pool{nullptr};
    PgePooledObject* m_firstAvailable{nullptr};
    blIteratorAPI::blRawArrayWrapper<T> m_rawArrayWrapper;

    // ---------------------------------------------------------------------------

}; // class PgeObjectPool
