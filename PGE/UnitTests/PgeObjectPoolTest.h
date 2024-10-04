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
        TestedPooledObject(PgeObjectPoolBase& parentPool, const float& n) : PgePooledObject(parentPool), m_n(n)
        {
        }

        ~TestedPooledObject() = default;

        TestedPooledObject(const TestedPooledObject&) = default;
        TestedPooledObject& operator=(const TestedPooledObject&) = default;
        TestedPooledObject(TestedPooledObject&&) = default;
        TestedPooledObject& operator=(TestedPooledObject&&) = default;

        const float& getValue() const
        {
            return m_n;
        }

    private:
        float m_n;
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
        AddSubTest("test_create_and_remove", (PFNUNITSUBTEST)&PgeObjectPoolTest::test_create_and_remove);
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
        PgeObjectPool<TestedPooledObject> pool("floats", capacity, 5.6f);
        bool b = true;

        b &= assertEquals("floats", pool.name(), "name") &
            assertEquals(capacity, pool.capacity(), "cap") &
            assertEquals(0u, pool.count(), "count");

        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            b &= assertEquals(5.6f, pool.elems()[i].getValue(), ("value " + std::to_string(i)).c_str()) &
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

    bool test_create_and_remove()
    {
        constexpr size_t capacity = 10;
        PgeObjectPool<TestedPooledObject> pool("floats", capacity, 5.6f);
        bool b = true;

        // create() test
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
            b &= assertEquals(5.6f, pool.elems()[i].getValue(), ("create value " + std::to_string(i)).c_str()) &
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

        // remove() test
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
            b &= assertEquals(5.6f, pool.elems()[i].getValue(), ("remove value " + std::to_string(i)).c_str()) &
                assertFalse(pool.elems()[i].used(), ("remove used " + std::to_string(i)).c_str()) &
                assertEquals(&pool, &(pool.elems()[i].getParentPool()), ("remove parentpool " + std::to_string(i)).c_str());
        }

        // next() ptrs are now in reversed order
        for (size_t i = pool.capacity() - 1; b && (i > 0); i--)
        {
            b &= assertEquals(&(pool.elems()[i-1]), pool.elems()[i].next(), ("remove next " + std::to_string(i)).c_str());
        }
        b &= assertTrue(nullptr == pool.elems()[0].next(), "last remove next");

        // nothing bad happens if we try to free any of the already freed
        for (size_t i = 0; b && (i < pool.capacity()); i++)
        {
            pool.remove(pool.elems()[i]);
            b &= assertEquals(0u, pool.count(), (" fake remove count " + std::to_string(i)).c_str());
        }

        return b;
    }

};