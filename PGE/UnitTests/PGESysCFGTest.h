#pragma once

/*
    ###################################################################################
    PGESysCFGTest.h
    Unit test for PGESysCFG.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGESysCFG.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PGESysCFGTest :
    public UnitTest
{
public:

    PGESysCFGTest() :
        UnitTest( __FILE__ )
    {};

protected:

    virtual void Initialize()
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PGESysCFGTest::testCtor1);
        AddSubTest("testGetMyDocsFolder", (PFNUNITSUBTEST) &PGESysCFGTest::testGetMyDocsFolder);
        AddSubTest("testGetLangFileName", (PFNUNITSUBTEST) &PGESysCFGTest::testGetLangFileName);
        AddSubTest("testReadLanguageData", (PFNUNITSUBTEST) &PGESysCFGTest::testReadLanguageData);
        AddSubTest("testAreProfilesInMyDocs", (PFNUNITSUBTEST) &PGESysCFGTest::testAreProfilesInMyDocs);
        AddSubTest("testGetPathToProfiles", (PFNUNITSUBTEST) &PGESysCFGTest::testGetPathToProfiles);
        AddSubTest("testGetProfilesCount", (PFNUNITSUBTEST) &PGESysCFGTest::testGetProfilesCount);
        AddSubTest("testGetProfilesList", (PFNUNITSUBTEST) &PGESysCFGTest::testGetProfilesList);
        AddSubTest("testGetProfilePlayersList", (PFNUNITSUBTEST) &PGESysCFGTest::testGetProfilePlayersList);
        AddSubTest("testAddProfile", (PFNUNITSUBTEST) &PGESysCFGTest::testAddProfile);
        AddSubTest("testDeleteProfile", (PFNUNITSUBTEST) &PGESysCFGTest::testDeleteProfile);
        AddSubTest("testGetProfile", (PFNUNITSUBTEST) &PGESysCFGTest::testGetProfile);
        AddSubTest("testSetProfile", (PFNUNITSUBTEST) &PGESysCFGTest::testSetProfile);
        AddSubTest("testGetVar1", (PFNUNITSUBTEST) &PGESysCFGTest::testGetVar1);
        AddSubTest("testGetVar2", (PFNUNITSUBTEST) &PGESysCFGTest::testGetVar2);
        AddSubTest("testDeleteVar", (PFNUNITSUBTEST) &PGESysCFGTest::testDeleteVar);
        AddSubTest("testGetVarsCount", (PFNUNITSUBTEST) &PGESysCFGTest::testGetVarsCount);
        AddSubTest("testReadConfiguration", (PFNUNITSUBTEST) &PGESysCFGTest::testReadConfiguration);
        AddSubTest("testWriteConfiguration", (PFNUNITSUBTEST) &PGESysCFGTest::testWriteConfiguration);
    }

    virtual bool setUp()
    {
        return true;
    }

    virtual void TearDown()
    {}


private:

    PGESysCFGTest(const PGESysCFGTest&)
    {};         

    PGESysCFGTest& operator=(const PGESysCFGTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PGESysCFG cfg("game title");
        return true;
    }

    bool testGetMyDocsFolder()
    {
        const PGESysCFG cfg("game title");

        return assertTrue( (cfg.getMyDocsFolder() == "C:\\Users\\PR00F\\Documents\\") ||
            (cfg.getMyDocsFolder() == "C:\\Users\\eszabdm\\Documents\\") );
    }

    bool testGetLangFileName()
    {
        const PGESysCFG cfg("game title");

        return assertEquals(std::string("gamedata\\language\\magyar_teszt.txt"), cfg.getLangFileName());
    }

    bool testReadLanguageData()
    {
        PGESysCFG cfg("game title");
        std::string** saLangData;
        const int nReadLangLines = cfg.readLanguageData(saLangData);

        const bool b1 = assertEquals(4, nReadLangLines, "read lang lines count") &
            assertNotNull(saLangData, "saLangData null");

        if ( !b1 )
        {
            return false;
        }

        const char* const saLangLinesExpected[] = {
            "asdasdasdasdasd",
            "asdasdasdasdasdasdasd",
            "asasdasdasasdasdasd",
            "asdasdasd", };
        bool bErrorFound = false;

        for (int i = 0; i < nReadLangLines; i++)
        {
            if ( bErrorFound = !assertEquals(std::string(saLangLinesExpected[i]), *saLangData[i]) )
                break;
        }

        for (int i = 0; i < nReadLangLines; i++)
        {
            delete saLangData[i];
        }
        delete[] saLangData;

        return b1 & !bErrorFound;
    }

    bool testAreProfilesInMyDocs()
    {
        const PGESysCFG cfg("game title");
        return assertFalse( cfg.areProfilesInMyDocs() );
    }

    bool testGetPathToProfiles()
    {
        const PGESysCFG cfg("game title");
        return assertFalse( cfg.getPathToProfiles().empty() );
    }

    bool testGetProfilesCount()
    {
        const PGESysCFG cfg("game title");
        return assertEquals(4, cfg.getProfilesCount());
    }

    bool testGetProfilesList()
    {
        const PGESysCFG cfg("game title");
        const std::string** const sProfileList = cfg.getProfilesList();
        const char* const saProfilesExpected[] = { "alma", "béla", "loller", "proof88" };
        bool bErrorFound = false;

        for (int i = 0; i < cfg.getProfilesCount(); i++)
            if ( bErrorFound = !assertEquals(std::string(saProfilesExpected[i]), *sProfileList[i]) )
                break;

        return !bErrorFound;
    }

    bool testGetProfilePlayersList()
    {
        const PGESysCFG cfg("game title");
        const std::string** const sProfilePlayersList = cfg.getProfilePlayersList();
        const char* const saProfilePlayersExpected[] = { "Alma 76", "Béla", "loller", "PR00F88" };
        bool bErrorFound = false;

        for (int i = 0; i < cfg.getProfilesCount(); i++)
            if ( bErrorFound = !assertEquals(std::string(saProfilePlayersExpected[i]), *sProfilePlayersList[i]) )
                break;

        return !bErrorFound;
    }

    bool testAddProfile()
    {
        PGESysCFG cfg("game title");
        cfg.SetProfile( 3 );  // proof88.cfg
        const int nOriginalUsersCount = cfg.getProfilesCount();
        int nOriginalVarsCount = cfg.getVarsCount();

        int iNewProfile = cfg.addProfile("testusername?", "testnickname");
        bool b = assertEquals(nOriginalUsersCount, cfg.getProfilesCount(), "wrongname1 1");
        b &= assertEquals(-2, iNewProfile, "wrongname1 2");

        iNewProfile = cfg.addProfile("*", "testnickname");
        b &= assertEquals(nOriginalUsersCount, cfg.getProfilesCount(), "wrongname2 1");
        b &= assertEquals(-2, iNewProfile, "wrongname2 2");

        iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "good 1");
        b &= assertGreater(iNewProfile, -1, "good 2");

        int iNewProfile2 = cfg.addProfile("testusername", "testothernickname");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "trysameagain 1");
        b &= assertEquals(-1, iNewProfile2, "trysameagain 2");

        b &= assertEquals(nOriginalVarsCount, cfg.getVarsCount(), "varscount");
        b &= assertEquals(3, cfg.getProfile(), "getprofile 1");

        if ( iNewProfile > -1 )
        {
            // we should be able to switch to newly created profile (which is the last in order)
            cfg.SetProfile( iNewProfile );
            b &= assertEquals(iNewProfile, cfg.getProfile(), "getprofile 3");
            b &= assertTrue(std::string("testnickname") == *(cfg.getProfilePlayersList()[iNewProfile]), "playername 1");
            b &= assertTrue(std::string("testusername") == *(cfg.getProfilesList()[iNewProfile]), "username 1");
            nOriginalVarsCount = cfg.getVarsCount();
        }

        // adding a profile that should change current profile index
        int iNewProfile3 = cfg.addProfile("01234", "test");
        // check if we are still currently at the last profile
        b &= assertEquals(nOriginalVarsCount, cfg.getVarsCount(), "varscount 2");
        b &= assertEquals(cfg.getProfilesCount()-1, cfg.getProfile(), "getprofile 2");

        if ( iNewProfile3 > -1 )
        {
            // we should be able to switch to newly created profile (which is the 1st in order)
            cfg.SetProfile( iNewProfile3 );
            b &= assertEquals(iNewProfile3, cfg.getProfile(), "getprofile 4");
            b &= assertEquals(0, cfg.getProfile(), "getprofile 5");
            b &= assertTrue(std::string("test") == *(cfg.getProfilePlayersList()[iNewProfile3]), "playername 2");
            b &= assertTrue(std::string("01234") == *(cfg.getProfilesList()[iNewProfile3]), "username 2");
        }

        // cleanup
        cfg.deleteProfile( 0 );
        cfg.deleteProfile( cfg.getProfilesCount()-1 );

        return b;
    }

    bool testDeleteProfile()
    {
        PGESysCFG cfg("game title");
        cfg.SetProfile( 3 );  // proof88.cfg
        const int nOriginalUsersCount = cfg.getProfilesCount();
        int nOriginalVarsCount = cfg.getVarsCount();

        int iNewProfile = cfg.addProfile("testusername", "testnickname");
        bool b = assertEquals(cfg.getProfilesCount()-1, iNewProfile, "addprofile 1");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "addprofile 2");

        if ( b )
        {
            b &= assertEquals(0, cfg.deleteProfile(iNewProfile), "delete 1");
            b &= assertEquals(nOriginalVarsCount, cfg.getVarsCount(), "varscount 1");
            b &= assertEquals(3, cfg.getProfile(), "getprofile 1");
        }

        // now test delete when it's the current profile
        iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertEquals(cfg.getProfilesCount()-1, iNewProfile, "addprofile 3");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "addprofile 4");

        if ( b )
        {
            cfg.SetProfile(iNewProfile);
            b &= assertEquals(0, cfg.deleteProfile(iNewProfile), "delete 2");
            b &= assertEquals(0, cfg.getVarsCount(), "varscount 2");
            b &= assertEquals(-1, cfg.getProfile(), "getprofile 2");
        }

        return b;
    }

    bool testGetProfile()
    {
        const PGESysCFG cfg("game title");
        return assertEquals(-1, cfg.getProfile(), "getProfile()");
    }

    bool testSetProfile()
    {
        PGESysCFG cfg("game title");
        bool b = true;

        cfg.SetProfile( -1 );
        b &= assertEquals(-1, cfg.getProfile(), "-1 1st");

        cfg.SetProfile( cfg.getProfilesCount() );
        b &= assertEquals(-1, cfg.getProfile(), "over max");

        cfg.SetProfile( 3 );  // proof88.cfg
        b &= assertEquals(3, cfg.getProfile(), "3");
        b &= assertGreater(cfg.getVarsCount(), 0, "getVarsCount() 1st");

        cfg.SetProfile( -5 );
        b &= assertEquals(3, cfg.getProfile(), "3 2nd");
        b &= assertGreater(cfg.getVarsCount(), 0, "getVarsCount() 2nd");

        cfg.SetProfile( -1 );
        b &= assertEquals(-1, cfg.getProfile(), "-1 2nd");
        b &= assertEquals(0, cfg.getVarsCount(), "getVarsCount() 3rd");

        return b;
    }

    bool testGetVar1()
    {
        // non-const version
        PGESysCFG cfg("game title");
        bool b = true;

        cfg.getVar("cl_alma") = 2;
        b &= assertEquals(0, cfg.getVarsCount(), "getVarsCount() 1");

        cfg.SetProfile( 3 );  // proof88.cfg
        const int nVarCountOriginal = cfg.getVarsCount();

        cfg.getVar("cl_alma") = 2;
        b &= assertEquals(nVarCountOriginal+1, cfg.getVarsCount(), "getVarsCount() 2");

        cfg.getVar("cl_alma") = 3;
        b &= assertEquals(nVarCountOriginal+1, cfg.getVarsCount(), "getVarsCount() 3");
        b &= assertTrue(std::string("PR00F88") == cfg.getVar("cl_name"), "name");

        return b;
    }

    bool testGetVar2()
    {
        // const version
        const PGESysCFG cfg("game title");

        PGEcfgVariable cvar = cfg.getVar("cl_alma");

        return assertFalse(cvar.getAsBool(), "b1") &
               assertEquals(0, cvar.getAsInt(), "b2") &
               assertTrue(cvar.getAsString().empty(), "b3");
    }

    bool testDeleteVar()
    {
        PGESysCFG cfg("game title");
        bool b = true;

        b &= assertEquals(0, cfg.getVarsCount(), "getVarsCount() 1");
        cfg.DeleteVar("cl_alma");  // try to remove anything without current profile
        b &= assertEquals(0, cfg.getVarsCount(), "getVarsCount() 2");

        cfg.SetProfile( 3 );  // proof88.cfg
        const int nVarCountOriginal = cfg.getVarsCount();
        cfg.getVar("cl_alma") = 2;
        b &= assertEquals(nVarCountOriginal+1, cfg.getVarsCount(), "getVarsCount() 3");
        cfg.DeleteVar("cl_alma");  // try to remove legally
        b &= assertEquals(nVarCountOriginal, cfg.getVarsCount(), "getVarsCount() 4");
        
        cfg.DeleteVar("cl_name");  // try to remove protected cvar
        b &= assertEquals(nVarCountOriginal, cfg.getVarsCount(), "getVarsCount() 5");

        return b;
    }

    bool testGetVarsCount()
    {
        PGESysCFG cfg("game title");

        bool b = assertEquals(0, cfg.getVarsCount(), "1");

        cfg.SetProfile( 3 );  // proof88.cfg
        b &= assertNotEquals(0, cfg.getVarsCount(), "2");
        
        return b;
    }

    bool testReadConfiguration()
    {
        PGESysCFG cfg("game title");
        bool b = true;

        b &= assertFalse(cfg.readConfiguration(), "1");

        cfg.SetProfile( 3 );  // proof88.cfg; this implies a readConfiguration(), but we call it anyway ...
        const int nOriginalCount1 = cfg.getVarsCount();
        cfg.getVar("cl_alma");
        const int nOriginalCount2 = cfg.getVarsCount();
        cfg.readConfiguration();

        b &= assertEquals(nOriginalCount1, cfg.getVarsCount(), "count 1");
        b &= assertEquals(nOriginalCount2, cfg.getVarsCount()+1, "count 2");

        cfg.SetProfile( -1 );
        b &= assertFalse(cfg.readConfiguration(), "1");

        return b;
    }

    bool testWriteConfiguration()
    {
        PGESysCFG cfg("game title");

        bool b = assertFalse(cfg.writeConfiguration(), "writecfg 1");

        int iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertGreater(iNewProfile, -1, "addprofile 1");

        if ( !b )
            return b;

        cfg.SetProfile( iNewProfile );
        cfg.getVar("cl_alma") = 15.6f;
        b &= assertTrue(cfg.writeConfiguration(), "writecfg 2");
        
        if ( !b )
            return b;

        cfg.SetProfile( -1 );
        cfg.SetProfile( iNewProfile );

        b &= assertEquals(iNewProfile, cfg.getProfile(), "addprofile 2");
        b &= assertEquals(2, cfg.getVarsCount(), "varscount");
        b &= assertEquals(15.6f, cfg.getVar("cl_alma").getAsFloat(), E, "alma");

        cfg.deleteProfile( iNewProfile );

        return b;
    }
    
}; // class PGESysCFGTest
