#pragma once

/*
    ###################################################################################
    PGEcfgFileTest.h
    Unit test for PGEcfgFile.
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Config/PGEcfgFile.h"
#include <stdio.h>  // for remove() for deleting file

class PGEcfgFileForcedValidateLoadFailure : public PGEcfgFile
{
public:
    PGEcfgFileForcedValidateLoadFailure(
        bool bRequireAllAcceptedVarsDefineRequirement,
        bool bCaseSensitiveVars) : PGEcfgFile(bRequireAllAcceptedVarsDefineRequirement, bCaseSensitiveVars)
    {} 

protected:
    virtual bool validateOnLoad(std::ifstream&) const override
    {
        return false;
    }
};

class PGEcfgFileForcedValidateSaveFailure : public PGEcfgFile
{
public:
    PGEcfgFileForcedValidateSaveFailure(
        bool bRequireAllAcceptedVarsDefineRequirement,
        bool bCaseSensitiveVars) : PGEcfgFile(bRequireAllAcceptedVarsDefineRequirement, bCaseSensitiveVars)
    {}

protected:
    virtual bool validateOnSave(std::ofstream&) const override
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

    virtual void initialize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        
        addSubTest("test_initially_empty_and_defaults_set", (PFNUNITSUBTEST) &PGEcfgFileTest::test_initially_empty_and_defaults_set);
        addSubTest("test_load_bad_assignment", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_bad_assignment);
        addSubTest("test_load_fail_unaccepted_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_unaccepted_var);
        addSubTest("test_load_accept_missing_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_accept_missing_var);
        addSubTest("test_load_fail_missing_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_missing_var);
        addSubTest("test_load_double_defined_var", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_double_defined_var);
        addSubTest("test_load_fail_case_sensitive_vars_with_case_insensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_case_sensitive_vars_with_case_insensivity);
        addSubTest("test_load_fail_case_sensitive_vars_with_case_sensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_fail_case_sensitive_vars_with_case_sensivity);
        addSubTest("test_load_good_case_sensitive_vars_with_case_sensivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_good_case_sensitive_vars_with_case_sensivity);
        addSubTest("test_load_good_without_case_sensitivity", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_good_without_case_sensitivity);
        addSubTest("test_load_good_with_case_sensitivity", (PFNUNITSUBTEST)&PGEcfgFileTest::test_load_good_with_case_sensitivity);
        addSubTest("test_load_not_allowed_when_already_loaded", (PFNUNITSUBTEST) &PGEcfgFileTest::test_load_not_allowed_when_already_loaded);
        addSubTest("test_set_accepted_vars_not_allowed_when_already_loaded", (PFNUNITSUBTEST) &PGEcfgFileTest::test_set_accepted_vars_not_allowed_when_already_loaded);
        addSubTest("test_override_validateOnLoad", (PFNUNITSUBTEST) &PGEcfgFileTest::test_override_validateOnLoad);
        addSubTest("test_save_fail_nothing_loaded", (PFNUNITSUBTEST)&PGEcfgFileTest::test_save_fail_nothing_loaded);
        addSubTest("test_save_good", (PFNUNITSUBTEST)&PGEcfgFileTest::test_save_good);
        addSubTest("test_override_validateOnSave", (PFNUNITSUBTEST)&PGEcfgFileTest::test_override_validateOnSave);
    }

    virtual void finalize() override
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

    bool test_initially_empty_and_defaults_set()
    {
        PGEcfgFile cfgFile(false, false);
        return assertTrue(cfgFile.getVars().empty(), "empty") &
            assertFalse(cfgFile.getAllAcceptedVarsDefineRequirement(), "getAllAcceptedVarsDefineRequirement") &
            assertFalse(cfgFile.getCaseSensitiveVars(), "getCaseSensitiveVars") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
    }

    bool test_load_bad_assignment()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_bad_assignment.txt"), "load");
        b &= assertTrue(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");

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
        
        return b & assertTrue(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
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
            assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name") &
            assertEquals("cfg_test_load_missing_var.txt", cfgFile.getFilename(), "filename") &
            assertEquals("gamedata/cfgs/", cfgFile.getPathToFile(), "path") &
            assertFalse(cfgFile.getTemplate().empty(), "template");
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

        return b & assertTrue(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
    }

    bool test_load_double_defined_var()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_double_defined_var.txt"), "load");

        return b & assertTrue(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
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

        return b & assertTrue(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
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

        return b & assertTrue(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
    }

    bool test_load_good_case_sensitive_vars_with_case_sensivity()
    {
        PGEcfgFile cfgFile(false, true);
        bool b = assertTrue(cfgFile.getCaseSensitiveVars(), "case");
        
        // "Name" is accepted and "name" is also accepted because we are case sensitive now and neither of them is unknown var since we did not define accepted vars!
        b &= assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_case_insensitive_vars.txt"), "load");

        return b & assertFalse(cfgFile.getVars().empty(), "empty") &
            assertTrue(cfgFile.getVars().find("Name") != cfgFile.getVars().end(), "Name") &
            assertTrue(cfgFile.getVars().find("name") != cfgFile.getVars().end(), "name") &
            assertEquals("cfg_test_load_case_insensitive_vars.txt", cfgFile.getFilename(), "filename") &
            assertEquals("gamedata/cfgs/", cfgFile.getPathToFile(), "path") &
            assertFalse(cfgFile.getTemplate().empty(), "template");
    }

    bool test_load_good_without_case_sensitivity()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty") &
            assertEquals("cfg_test_load_good.txt", cfgFile.getFilename(), "filename") &
            assertEquals("gamedata/cfgs/", cfgFile.getPathToFile(), "path") &
            assertFalse(cfgFile.getTemplate().empty(), "template 1");

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

            b &= assertEquals("", cfgFile.getVars()["name"].getShortHint(), "name short hint") &
                assertEquals(" However, this is a one-liner short explanation.", cfgFile.getVars()["cap_max"].getShortHint(), "cap_max short hint") &
                assertEquals("", cfgFile.getVars()["reload_per_mag"].getShortHint(), "reload_per_mag short hint") &
                assertEquals("", cfgFile.getVars()["reload_time"].getShortHint(), "reload_time short hint") &
                assertEquals("", cfgFile.getVars()["testvar"].getShortHint(), "testVar short hint") &
                assertEquals("", cfgFile.getVars()["testvar2"].getShortHint(), "testVar2 short hint") &
                assertEquals(" Setting a var to empty string is easy, just write nothing after '=':", cfgFile.getVars()["testvar3"].getShortHint(), "testVar3 short hint") &
                assertEquals(" You can also place space chars, they will be preserved:", cfgFile.getVars()["testvar4"].getShortHint(), "testVar4 short hint") &
                assertEquals(" However, values having non-space chars as well, are left-trimmed:", cfgFile.getVars()["testvar5"].getShortHint(), "testVar5 short hint");

            b &= assertTrue(cfgFile.getVars()["name"].getLongHint().empty(), "name long hint") &
                assertTrue(cfgFile.getVars()["reload_per_mag"].getLongHint().empty(), "reload_per_mag long hint") &
                assertTrue(cfgFile.getVars()["reload_time"].getLongHint().empty(), "reload_time long hint") &
                assertTrue(cfgFile.getVars()["testvar"].getLongHint().empty(), "testVar long hint") &
                assertTrue(cfgFile.getVars()["testvar2"].getLongHint().empty(), "testVar2 long hint") &
                assertTrue(cfgFile.getVars()["testvar3"].getLongHint().empty(), "testVar3 long hint") &
                assertTrue(cfgFile.getVars()["testvar4"].getLongHint().empty(), "testVar4 long hint");

            const std::vector<std::string> sExpectedLongHintForCapMax = {
                " Variables can also have a longer, even multi-line more detailed explanation below them.",
                "",
                " This explanation lasts until the next non-comment line."
            };
            b &= assertEquals(sExpectedLongHintForCapMax.size(), cfgFile.getVars()["cap_max"].getLongHint().size(), "cap_max long hint size");
            if (b)
            {
                for (size_t i = 0; i < sExpectedLongHintForCapMax.size(); i++)
                {
                    b &= assertEquals(
                        sExpectedLongHintForCapMax[i],
                        cfgFile.getVars()["cap_max"].getLongHint()[i],
                        (std::string("cap_max long hint string ") + std::to_string(i)).c_str());
                }
            }

            const std::vector<std::string> sExpectedLongHintForTestVar5 = {
                " So this variable will have \"alma    \" stored.",
                " I might change this behavior, but currently I leave it as it is."
            };
            b &= assertEquals(sExpectedLongHintForTestVar5.size(), cfgFile.getVars()["testvar5"].getLongHint().size(), "testvar5 long hint size");
            if (b)
            {
                for (size_t i = 0; i < sExpectedLongHintForTestVar5.size(); i++)
                {
                    b &= assertEquals(
                        sExpectedLongHintForTestVar5[i],
                        cfgFile.getVars()["testvar5"].getLongHint()[i],
                        (std::string("testvar5 long hint string ") + std::to_string(i)).c_str());
                }
            }

            const std::vector<std::string> sExpectedTemplate = {
                "# Lines starting with hashtag are comments and ignored by loader",
                "",
                "# Blank lines are also ignored by loader",
                "",
                "name",
                "",
                "# Variables can have a one-liner short explanation comment right above them.",
                "# This is NOT such a one-liner since this is not right above the next variable.",
                "",
                "cap_max",
                "",
                "reload_per_mag",
                "reload_time",
                "",
                "testvar",
                "",
                "testvar2",
                "",
                "testvar3",
                "",
                "testvar4",
                "",
                "testvar5"
            };
            b &= assertEquals(sExpectedTemplate.size(), cfgFile.getTemplate().size(), "template 2 size");
            if (b)
            {
                for (size_t i = 0; i < sExpectedTemplate.size(); i++)
                {
                    b &= assertEquals(sExpectedTemplate[i], cfgFile.getTemplate()[i], (std::string("template 2 string ") + std::to_string(i)).c_str());
                }
            }
        }

        return b;
    }

    bool test_load_good_with_case_sensitivity()
    {
        PGEcfgFile cfgFile(false, true);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty") &
            assertEquals("cfg_test_load_good.txt", cfgFile.getFilename(), "filename") &
            assertEquals("gamedata/cfgs/", cfgFile.getPathToFile(), "path") &
            assertFalse(cfgFile.getTemplate().empty(), "template 1");

        b &= assertTrue(cfgFile.getVars().find("Name") != cfgFile.getVars().end(), "Name var") &
            assertTrue(cfgFile.getVars().find("cap_max") != cfgFile.getVars().end(), "reload_per_mag var") &
            assertTrue(cfgFile.getVars().find("reload_per_mag") != cfgFile.getVars().end(), "reload_per_mag var") &
            assertTrue(cfgFile.getVars().find("reload_time") != cfgFile.getVars().end(), "reload_time var") &
            assertTrue(cfgFile.getVars().find("testVar") != cfgFile.getVars().end(), "testVar var") &
            assertTrue(cfgFile.getVars().find("testVar2") != cfgFile.getVars().end(), "testVar2 var") &
            assertTrue(cfgFile.getVars().find("testVar3") != cfgFile.getVars().end(), "testVar3 var") &
            assertTrue(cfgFile.getVars().find("testVar4") != cfgFile.getVars().end(), "testVar4 var") &
            assertTrue(cfgFile.getVars().find("testVar5") != cfgFile.getVars().end(), "testVar5 var");

        if (b)
        {
            b &= assertEquals("Sample Weapon 1", cfgFile.getVars()["Name"].getAsString(), "Name value") &
                assertEquals("999", cfgFile.getVars()["cap_max"].getAsString(), "cap_max value") &
                assertEquals("true", cfgFile.getVars()["reload_per_mag"].getAsString(), "reload_per_mag value") &
                assertEquals("3000", cfgFile.getVars()["reload_time"].getAsString(), "reload_time value") &
                assertEquals("Values can have spaces, since they last until end of line!", cfgFile.getVars()["testVar"].getAsString(), "testVar value") &
                assertEquals("Values can have spaces, since they last until end of line!", cfgFile.getVars()["testVar2"].getAsString(), "testVar2 value") &
                assertEquals("", cfgFile.getVars()["testVar3"].getAsString(), "testVar3 value") &
                assertEquals("   ", cfgFile.getVars()["testVar4"].getAsString(), "testVar4 value") &
                assertEquals("alma    ", cfgFile.getVars()["testVar5"].getAsString(), "testVar5 value");

            b &= assertEquals("", cfgFile.getVars()["Name"].getShortHint(), "Name short hint") &
                assertEquals(" However, this is a one-liner short explanation.", cfgFile.getVars()["cap_max"].getShortHint(), "cap_max short hint") &
                assertEquals("", cfgFile.getVars()["reload_per_mag"].getShortHint(), "reload_per_mag short hint") &
                assertEquals("", cfgFile.getVars()["reload_time"].getShortHint(), "reload_time short hint") &
                assertEquals("", cfgFile.getVars()["testVar"].getShortHint(), "testVar short hint") &
                assertEquals("", cfgFile.getVars()["testVar2"].getShortHint(), "testVar2 short hint") &
                assertEquals(" Setting a var to empty string is easy, just write nothing after '=':", cfgFile.getVars()["testVar3"].getShortHint(), "testVar3 short hint") &
                assertEquals(" You can also place space chars, they will be preserved:", cfgFile.getVars()["testVar4"].getShortHint(), "testVar4 short hint") &
                assertEquals(" However, values having non-space chars as well, are left-trimmed:", cfgFile.getVars()["testVar5"].getShortHint(), "testVar5 short hint");

            b &= assertTrue(cfgFile.getVars()["Name"].getLongHint().empty(), "Name long hint") &
                assertTrue(cfgFile.getVars()["reload_per_mag"].getLongHint().empty(), "reload_per_mag long hint") &
                assertTrue(cfgFile.getVars()["reload_time"].getLongHint().empty(), "reload_time long hint") &
                assertTrue(cfgFile.getVars()["testVar"].getLongHint().empty(), "testVar long hint") &
                assertTrue(cfgFile.getVars()["testVar2"].getLongHint().empty(), "testVar2 long hint") &
                assertTrue(cfgFile.getVars()["testVar3"].getLongHint().empty(), "testVar3 long hint") &
                assertTrue(cfgFile.getVars()["testVar4"].getLongHint().empty(), "testVar4 long hint");

            const std::vector<std::string> sExpectedLongHintForCapMax = {
                " Variables can also have a longer, even multi-line more detailed explanation below them.",
                "",
                " This explanation lasts until the next non-comment line."
            };
            b &= assertEquals(sExpectedLongHintForCapMax.size(), cfgFile.getVars()["cap_max"].getLongHint().size(), "cap_max long hint size");
            if (b)
            {
                for (size_t i = 0; i < sExpectedLongHintForCapMax.size(); i++)
                {
                    b &= assertEquals(
                        sExpectedLongHintForCapMax[i],
                        cfgFile.getVars()["cap_max"].getLongHint()[i],
                        (std::string("cap_max long hint string ") + std::to_string(i)).c_str());
                }
            }

            const std::vector<std::string> sExpectedLongHintForTestVar5 = {
                " So this variable will have \"alma    \" stored.",
                " I might change this behavior, but currently I leave it as it is."
            };
            b &= assertEquals(sExpectedLongHintForTestVar5.size(), cfgFile.getVars()["testVar5"].getLongHint().size(), "testVar5 long hint size");
            if (b)
            {
                for (size_t i = 0; i < sExpectedLongHintForTestVar5.size(); i++)
                {
                    b &= assertEquals(
                        sExpectedLongHintForTestVar5[i],
                        cfgFile.getVars()["testVar5"].getLongHint()[i],
                        (std::string("testVar5 long hint string ") + std::to_string(i)).c_str());
                }
            }

            const std::vector<std::string> sExpectedTemplate = {
                "# Lines starting with hashtag are comments and ignored by loader",
                "",
                "# Blank lines are also ignored by loader",
                "",
                "Name",
                "",
                "# Variables can have a one-liner short explanation comment right above them.",
                "# This is NOT such a one-liner since this is not right above the next variable.",
                "",
                "cap_max",
                "",
                "reload_per_mag",
                "reload_time",
                "",
                "testVar",
                "",
                "testVar2",
                "",
                "testVar3",
                "",
                "testVar4",
                "",
                "testVar5"
            };
            b &= assertEquals(sExpectedTemplate.size(), cfgFile.getTemplate().size(), "template 2 size");
            if (b)
            {
                for (size_t i = 0; i < sExpectedTemplate.size(); i++)
                {
                    b &= assertEquals(sExpectedTemplate[i], cfgFile.getTemplate()[i], (std::string("template 2 string ") + std::to_string(i)).c_str());
                }
            }
        }

        return b;
    }

    bool test_load_not_allowed_when_already_loaded()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty");
        
        b &= assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load");
        b &= assertFalse(cfgFile.getVars().empty(), "not empty") &
            assertEquals("cfg_test_load_good.txt", cfgFile.getFilename(), "filename") &
            assertEquals("gamedata/cfgs/", cfgFile.getPathToFile(), "path") &
            assertFalse(cfgFile.getTemplate().empty(), "template");

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
        PGEcfgFileForcedValidateLoadFailure cfgFile(false, false);
        bool b = assertFalse(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load") &
            assertTrue(cfgFile.getFilename().empty(), "filename") &
            assertTrue(cfgFile.getPathToFile().empty(), "path") &
            assertTrue(cfgFile.getTemplate().empty(), "template");
        
        return b;
    }

    bool test_save_fail_nothing_loaded()
    {
        PGEcfgFile cfgFile(false, false);

        return assertFalse(cfgFile.save("asdasdasd.txt"), "file name specified") &
            assertFalse(cfgFile.save(""), "without filename");
    }

    bool assertTemplateEquals(
        const std::vector<std::string>& vsExpected,
        const std::vector<std::string>& vsChecked,
        const std::string& sMessage)
    {
        bool b = assertTrue(
            vsExpected.size() == vsChecked.size() ||
            /* it is allowed for the written file to have 1 extra last empty line! */
            (vsExpected.size() == (vsChecked.size() - 1)), "template size");

        if (b)
        {
            for (size_t i = 0; i < vsExpected.size(); i++)
            {
                b &= assertEquals(vsExpected[i], vsChecked[i], (std::string("template line i: ") + std::to_string(i)).c_str());
            }
            if ((vsExpected.size() == (vsChecked.size() - 1)))
            {
                b &= assertTrue(vsChecked[vsChecked.size() - 1].empty(), "template last line empty");
            }
        }

        return assertTrue(b, sMessage.c_str());
    }

    bool assertVarsEquals(
        const std::map<std::string, PGEcfgVariable>& mapVarsExpected,
        const std::map<std::string, PGEcfgVariable>& mapVarsChecked,
        const std::string& sMessage)
    {
        bool b = assertEquals(mapVarsExpected.size(), mapVarsChecked.size(), "vars size");

        if (b)
        {
            auto itExpected = mapVarsExpected.begin();
            auto itChecked = mapVarsChecked.begin();

            while ((itExpected != mapVarsExpected.end()) && (itChecked != mapVarsChecked.end()))
            {
                b &= assertEquals(itExpected->first, itChecked->first, "var name");
                b &= assertTrue(
                    itExpected->second == itChecked->second,
                    (std::string("var value: \"") + itExpected->second.getAsString() + "\" vs \"" + itChecked->second.getAsString() + "\"").c_str());
                
                ++itExpected;
                ++itChecked;
            }
        }

        return assertTrue(b, sMessage.c_str());
    }

    bool test_save_good()
    {
        PGEcfgFile cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load initial");

        if (!b)
        {
            return false;
        }

        const auto originalVars = cfgFile.getVars();
        const auto originalTemplate = cfgFile.getTemplate();

        constexpr char* szFileWeWrite = "gamedata/cfgs/cfg_test_save_good.txt";
        remove(szFileWeWrite); // make sure it is not already there

        // first test saving to a file which we never read in any other test case
        b &= assertTrue(cfgFile.save(szFileWeWrite), "save 1");
        if (!b)
        {
            return false;
        }

        cfgFile.getVars().clear();
        cfgFile.getTemplate().clear();
        b &= assertTrue(cfgFile.load(szFileWeWrite), "load 1");
        b &= assertVarsEquals(originalVars, cfgFile.getVars(), "getVars 1");
        b &= assertTemplateEquals(originalTemplate, cfgFile.getTemplate(), "template 1");

        if (!b)
        {
            // in this case dont delete the file so it is easier to debug if it is left on disk
            return false;
        }

        b &= assertEquals(0, remove(szFileWeWrite), "file delete");
        if (!b)
        {
            // if we cannot delete the file, the remaining tests make no sense since we want to write again from scratch
            return false;
        }

        // now we can test saving to the same loaded file by NOT specifying file name
        b &= assertTrue(cfgFile.save(), "save 2");
        if (!b)
        {
            return false;
        }

        cfgFile.getVars().clear();
        cfgFile.getTemplate().clear();
        b &= assertTrue(cfgFile.load(szFileWeWrite), "load 2");
        b &= assertVarsEquals(originalVars, cfgFile.getVars(), "getVars 2");
        b &= assertTemplateEquals(originalTemplate, cfgFile.getTemplate(), "template 2");

        if (b)
        {
            remove(szFileWeWrite);
        }

        return b;
    }

    bool test_override_validateOnSave()
    {
        PGEcfgFileForcedValidateSaveFailure cfgFile(false, false);

        bool b = assertTrue(cfgFile.load("gamedata/cfgs/cfg_test_load_good.txt"), "load initial");

        if (!b)
        {
            return false;
        }

        constexpr char* szFileWeWrite = "gamedata/cfgs/cfg_test_save_good.txt";
        remove(szFileWeWrite); // make sure it is not already there

        b &= assertFalse(cfgFile.save(szFileWeWrite), "save");

        if (b)
        {
            remove(szFileWeWrite);
        }

        return b;
    }

};
