#pragma once

/*
    ###################################################################################
    PGEcfgProfilesTest.h
    Unit test for PGEcfgProfiles.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Config/PGEcfgProfiles.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess : public PGEcfgProfiles
{
public:
    explicit PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess(const char* gameTitle) :
        PGEcfgProfiles(gameTitle)
    {}

    std::map<std::string, PGEcfgVariable>& getCommandLineVars()
    {
        return PGEcfgProfiles::getCommandLineVars();
    }
};

class PGEcfgProfilesTest :
    public UnitTest
{
public:

    PGEcfgProfilesTest() :
        UnitTest( __FILE__ )
    {
        
    };

    virtual ~PGEcfgProfilesTest()
    {
        
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgProfiles::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testCtor1);
        AddSubTest("testGetMyDocsFolder", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetMyDocsFolder);
        AddSubTest("testGetLangFileName", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetLangFileName);
        AddSubTest("testReadLanguageData", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testReadLanguageData);
        AddSubTest("testAreProfilesInMyDocs", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testAreProfilesInMyDocs);
        AddSubTest("testGetPathToProfiles", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetPathToProfiles);
        AddSubTest("testGetProfilesCount", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetProfilesCount);
        AddSubTest("testGetProfilesList", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetProfilesList);
        AddSubTest("testGetProfilePlayersList", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetProfilePlayersList);
        AddSubTest("testAddProfile", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testAddProfile);
        AddSubTest("testDeleteProfile", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testDeleteProfile);
        AddSubTest("testGetProfile", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetProfile);
        AddSubTest("testSetProfile", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testSetProfile);
        AddSubTest("testDeleteVar", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testDeleteVar);
        AddSubTest("testGetVarsCount", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetVarsCount);
        AddSubTest("testReadConfiguration", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testReadConfiguration);
        AddSubTest("testWriteConfiguration", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testWriteConfiguration);
        AddSubTest("testProcessEmptyCommandLine", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessEmptyCommandLine);
        AddSubTest("testProcessIrrelevantCommandLine", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessIrrelevantCommandLine);
        AddSubTest("testProcessRelevantCommandLine", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessRelevantCommandLine);
        AddSubTest("testProcessRelevantCommandLineWithDisallowedAssignment", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessRelevantCommandLineWithDisallowedAssignment);
    }

    virtual bool setUp() override
    {
        return true;
    }

    virtual void TearDown() override
    {}

    virtual void Finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgProfiles::getLoggerModuleName(), false);    
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), false);
    }


private:

    PGEcfgProfilesTest(const PGEcfgProfilesTest&)
    {};         

    PGEcfgProfilesTest& operator=(const PGEcfgProfilesTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PGEcfgProfiles cfg("game title");
        return true;
    }

    bool testGetMyDocsFolder()
    {
        const PGEcfgProfiles cfg("game title");

        return assertTrue( (cfg.getMyDocsFolder() == "C:\\Users\\PR00F\\Documents\\") ||
            (cfg.getMyDocsFolder() == "C:\\Users\\eszabdm\\Documents\\") ||
            (cfg.getMyDocsFolder() == "C:\\Users\\pr0o0\\Documents\\") );
    }

    bool testGetLangFileName()
    {
        const PGEcfgProfiles cfg("game title");

        return assertEquals(std::string("gamedata\\language\\magyar_teszt.txt"), cfg.getLangFileName());
    }

    bool testReadLanguageData()
    {
        PGEcfgProfiles cfg("game title");
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
        const PGEcfgProfiles cfg("game title");
        return assertFalse( cfg.areProfilesInMyDocs() );
    }

    bool testGetPathToProfiles()
    {
        const PGEcfgProfiles cfg("game title");
        return assertNotEquals(std::string::npos, cfg.getPathToProfiles().find("\\profiles\\") );
    }

    bool testGetProfilesCount()
    {
        const PGEcfgProfiles cfg("game title");
        return assertEquals(4, cfg.getProfilesCount());
    }

    bool testGetProfilesList()
    {
        const PGEcfgProfiles cfg("game title");
        const std::string** const sProfileList = cfg.getProfilesList();
        const char* const saProfilesExpected[4] = { "alma", "béla", "loller", "proof88" };
        bool bErrorFound = false;

        for (int i = 0; i < cfg.getProfilesCount() && (i < 4); i++)
        {
            if ( bErrorFound = !assertEquals(std::string(saProfilesExpected[i]), *sProfileList[i]) )
                break;
        }

        return !bErrorFound;
    }

    bool testGetProfilePlayersList()
    {
        const PGEcfgProfiles cfg("game title");
        const std::string** const sProfilePlayersList = cfg.getProfilePlayersList();
        const char* const saProfilePlayersExpected[4] = { "Alma 76", "Béla", "loller", "PR00F88" };
        bool bErrorFound = false;

        for (int i = 0; i < cfg.getProfilesCount() && (i < 4); i++)
        {
            if ( bErrorFound = !assertEquals(std::string(saProfilePlayersExpected[i]), *sProfilePlayersList[i]) )
                break;
        }

        return !bErrorFound;
    }

    bool testAddProfile()
    {
        PGEcfgProfiles cfg("game title");
        cfg.SetProfile( 3 );  // proof88.cfg
        const int nOriginalUsersCount = cfg.getProfilesCount();
        size_t nOriginalVarsCount = cfg.getVars().size();

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

        b &= assertEquals(nOriginalVarsCount, cfg.getVars().size(), "varscount");
        b &= assertEquals(3, cfg.getProfile(), "getprofile 1");

        if ( iNewProfile > -1 )
        {
            // we should be able to switch to newly created profile (which is the last in order)
            cfg.SetProfile( iNewProfile );
            b &= assertEquals(iNewProfile, cfg.getProfile(), "getprofile 3");
            b &= assertTrue(std::string("testnickname") == *(cfg.getProfilePlayersList()[iNewProfile]), "playername 1");
            b &= assertTrue(std::string("testusername") == *(cfg.getProfilesList()[iNewProfile]), "username 1");
            nOriginalVarsCount = cfg.getVars().size();
        }

        // adding a profile that should change current profile index
        int iNewProfile3 = cfg.addProfile("01234", "test");
        // check if we are still currently at the last profile
        b &= assertEquals(nOriginalVarsCount, cfg.getVars().size(), "varscount 2");
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
        PGEcfgProfiles cfg("game title");
        cfg.SetProfile( 3 );  // proof88.cfg
        const int nOriginalUsersCount = cfg.getProfilesCount();
        size_t nOriginalVarsCount = cfg.getVars().size();

        bool b = assertEquals(-1, cfg.deleteProfile(nOriginalUsersCount+1), "delete non-existing profile");

        int iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertEquals(cfg.getProfilesCount()-1, iNewProfile, "addprofile 1");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "addprofile 2");

        if ( b )
        {
            b &= assertEquals(0, cfg.deleteProfile(iNewProfile), "delete 1");
            b &= assertEquals(nOriginalVarsCount, cfg.getVars().size(), "varscount 1");
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
            b &= assertEquals(0u, cfg.getVars().size(), "varscount 2");
            b &= assertEquals(-1, cfg.getProfile(), "getprofile 2");
        }

        return b;
    }

    bool testGetProfile()
    {
        const PGEcfgProfiles cfg("game title");
        return assertEquals(-1, cfg.getProfile(), "getProfile()");
    }

    bool testSetProfile()
    {
        PGEcfgProfiles cfg("game title");
        bool b = true;

        cfg.SetProfile( -1 );
        b &= assertEquals(-1, cfg.getProfile(), "-1 1st");

        cfg.SetProfile( cfg.getProfilesCount() );
        b &= assertEquals(-1, cfg.getProfile(), "over max");

        cfg.SetProfile( 3 );  // proof88.cfg
        b &= assertEquals(3, cfg.getProfile(), "3");
        b &= assertGreater(cfg.getVars().size(), 0u, "getVarsCount() 1st");

        cfg.SetProfile( -5 );
        b &= assertEquals(3, cfg.getProfile(), "3 2nd");
        b &= assertGreater(cfg.getVars().size(), 0u, "getVarsCount() 2nd");

        cfg.SetProfile( -1 );
        b &= assertEquals(-1, cfg.getProfile(), "-1 2nd");
        b &= assertEquals(0u, cfg.getVars().size(), "getVarsCount() 3rd");

        return b;
    }

    bool testDeleteVar()
    {
        PGEcfgProfiles cfg("game title");
        bool b = true;

        b &= assertEquals(0u, cfg.getVars().size(), "getVarsCount() 1");
        cfg.DeleteVar("cl_alma");  // try to remove anything without current profile
        b &= assertEquals(0u, cfg.getVars().size(), "getVarsCount() 2");

        cfg.SetProfile( 3 );  // proof88.cfg
        const size_t nVarCountOriginal = cfg.getVars().size();
        cfg.getVars()["cl_alma"] = 2;
        b &= assertEquals(nVarCountOriginal+1, cfg.getVars().size(), "getVarsCount() 3");
        cfg.DeleteVar("cl_alma");  // try to remove legally
        b &= assertEquals(nVarCountOriginal, cfg.getVars().size(), "getVarsCount() 4");
        
        cfg.DeleteVar("cl_name");  // try to remove protected cvar
        b &= assertEquals(nVarCountOriginal, cfg.getVars().size(), "getVarsCount() 5");

        return b;
    }

    bool testGetVarsCount()
    {
        PGEcfgProfiles cfg("game title");

        bool b = assertEquals(0u, cfg.getVars().size(), "1");

        cfg.SetProfile( 3 );  // proof88.cfg
        b &= assertNotEquals(0u, cfg.getVars().size(), "2");
        
        return b;
    }

    bool testReadConfiguration()
    {
        PGEcfgProfiles cfg("game title");
        bool b = true;

        b &= assertFalse(cfg.readConfiguration(), "1");

        cfg.SetProfile( 3 );  // proof88.cfg; this implies a readConfiguration(), but we call it anyway ...
        const size_t nOriginalCount1 = cfg.getVars().size();

        b &= assertTrue(cfg.readConfiguration(), "readconf 1");
        b &= assertEquals(nOriginalCount1, cfg.getVars().size(), "count 0");

        // check if added cvar is deleted upon re-reading config
        cfg.getVars()["cl_alma"];
        b &= assertTrue(cfg.readConfiguration(), "readconf 2");
        b &= assertEquals(nOriginalCount1, cfg.getVars().size(), "count 1");

        // check if command line vars are overriding read config
        cfg.ProcessCommandLine("    --cl_server_ip=168.1.2.3   --new_cvar=5.0    ");
        b &= assertTrue(cfg.readConfiguration(), "readconf 3");
        b &= assertEquals(nOriginalCount1 + 1, cfg.getVars().size(), "count 2"); // new_cvar is new variable that is why we add 1

        if (b)
        {
            b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 1") &
                assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 1");
        }

        cfg.SetProfile( -1 );
        b &= assertFalse(cfg.readConfiguration(), "readconf 4");

        return b;
    }

    bool testWriteConfiguration()
    {
        PGEcfgProfiles cfg("game title");

        bool b = assertFalse(cfg.writeConfiguration(), "writecfg 1");

        int iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertGreater(iNewProfile, -1, "addprofile 1");

        if ( !b )
            return b;

        cfg.SetProfile( iNewProfile );
        cfg.getVars()["cl_alma"] = 15.6f;
        b &= assertTrue(cfg.writeConfiguration(), "writecfg 2");
        
        if ( !b )
            return b;

        cfg.SetProfile( -1 );
        cfg.SetProfile( iNewProfile );

        b &= assertEquals(iNewProfile, cfg.getProfile(), "addprofile 2");
        b &= assertEquals(2u, cfg.getVars().size(), "varscount");
        b &= assertEquals(15.6f, cfg.getVars()["cl_alma"].getAsFloat(), E, "alma");

        cfg.deleteProfile( iNewProfile );

        return b;
    }

    bool testProcessEmptyCommandLine()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg("game title");

        cfg.ProcessCommandLine("");

        bool b = assertTrue(cfg.getVars().empty(), "cvars empty") &
            assertTrue(cfg.getCommandLineVars().empty(), "cl cvars empty");

        return b;
    }

    bool testProcessIrrelevantCommandLine()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg("game title");

        cfg.ProcessCommandLine("aghhahade gaghagih --asg ijag i = df -- --=");

        bool b = assertTrue(cfg.getVars().empty(), "cvars empty") &
            assertTrue(cfg.getCommandLineVars().empty(), "cl cvars empty");

        return b;
    }

    bool testProcessRelevantCommandLine()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg("game title");

        cfg.SetProfile(3);  // proof88.cfg
        const size_t nOriginalVarsCount = cfg.getVars().size();

        cfg.ProcessCommandLine("    --cl_server_ip=168.1.2.3   --new_cvar=5.0    ");

        bool b = assertFalse(cfg.getVars().empty(), "cvars not empty") &
            assertFalse(cfg.getCommandLineVars().empty(), "cl cvars not empty") &
            assertEquals(2u, cfg.getCommandLineVars().size(), "cl cvars size") &
            assertEquals(nOriginalVarsCount + 1u, cfg.getVars().size(), "cvars size") /* only new_cvar should be new variable */ &
            assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 1") &
            assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 1");

        if (b)
        {
            b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 1") &
                assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 1") &
                assertEquals("168.1.2.3", cfg.getCommandLineVars()["cl_server_ip"].getAsString(), "cl_server_ip value 2") &
                assertEquals("5.0", cfg.getCommandLineVars()["new_cvar"].getAsString(), "new_cvar value 2");
        }

        // check if command line vars are automatically re-applied after loading another profile config file
        cfg.SetProfile(-1);
        cfg.SetProfile(2);  // loller.cfg

        b &= assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 2") &
            assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 2");

        if (b)
        {
            b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 3") &
                assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 3");
        }

        return b;
    }

    bool testProcessRelevantCommandLineWithDisallowedAssignment()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg("game title");

        cfg.SetProfile(3);  // proof88.cfg
        const size_t nOriginalVarsCount = cfg.getVars().size();
        const std::string sOriginalClName1 = cfg.getVars()["cl_name"].getAsString();

        // cl_name is forbidden in command line, we need to skip that, but process the rest
        cfg.ProcessCommandLine("    --cl_server_ip=168.1.2.3   --cl_name=almafa  --new_cvar=5.0    ");

        bool b = assertFalse(cfg.getVars().empty(), "cvars not empty") &
            assertFalse(cfg.getCommandLineVars().empty(), "cl cvars not empty") &
            assertEquals(2u, cfg.getCommandLineVars().size(), "cl cvars size") &
            assertEquals(nOriginalVarsCount + 1u, cfg.getVars().size(), "cvars size") /* only new_cvar should be new variable */ &
            assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 1") &
            assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 1");

        if (b)
        {
            b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 1") &
                assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 1") &
                assertEquals("168.1.2.3", cfg.getCommandLineVars()["cl_server_ip"].getAsString(), "cl_server_ip value 2") &
                assertEquals("5.0", cfg.getCommandLineVars()["new_cvar"].getAsString(), "new_cvar value 2");
        }

        b &= assertEquals(sOriginalClName1, cfg.getVars()["cl_name"].getAsString(), "cl_name unchanged 1");

        // check if command line vars are automatically re-applied after loading another profile config file
        cfg.SetProfile(-1);
        cfg.SetProfile(2);  // loller.cfg
        const std::string sOriginalClName2 = cfg.getVars()["cl_name"].getAsString();

        b &= assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 2") &
            assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 2");

        if (b)
        {
            b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 3") &
                assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 3");
        }

        b &= assertEquals(sOriginalClName2, cfg.getVars()["cl_name"].getAsString(), "cl_name unchanged 2");

        return b;
    }
    
}; // class PGEcfgProfilesTest
