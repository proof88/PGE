#pragma once

/*
    ###################################################################################
    PFLFixFIFOTest.h
    Unit test for PR00F Foundation Library.
    Please see UnitTest.h about my statement of using "bitwise and" operator with bool operands.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../../../PFL/PFL/FixFIFO.h"

class PFLFixFIFOTest :
    public UnitTest
{
public:

    PFLFixFIFOTest() : UnitTest(__FILE__, "")
    {
    }

    ~PFLFixFIFOTest() = default;

    PFLFixFIFOTest(const PFLFixFIFOTest&) = delete;
    PFLFixFIFOTest& operator=(const PFLFixFIFOTest&) = delete;
    PFLFixFIFOTest(PFLFixFIFOTest&&) = delete;
    PFLFixFIFOTest& operator=(PFLFixFIFOTest&&) = delete;

protected:

    virtual void initialize() override
    {
        addSubTest("test_ctor_zero_cap", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_ctor_zero_cap));
        addSubTest("test_ctor", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_ctor));
        addSubTest("test_push_back", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_push_back));
        addSubTest("test_front_empty", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_front_empty));
        addSubTest("test_pop_front_empty", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_pop_front_empty));
        addSubTest("test_pop_front_noreturn_empty", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_pop_front_noreturn_empty));
        addSubTest("test_front_and_pop_front", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_front_and_pop_front));
        addSubTest("test_front_and_pop_front_noreturn", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_front_and_pop_front_noreturn));
        addSubTest("test_clear", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_clear));
        addSubTest("test_begin_index_next_index", static_cast<PFNUNITSUBTEST>(&PFLFixFIFOTest::test_begin_index_next_index));
    }

private:

    bool assertFifoElemOrderSameAsVectorElemOrder(const pfl::FixFIFO<int>& fifo, const std::vector<int>& vec, const char* szText = "")
    {
        bool b = true;
        try
        {
            b &= assertEquals(vec.size(), fifo.size(), ("fifo size different than vec size, " + std::string(szText) + "!").c_str());
            if (b)
            {
                size_t fifoIndex = fifo.begin_index();
                for (size_t i = 0; i < vec.size(); i++)
                {
                    b &= assertEquals(
                        vec[i],
                        fifo.underlying_array()[fifoIndex],
                        ("vec vs fifo elem mismatch: index = " + std::to_string(i) + ", " +
                        toString(vec[i]) + " != " + toString(fifo.underlying_array()[fifoIndex]) +
                        ", " + std::string(szText)).c_str());
                    fifoIndex = fifo.next_index(fifoIndex);
                }
            }
        }
        catch (const std::exception&)
        {
            b = assertFalse(true, ("Exception thrown in assertFifoElemOrderSameAsVectorElemOrder(), " + std::string(szText) + "!").c_str());
        }
        return b;
    }

    bool test_ctor_zero_cap()
    {
        static constexpr size_t capacity = 0;

        bool b = true;
        try
        {
            pfl::FixFIFO<int> fifo(capacity);
            b = false;
        }
        catch (const std::exception& e)
        {
            b &= assertEquals(
                std::string("Capacity must be positive!"),
                std::string(e.what()),
                "ex what");
        }

        return b;
    }

    bool test_ctor()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        return (assertEquals(capacity, fifo.capacity(), "cap") &
            assertEquals(0u, fifo.size(), "size") &
            assertTrue(fifo.empty(), "empty") &
            assertFalse(fifo.full(), "full") &
            assertEquals(0u, fifo.begin_index(), "begin_index") &
            assertEquals(0u, fifo.end_index(), "end_index") &
            assertEquals(capacity - 1, fifo.rbegin_index(), "rbegin_index") &
            assertEquals(1u, fifo.next_index(0u), "next_index") &
            assertEquals(capacity - 1, fifo.prev_index(0u), "prev_index")) != 0;
    }

    bool test_push_back()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;
        b &= assertTrue(fifo.push_back(5), "push 5");
        b &= assertEquals(5, fifo.front(), "front 5") &
            assertFalse(fifo.empty(), "empty 5") &
            assertFalse(fifo.full(), "full 5") &
            assertEquals(1u, fifo.size(), "size 5") &
            assertEquals(0u, fifo.begin_index(), "begin_index 5") &
            assertEquals(1u, fifo.end_index(), "end_index 5") &
            assertEquals(0u, fifo.rbegin_index(), "rbegin_index 5");

        b &= assertTrue(fifo.push_back(3), "push 3");
        b &= assertEquals(5, fifo.front(), "front 3") &
            assertFalse(fifo.empty(), "empty 3") &
            assertFalse(fifo.full(), "full 3") &
            assertEquals(2u, fifo.size(), "size 3") &
            assertEquals(0u, fifo.begin_index(), "begin_index 3") &
            assertEquals(2u, fifo.end_index(), "end_index 3") &
            assertEquals(1u, fifo.rbegin_index(), "rbegin_index 3");

        b &= assertTrue(fifo.push_back(1), "push 1");
        b &= assertEquals(5, fifo.front(), "front 1") &
            assertFalse(fifo.empty(), "empty 1") &
            assertTrue(fifo.full(), "full 1") &
            assertEquals(3u, fifo.size(), "size 1") &
            assertEquals(0u, fifo.begin_index(), "begin_index 1") &
            assertEquals(0u, fifo.end_index(), "end_index 1") &
            assertEquals(2u, fifo.rbegin_index(), "rbegin_index 1");

        b &= assertFifoElemOrderSameAsVectorElemOrder(fifo, { 5, 3, 1 }, "check 1");

        // this last won't fit in
        b &= assertFalse(fifo.push_back(0), "push 0");
        // nothing changed
        b &= assertEquals(5, fifo.front(), "front 0") &
            assertFalse(fifo.empty(), "empty 0") &
            assertTrue(fifo.full(), "full 0") &
            assertEquals(3u, fifo.size(), "size 0") &
            assertEquals(0u, fifo.begin_index(), "begin_index 0") &
            assertEquals(0u, fifo.end_index(), "end_index 0") &
            assertEquals(2u, fifo.rbegin_index(), "rbegin_index 0");
        b &= assertFifoElemOrderSameAsVectorElemOrder(fifo, { 5, 3, 1 }, "check 2");

        // now try again with force
        fifo.push_back_forced(0);
        b &= assertEquals(3, fifo.front(), "front 0 forced") &
            assertFalse(fifo.empty(), "empty 0 forced") &
            assertTrue(fifo.full(), "full 0 forced") &
            assertEquals(3u, fifo.size(), "size 0 forced") &
            assertEquals(1u, fifo.begin_index(), "begin_index 0 forced") &
            assertEquals(1u, fifo.end_index(), "end_index 0 forced") &
            assertEquals(0u, fifo.rbegin_index(), "rbegin_index 0 forced");
        b &= assertFifoElemOrderSameAsVectorElemOrder(fifo, { 3, 1, 0 }, "check 3");

        // these are just left here to always remember
        b &= assertEquals(0, 0 % 3, "0 % 3");
        b &= assertEquals(1, 1 % 3, "1 % 3");
        b &= assertEquals(2, 2 % 3, "2 % 3");
        b &= assertEquals(0, 3 % 3, "3 % 3");
        b &= assertEquals(1, 4 % 3, "4 % 3");

        return b;
    }

    bool test_front_empty()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;

        try
        {
            fifo.front();
            b = assertFalse(true, "Exception needed!");
        }
        catch (const std::exception& e)
        {
            b &= assertEquals(
                std::string("Container is empty!"),
                std::string(e.what()),
                "ex what");
        }

        return b;
    }

    bool test_pop_front_empty()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;

        try
        {
            fifo.pop_front();
            b = assertFalse(true, "Exception needed!");
        }
        catch (const std::exception& e)
        {
            b &= assertEquals(
                std::string("Container is empty!"),
                std::string(e.what()),
                "ex what");
        }

        return b;
    }

    bool test_pop_front_noreturn_empty()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;

        try
        {
            fifo.pop_front_noreturn();
            b = assertFalse(true, "Exception needed!");
        }
        catch (const std::exception& e)
        {
            b &= assertEquals(
                std::string("Container is empty!"),
                std::string(e.what()),
                "ex what");
        }

        return b;
    }

    bool test_front_and_pop_front()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;
        b &= assertTrue(fifo.push_back(5), "push 5");
        b &= assertTrue(fifo.push_back(3), "push 3");
        b &= assertTrue(fifo.push_back(1), "push 1");

        try
        {
            const std::vector<int> elemsToBePopped{ 1, 3, 5 };
            size_t vecIndex = elemsToBePopped.size();
            while (!fifo.empty() && (vecIndex > 0))
            {
                b &= assertEquals(vecIndex, fifo.size(), "size");
                vecIndex--;
                const auto expectedElem = elemsToBePopped[vecIndex];
                b &= assertEquals(expectedElem, fifo.front(), "front");
                b &= assertEquals(expectedElem, fifo.pop_front(), "pop front");
            }

            b &= assertTrue(fifo.empty(), "empty final") &
                assertEquals(0u, fifo.size(), "size final");
        }
        catch (const std::exception&)
        {
            b = assertFalse(true, "Exception thrown!");
        }

        return b;
    }

    bool test_front_and_pop_front_noreturn()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;
        b &= assertTrue(fifo.push_back(5), "push 5");
        b &= assertTrue(fifo.push_back(3), "push 3");
        b &= assertTrue(fifo.push_back(1), "push 1");

        try
        {
            const std::vector<int> elemsToBePopped{ 1, 3, 5 };
            size_t vecIndex = elemsToBePopped.size();
            while (!fifo.empty() && (vecIndex > 0))
            {
                b &= assertEquals(vecIndex, fifo.size(), "size");
                vecIndex--;
                const auto expectedElem = elemsToBePopped[vecIndex];
                b &= assertEquals(expectedElem, fifo.front(), "front");
                fifo.pop_front_noreturn();
            }

            b &= assertTrue(fifo.empty(), "empty final") &
                assertEquals(0u, fifo.size(), "size final");
        }
        catch (const std::exception&)
        {
            b = assertFalse(true, "Exception thrown!");
        }

        return b;
    }

    bool test_clear()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        fifo.clear(); // just see nothing bad happens when invoked for empty fifo

        bool b = true;
        b &= assertTrue(fifo.push_back(5), "push 5");
        b &= assertTrue(fifo.push_back(3), "push 3");
        b &= assertTrue(fifo.push_back(1), "push 1");

        fifo.clear();

        b &= assertEquals(capacity, fifo.capacity(), "cap") &
            assertEquals(0u, fifo.size(), "size") &
            assertTrue(fifo.empty(), "empty") &
            assertFalse(fifo.full(), "full") &
            assertEquals(0u, fifo.begin_index(), "begin_index") &
            assertEquals(0u, fifo.end_index(), "end_index") &
            assertEquals(capacity - 1, fifo.rbegin_index(), "rbegin_index") &
            assertEquals(1u, fifo.next_index(0u), "next_index") &
            assertEquals(capacity - 1, fifo.prev_index(0u), "prev_index");

        try
        {
            fifo.front();
            b = assertFalse(true, "Exception needed!");
        }
        catch (const std::exception& e)
        {
            b &= assertEquals(
                std::string("Container is empty!"),
                std::string(e.what()),
                "ex what");
        }

        return b;
    }

    bool test_begin_index_next_index()
    {
        static constexpr size_t capacity = 3;
        pfl::FixFIFO<int> fifo(capacity);

        bool b = true;
        b &= assertTrue(fifo.push_back(5), "push 5 1");
        b &= assertTrue(fifo.push_back(3), "push 3 1");
        b &= assertTrue(fifo.push_back(1), "push 1 1");

        b &= assertEquals(0u, fifo.begin_index(), "begin_index") &
            assertEquals(0u, fifo.end_index(), "end_index") &
            assertEquals(capacity - 1, fifo.rbegin_index(), "rbegin_index");

        if (!b)
        {
            return false;
        }

        // ######################################## Queue: 5 (front), 3, 1 (back)

        auto iFwd = fifo.begin_index();
        auto iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(5, fifo.underlying_array()[iFwd], "iFwd 1");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 1");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(3, fifo.underlying_array()[iFwd], "iFwd 2");
        b &= assertEquals(3, fifo.underlying_array()[iRwd], "iRwd 2");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 3");
        b &= assertEquals(5, fifo.underlying_array()[iRwd], "iRwd 3");
        
        fifo.pop_front();  // ######################################## Queue: 3 (front), 1 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(3, fifo.underlying_array()[iFwd], "iFwd 4");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 4");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 5");
        b &= assertEquals(3, fifo.underlying_array()[iRwd], "iRwd 5");

        fifo.pop_front();  // ######################################## Queue: 1

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 6");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 6");

        b &= assertTrue(fifo.push_back(7), "push 7 1");
        b &= assertTrue(fifo.push_back(9), "push 9 1");

        // ######################################## Queue: 1 (front), 7, 9 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 7");
        b &= assertEquals(9, fifo.underlying_array()[iRwd], "iRwd 7");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(7, fifo.underlying_array()[iFwd], "iFwd 8");
        b &= assertEquals(7, fifo.underlying_array()[iRwd], "iRwd 8");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(9, fifo.underlying_array()[iFwd], "iFwd 9");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 9");

        fifo.pop_front();  // ######################################## Queue: 7 (front), 9 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(7, fifo.underlying_array()[iFwd], "iFwd 10");
        b &= assertEquals(9, fifo.underlying_array()[iRwd], "iRwd 10");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(9, fifo.underlying_array()[iFwd], "iFwd 11");
        b &= assertEquals(7, fifo.underlying_array()[iRwd], "iRwd 11");

        fifo.pop_front();  // ######################################## Queue: 9

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(9, fifo.underlying_array()[iFwd], "iFwd 12");
        b &= assertEquals(9, fifo.underlying_array()[iRwd], "iRwd 12");

        fifo.pop_front();  // ######################################## Queue: empty

        b &= assertTrue(fifo.push_back(5), "push 5 2");
        b &= assertTrue(fifo.push_back(3), "push 3 2");
        b &= assertTrue(fifo.push_back(1), "push 1 2");

        // ######################################## Queue: 5 (front), 3, 1 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(5, fifo.underlying_array()[iFwd], "iFwd 13");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 13");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(3, fifo.underlying_array()[iFwd], "iFwd 14");
        b &= assertEquals(3, fifo.underlying_array()[iRwd], "iRwd 14");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 15");
        b &= assertEquals(5, fifo.underlying_array()[iRwd], "iRwd 15");

        fifo.pop_front();  // ######################################## Queue: 3 (front), 1 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(3, fifo.underlying_array()[iFwd], "iFwd 16");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 16");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 17");
        b &= assertEquals(3, fifo.underlying_array()[iRwd], "iRwd 17");

        b &= assertTrue(fifo.push_back(7), "push 7 2");  // ######################################## Queue: 3 (front), 1, 7 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(3, fifo.underlying_array()[iFwd], "iFwd 18");
        b &= assertEquals(7, fifo.underlying_array()[iRwd], "iRwd 18");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 19");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 19");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(7, fifo.underlying_array()[iFwd], "iFwd 20");
        b &= assertEquals(3, fifo.underlying_array()[iRwd], "iRwd 20");

        fifo.pop_front();  // ######################################## Queue: 1 (front), 7 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 21");
        b &= assertEquals(7, fifo.underlying_array()[iRwd], "iRwd 21");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(7, fifo.underlying_array()[iFwd], "iFwd 22");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 22");

        b &= assertTrue(fifo.push_back(9), "push 9 2");  // ######################################## Queue: 1 (front), 7, 9 (back)

        iFwd = fifo.begin_index();
        iRwd = fifo.rbegin_index();
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(1, fifo.underlying_array()[iFwd], "iFwd 23");
        b &= assertEquals(9, fifo.underlying_array()[iRwd], "iRwd 23");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(7, fifo.underlying_array()[iFwd], "iFwd 24");
        b &= assertEquals(7, fifo.underlying_array()[iRwd], "iRwd 24");

        iFwd = fifo.next_index(iFwd);
        iRwd = fifo.prev_index(iRwd);
        assert(iFwd < capacity);
        assert(iRwd < capacity);
        b &= assertEquals(9, fifo.underlying_array()[iFwd], "iFwd 25");
        b &= assertEquals(1, fifo.underlying_array()[iRwd], "iRwd 25");

        return b;
    }

}; // class PFLFixFIFOTest
