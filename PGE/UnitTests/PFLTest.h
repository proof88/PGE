#pragma once

/*
    ###################################################################################
    PFLTest.h
    Unit test for PR00F Foundation Library.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../../../PFL/PFL/PFL.h"

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

    virtual void Initialize()
    {
        AddSubTest("testFileExists", (PFNUNITSUBTEST) &PFLTest::testFileExists);
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
    }

    virtual bool setUp()
    {
        return true;    
    }

    virtual void TearDown()
    {
        Finalize();
    }

    virtual void Finalize()
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
            assertEquals("", PFL::changeExtension("", "jpg"), "empty");
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

}; // class PFLTest
