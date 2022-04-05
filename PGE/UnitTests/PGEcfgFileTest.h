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
        
        AddSubTest("test_ows_initially_empty_and_defaults_set", (PFNUNITSUBTEST) &PGEcfgFileTest::test_ows_initially_empty_and_defaults_set);
        AddSubTest("test_load_bad_assignment", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_bad_assignment);
        AddSubTest("test_load_fail_unaccepted_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_unaccepted_var);
        AddSubTest("test_load_accept_missing_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_accept_missing_var);
        AddSubTest("test_load_fail_missing_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_missing_var);
        AddSubTest("test_load_double_defined_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_double_defined_var);
        AddSubTest("test_load_fail_case_sensitive_vars_with_case_insensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_case_sensitive_vars_with_case_insensivity);
        AddSubTest("test_load_fail_case_sensitive_vars_with_case_sensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_case_sensitive_vars_with_case_sensivity);
        AddSubTest("test_load_good_case_sensitive_vars_with_case_sensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_good_case_sensitive_vars_with_case_sensivity);
        AddSubTest("test_load_good", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_good);
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

    bool test_ows_initially_empty_and_defaults_set()
    {
        PGEcfgFile cfgFile;
        return assertTrue(cfgFile.getVars().empty(), "empty") &
            assertFalse(cfgFile.getAllAcceptedVarsDefineRequirement(), "getAllAcceptedVarsDefineRequirement") &
            assertFalse(cfgFile.getCaseSensitiveVars(), "getCaseSensitiveVars");
    }

    bool test_load_bad_assignment()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);

        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_bad_assignment.txt"), "load");
        b &= assertTrue(cfgFile.getVars().empty(), "empty");

        return b;
    }

    bool test_load_fail_unaccepted_var()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);
        cfgFile.getAcceptedVars().insert("name");
        // since acceptedVars is not empty, load should fail on unknown var "almafa"
        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_unaccepted_var.txt"), "load");
        
        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_accept_missing_var()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);
        cfgFile.getAcceptedVars().insert("name");
        cfgFile.getAcceptedVars().insert("almafa");

        // since acceptedVars is not empty, but we accept missing vars by default, we dont fail on missing "almafa"
        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_missing_var.txt"), "load");

        return b & assertFalse(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name");
    }

    bool test_load_fail_missing_var()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(true);
        cfgFile.getAcceptedVars().insert("name");
        cfgFile.getAcceptedVars().insert("almafa");

        // since acceptedVars is not empty, and we dont accept missing vars, we should fail on missing "almafa"
        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_missing_var.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_double_defined_var()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);

        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_double_defined_var.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_fail_case_sensitive_vars_with_case_insensivity()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);
        cfgFile.getAcceptedVars().insert("name");

        // "Name" and "name" vars are both accepted from case insensitivity perspective but now
        // they are multiple defined vars so we should fail now
        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_fail_case_sensitive_vars_with_case_sensivity()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(true);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);
        cfgFile.getAcceptedVars().insert("Name");

        // "Name" is accepted but "name" is not, since we are case sensitive now and that is unknown var
        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty");
    }

    bool test_load_good_case_sensitive_vars_with_case_sensivity()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(true);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);

        // "Name" is accepted and "name" is also accepted because we are case sensitive now and neither of them is unknown var since we did not define accepted vars!
        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertFalse(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getVars().find("Name") != cfgFile.getVars().end(), "Name") &
            assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name");
    }

    bool test_load_good()
    {
        PGEcfgFile cfgFile;
        cfgFile.SetCaseSensitiveVars(false);
        cfgFile.SetAllAcceptedVarsDefineRequirement(false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty");

        return b;
    }

}; 