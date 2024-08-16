#pragma once

/*
    ###################################################################################
    PFLTest.h
    Unit test for PR00F Foundation Library.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../../../PFL/PFL/PFL.h"
#include <set>


#ifndef E
#define E 0.0001f
#endif // EPSILON

class PFLTest :
    public UnitTest
{
public:

    PFLTest() : UnitTest(__FILE__, "")
    {
    }

    ~PFLTest()
    {
        Finalize();
    } // ~PFLTest()

protected:

    virtual void Initialize() override
    {
        AddSubTest("testGetTimeDiffInUs", (PFNUNITSUBTEST) &PFLTest::testGetTimeDiffInUs);    
        AddSubTest("testUpdateForMinDuration_1", (PFNUNITSUBTEST) &PFLTest::testUpdateForMinDuration_1);
        AddSubTest("testUpdateForMinDuration_2", (PFNUNITSUBTEST) &PFLTest::testUpdateForMinDuration_2);    
        AddSubTest("testUpdateForMaxDuration_1", (PFNUNITSUBTEST) &PFLTest::testUpdateForMaxDuration_1);
        AddSubTest("testUpdateForMaxDuration_2", (PFNUNITSUBTEST) &PFLTest::testUpdateForMaxDuration_2);    
        AddSubTest("testGetExtension", (PFNUNITSUBTEST) &PFLTest::testGetExtension);
        AddSubTest("testGetDirectory", (PFNUNITSUBTEST) &PFLTest::testGetDirectory);
        AddSubTest("testGetFilename", (PFNUNITSUBTEST) &PFLTest::testGetFilename);
        AddSubTest("testChangeExtension", (PFNUNITSUBTEST) &PFLTest::testChangeExtension);
        AddSubTest("testNumCharAppears", (PFNUNITSUBTEST) &PFLTest::testNumCharAppears);
        AddSubTest("testStrClrLeads", (PFNUNITSUBTEST) &PFLTest::testStrClrLeads);
        AddSubTest("testStrClrTrails", (PFNUNITSUBTEST) &PFLTest::testStrClrTrails);
        AddSubTest("testStrClr", (PFNUNITSUBTEST) &PFLTest::testStrClr);
        AddSubTest("testPi", (PFNUNITSUBTEST) &PFLTest::testPi);
        AddSubTest("testRoundf", (PFNUNITSUBTEST) &PFLTest::testRoundf);
        AddSubTest("testRoundi", (PFNUNITSUBTEST) &PFLTest::testRoundi);
        AddSubTest("testConstrain", (PFNUNITSUBTEST) &PFLTest::testConstrain);
        AddSubTest("testDegToRad", (PFNUNITSUBTEST) &PFLTest::testDegToRad);
        AddSubTest("testRadToDeg", (PFNUNITSUBTEST) &PFLTest::testRadToDeg);
        AddSubTest("testRandom1", (PFNUNITSUBTEST) &PFLTest::testRandom1);
        AddSubTest("testRandom2", (PFNUNITSUBTEST)&PFLTest::testRandom2);
        AddSubTest("testLerp", (PFNUNITSUBTEST) &PFLTest::testLerp);
        AddSubTest("testSmooth", (PFNUNITSUBTEST)&PFLTest::testSmooth);
    }

    virtual bool setUp() override
    {
        return true;    
    }

    virtual void TearDown() override
    {
        Finalize();
    }

    virtual void Finalize() override
    {
    }



private:

    // ---------------------------------------------------------------------------

    PFLTest(const PFLTest&)
    {};         

    PFLTest& operator=(const PFLTest&)
    {
        return *this;
    };

    bool testGetTimeDiffInUs()
    {
        PFL::timeval time1;
        PFL::timeval time2;

        time1.tv_sec = time1.tv_usec = 0;
        time2.tv_sec = time2.tv_usec = 0;

        bool l = assertEquals(0, PFL::getTimeDiffInUs(time2, time1), "1");

        time2.tv_sec = 2;
        l &= assertEquals(2000000, PFL::getTimeDiffInUs(time2, time1), "2");

        time1.tv_usec = 500000;
        l &= assertEquals(1500000, PFL::getTimeDiffInUs(time2, time1), "3");

        return l;
    }

    bool testUpdateForMinDuration_1()
    {
        PFL::timeval time1;
        time1.tv_sec = 10;
        time1.tv_usec = 0;

        bool l = assertFalse(PFL::updateForMinDuration(time1, 20000000), "1") &
            assertEquals(10, time1.tv_sec, "sec 1") &
            assertEquals(0, time1.tv_usec, "usec 1");
        
        l &= assertTrue(PFL::updateForMinDuration(time1, 6500000), "2") &
            assertEquals(6, time1.tv_sec, "sec 2") &
            assertEquals(500000, time1.tv_usec, "usec 2");

        l &= assertTrue(PFL::updateForMinDuration(time1, 400000), "3") &
            assertEquals(0, time1.tv_sec, "sec 3") &
            assertEquals(400000, time1.tv_usec, "usec 3");

        return l;
    }

    bool testUpdateForMinDuration_2()
    {
        PFL::timeval time;
        PFL::timeval timeStart, timeEnd;

        time.tv_sec = 999;
        time.tv_usec = 999999;

        timeStart.tv_sec = 10;
        timeStart.tv_usec = 0;
        timeEnd.tv_sec = 20;
        timeEnd.tv_usec = 500000;

        bool l = assertTrue(PFL::updateForMinDuration(time, timeStart, timeEnd), "1") &
            assertEquals(10, time.tv_sec, "sec 1") &
            assertEquals(500000, time.tv_usec, "usec 1");

        timeStart.tv_sec = 10;
        timeStart.tv_usec = 700000;

        l = assertTrue(PFL::updateForMinDuration(time, timeStart, timeEnd), "2") &
            assertEquals(9, time.tv_sec, "sec 2") &
            assertEquals(800000, time.tv_usec, "usec 2");

        timeStart.tv_sec = 0;
        timeStart.tv_usec = 0;

        l = assertFalse(PFL::updateForMinDuration(time, timeStart, timeEnd), "3") &
            assertEquals(9, time.tv_sec, "sec 3") &
            assertEquals(800000, time.tv_usec, "usec 3");

        return l;
    }

    bool testUpdateForMaxDuration_1()
    {
        PFL::timeval time1;
        time1.tv_sec = 10;
        time1.tv_usec = 0;

        bool l = assertFalse(PFL::updateForMaxDuration(time1, 5000000), "1") &
            assertEquals(10, time1.tv_sec, "sec 1") &
            assertEquals(0, time1.tv_usec, "usec 1");
        
        l &= assertTrue(PFL::updateForMaxDuration(time1, 25300000), "2") &
            assertEquals(25, time1.tv_sec, "sec 2") &
            assertEquals(300000, time1.tv_usec, "usec 2");

        return l;
    }

    bool testUpdateForMaxDuration_2()
    {
        PFL::timeval time;
        PFL::timeval timeStart, timeEnd;

        time.tv_sec = 0;
        time.tv_usec = 0;

        timeStart.tv_sec = 10;
        timeStart.tv_usec = 0;
        timeEnd.tv_sec = 20;
        timeEnd.tv_usec = 500000;

        bool l = assertTrue(PFL::updateForMaxDuration(time, timeStart, timeEnd), "1") &
            assertEquals(10, time.tv_sec, "sec 1") &
            assertEquals(500000, time.tv_usec, "usec 1");

        timeEnd.tv_sec = 30;
        timeEnd.tv_usec = 700000;

        l = assertTrue(PFL::updateForMaxDuration(time, timeStart, timeEnd), "2") &
            assertEquals(20, time.tv_sec, "sec 2") &
            assertEquals(700000, time.tv_usec, "usec 2");

        timeStart.tv_sec = 20;
        timeStart.tv_usec = 0;

        l = assertFalse(PFL::updateForMaxDuration(time, timeStart, timeEnd), "3") &
            assertEquals(20, time.tv_sec, "sec 3") &
            assertEquals(700000, time.tv_usec, "usec 3");

        return l;
    }

    bool testFileExists()
    {
        return assertTrue(PFL::fileExists("C:\\Windows\\Notepad.exe"), "notepad") &
            assertFalse(PFL::fileExists("C:\\92569234523486"), "fake");
    }

    bool testGetExtension()
    {
        return assertEquals("exe", PFL::getExtension("C:\\Windows\\Notepad.exe"), "notepad") &
            assertEquals("", PFL::getExtension("alma"), "alma") &
            assertEquals("", PFL::getExtension("alma."), "alma dot") &
            assertEquals("", PFL::getExtension("alma.."), "alma dot dot") &
            assertEquals("tga", PFL::getExtension("macskajancsi.exe.pdf.tga"), "tga");
    }

    bool testGetDirectory()
    {
        return assertEquals("C:\\Windows\\", PFL::getDirectory("    C:\\Windows\\Notepad.exe    "), "notepad") &
            assertEquals("", PFL::getDirectory("alma"), "alma") &
            assertEquals("alma\\", PFL::getDirectory("alma\\"), "alma 2") &
            assertEquals("", PFL::getDirectory("\\"), "backslash") &
            assertEquals("", PFL::getDirectory("\\\\\\"), "backslashslashslash") &
            assertEquals("C:/Windows/", PFL::getDirectory("    C:/Windows/Notepad.exe    "), "notepad 2") &
            assertEquals("alma/", PFL::getDirectory("alma/"), "alma 3") &
            assertEquals("", PFL::getDirectory("/"), "slash") &
            assertEquals("", PFL::getDirectory("///"), "slashslashslash");
    }

    bool testGetFilename()
    {
        return assertEquals("Notepad.exe", PFL::getFilename("C:\\Windows\\Notepad.exe"), "notepad") &
            assertEquals("Notepad.exe", PFL::getFilename("C:/Windows/Notepad.exe"), "notepad 2") &
            assertEquals("alma", PFL::getFilename("alma"), "alma") &
            assertEquals("alma", PFL::getFilename("alma."), "alma dot") &
            assertEquals("alma", PFL::getFilename("alma.."), "alma dot dot") &
            assertEquals("alma..txt", PFL::getFilename("alma..txt"), "alma dot dot txt") &
            assertEquals("macskajancsi.exe.pdf.tga", PFL::getFilename("macskajancsi.exe.pdf.tga"), "tga") &
            assertEquals("", PFL::getFilename(""), "empty");
    }

    bool testChangeExtension()
    {
        return assertEquals("Notepad.jpg", PFL::changeExtension("Notepad.exe", "jpg"), "notepad") &
            assertEquals("Notepad.jpg", PFL::changeExtension("Notepad", "jpg"), "notepad no ext") &
            assertEquals("C:\\Windows\\Notepad.tga", PFL::changeExtension("C:\\Windows\\Notepad.exe", "tga"), "notepad fullpath") &
            assertEquals("C:/Windows/Notepad.tga", PFL::changeExtension("C:/Windows/Notepad.exe", "tga"), "notepad fullpath 2") &
            assertEquals("", PFL::changeExtension("", "jpg"), "empty") &
            assertEquals("Notepad", PFL::changeExtension("Notepad.exe", ""), "remove ext 1") &
            assertEquals("Notepad", PFL::changeExtension("Notepad", ""), "remove ext 2");
    }

    bool testNumCharAppears()
    {
        return assertEquals((unsigned) 0, PFL::numCharAppears('g', "", 0), "empty") &
            assertEquals((unsigned) 1, PFL::numCharAppears('g', "g", 1), "1") &
            assertEquals((unsigned) 4, PFL::numCharAppears('g', "iasjoigmnoasn ongoingoasn go", 28), "4") &
            assertEquals((unsigned) 2, PFL::numCharAppears('g', "ggg", 2), "size");
    }

    bool testStrClrTrails()
    {
        char str1[] = "alma";
        char str2[] = "   alma alma  \t \t ";
        char str3[] = "   ";
        char str4[] = "  macska   \t";

        return assertEquals(strlen(str1), PFL::strClrTrails(str1), "len1") &
               assertEquals(strlen(str2), PFL::strClrTrails(str2), "len2") &
               assertEquals(strlen(str3), PFL::strClrTrails(str3), "len3") &  
               assertEquals(strlen(str4), PFL::strClrTrails(str4, 'x'), "len4") &
               assertEquals(std::string("alma"), std::string(str1), "simple alma") &
               assertEquals(std::string("   alma alma"), std::string(str2), "long alma") &
               assertEquals(std::string(""), std::string(str3), "spaces only") &
               assertEquals(std::string("  macska   "), std::string(str4), "macska");
    }

    bool testStrClrLeads()
    {
        char str1[] = "alma";
        char str2[] = "   alma alma  \t \t ";
        char str3[] = "   ";
        char str4[] = "\t  macska   \t";
        
        return assertEquals(strlen(str1), PFL::strClrLeads(str1), "len1") &
               assertEquals(strlen(str2), PFL::strClrLeads(str2), "len2") &
               assertEquals(strlen(str3), PFL::strClrLeads(str3), "len3") & 
               assertEquals(strlen(str4), PFL::strClrLeads(str4, 'x'), "len4") &
               assertEquals(std::string("alma"), std::string(str1), "simple alma") &
               assertEquals(std::string("alma alma  \t \t "), std::string(str2), "long alma") &
               assertEquals(std::string(""), std::string(str3), "spaces only") &
               assertEquals(std::string("  macska   \t"), std::string(str4), "macska");
    }

    bool testStrClr()
    {
        char str1[] = "alma";
        char str2[] = "   alma alma  \t \t ";
        char str3[] = "   ";
        char str4[] = "\t\t  macska   \t";
        
        return assertEquals(strlen(str1), PFL::strClr(str1), "len1") &
               assertEquals(strlen(str2), PFL::strClr(str2), "len2") &
               assertEquals(strlen(str3), PFL::strClr(str3), "len3") &
               assertEquals(strlen(str4), PFL::strClr(str4, 'x'), "len4") &
               assertEquals(std::string("alma"), std::string(str1), "simple alma") &
               assertEquals(std::string("alma alma"), std::string(str2), "long alma") &
               assertEquals(std::string(""), std::string(str3), "spaces only") &
               assertEquals(std::string("  macska   "), std::string(str4), "macska");
    } 

    bool testPi()
    {
        return assertBetween(3.14f, 3.15f, PFL::pi(), "pi()") &
            assertEquals(PFL::PI, PFL::pi(), 0.001f, "PFL_PI");
    }

    bool testRoundf()
    {
        return assertEquals(4.0f, PFL::roundf(4.15f), 0.001f, "4.15f") &
            assertEquals(4.0f, PFL::roundf(4.49f), 0.001f, "4.49f") &
            assertEquals(4.0f, PFL::roundf(3.5f), 0.001f, "3.5f") &
            assertEquals(-4.0f, PFL::roundf(-4.49f), 0.001f, "-4.49f") &
            assertEquals(-4.0f, PFL::roundf(-3.5f), 0.001f, "-3.5f");
    }

    bool testRoundi()
    {
        return assertEquals(4, PFL::roundi(4.15f), "4.15f") &
            assertEquals(4, PFL::roundi(4.49f), "4.49f") &
            assertEquals(4, PFL::roundi(3.5f), "3.5f") &
            assertEquals(-4, PFL::roundi(-4.49f), "-4.49f") &
            assertEquals(-4, PFL::roundi(-3.5f), "-3.5f");
    }

    bool testConstrain()
    {
        return assertEquals(3, PFL::constrain(3, 2, 4), "3, 2, 4") &
            assertEquals(2, PFL::constrain(1, 2, 4), "1, 2, 4") &
            assertEquals(4, PFL::constrain(5, 2, 4), "5, 2, 4");
    }

    bool testDegToRad()
    {
        return assertEquals(0.0f, PFL::degToRad(0.0f), 0.001f, "0.0f") &
            assertEquals(PFL::pi()/180.0f, PFL::degToRad(1.0f), 0.001f, "0.0f") &
            assertEquals(PFL::pi(), PFL::degToRad(180.0f), 0.001f, "180.0f");
    }

    bool testRadToDeg()
    {
        return assertEquals(0.0f, PFL::radToDeg(0.0f), 0.001f, "pi/180") &
            assertEquals(1.0f, PFL::radToDeg(PFL::pi()/180.0f), 0.001f, "pi/180") &
            assertEquals(180.0f, PFL::radToDeg(PFL::pi()), 0.001f, "pi");
    }   

    bool testRandom1()
    {
        std::set<int> stdset;
        for (int i = 1; i <= 20; i++)
            stdset.insert(i);

        int i = 0;
        while ( !stdset.empty() && (i < 500) )
        {
            stdset.erase( PFL::random(1, 20) );
            i++;
        }

        return assertTrue(stdset.empty(), "empty");
    }  

    bool testRandom2()
    {
        std::set<int> stdset;
        for (int i = -10; i <= 10; i++)
            stdset.insert(i);

        int i = 0;
        while (!stdset.empty() && (i < 500))
        {
            stdset.erase(PFL::random(-10, 10));
            i++;
        }

        return assertTrue(stdset.empty(), "empty");
    }

    bool testLerp()
    {
        return assertEquals(1.f, PFL::lerp(1.f, 3.f, 0.f), "lower wall") &
            assertEquals(3.f, PFL::lerp(1.f, 3.f, 1.f), "higher wall") &
            assertEquals(1.f, PFL::lerp(1.f, 3.f, -10.f), "below lower wall") &
            assertEquals(3.f, PFL::lerp(1.f, 3.f, 10.f), "above higher wall") &
            assertEquals(2.f, PFL::lerp(1.f, 3.f, 0.5f), "center");
    }

    bool testSmooth()
    {
        constexpr float fTarget = 1.f;
        constexpr float fStart = 0.f;
        
        bool b = true;

        float fCurrent = fStart;
        int i = 0;
        while (fCurrent != fTarget)
        {
            ++i;
            fCurrent = PFL::smooth(fCurrent, fTarget, 2.f);
            if (i > 40)
            {
                b &= assertFalse(true,
                    (std::string("iteration limit reached 1, fCurrent: ") + std::to_string(fCurrent) + ", fTarget: " + std::to_string(fTarget)).c_str());
                break;  // avoid infinite loop
            }
        }

        fCurrent = fStart;
        i = 0;
        while (fCurrent != fTarget)
        {
            ++i;
            fCurrent = PFL::smooth(fCurrent, fTarget, 5.f);
            if (i > 40)
            {
                b &= assertFalse(true,
                    (std::string("iteration limit reached 2, fCurrent: ") + std::to_string(fCurrent) + ", fTarget: " + std::to_string(fTarget)).c_str());
                break;  // avoid infinite loop
            }
        }

        fCurrent = fStart;
        i = 0;
        while (fCurrent != fTarget)
        {
            ++i;
            fCurrent = PFL::smooth(fCurrent, fTarget, 1.f);
            if (i > 40)
            {
                b &= assertFalse(true,
                    (std::string("iteration limit reached 3, fCurrent: ") + std::to_string(fCurrent) + ", fTarget: " + std::to_string(fTarget)).c_str());
                break;  // avoid infinite loop
            }
        }
        b &= assertEquals(1, i, "with speed 1.f iteration count should be 1!");

        fCurrent = fStart;
        i = 0;
        while (fCurrent != fTarget)
        {
            ++i;
            fCurrent = PFL::smooth(fCurrent, fTarget, 0.5f);
            if (i > 40)
            {
                b &= assertFalse(true,
                    (std::string("iteration limit reached 3, fCurrent: ") + std::to_string(fCurrent) + ", fTarget: " + std::to_string(fTarget)).c_str());
                break;  // avoid infinite loop
            }
        }
        b &= assertEquals(1, i, "with speed lower than 1.f iteration count should be 1!");

        fCurrent = fStart;
        i = 0;
        while (fCurrent != fTarget)
        {
            ++i;
            fCurrent = PFL::smooth(fCurrent, fTarget, -2.f);
            if (i > 40)
            {
                b &= assertFalse(true,
                    (std::string("iteration limit reached 3, fCurrent: ") + std::to_string(fCurrent) + ", fTarget: " + std::to_string(fTarget)).c_str());
                break;  // avoid infinite loop
            }
        }
        b &= assertEquals(1, i, "with speed lower than 1.f iteration count should be 1!");

        return b;
    }

}; // class PFLTest
