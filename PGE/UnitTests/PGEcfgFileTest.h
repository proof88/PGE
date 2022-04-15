#pragma once

/*
    ###################################################################################
    PGEcfgFileTest.h
    Unit test for PGEcfgFile.
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGEcfgFile.h"

class PGEcfgFileDerived : public PGEcfgFile
{
public:
    PGEcfgFileDerived(
        bool bRequireAllAcceptedVarsDefineRequirement,
        bool bCaseSensitiveVars) : PGEcfgFile(bRequireAllAcceptedVarsDefineRequirement, bCaseSensitiveVars)
    {} 

protected:
    virtual bool validateOnLoad(std::ifstream&) const /* override */
    {
        return false;
    }
};

class PGEcfgFileTest :
    public UnitTest
{
public:

    PGEcfgFileTest() :
        UnitTest( __FILE__ )
    {
    }

protected:

    virtual void Initialize()
    {
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        
        AddSubTest("test_wm_initially_empty_and_defaults_set", (PFNUNITSUBTEST) &PGEcfgFileTest::test_wm_initially_empty_and_defaults_set);
        AddSubTest("test_load_bad_assignment", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_bad_assignment);
        AddSubTest("test_load_fail_unaccepted_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_unaccepted_var);
        AddSubTest("test_load_accept_missing_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_accept_missing_var);
        AddSubTest("test_load_fail_missing_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_missing_var);
        AddSubTest("test_load_double_defined_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_double_defined_var);
        AddSubTest("test_load_fail_case_sensitive_vars_with_case_insensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_case_sensitive_vars_with_case_insensivity);
        AddSubTest("test_load_fail_case_sensitive_vars_with_case_sensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_case_sensitive_vars_with_case_sensivity);
        AddSubTest("test_load_good_case_sensitive_vars_with_case_sensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_good_case_sensitive_vars_with_case_sensivity);
        AddSubTest("test_load_good", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_good);
        AddSubTest("test_load_not_allowed_when_already_loaded", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_not_allowed_when_already_loaded);
        AddSubTest("test_set_accepted_vars_not_allowed_when_already_loaded", (PFNUNITSUBTEST) &PGEcfgFileTest::test_set_accepted_vars_not_allowed_when_already_loaded);
        AddSubTest("test_override_validateOnLoad", (PFNUNITSUBTEST) &PGEcfgFileTest::test_override_validateOnLoad);
    }

    virtual void Finalize()
    {
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), false);
    }

private:

    // ---------------------------------------------------------------------------

    PGEcfgFileTest(const PGEcfgFileTest&)
    {};         

    PGEcfgFileTest& operator=(const PGEcfgFileTest&)
    {
        return *this;
    };

    bool test_wm_initially_empty_and_defaults_set()
    {
        PGEcfgFile cfgFile(false, false);
        return assertTrue(cfgFile.getVars().empty(), "empty") &
            assertFalse(cfgFile.getAllAcceptedVarsDefineRequirement(), "getAllAcceptedVarsDefineRequirement") &
            assertFalse(cfgFile.getCaseSensitiveVars(), "getCaseSensitiveVars");
    }

    bool test_load_bad_assignment()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_bad_assignment.txt"), "load");
        b &= assertTrue(cfgFile.getVars().empty(), "empty");

        return b;
    }

    bool test_load_fail_unaccepted_var()
    {
        PGEcfgFile cfgFile(false, false);
        std::set<std::string> acceptedVars;
        acceptedVars.insert("name");
        bool b = assertTrue(cfgFile.setAcceptedVars(acceptedVars), "setAccepted");
        b &= assertFalse(cfgFile.getAcceptedVars().empty(), "accepted empty");

        // since acceptedVars is not empty, load should fail on unknown var "almafa"
        b &= assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_unaccepted_var.txt"), "load");
        
        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_accept_missing_var()
    {
        PGEcfgFile cfgFile(false, false);
        std::set<std::string> acceptedVars;
        acceptedVars.insert("name");
        acceptedVars.insert("almafa");
        bool b = assertTrue(cfgFile.setAcceptedVars(acceptedVars), "setAccepted");
        b &= assertFalse(cfgFile.getAcceptedVars().empty(), "accepted empty");

        // since acceptedVars is not empty, but we accept missing vars by default, we dont fail on missing "almafa"
        b &= assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_missing_var.txt"), "load");

        return b & assertFalse(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name");
    }

    bool test_load_fail_missing_var()
    {
        PGEcfgFile cfgFile(true, false);
        bool b = assertTrue(cfgFile.getAllAcceptedVarsDefineRequirement(), "req");

        std::set<std::string> acceptedVars;
        acceptedVars.insert("name");
        acceptedVars.insert("almafa");
        b &= assertTrue(cfgFile.setAcceptedVars(acceptedVars), "setAccepted");
        b &= assertFalse(cfgFile.getAcceptedVars().empty(), "accepted empty");

        // since acceptedVars is not empty, and we dont accept missing vars, we should fail on missing "almafa"
        b &= assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_missing_var.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_double_defined_var()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_double_defined_var.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_fail_case_sensitive_vars_with_case_insensivity()
    {
        PGEcfgFile cfgFile(false, false);
        std::set<std::string> acceptedVars;
        acceptedVars.insert("name");
        bool b = assertTrue(cfgFile.setAcceptedVars(acceptedVars), "setAccepted");
        b &= assertFalse(cfgFile.getAcceptedVars().empty(), "accepted empty");

        // "Name" and "name" vars are both accepted from case insensitivity perspective but now
        // they are multiple defined vars so we should fail now
        b &= assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_fail_case_sensitive_vars_with_case_sensivity()
    {
        PGEcfgFile cfgFile(false, true);
        bool b = assertTrue(cfgFile.getCaseSensitiveVars(), "case");

        std::set<std::string> acceptedVars;
        acceptedVars.insert("Name");
        b &= assertTrue(cfgFile.setAcceptedVars(acceptedVars), "setAccepted");
        b &= assertFalse(cfgFile.getAcceptedVars().empty(), "accepted empty");

        // "Name" is accepted but "name" is not, since we are case sensitive now and that is unknown var
        b &= assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_good_case_sensitive_vars_with_case_sensivity()
    {
        PGEcfgFile cfgFile(false, true);
        bool b = assertTrue(cfgFile.getCaseSensitiveVars(), "case");
        
        // "Name" is accepted and "name" is also accepted because we are case sensitive now and neither of them is unknown var since we did not define accepted vars!
        b &= assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertFalse(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getVars().find("Name") != cfgFile.getVars().end(), "Name") &
            assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name");
    }

    bool test_load_good()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty");

        b &= assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name var") &
            assertTrue(cfgFile.getVars().find("cap_max") != cfgFile.getVars().end(), "reload_per_mag var") &
            assertTrue(cfgFile.getVars().find("reload_per_mag") != cfgFile.getVars().end(), "reload_per_mag var") &
            assertTrue(cfgFile.getVars().find("reload_time") != cfgFile.getVars().end(), "reload_time var") &
            assertTrue(cfgFile.getVars().find("testvar") != cfgFile.getVars().end(), "testVar var") &
            assertTrue(cfgFile.getVars().find("testvar2") != cfgFile.getVars().end(), "testVar2 var") &
            assertTrue(cfgFile.getVars().find("testvar3") != cfgFile.getVars().end(), "testVar3 var") &
            assertTrue(cfgFile.getVars().find("testvar4") != cfgFile.getVars().end(), "testVar4 var") &
            assertTrue(cfgFile.getVars().find("testvar5") != cfgFile.getVars().end(), "testVar5 var");

        if ( b )
        {
            b &= assertEquals("Sample Weapon 1", cfgFile.getVars()["name"].getAsString(), "name value") &
                assertEquals("999", cfgFile.getVars()["cap_max"].getAsString(), "cap_max value") &
                assertEquals("true", cfgFile.getVars()["reload_per_mag"].getAsString(), "reload_per_mag value") &
                assertEquals("3000", cfgFile.getVars()["reload_time"].getAsString(), "reload_time value") &
                assertEquals("Values can have spaces, since they last until end of line!", cfgFile.getVars()["testvar"].getAsString(), "testVar value") &
                assertEquals("Values can have spaces, since they last until end of line!", cfgFile.getVars()["testvar2"].getAsString(), "testVar2 value") &
                assertEquals("", cfgFile.getVars()["testvar3"].getAsString(), "testVar3 value") &
                assertEquals("   ", cfgFile.getVars()["testvar4"].getAsString(), "testVar4 value") &
                assertEquals("alma    ", cfgFile.getVars()["testvar5"].getAsString(), "testVar5 value");
        }

        return b;
    }

    bool test_load_not_allowed_when_already_loaded()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty");
        
        b &= assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty");

        return b;
    }

    bool test_set_accepted_vars_not_allowed_when_already_loaded()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty");

        std::set<std::string> acceptedVars;
        acceptedVars.insert("Name");
        
        b &= assertFalse(cfgFile.setAcceptedVars(acceptedVars), "setAccepted");
        b &= assertTrue(cfgFile.getAcceptedVars().empty(), "accepted empty");

        return b;
    }

    bool test_override_validateOnLoad()
    {
        PGEcfgFileDerived cfgFile(false, false);
        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        
        return b;
    }

}; 