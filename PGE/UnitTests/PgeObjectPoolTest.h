#pragma once

/*
    ###################################################################################
    PgeObjectPoolTest.h
    Unit test for PgeObjectPool.
    Made by PR00F88, West Whiskhyll Entertainment
    2024
    Please see UnitTest.h about my statement of using "bitwise and" operator with bool operands.
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Memory/PgeObjectPool.h"

class PgeObjectPoolTest :
    public UnitTest
{
public:

    class TestedPooledObject : public PgePooledObject
    {
    public:
        TestedPooledObject(PgeObjectPoolBase& parentPool, const size_t& n) : PgePooledObject(parentPool), m_n(n)
        {
        }

        ~TestedPooledObject() = default;

        TestedPooledObject(const TestedPooledObject&) = default;
        TestedPooledObject& operator=(const TestedPooledObject&) = default;
        TestedPooledObject(TestedPooledObject&&) = default;
        TestedPooledObject& operator=(TestedPooledObject&&) = default;

        const size_t& getValue() const
        {
            return m_n;
        }

        void setValue(const size_t& n)
        {
            m_n = n;
        }

    private:
        size_t m_n;
    };

    PgeObjectPoolTest() :
        UnitTest(__FILE__)
    {
    }

    ~PgeObjectPoolTest() = default;

    PgeObjectPoolTest(const PgeObjectPoolTest&) = delete;
    PgeObjectPoolTest& operator=(const PgeObjectPoolTest&) = delete;
    PgeObjectPoolTest(PgeObjectPoolTest&&) = delete;
    PgeObjectPoolTest& operator=(PgeObjectPoolTest&&) = delete;

protected:

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PgeObjectPool<int>::getLoggerModuleName(), true);

        AddSubTest("test_ctor", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_ctor);
        AddSubTest("test_create", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_create);
        AddSubTest("test_remove_func_of_pool", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_remove_func_of_pool);
        AddSubTest("test_remove_func_of_object", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_remove_func_of_object);
        AddSubTest("test_directly_exposed_iterators", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_directly_exposed_iterators);
        //AddSubTest("test_additional_iterators", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_additional_iterators);
    }

    virtual bool setUp() override
    {
        return true;
    }

    virtual void TearDown() override
    {
    }

    virtual void Finalize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PgeObjectPool<int>::getLoggerModuleName(), false);
    }

private:

    // ---------------------------------------------------------------------------

    bool test_ctor()
    {
        constexpr size_t capacity = 10;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3);
        bool b = true;

        b &= assertEquals("ints", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(0u, pool.count(), "count");

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(3u, pool.elems()[i].getValue(), ("value " + std::to_string(i)).c_str()) &
                assertFalse(pool.elems()[i].used(), ("used " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("parentpool " + std::to_string(i)).c_str());
        }

        for (size_t i = 0; b && (i < pool.capacity() - 1); i++)
        {
            b &= assertEquals(&(pool.elems()[i + 1]), pool.elems()[i].next(), ("next " + std::to_string(i)).c_str());
        }
        b &= assertTrue(nullptr == pool.elems()[pool.capacity() - 1].next(), "last next");

        return b;
    }

    bool test_create()
    {
        constexpr size_t capacity = 10;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3);
        bool b = true;

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(), ("create " + std::to_string(i)).c_str());
            b &= assertEquals(i + 1, pool.count(), ("create count " + std::to_string(i)).c_str());
        }
        b &= assertEquals(pool.capacity(), pool.count(), "last create count") &
            assertEquals(capacity, pool.capacity(), "cap 1");

        // elem data unchanged but now they are "used"
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(3u, pool.elems()[i].getValue(), ("create value " + std::to_string(i)).c_str()) &
                assertTrue(pool.elems()[i].used(), ("create used " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("create parentpool " + std::to_string(i)).c_str());
        }

        // next() ptrs also unchanged
        for (size_t i = 0; b && (i < pool.capacity() - 1); i++)
        {
            b &= assertEquals(&(pool.elems()[i + 1]), pool.elems()[i].next(), ("create next " + std::to_string(i)).c_str());
        }
        b &= assertTrue(nullptr == pool.elems()[pool.capacity() - 1].next(), "last create next");

        b &= assertNull(pool.create(), "create after all created already");

        return b;
    }

    bool test_remove_func_of_pool()
    {
        // testing pool.remove(obj), should be same as test_remove_func_of_objectt() but with different remove() being tested

        constexpr size_t capacity = 10;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3);
        bool b = true;

        // negative remove() test: nothing bad happens if we try to free any of the already freed/usable
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.remove(pool.elems()[i]);
            b &= assertEquals(0u, pool.count(), (" fake remove 1 count " + std::to_string(i)).c_str());
        }

        // now create them
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(), ("create " + std::to_string(i)).c_str());
        }

        // negative remove() test: try removing a pooled object NOT managed by this pool!
        PgeObjectPool<TestedPooledObject> pool_2("different ints", 5, 5);
        TestedPooledObject* const pooledByAnotherPool = static_cast<TestedPooledObject*>(pool_2.create());
        b &= assertNotNull(pooledByAnotherPool, "create 2");

        if (b)
        {
            try
            {
                pool.remove(*pooledByAnotherPool); // should throw ex
                b = false; 
            }
            catch (const std::exception&)
            {
                b = true;
                b &= assertEquals(pool.capacity(), pool.count(), "pool count() after negative removal of non-pooled object");
                b &= assertEquals(1u, pool_2.count(), "pool_2 count() after negative removal of non-pooled object");
                b &= assertTrue(pooledByAnotherPool->used(), "pooledByAnotherPool used() after negative removal of non-pooled object");
            }
        }

        // positive remove() test
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.remove(pool.elems()[i]);
            b &= assertEquals(pool.capacity() - i - 1, pool.count(), ("remove count " + std::to_string(i)).c_str());
        }
        b &= assertEquals(0u, pool.count(), "last remove count") &
            assertEquals(capacity, pool.capacity(), "cap 2");

        // elem data unchanged but now they are "free" again
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(3u, pool.elems()[i].getValue(), ("remove value " + std::to_string(i)).c_str()) &
                assertFalse(pool.elems()[i].used(), ("remove used " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("remove parentpool " + std::to_string(i)).c_str());
        }

        // next() ptrs are now in reversed order
        for (size_t i = pool.capacity() - 1; b && (i > 0); i--)
        {
            b &= assertEquals(&(pool.elems()[i - 1]), pool.elems()[i].next(), ("remove next " + std::to_string(i)).c_str());
        }
        b &= assertTrue(nullptr == pool.elems()[0].next(), "last remove next");

        // negative remove() test: again, nothing bad happens if we try to free any of the already freed/usable
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.remove(pool.elems()[i]);
            b &= assertEquals(0u, pool.count(), (" fake remove 2 count " + std::to_string(i)).c_str());
        }

        return b;
    }

    bool test_remove_func_of_object()
    {
        // testing pooled_object.remove(), should be same as test_remove_func_of_pool() but with different remove() being tested

        constexpr size_t capacity = 10;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3);
        bool b = true;

        // negative remove() test: nothing bad happens if we try to free any of the already freed/usable
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.elems()[i].remove();
            b &= assertEquals(0u, pool.count(), (" fake remove 1 count " + std::to_string(i)).c_str());
        }

        // now create them
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(), ("create " + std::to_string(i)).c_str());
        }

        // negative remove() test: try removing a pooled object NOT managed by this pool, cannot be done using pooled_object's remove(), nothing to test!

        // positive remove() test
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.elems()[i].remove();
            b &= assertEquals(pool.capacity() - i - 1, pool.count(), ("remove count " + std::to_string(i)).c_str());
        }
        b &= assertEquals(0u, pool.count(), "last remove count") &
            assertEquals(capacity, pool.capacity(), "cap 2");

        // elem data unchanged but now they are "free" again
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(3u, pool.elems()[i].getValue(), ("remove value " + std::to_string(i)).c_str()) &
                assertFalse(pool.elems()[i].used(), ("remove used " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("remove parentpool " + std::to_string(i)).c_str());
        }

        // next() ptrs are now in reversed order
        for (size_t i = pool.capacity() - 1; b && (i > 0); i--)
        {
            b &= assertEquals(&(pool.elems()[i - 1]), pool.elems()[i].next(), ("remove next " + std::to_string(i)).c_str());
        }
        b &= assertTrue(nullptr == pool.elems()[0].next(), "last remove next");

        // negative remove() test: again, nothing bad happens if we try to free any of the already freed/usable
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.elems()[i].remove();
            b &= assertEquals(0u, pool.count(), (" fake remove 2 count " + std::to_string(i)).c_str());
        }

        return b;
    }

    bool test_directly_exposed_iterators()
    {
        constexpr size_t capacity = 10;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 0);
        
        for (size_t i = 0; i < pool.capacity(); i++)
        {
            pool.elems()[i].setValue(i);
        }
        
        bool b = true;
        size_t i = 0;
        for (auto it = pool.begin(); b && (it != pool.end()); ++it)
        {
            b &= assertEquals(i, it->getValue(), ("value 1 " + std::to_string(i)).c_str()) &
                assertFalse(it->used(), ("used 1 " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(it->getParentPool()), ("parentpool 1 " + std::to_string(i)).c_str());
            i++;
        }

        //i = 0;
        //for (auto it = pool.cbegin(); b && (it != pool.cend()); it++)
        //{
        //    b &= assertEquals(i, it->getValue(), ("value 2 " + std::to_string(i)).c_str()) &
        //        assertFalse(it->used(), ("used 2 " + std::to_string(i)).c_str()) &
        //        assertEquals(&pool, &(it->getParentPool()), ("parentpool 2 " + std::to_string(i)).c_str());
        //    i++;
        //}
        
        i = 0;
        for (auto it = pool.rbegin(); b && (it != pool.rend()); it++)
        {
            b &= assertEquals(pool.capacity() - i - 1, it->getValue(), ("value 3 " + std::to_string(i)).c_str()) &
                assertFalse(it->used(), ("used 3 " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(it->getParentPool()), ("parentpool 3 " + std::to_string(i)).c_str());
            i++;
        }
        
        //i = 0;
        //for (auto it = pool.crbegin(); b && (it != pool.crend()); it++)
        //{
        //    b &= assertEquals(i, it->getValue(), ("value 4 " + std::to_string(i)).c_str()) &
        //        assertFalse(it->used(), ("used 4 " + std::to_string(i)).c_str()) &
        //        assertEquals(&pool, &(it->getParentPool()), ("parentpool 4 " + std::to_string(i)).c_str());
        //    i++;
        //}

        return b;
    }

    //bool test_additional_iterators()
    //{
    //    constexpr size_t capacity = 10;
    //    PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3);
    //
    //    for (size_t i = 0; i < pool.capacity(); i++)
    //    {
    //        pool.elems()[i].setValue(i);
    //    }
    //    bool b = true;
    //
    //    const blIteratorAPI::blLinearIterator<decltype(pool.rawArrayWrapper())> itLinear(pool.rawArrayWrapper());
    //    size_t i = 0;
    //    for (auto it = itLinear.begin(); b && (it != itLinear.end()); it++)
    //    {
    //        b &= assertEquals(3u, it->getValue(), ("value 1 " + std::to_string(i)).c_str()) &
    //            assertFalse(it->used(), ("used 1 " + std::to_string(i)).c_str()) &
    //            assertEquals(&pool, &(it->getParentPool()), ("parentpool 1 " + std::to_string(i)).c_str()) &
    //            assertEquals(static_cast<int>(i), it.getDistanceFromBeginToIter(), ("distance 1 " + std::to_string(i)).c_str());
    //        i++;
    //    }
    //
    //    return b;
    //}

};