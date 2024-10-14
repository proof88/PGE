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

        /**
        * Without arguments, this is invoked by PgeObjectPool::create() without parameters passed.
        * So no need for the user to explicitly invoke this function (but allowed).
        */
        void init()
        {
        }

        /**
        * With arguments, this is invoked by PgeObjectPool::create(...) with parameters passed.
        * So no need for the user to explicitly invoke this function (but allowed).
        */
        void init(const size_t& n)
        {
            setValue(n);
        }

        virtual void onSetUsed() override
        {
            // used() already tells the new used state
            if (used())
            {
                if (m_n == 1234567u)
                {
                    m_n = 7654321u;
                }
            }
            else
            {
                if (m_n == 7654321u)
                {
                    m_n = 1234567u;
                }
            }
        }

    protected:
        /**
        * Only PgeObjectPool should instantiate such pooled object, and passing itself to this instance.
        */
        template<typename T>
        friend class PgeObjectPool;

        TestedPooledObject(PgeObjectPoolBase& parentPool, const size_t& n) : PgePooledObject(parentPool), m_n(n)
        {
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

        AddSubTest("test_ctor_without_args", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_ctor_without_args);
        AddSubTest("test_ctor_with_args_positive", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_ctor_with_args_positive);
        AddSubTest("test_ctor_with_args_negative", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_ctor_with_args_negative);
        AddSubTest("test_deallocate_normal", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_deallocate_normal);
        AddSubTest("test_deallocate_degen", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_deallocate_degen);
        AddSubTest("test_create_without_parameters", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_create_without_parameters);
        AddSubTest("test_create_with_parameters", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_create_with_parameters);
        AddSubTest("test_remove_func_of_pool", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_remove_func_of_pool);
        AddSubTest("test_remove_func_of_object", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_remove_func_of_object);
        AddSubTest("test_create_and_remove_invoke_onsetused", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_create_and_remove_invoke_onsetused);
        AddSubTest("test_directly_exposed_iterators", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_directly_exposed_iterators);
        //AddSubTest("test_additional_iterators", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_additional_iterators);
        AddSubTest("test_clear", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_clear);
        AddSubTest("test_reserve_empty", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_reserve_empty);
        AddSubTest("test_reserve_positive", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_reserve_positive);
        AddSubTest("test_reserve_negative", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_reserve_negative);
        AddSubTest("test_erase_positive", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_erase_positive);
        AddSubTest("test_erase_negative", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_erase_negative);
        AddSubTest("test_erase_and_clear_invoke_onsetused", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_erase_and_clear_invoke_onsetused);
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

    bool test_ctor_without_args()
    {
        constexpr size_t capacity = 0u;
        PgeObjectPool<TestedPooledObject> pool;
        bool b = true;

        b &= assertEquals("unnamed pool", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(0u, pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty") &
            assertTrue(nullptr == pool.elems(), "elems");

        size_t i = 0;
        for (const auto& obj : pool)
        {
            static_cast<void>(obj);
            i++;
        }
        b &= assertEquals(0u, i);

        return b;
    }

    bool test_ctor_with_args_positive()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        b &= assertEquals("ints", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(capacity * sizeof(TestedPooledObject), pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty");

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

    bool test_ctor_with_args_negative()
    {
        constexpr size_t capacity = 0u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        b &= assertEquals("ints", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(0u, pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty") &
            assertTrue(nullptr == pool.elems(), "elems");

        size_t i = 0;
        for (const auto& obj : pool)
        {
            static_cast<void>(obj);
            i++;
        }
        b &= assertEquals(0u, i);

        return b;
    }

    bool test_deallocate_normal()
    {
        PgeObjectPool<TestedPooledObject> pool("ints", 10u, 3u);
        bool b = true;

        b &= assertNotNull(pool.create(), "create");

        pool.deallocate();

        b &= assertEquals("unnamed pool", pool.name(), "name") &
            assertEquals(0u, pool.capacity(), "cap") &
            assertEquals(0u, pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty") &
            assertTrue(nullptr == pool.elems(), "elems");

        // this is not a reserve() test, just check if reserve can succeed after deallocate()
        pool.reserve("ints", 5u, 3u);
        b &= assertEquals("ints", pool.name(), "name") &
            assertEquals(5u, pool.capacity(), "cap") &
            assertEquals(5u * sizeof(TestedPooledObject), pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty");

        return b;
    }

    bool test_deallocate_degen()
    {
        PgeObjectPool<TestedPooledObject> pool("ints", 10u, 3u);
        bool b = true;

        b &= assertNotNull(pool.create(), "create");

        // double deallocate
        pool.deallocate();
        pool.deallocate();

        b &= assertEquals("unnamed pool", pool.name(), "name") &
            assertEquals(0u, pool.capacity(), "cap") &
            assertEquals(0u, pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty") &
            assertTrue(nullptr == pool.elems(), "elems");

        // deallocate already non-allocated
        PgeObjectPool<TestedPooledObject> pool_nomemory;
        pool_nomemory.deallocate();

        b &= assertEquals("unnamed pool", pool_nomemory.name(), "name") &
            assertEquals(0u, pool_nomemory.capacity(), "cap") &
            assertEquals(0u, pool_nomemory.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool_nomemory.count(), "count") &
            assertTrue(pool_nomemory.empty(), "empty") &
            assertTrue(nullptr == pool_nomemory.elems(), "elems");

        return b;
    }

    bool test_create_without_parameters()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(), ("create " + std::to_string(i)).c_str());
            b &= assertEquals(i + 1, pool.count(), ("create count " + std::to_string(i)).c_str());
        }
        b &= assertEquals(pool.capacity(), pool.count(), "last create count") &
            assertEquals(capacity, pool.capacity(), "cap 1") &
            assertEquals(capacity * sizeof(TestedPooledObject), pool.capacityBytes(), "cap 1 bytes") &
            assertFalse(pool.empty(), "empty 1");

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

    bool test_create_with_parameters()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(5u), ("create " + std::to_string(i)).c_str());
            b &= assertEquals(i + 1, pool.count(), ("create count " + std::to_string(i)).c_str());
        }
        b &= assertEquals(pool.capacity(), pool.count(), "last create count") &
            assertEquals(capacity, pool.capacity(), "cap 1") &
            assertEquals(capacity * sizeof(TestedPooledObject), pool.capacityBytes(), "cap 1 bytes") &
            assertFalse(pool.empty(), "empty 1");

        // elem data changed as specified as parameter for create(), and now they are "used"
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(5u, pool.elems()[i].getValue(), ("create value " + std::to_string(i)).c_str()) &
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

        constexpr size_t capacity = 10u;
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
        TestedPooledObject* const pooledByAnotherPool = pool_2.create();
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
            assertEquals(capacity, pool.capacity(), "cap 2") &
            assertTrue(pool.empty(), "empty 1");

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
        b &= assertTrue(pool.empty(), "empty 2");

        return b;
    }

    bool test_remove_func_of_object()
    {
        // testing pooled_object.remove(), should be same as test_remove_func_of_pool() but with different remove() being tested

        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
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
            assertEquals(capacity, pool.capacity(), "cap 2") &
            assertTrue(pool.empty(), "empty 1");

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
        b &= assertTrue(pool.empty(), "empty 2");

        return b;
    }

    bool test_create_and_remove_invoke_onsetused()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 1234567u /* magic number to make TestedPooledObject::onSetUsed() react */);
        bool b = true;

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            const auto* const ptr = pool.create();
            b &= assertNotNull(ptr, ("create " + std::to_string(i)).c_str());
            if (b)
            {
                b &= assertEquals(7654321u, ptr->getValue(), ("values 1, i: " + std::to_string(i)).c_str());
            }
        }

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            // remove() can be called in 2 ways: from the pool or from the pooled object itself, let's test both alternating
            if ((i % 2) == 0)
            {
                pool.remove(pool.elems()[i]);
            }
            else
            {
                pool.elems()[i].remove();
            }

            b &= assertEquals(1234567u, pool.elems()[i].getValue(), ("values 2, i: " + std::to_string(i)).c_str());
        }

        return b;
    }

    bool test_directly_exposed_iterators()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 0u);
        
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

        i = 0;
        for (const auto& obj : pool)
        {
            b &= assertEquals(i, obj.getValue(), ("value 2 " + std::to_string(i)).c_str()) &
                assertFalse(obj.used(), ("used 2 " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(obj.getParentPool()), ("parentpool 2 " + std::to_string(i)).c_str());
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
    //    constexpr size_t capacity = 10u;
    //    PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
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

    bool test_clear()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        // negative test: nothing bad happens if we return them now
        pool.clear();
        b &= assertEquals(0u, pool.count(), "fake clear 1") &
            assertTrue(pool.empty(), "empty 1");

        // now create them
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(), ("create " + std::to_string(i)).c_str());
        }
        b &= assertFalse(pool.empty(), "empty 2");

        // positive test
        pool.clear();
        b &= assertTrue(pool.empty(), "empty 3");

        // elem data unchanged but now they are "free" again
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(3u, pool.elems()[i].getValue(), ("clear value " + std::to_string(i)).c_str()) &
                assertFalse(pool.elems()[i].used(), ("clear used " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("clear parentpool " + std::to_string(i)).c_str());
        }

        // next() ptrs are now in reversed order
        for (size_t i = pool.capacity() - 1; b && (i > 0); i--)
        {
            b &= assertEquals(&(pool.elems()[i - 1]), pool.elems()[i].next(), ("clear next " + std::to_string(i)).c_str());
        }
        b &= assertTrue(nullptr == pool.elems()[0].next(), "last clear next");

        // negative test again: nothing bad happens if we return them now again
        pool.clear();
        b &= assertEquals(0u, pool.count(), "fake clear 2");
        b &= assertTrue(pool.empty(), "empty 3");

        return b;
    }

    bool test_reserve_empty()
    {
        constexpr size_t capacity = 0u;
        PgeObjectPool<TestedPooledObject> pool;
        bool b = true;

        b &= assertEquals("unnamed pool", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(capacity * sizeof(TestedPooledObject), pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty") &
            assertTrue(nullptr == pool.elems(), "elems");

        size_t i = 0;
        for (const auto& obj : pool)
        {
            static_cast<void>(obj);
            i++;
        }
        b &= assertEquals(0u, i);

        return b;
    }

    bool test_reserve_positive()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool;
        bool b = true;

        pool.reserve("ints", capacity, 3u);

        b &= assertEquals("ints", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(capacity * sizeof(TestedPooledObject), pool.capacityBytes(), "cap bytes") &
            assertEquals(0u, pool.count(), "count") &
            assertTrue(pool.empty(), "empty");

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

    bool test_reserve_negative()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        // there will be 1 used elem
        b &= assertNotNull(pool.create(5u), "create");

        try
        {
            pool.reserve("ints", capacity, 3u); // should throw ex
            b = false;
        }
        catch (const std::exception&)
        {
            // check if pool is intact

            b &= assertEquals("ints", pool.name(), "name") &
                assertEquals(capacity, pool.capacity(), "cap") &
                assertEquals(capacity * sizeof(TestedPooledObject), pool.capacityBytes(), "cap bytes") &
                assertEquals(1u, pool.count(), "count") &
                assertFalse(pool.empty(), "empty");

            for (size_t i = 0; b && (i < pool.capacity()); i++)
            {
                if (!pool.elems()[i].used())
                {
                    b &= assertEquals(3u, pool.elems()[i].getValue(), ("value " + std::to_string(i)).c_str());
                }
                else
                {
                    b &= assertEquals(5u, pool.elems()[i].getValue(), ("value " + std::to_string(i)).c_str());
                }
                b &= assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("parentpool " + std::to_string(i)).c_str());
            }

            for (size_t i = 0; b && (i < pool.capacity() - 1); i++)
            {
                if (!pool.elems()[i].used())
                {
                    b &= assertEquals(&(pool.elems()[i + 1]), pool.elems()[i].next(), ("next " + std::to_string(i)).c_str());
                }
            }
            b &= assertTrue(nullptr == pool.elems()[pool.capacity() - 1].next(), "last next");
        }

        return b;
    }

    bool test_erase_positive()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        // erasing the 1st element is also valid, it is not used, nothing will happen
        b &= assertTrue(pool.erase(pool.begin()) == (pool.begin()+1), "erase begin");

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertNotNull(pool.create(), ("create " + std::to_string(i)).c_str());
        }
        b &= assertEquals(pool.capacity(), pool.count(), "last create count");

        auto it = pool.begin();
        while (it != pool.end())
        {
            it = pool.erase(it);
        }
        b &= assertTrue(pool.empty(), "empty") &
            assertTrue(pool.end() == it, "it end");

        return b;
    }

    bool test_erase_negative()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 3u);
        bool b = true;

        b &= assertTrue(pool.erase(pool.end()) == pool.end(), "erase end");

        // create 1 pooledobj just to make the pool non-empty
        const TestedPooledObject* const pooled = pool.create();
        b &= assertNotNull(pooled, "create 1");

        // create another pool with 1 pooledobj
        PgeObjectPool<TestedPooledObject> pool_2("different ints", 5, 5);
        const TestedPooledObject* const pooledByAnotherPool = pool_2.create();
        b &= assertNotNull(pooledByAnotherPool, "create 2");
        b &= assertTrue(pool_2.begin().getConstPtr() == pooledByAnotherPool, "pool_2 begin");

        if (b)
        {
            // try erasing pool_2's pooledobj from pool 1, expect it to fail and no changes to any pool
            try
            {
                b &= assertTrue(pool.erase(pool_2.begin()) == pool.end(), "erase another pool's object"); // should throw
                b = false;
            }
            catch (const std::exception&)
            {
                b &= assertEquals(1u, pool.count(), "pool elem count unchanged") &
                    assertEquals(1u, pool_2.count(), "pool_2 elem count unchanged") &
                    assertTrue(pool.begin()->used(), "pool elem still used") &
                    assertTrue(pool_2.begin()->used(), "pool_2 elem still used") &
                    assertTrue(pool.begin().getConstPtr() == pooled, "pool begin") &
                    assertTrue(pool_2.begin().getConstPtr() == pooledByAnotherPool, "pool_2 begin 2");
            }
        }

        return b;
    }

    bool test_erase_and_clear_invoke_onsetused()
    {
        constexpr size_t capacity = 10u;
        PgeObjectPool<TestedPooledObject> pool("ints", capacity, 1234567u /* magic number to make TestedPooledObject::onSetUsed() react */);
        bool b = true;

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            const auto* const ptr = pool.create();
            b &= assertNotNull(ptr, ("create " + std::to_string(i)).c_str());
            if (b)
            {
                b &= assertEquals(7654321u, ptr->getValue(), ("values 1, i: " + std::to_string(i)).c_str());
            }
        }

        // test erase() triggering TestedPooledObject::onSetUsed()
        pool.erase(pool.begin());
        b &= assertEquals(1234567u, pool.begin()->getValue(), "value 2");

        // test clear() triggering TestedPooledObject::onSetUsed()
        pool.clear();

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(1234567u, pool.elems()[i].getValue(), ("values 3, i: " + std::to_string(i)).c_str());
        }

        return b;
    }

};