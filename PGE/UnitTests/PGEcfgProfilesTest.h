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
    explicit PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess() :
        PGEcfgProfiles()
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

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgProfiles::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testCtor1);
        AddSubTest("testReinitializeBeforeInit", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testReinitializeBeforeInit);
        AddSubTest("testReinitializeAfterInit", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testReinitializeAfterInit);
        AddSubTest("testShutdownBeforeInit", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testShutdownBeforeInit);
        AddSubTest("testShutdownAfterInit", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testShutdownAfterInit);
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
        AddSubTest("testGetProfileIndex", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetProfileIndex);
        AddSubTest("testSetProfileByIndex", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testSetProfileByIndex);
        AddSubTest("testSetProfileByName", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testSetProfileByName);
        AddSubTest("testDeleteVar", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testDeleteVar);
        AddSubTest("testGetVarsCount", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testGetVarsCount);
        AddSubTest("testReadConfiguration", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testReadConfiguration);
        AddSubTest("testWriteConfiguration", (PFNUNITSUBTEST) &PGEcfgProfilesTest::testWriteConfiguration);
        AddSubTest("testProcessEmptyCommandLine", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessEmptyCommandLine);
        AddSubTest("testProcessIrrelevantCommandLine", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessIrrelevantCommandLine);
        AddSubTest("testProcessRelevantCommandLine", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessRelevantCommandLine);
        // I disabled this test because now I think cl_name setting from commandline should be allowed ...
        //AddSubTest("testProcessRelevantCommandLineWithDisallowedAssignment", (PFNUNITSUBTEST)&PGEcfgProfilesTest::testProcessRelevantCommandLineWithDisallowedAssignment);
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
        PGEcfgProfiles cfg;
        const PGEcfgProfiles& cfgAsConst = cfg;
        std::string** saLangData;

        return assertTrue(cfg.getMyDocsFolder().empty(), "mydocs empty") &
            assertTrue(cfg.getProfileName().empty(), "active profile name empty") &
            assertEquals(-1, cfg.getProfileIndex(), "active profile index") &
            assertTrue(cfgAsConst.getCommandLineVars().empty(), "command line") &
            assertEquals(0, cfg.getProfilesCount(), "profiles count") &
            assertNull(cfg.getProfilePlayersList(), "players list") &
            assertNull(cfg.getProfilesList(), "profiles list") &
            assertTrue(cfg.getVars().empty(), "getVars") &
            assertTrue(cfg.getTemplate().empty(), "template") &
            assertFalse(cfg.readConfiguration(), "readConfiguration") &
            assertTrue(cfg.getPathToProfiles().empty(), "path to profiles") &
            assertFalse(cfg.areProfilesInMyDocs(), "profiles are in mydocs") &
            assertTrue(cfg.getFilename().empty(), "filename") &
            assertTrue(cfg.getLangFileName().empty(), "lang filename") &
            assertEquals(0, cfg.readLanguageData(saLangData), "lang data") &
            assertTrue(cfg.getAcceptedVars().empty(), "accepted vars") &
            assertFalse(cfg.getCaseSensitiveVars(), "case sensitive vars") &
            assertFalse(cfg.getAllAcceptedVarsDefineRequirement(), "all accepted vars define req") &
            assertEquals(-3, cfg.addProfile("asd", "asd"), "addProfile") &
            assertEquals(-1, cfg.deleteProfile(0), "deleteProfile");
    }

    bool testReinitializeBeforeInit()
    {
        PGEcfgProfiles cfg;

        // values are tested in the separate test cases

        return assertTrue(cfg.reinitialize("asd"), "reinit");
    }

    bool testReinitializeAfterInit()
    {
        PGEcfgProfiles cfg;

        const bool b = assertTrue(cfg.reinitialize("asd"), "reinit");
        return b & assertTrue(cfg.reinitialize("asd"), "reinit 2");
    }

    bool testShutdownBeforeInit()
    {
        PGEcfgProfiles cfg;
        const PGEcfgProfiles& cfgAsConst = cfg;
        std::string** saLangData;

        cfg.shutdown();

        return assertTrue(cfg.getMyDocsFolder().empty(), "mydocs empty") &
            assertTrue(cfg.getProfileName().empty(), "active profile name empty") &
            assertEquals(-1, cfg.getProfileIndex(), "active profile index") &
            assertTrue(cfgAsConst.getCommandLineVars().empty(), "command line") &
            assertEquals(0, cfg.getProfilesCount(), "profiles count") &
            assertNull(cfg.getProfilePlayersList(), "players list") &
            assertNull(cfg.getProfilesList(), "profiles list") &
            assertTrue(cfg.getVars().empty(), "getVars") &
            assertTrue(cfg.getTemplate().empty(), "template") &
            assertFalse(cfg.readConfiguration(), "readConfiguration") &
            assertTrue(cfg.getPathToProfiles().empty(), "path to profiles") &
            assertFalse(cfg.areProfilesInMyDocs(), "profiles are in mydocs") &
            assertTrue(cfg.getFilename().empty(), "filename") &
            assertTrue(cfg.getLangFileName().empty(), "lang filename") &
            assertEquals(0, cfg.readLanguageData(saLangData), "lang data") &
            assertTrue(cfg.getAcceptedVars().empty(), "accepted vars") &
            assertFalse(cfg.getCaseSensitiveVars(), "case sensitive vars") &
            assertFalse(cfg.getAllAcceptedVarsDefineRequirement(), "all accepted vars define req") &
            assertEquals(-3, cfg.addProfile("asd", "asd"), "addProfile") &
            assertEquals(-1, cfg.deleteProfile(0), "deleteProfile");
    }

    bool testShutdownAfterInit()
    {
        PGEcfgProfiles cfg;
        const PGEcfgProfiles& cfgAsConst = cfg;
        std::string** saLangData;

        const bool b = assertTrue(cfg.reinitialize(""), "reinit");
        cfg.shutdown();

        return b & assertTrue(cfg.getMyDocsFolder().empty(), "mydocs empty") &
            assertTrue(cfg.getProfileName().empty(), "active profile name empty") &
            assertEquals(-1, cfg.getProfileIndex(), "active profile index") &
            assertTrue(cfgAsConst.getCommandLineVars().empty(), "command line") &
            assertEquals(0, cfg.getProfilesCount(), "profiles count") &
            assertNull(cfg.getProfilePlayersList(), "players list") &
            assertNull(cfg.getProfilesList(), "profiles list") &
            assertTrue(cfg.getVars().empty(), "getVars") &
            assertTrue(cfg.getTemplate().empty(), "template") &
            assertFalse(cfg.readConfiguration(), "readConfiguration") &
            assertTrue(cfg.getPathToProfiles().empty(), "path to profiles") &
            assertFalse(cfg.areProfilesInMyDocs(), "profiles are in mydocs") &
            assertTrue(cfg.getFilename().empty(), "filename") &
            assertTrue(cfg.getLangFileName().empty(), "lang filename") &
            assertEquals(0, cfg.readLanguageData(saLangData), "lang data") &
            assertTrue(cfg.getAcceptedVars().empty(), "accepted vars") &
            assertFalse(cfg.getCaseSensitiveVars(), "case sensitive vars") &
            assertFalse(cfg.getAllAcceptedVarsDefineRequirement(), "all accepted vars define req") &
            assertEquals(-3, cfg.addProfile("asd", "asd"), "addProfile") &
            assertEquals(-1, cfg.deleteProfile(0), "deleteProfile");
    }

    bool testGetMyDocsFolder()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");
        const std::string sMyDocsFolder = cfg.getMyDocsFolder();  /* e.g.: C:\\Users\\PR00F\\Documents\\ */

        return b && ((sMyDocsFolder.find("C:\\Users\\") != std::string::npos) || (sMyDocsFolder.find("\\OneDrive\\") != std::string::npos)) &&
            ((sMyDocsFolder.find("\\Documents\\") != std::string::npos) || (sMyDocsFolder.find("\\Dokumentumok\\") != std::string::npos));
    }

    bool testGetLangFileName()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        return b && assertEquals(std::string("gamedata\\language\\magyar_teszt.txt"), cfg.getLangFileName());
    }

    bool testReadLanguageData()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

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

        return b & b1 & !bErrorFound;
    }

    bool testAreProfilesInMyDocs()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");
        return b & assertFalse( cfg.areProfilesInMyDocs() );
    }

    bool testGetPathToProfiles()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        return b & assertNotEquals(std::string::npos, cfg.getPathToProfiles().find("\\profiles\\") );
    }

    bool testGetProfilesCount()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        return b & assertEquals(4, cfg.getProfilesCount());
    }

    bool testGetProfilesList()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        const std::string** const sProfileList = cfg.getProfilesList();
        const char* const saProfilesExpected[4] = { "alma", "béla", "loller", "proof88" };
        bool bErrorFound = false;

        for (int i = 0; i < cfg.getProfilesCount() && (i < 4); i++)
        {
            if ( bErrorFound = !assertEquals(std::string(saProfilesExpected[i]), *sProfileList[i]) )
                break;
        }

        return b & !bErrorFound;
    }

    bool testGetProfilePlayersList()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        const std::string** const sProfilePlayersList = cfg.getProfilePlayersList();
        const char* const saProfilePlayersExpected[4] = { "Alma 76", "Béla", "loller", "PR00F88" };
        bool bErrorFound = false;

        for (int i = 0; i < cfg.getProfilesCount() && (i < 4); i++)
        {
            if ( bErrorFound = !assertEquals(std::string(saProfilePlayersExpected[i]), *sProfilePlayersList[i]) )
                break;
        }

        return b & !bErrorFound;
    }

    bool testAddProfile()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertTrue(cfg.setProfile( 3 ), "setprofile ret 1");  // proof88.cfg
        const int nOriginalUsersCount = cfg.getProfilesCount();
        size_t nOriginalVarsCount = cfg.getVars().size();

        int iNewProfile = cfg.addProfile("testusername?", "testnickname");
        b &= assertEquals(nOriginalUsersCount, cfg.getProfilesCount(), "wrongname1 1");
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
        b &= assertEquals(3, cfg.getProfileIndex(), "getprofile 1i");
        b &= assertEquals("proof88", cfg.getProfileName(), "getprofile 1n");

        if ( iNewProfile > -1 )
        {
            // we should be able to switch to newly created profile (which is the last in order)
            b &= assertTrue(cfg.setProfile( iNewProfile ), "setprofile ret 2");
            b &= assertEquals(iNewProfile, cfg.getProfileIndex(), "getprofile 3i");
            b &= assertEquals("testusername", cfg.getProfileName(), "getprofile 3n");
            b &= assertTrue(std::string("testnickname") == *(cfg.getProfilePlayersList()[iNewProfile]), "playername 1");
            b &= assertTrue(std::string("testusername") == *(cfg.getProfilesList()[iNewProfile]), "username 1");
            nOriginalVarsCount = cfg.getVars().size();
        }

        // adding a profile that should change current profile index
        int iNewProfile3 = cfg.addProfile("01234", "test");
        // check if we are still currently at the last profile
        b &= assertEquals(nOriginalVarsCount, cfg.getVars().size(), "varscount 2");
        b &= assertEquals(cfg.getProfilesCount()-1, cfg.getProfileIndex(), "getprofile 3i");
        b &= assertEquals("testusername", cfg.getProfileName(), "getprofile 3n");

        if ( iNewProfile3 > -1 )
        {
            // we should be able to switch to newly created profile (which is the 1st in order)
            b &= assertTrue(cfg.setProfile( iNewProfile3 ), "setprofile ret 3");
            b &= assertEquals(iNewProfile3, cfg.getProfileIndex(), "getprofile 4i");
            b &= assertEquals(0, cfg.getProfileIndex(), "getprofile 5i");
            b &= assertEquals("01234", cfg.getProfileName(), "getprofile 5n");
            b &= assertTrue(std::string("test") == *(cfg.getProfilePlayersList()[iNewProfile3]), "playername 2");
            b &= assertTrue(std::string("01234") == *(cfg.getProfilesList()[iNewProfile3]), "username 2");
        }

        // cleanup
        if (b)
        {
            cfg.deleteProfile(0);
            cfg.deleteProfile(cfg.getProfilesCount() - 1);
        }

        return b;
    }

    bool testDeleteProfile()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertTrue(cfg.setProfile( 3 ), "setprofile ret 1");  // proof88.cfg
        const int nOriginalUsersCount = cfg.getProfilesCount();
        size_t nOriginalVarsCount = cfg.getVars().size();

        b &= assertEquals(-1, cfg.deleteProfile(nOriginalUsersCount+1), "delete non-existing profile");

        int iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertEquals(cfg.getProfilesCount()-1, iNewProfile, "addprofile 1");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "addprofile 2");

        if ( b )
        {
            b &= assertEquals(0, cfg.deleteProfile(iNewProfile), "delete 1");
            b &= assertEquals(nOriginalVarsCount, cfg.getVars().size(), "varscount 1");
            b &= assertEquals(3, cfg.getProfileIndex(), "getprofile 1i");
            b &= assertEquals("proof88", cfg.getProfileName(), "getprofile 1n");
        }

        // now test delete when it's the current profile
        iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertEquals(cfg.getProfilesCount()-1, iNewProfile, "addprofile 3");
        b &= assertEquals(nOriginalUsersCount+1, cfg.getProfilesCount(), "addprofile 4");

        if ( b )
        {
            b &= assertTrue(cfg.setProfile(iNewProfile), "setprofile ret 2");
            b &= assertEquals(0, cfg.deleteProfile(iNewProfile), "delete 2");
            b &= assertEquals(0u, cfg.getVars().size(), "varscount 2");
            b &= assertEquals(-1, cfg.getProfileIndex(), "getprofile 2i");
            b &= assertEquals("", cfg.getProfileName(), "getprofile 2n");
        }

        return b;
    }

    bool testGetProfileIndex()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        return b & assertEquals(-1, cfg.getProfileIndex(), "getProfileIndex()");
    }

    bool testGetProfileName()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        return b & assertEquals("", cfg.getProfileName(), "getProfileName()");
    }

    bool testSetProfileByIndex()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertTrue(cfg.setProfile( -1 ), "ret 1");
        b &= assertEquals(-1, cfg.getProfileIndex(), "index 1");
        b &= assertEquals("", cfg.getProfileName(), "name 1");
        b &= assertTrue(cfg.getVars().empty(), "getVars empty() 1");
        b &= assertTrue(cfg.getTemplate().empty(), "template 1");

        b &= assertFalse(cfg.setProfile( cfg.getProfilesCount() ), "ret 2");
        b &= assertEquals(-1, cfg.getProfileIndex(), "index 2");
        b &= assertEquals("", cfg.getProfileName(), "name 2");
        b &= assertTrue(cfg.getVars().empty(), "getVars empty() 2");
        b &= assertTrue(cfg.getTemplate().empty(), "template 2");

        b &= assertTrue(cfg.setProfile( 3 ), "ret 3");  // proof88.cfg
        b &= assertEquals(3, cfg.getProfileIndex(), "index 3");
        b &= assertEquals("proof88", cfg.getProfileName(), "name 3");
        b &= assertFalse(cfg.getVars().empty(), "getVars empty() 3");
        b &= assertFalse(cfg.getTemplate().empty(), "template 3");

        b &= assertFalse(cfg.setProfile( -5 ), "ret 4");
        b &= assertEquals(3, cfg.getProfileIndex(), "index 4");
        b &= assertEquals("proof88", cfg.getProfileName(), "name 4");
        b &= assertFalse(cfg.getVars().empty(), "getVars empty() 4");
        b &= assertFalse(cfg.getTemplate().empty(), "template 4");

        b &= assertTrue(cfg.setProfile( -1 ), "ret 5");
        b &= assertEquals(-1, cfg.getProfileIndex(), "index 5");
        b &= assertEquals("", cfg.getProfileName(), "name 5");
        b &= assertTrue(cfg.getVars().empty(), "getVars empty() 5");
        b &= assertTrue(cfg.getTemplate().empty(), "template 5");

        return b;
    }

    bool testSetProfileByName()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertTrue(cfg.setProfile(""), "ret 1");
        b &= assertEquals(-1, cfg.getProfileIndex(), "index 1");
        b &= assertEquals("", cfg.getProfileName(), "name 1");
        b &= assertTrue(cfg.getVars().empty(), "getVars empty() 1");
        b &= assertTrue(cfg.getTemplate().empty(), "template 1");

        b &= assertTrue(cfg.setProfile("proof88"), "ret 2");
        b &= assertEquals(3, cfg.getProfileIndex(), "index 2");
        b &= assertEquals("proof88", cfg.getProfileName(), "name 2");
        b &= assertFalse(cfg.getVars().empty(), "getVars empty() 2");
        b &= assertFalse(cfg.getTemplate().empty(), "template 2");

        b &= assertTrue(cfg.setProfile(""), "ret 3");
        b &= assertEquals(-1, cfg.getProfileIndex(), "index 3");
        b &= assertEquals("", cfg.getProfileName(), "name 3");
        b &= assertTrue(cfg.getVars().empty(), "getVars empty() 3");
        b &= assertTrue(cfg.getTemplate().empty(), "template 3");

        return b;
    }

    bool testDeleteVar()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertEquals(0u, cfg.getVars().size(), "getVarsCount() 1");
        cfg.DeleteVar("cl_alma");  // try to remove anything without current profile
        b &= assertEquals(0u, cfg.getVars().size(), "getVarsCount() 2");

        b &= assertTrue(cfg.setProfile( 3 ), "ret 1");  // proof88.cfg
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
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertEquals(0u, cfg.getVars().size(), "1");

        b &= assertTrue(cfg.setProfile( 3 ), "ret 1");  // proof88.cfg
        b &= assertNotEquals(0u, cfg.getVars().size(), "2");
        
        return b;
    }

    bool testReadConfiguration()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertFalse(cfg.readConfiguration(), "1");

        b &= assertTrue(cfg.setProfile( 3 ), "setProfile ret 1");  // proof88.cfg; this implies a readConfiguration(), but we call it anyway ...
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

        b &= assertTrue(cfg.setProfile( -1 ), "setProfile ret 2");
        b &= assertFalse(cfg.readConfiguration(), "readconf 4");

        return b;
    }

    bool testWriteConfiguration()
    {
        PGEcfgProfiles cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertFalse(cfg.writeConfiguration(), "writecfg 1");

        const int iNewProfile = cfg.addProfile("testusername", "testnickname");
        b &= assertGreater(iNewProfile, -1, "addprofile 1");

        if (!b)
        {
            return b;
        }

        b &= assertTrue(cfg.setProfile( iNewProfile ), "setProfile ret 1");

        // variables wont be written to file if template doesnt contain them so we need to add them to template
        cfg.getTemplate().push_back("cl_alma");

        cfg.getVars()["cl_alma"] = 15.6f;
        b &= assertTrue(cfg.writeConfiguration(), "writecfg 2");
        
        if (!b)
        {
            cfg.deleteProfile(iNewProfile);
            return b;
        }

        b &= assertTrue(cfg.setProfile( -1 ), "setProfile ret 2");
        b &= assertTrue(cfg.setProfile( iNewProfile ), "setProfile ret 3");

        b &= assertEquals(iNewProfile, cfg.getProfileIndex(), "addprofile 2i");
        b &= assertEquals("testusername", cfg.getProfileName(), "addprofile 2n");
        b &= assertEquals(2u, cfg.getVars().size(), "varscount");
        b &= assertEquals(15.6f, cfg.getVars()["cl_alma"].getAsFloat(), E, "alma");

        cfg.deleteProfile( iNewProfile );

        return b;
    }

    bool testProcessEmptyCommandLine()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        cfg.ProcessCommandLine("");

        b &= assertTrue(cfg.getVars().empty(), "cvars empty") &
            assertTrue(cfg.getCommandLineVars().empty(), "cl cvars empty");

        return b;
    }

    bool testProcessIrrelevantCommandLine()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        cfg.ProcessCommandLine("aghhahade gaghagih --asg ijag i = df -- --=");

        b &= assertTrue(cfg.getVars().empty(), "cvars empty") &
            assertTrue(cfg.getCommandLineVars().empty(), "cl cvars empty");

        return b;
    }

    bool testProcessRelevantCommandLine()
    {
        PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg;
        bool b = assertTrue(cfg.reinitialize("game title"), "reinit");

        b &= assertTrue(cfg.setProfile(3), "setProfile ret 1");  // proof88.cfg
        const size_t nOriginalVarsCount = cfg.getVars().size();

        cfg.ProcessCommandLine("    --cl_server_ip=168.1.2.3   --new_cvar=5.0    ");

        b &= assertFalse(cfg.getVars().empty(), "cvars not empty") &
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
        b &= assertTrue(cfg.setProfile(-1), "setProfile ret 2");
        b &= assertTrue(cfg.setProfile(2), "setProfile ret 3");  // loller.cfg

        b &= assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 2") &
            assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 2");

        if (b)
        {
            b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 3") &
                assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 3");
        }

        return b;
    }

    //bool testProcessRelevantCommandLineWithDisallowedAssignment()
    //{
    //    PGEcfgProfilesWithPublicNonConstCommandLineCVarAccess cfg;
    //    bool b = assertTrue(cfg.reinitialize("game title"), "reinit");
    //
    //    b &= assertTrue(cfg.setProfile(3), "setProfile ret 1");  // proof88.cfg
    //    const size_t nOriginalVarsCount = cfg.getVars().size();
    //    const std::string sOriginalClName1 = cfg.getVars()["cl_name"].getAsString();
    //
    //    // cl_name is forbidden in command line, we need to skip that, but process the rest
    //    cfg.ProcessCommandLine("    --cl_server_ip=168.1.2.3   --cl_name=almafa  --new_cvar=5.0    ");
    //
    //    b &= assertFalse(cfg.getVars().empty(), "cvars not empty") &
    //        assertFalse(cfg.getCommandLineVars().empty(), "cl cvars not empty") &
    //        assertEquals(2u, cfg.getCommandLineVars().size(), "cl cvars size") &
    //        assertEquals(nOriginalVarsCount + 1u, cfg.getVars().size(), "cvars size") /* only new_cvar should be new variable */ &
    //        assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 1") &
    //        assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 1");
    //
    //    if (b)
    //    {
    //        b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 1") &
    //            assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 1") &
    //            assertEquals("168.1.2.3", cfg.getCommandLineVars()["cl_server_ip"].getAsString(), "cl_server_ip value 2") &
    //            assertEquals("5.0", cfg.getCommandLineVars()["new_cvar"].getAsString(), "new_cvar value 2");
    //    }
    //
    //    b &= assertEquals(sOriginalClName1, cfg.getVars()["cl_name"].getAsString(), "cl_name unchanged 1");
    //
    //    // check if command line vars are automatically re-applied after loading another profile config file
    //    b &= assertTrue(cfg.setProfile(-1), "setProfile ret 2");
    //    b &= assertTrue(cfg.setProfile(2), "setProfile ret 3");  // loller.cfg
    //    const std::string sOriginalClName2 = cfg.getVars()["cl_name"].getAsString();
    //
    //    b &= assertTrue(cfg.getVars().find("cl_server_ip") != cfg.getVars().end(), "cl_server_ip found 2") &
    //        assertTrue(cfg.getVars().find("new_cvar") != cfg.getVars().end(), "new_cvar found 2");
    //
    //    if (b)
    //    {
    //        b = assertEquals("168.1.2.3", cfg.getVars()["cl_server_ip"].getAsString(), "cl_server_ip value 3") &
    //            assertEquals("5.0", cfg.getVars()["new_cvar"].getAsString(), "new_cvar value 3");
    //    }
    //
    //    b &= assertEquals(sOriginalClName2, cfg.getVars()["cl_name"].getAsString(), "cl_name unchanged 2");
    //
    //    return b;
    //}
    
}; // class PGEcfgProfilesTest
