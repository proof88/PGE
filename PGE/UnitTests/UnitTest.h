#pragma once

/*
    ###################################################################################
    UnitTest.h
    Basic Unit Test class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

/**
    What I have learned from TDD training:
    - as implementation needs to be more generic than specific, unit test cases need to be more specific than generic;
    - unit tests should express usage of the tested code;
    - name of the unit test cases should be well readable, contain proposals and assertions, e.g.:
      instead of testObject3DCtor(), name should be testThatObjectIsInitiallyEmpty().
	  Jon Jagger recommends snake case naming convention for these long names, e.g. test_that_object_is_initially_empty();
    - unit tests don't need to go down to function-level. I can choose the unit, but still need to avoid heavyweight testing;
    - unit test is not neccessarily small but FAST test (ALL tests execution time <30 seconds);
    - since we are continuously creating tests and doing the implementation, test execution speed is really crucial, fast feedback is needed anytime on-demand;
    - unit tests should offer high code coverage, including error handling paths as well, so both success and failure scenarios;
    - unit tests are automated;
    - unit tests need to be deterministic (no intermittent outcome);
    - unit tests are independent of each other, can be run in arbitrary order;
    - unit tests shouldn't touch database, filesystem, network, etc., instead test doubles (stubs/spies/mocks/fakes/dummies) should be used;
    - if database, filesystem, network, etc. is used so the test is a little bit slower, it can be unit test also, but may be marked as "slow", so
      a different set of tests can be run on-demand (fast tests), and then sometimes all tests including the "slow" tests.
*/

/**
    This class should be the base of every unit tester class.
    It should be used in the following way:
    1. - Create the actual unit test class by deriving from this class (mandatory);
    2. - Override Initialize() (optional):
         it will be called by run() only once before running any test;
    3. - Override setUp() (optional):
         it will be called by run() right before any test (after Initialize());
    4. - Override testMethod() (optional):
         write the actual test code into it; note that it won't be invoked if setUp() failed;
    5. - Override TearDown() (optional):
         it will be called by run() right after any test, also if no test was invoked due to setUp() failed;
    6. - You can create unit-subtests (optional):
         use AddSubTest() in Initialize() or the class constructor to add your subtest methods to the test;
    7. - call run() to run the test(s).
         If the test fails, use getMessage() to get the cause of the failure.
         Any call to AddToMessage() adds message into the string array returned by getMessage().

    General rule: although AddToMessage() can be used manually to save error messages, it is recommended to use
    the assertXXX() functions instead that automatically call AddToMessage() with actual values in case of assertion failure.

    If you decide to make unit-subtests, you should bear in mind the following:
    - AddSubTest() expects a PFNUNITSUBTEST pointer, so the signature of your unit-subtests should match described by PFNUNITSUBTEST:
      bool unitSubTest(void);
    - a unit-subtest should return true on pass and false on fail;
    - it is ok to use multiple assertions in a single subtest but using the optional message parameters of the assertion methods is highly recommended.


    Example unit test class:

        class ColorTest :
            public UnitTest
        {
        public:

            ColorTest() :
                UnitTest( __FILE__ )
            {
                AddSubTest("testCtor", (PFNUNITSUBTEST) &ColorTest::testCtor);
                AddSubTest("testGetRed", (PFNUNITSUBTEST) &ColorTest::testGetRed);
                ...
            }

        private:

            bool testCtor()
            {
                const Color clr(1,2,3,4);
                return assertEquals(1, (int) clr.getRed(), "red") &
                    assertEquals(2, (int) clr.getGreen(), "green") &
                    assertEquals(3, (int) clr.getBlue(), "blue") &
                    assertEquals(4, (int) clr.getAlpha(), "alpha");        
            }

            bool testGetRed()
            {
                const Color clr(1,2,3,4);
                return assertEquals(1, clr.getRed());
            }

            ...

        };
*/

class UnitTest
{
public:

    /**
        @param testFile The file where the test is defined.
        @param testName The name of the test. If empty, itt will be "Unnamed Test".
    */
    UnitTest(const std::string& testFile = "", const std::string& testName = "")
    {
        Reset();
        if ( testName.empty() && testFile.empty() )
            sTestName = "Unnamed Test";
        else
            sTestName = testName;

        if ( !testFile.empty() )
            sTestFile = getFilename(testFile);
    } 


    virtual ~UnitTest()
    {
        sMessages.clear();
        tSubTests.clear();
    } 


    const std::string& getName() const
    {
        return sTestName;
    } 


    const std::string& getFile() const
    {
        return sTestFile;
    }


    /**
        @return Error messages after run().
    */
    const std::vector<std::string>& getMessages() const
    {
        return sMessages;
    }


    /**
        Adds the given error message to the messages.
    */
    void AddToMessages(const char* msg)
    {
        sMessages.push_back(msg);
    }


    /**
        Adds an "Assertion failed!" error message to the messages if the given statement is false.
        
        @param statement The statement being checked.
        @param msg       The optional message to be concatenated to the default error message.

        @return          The value of the given statement.
    */
    bool assertTrue(bool statement, const char* msg = NULL)
    {
        if ( !statement )
        {
            if ( msg == NULL )
            {
                AddToMessages("Assertion failed!");
            }
            else
            {
                AddToMessages(std::string("Assertion failed: ").append(msg).c_str());
            }
        }
        return statement;
    }


    /**
        Same as assertTrue(), except it expects the given statement to be false.

        @param statement The statement being checked.
        @param msg       The optional message to be concatenated to the default error message.

        @return          True if the given statement is false, false otherwise.
    */
    bool assertFalse(bool statement, const char* msg = NULL)
    {
        return assertTrue(!statement, msg);
    }


    /**
        Adds an error message if the given checked value doesn't equal to the expected value.

        @param expected The expected value.
        @param checked  The checked value.
        @param msg      Optional error message.

        @return         True if checked equals to expected, false otherwise.
    */
    template <class T, class S>
    bool assertEquals(const T& expected, const S& checked, const char* msg = NULL)
    {
        std::string asdasd;        
        if ( msg != NULL )
            asdasd = toString(checked).append(" should be ").append(toString(expected)).append(", ").append(msg);

        return msg == NULL ?
            assertTrue( expected == checked,
                        toString(checked).append(" should be ").append(toString(expected)).append("!").c_str()) : 
            assertTrue( expected == checked, asdasd.c_str() );
    }


    /**
        Specialization of assertEquals() for chars to avoid treating 0 as \0, etc.

        @param expected The expected value.
        @param checked  The checked value.
        @param msg      Optional error message.

        @return         True if checked equals to expected, false otherwise.
    */
    bool assertEquals(unsigned char expected, unsigned char checked, const char* msg = NULL)
    {
        return assertEquals(static_cast<int>(expected), static_cast<int>(checked), msg);
    }

    bool assertEquals(char expected, unsigned char checked, const char* msg = NULL)
    {
        return assertEquals(static_cast<int>(expected), static_cast<int>(checked), msg);
    }

    bool assertEquals(unsigned char expected, char checked, const char* msg = NULL)
    {
        return assertEquals(static_cast<int>(expected), static_cast<int>(checked), msg);
    }

    bool assertEquals(char expected, char checked, const char* msg = NULL)
    {
        return assertEquals(static_cast<int>(expected), static_cast<int>(checked), msg);
    }


    /**
        Specialization of assertEquals() for floats. Notice the extra epsilon parameter.

        @param expected The expected value.
        @param checked  The checked value.
        @param epsilon  The maximum allowed difference between the expected and checked value.
        @param msg      Optional error message.

        @return         True if checked value is not farther from expected value than epsilon, false otherwise.
    */
    bool assertEquals(float expected, float checked, float epsilon, const char* msg = NULL)
    {
        return msg == NULL ?
            assertTrue( abs(expected-checked) <= epsilon,
                        toString(checked).append(" should be ").append(toString(expected)).append("!").c_str() ) : 
            assertTrue( abs(expected-checked) <= epsilon,
                        toString(checked).append(" should be ").append(toString(expected)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given checked value equals to the other value.

        @param comparedTo The other value comparing to.
        @param checked    The checked value.
        @param msg        Optional error message.

        @return           True if checked doesn't equal to the other value, false otherwise.
    */
    template <class T, class S>
    bool assertNotEquals(const T& comparedTo, const S& checked, const char* msg = NULL)
    {
        return msg == NULL ?
            assertTrue( comparedTo != checked,
                        toString(checked).append(" should NOT be ").append(toString(comparedTo)).append("!").c_str() ) : 
            assertTrue( comparedTo != checked,
                        toString(checked).append(" should NOT be ").append(toString(comparedTo)).append(", ").append(msg).c_str() );
    }


    /**
        Specialization of assertNotEquals() for chars to avoid treating 0 as \0, etc.

        @param comparedTo The other value comparing to.
        @param checked    The checked value.
        @param msg        Optional error message.

        @return           True if checked doesn't equal to the other value, false otherwise.
    */
    bool assertNotEquals(unsigned char comparedTo, unsigned char checked, const char* msg = NULL)
    {
        return assertNotEquals((int) comparedTo, (int) checked, msg);
    }


    /**
        Specialization of assertNotEquals() for floats. Notice the extra epsilon parameter.

        @param comparedTo The other value comparing to.
        @param checked    The checked value.
        @param epsilon    The maximum disallowed difference between the checked and other value.
        @param msg        Optional error message.

        @return           True if checked value is farther from the other value than epsilon, false otherwise.
    */
    bool assertNotEquals(float comparedTo, float checked, float epsilon, const char* msg = NULL)
    {
        return msg == NULL ?
            assertTrue( abs(comparedTo-checked) > epsilon,
                            toString(checked).append(" should NOT be ").append(toString(comparedTo)).append("!").c_str() ) : 
            assertTrue( abs(comparedTo-checked) > epsilon,
                            toString(checked).append(" should NOT be ").append(toString(comparedTo)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given value is not inside the given interval.

        @param minVal  The start of the interval.
        @param maxVal  The end of the interval. 
        @param checked The checked value.
        @param msg     Optional error message.

        @return True if the given value is inside the given interval, false otherwise.
    */
    template <class T, class S>
    bool assertBetween(const T& minVal, const T& maxVal, const S& checked, const char* msg = NULL)
    {
        return msg == NULL ?
            assertTrue( minVal <= checked && maxVal >= checked,
                        std::string("out of range: ").append(toString(minVal)).append(" <= ").append(
                            toString(checked)).append(" <= ").append(toString(maxVal)).append(" !").c_str() ) : 
            assertTrue( minVal <= checked && maxVal >= checked,
                        std::string("out of range: ").append(toString(minVal)).append(" <= ").append(
                            toString(checked)).append(" <= ").append(toString(maxVal)).append(", ").append(msg).c_str() );
    }


    /**
        Specialization of assertBetween() for floats. Notice the extra epsilon parameter.
        Adds an error message if the given value is not inside the given interval.

        @param minVal  The start of the interval.
        @param maxVal  The end of the interval. 
        @param checked The checked value.
        @param epsilon The maximum allowed absolute difference between the checked value and boundaries of the interval towards outside.
        @param msg     Optional error message.

        @return True if the given value is inside the given interval, false otherwise.
    */
    bool assertBetween(float minVal, float maxVal, float checked, float epsilon, const char* msg = NULL)
    {
        bool b = ( (minVal < checked) || (abs(minVal-checked) <= epsilon) ) &&
                 ( (maxVal > checked) || (abs(maxVal-checked) <= epsilon) );

        return msg == NULL ?
            assertTrue( b,
                        std::string("out of range: ").append(toString(minVal)).append(" <= ").append(
                            toString(checked)).append(" <= ").append(toString(maxVal)).append(" !").c_str() ) : 
            assertTrue( b,
                        std::string("out of range: ").append(toString(minVal)).append(" <= ").append(
                            toString(checked)).append(" <= ").append(toString(maxVal)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given value is not less than the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is less than the other value.
    */
    template <class T, class S>
    bool assertLess(const T& checked, const S& comparedTo, const char* msg = NULL)
    {
        std::string asdasd;        
        if ( msg != NULL )
            asdasd = toString(checked).append(" should be < ").append(toString(comparedTo)).append(", ").append(msg);

        return msg == NULL ?
            assertTrue( checked < comparedTo,
                        toString(checked).append(" should be < ").append(toString(comparedTo)).append("!").c_str()) : 
            assertTrue( checked < comparedTo, asdasd.c_str() );
    }


    /**
        Specialization of assertLess() for chars to avoid treating 0 as \0, etc.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is less than the other value.
    */
    bool assertLess(unsigned char checked, unsigned char comparedTo, const char* msg = NULL)
    {
        return assertLess((int) checked, (int) comparedTo, msg);
    }


    /**
        Specialization of assertLess() for floats.
        Adds an error message if the given value is not less than the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is less than the other value.
    */
    bool assertLess(float checked, float comparedTo, const char* msg = NULL)
    {
        // TODO: currently this is same as the template function, as for some reason we do not use any epsilon in this specialization,
        // probably we can delete this specialization in the future ...
        return msg == NULL ?
            assertTrue( checked < comparedTo,
                        toString(checked).append(" should be < ").append(toString(comparedTo)).append("!").c_str() ) : 
            assertTrue( checked < comparedTo,
                        toString(checked).append(" should be < ").append(toString(comparedTo)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given value is not less than or equal to the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is less than or equal to the other value.
    */
    template <class T, class S>
    bool assertLequals(const T& checked, const S& comparedTo, const char* msg = NULL)
    {
        std::string asdasd;        
        if ( msg != NULL )
            asdasd = toString(checked).append(" should be <= ").append(toString(comparedTo)).append(", ").append(msg);

        return msg == NULL ?
            assertTrue( checked <= comparedTo,
                        toString(checked).append(" should be <= ").append(toString(comparedTo)).append("!").c_str()) : 
            assertTrue( checked <= comparedTo, asdasd.c_str() );
    }


    /**
        Specialization of assertLequals() for chars to avoid treating 0 as \0, etc.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is less than or equal to the other value.
    */
    bool assertLequals(unsigned char checked, unsigned char comparedTo, const char* msg = NULL)
    {
        return assertLequals((int) checked, (int) comparedTo, msg);
    }


    /**
        Specialization of assertLequals() for floats. Notice the extra epsilon parameter.
        Adds an error message if the given value is not less than or equal to the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param epsilon     The maximum allowed difference between the checked and the other value.
        @param msg         Optional error message.

        @return            True if checked value is less than or equal to the other value.
    */
    bool assertLequals(float checked, float comparedTo, float epsilon, const char* msg = NULL)
    {
        return msg == NULL ?
            assertTrue( (checked < comparedTo) || (abs(comparedTo-checked) <= epsilon),
                        toString(checked).append(" should be <= ").append(toString(comparedTo)).append("!").c_str() ) : 
            assertTrue( checked < comparedTo,
                        toString(checked).append(" should be <= ").append(toString(comparedTo)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given value is not greater than the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is greater than the other value.
    */
    template <class T, class S>
    bool assertGreater(const T& checked, const S& comparedTo, const char* msg = NULL)
    {
        std::string asdasd;        
        if ( msg != NULL )
            asdasd = toString(checked).append(" should be > ").append(toString(comparedTo)).append(", ").append(msg);

        return msg == NULL ?
            assertTrue( checked > comparedTo,
                        toString(checked).append(" should be > ").append(toString(comparedTo)).append("!").c_str()) : 
            assertTrue( checked > comparedTo, asdasd.c_str() );
    }


    /**
        Specialization of assertGreater() for chars to avoid treating 0 as \0, etc.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is greater than the other value.
    */
    bool assertGreater(unsigned char checked, unsigned char comparedTo, const char* msg = NULL)
    {
        return assertGreater((int) checked, (int) comparedTo, msg);
    }


    /**
        Specialization of assertGreater() for floats.
        Adds an error message if the given value is not greater than the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is greater than the other value.
    */
    bool assertGreater(float checked, float comparedTo, const char* msg = NULL)
    {
        // TODO: currently this is same as the template function, as for some reason we do not use any epsilon in this specialization,
        // probably we can delete this specialization in the future ...
        return msg == NULL ?
            assertTrue( checked > comparedTo,
                        toString(checked).append(" should be > ").append(toString(comparedTo)).append("!").c_str() ) : 
            assertTrue( checked > comparedTo,
                        toString(checked).append(" should be > ").append(toString(comparedTo)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given value is not greater than or equal to the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is greater than or equal to the other value.
    */
    template <class T, class S>
    bool assertGequals(const T& checked, const S& comparedTo, const char* msg = NULL)
    {
        std::string asdasd;        
        if ( msg != NULL )
            asdasd = toString(checked).append(" should be >= ").append(toString(comparedTo)).append(", ").append(msg);

        return msg == NULL ?
            assertTrue( checked >= comparedTo,
                        toString(checked).append(" should be >= ").append(toString(comparedTo)).append("!").c_str()) : 
            assertTrue( checked >= comparedTo, asdasd.c_str() );
    }


    /**
        Specialization of assertGequals() for chars to avoid treating 0 as \0, etc.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param msg         Optional error message.

        @return            True if checked value is greater than or equal to the other value.
    */
    bool assertGequals(unsigned char checked, unsigned char comparedTo, const char* msg = NULL)
    {
        return assertGequals((int) checked, (int) comparedTo, msg);
    }


    /**
        Specialization of assertGequals() for floats. Notice the extra epsilon parameter.
        Adds an error message if the given value is not greater than or equal to the other.

        @param checked     The checked value.
        @param comparedTo  The other value we compare to.
        @param epsilon     The maximum allowed difference between the checked and the other value.
        @param msg         Optional error message.

        @return            True if checked value is greater than or equal to the other value.
    */
    bool assertGequals(float checked, float comparedTo, float epsilon, const char* msg = NULL)
    {
        return msg == NULL ?
            assertTrue( (checked > comparedTo) || (abs(comparedTo-checked) <= epsilon),
                        toString(checked).append(" should be >= ").append(toString(comparedTo)).append("!").c_str() ) : 
            assertTrue( (checked > comparedTo) || (abs(comparedTo-checked) <= epsilon),
                        toString(checked).append(" should be >= ").append(toString(comparedTo)).append(", ").append(msg).c_str() );
    }


    /**
        Adds an error message if the given pointer is not a null pointer.

        @param checked The checked pointer.
        @param msg     Optional error message.

        @return True if the given pointer is null, false otherwise.
    */
    bool assertNull(const void* checked, const char* msg = NULL)
    {
        return msg == NULL ? assertTrue(checked == (void*)0, "pointer should be NULL") :
                             assertTrue(checked == (void*)0, std::string("pointer should be NULL, ").append(msg).c_str());
    }


    /**
        Adds an error message if the given pointer is a null pointer.

        @param checked The checked pointer.
        @param msg     Optional error message.

        @return True if the given pointer is not null, false otherwise.
    */
    bool assertNotNull(const void* checked, const char* msg = NULL)
    {
        return msg == NULL ? assertTrue(checked != (void*)0, "pointer is NULL") :
                             assertTrue(checked != (void*)0, std::string("pointer is NULL, ").append(msg).c_str());
    }


    /**
        @return True if the test has passed, false otherwise.
    */
    bool isPassed() const
    {
        return bTestRan && sMessages.empty();
    }


    /**
        Executes the test. Runs the overridable testMethod() and every unit-subtests.
        run() calls functions in the following order:
         - Initialize();
         - if ( setUp() )
           - TestMethod();
         - TearDown();
         - for all subtests:
           - if ( setUp() )
             - subtest();
           - TearDown();
         - Finalize();

        @return True if the test passed, false otherwise.
    */
    bool run()
    {
        Reset();
        bTestRan = true;
        Initialize();
        bool bSkipAllSubTests;
        if ( setUp() )
        {
            bSkipAllSubTests = false;
            if (!testMethod())
            {
                AddToMessages(std::string("  <").append(sTestFile).append("> failed!").c_str());
            }
        }
        else
        {
            bSkipAllSubTests = true;
            AddToMessages(std::string("  <").append(sTestFile).append("> setUp() failed!").c_str());
        }
        TearDown();
        if ( !bSkipAllSubTests )
        {
            for (std::vector<TUNITSUBTESTFUNCNAMEPAIR>::size_type i = 0; i < tSubTests.size(); ++i)
            {
                PFNUNITSUBTEST func = tSubTests[i].first;
                if ( setUp() )
                {
                    if ( (this->* func)() )
                        ++nSucceededSubTests;
                    else
                        AddToMessages(std::string("  <").append(tSubTests[i].second).append("> failed!").c_str());
                }
                else
                {
                     AddToMessages(std::string("  <").append(tSubTests[i].second).append("> SKIPPED due to setUp() failed!").c_str());
                }
                TearDown();
            }
        }
        Finalize();
        return isPassed();
    }

    
    /**
        @return Returns the number of subtests in the test.
    */
    int getSubTestCount() const
    {
        return tSubTests.size();
    }


    /**
        @return Returns the number of passed subtests in the test.
    */
    int getPassedSubTestCount() const
    {
        return nSucceededSubTests;
    }        
    

protected:
    typedef bool (UnitTest::* PFNUNITSUBTEST) (void);  /**< Type for a unit-subtest function pointer. */

    // ---------------------------------------------------------------------------

    virtual void Initialize() {}; /**< This gets called before running the test, use this instead of a ctor. */ 

    virtual bool testMethod() { return true; }; /**< This may be overridden in actual unit tests. It won't be invoked if setUp() returned false! */

    virtual bool setUp() { return true; };   /**< This gets called before testMethod() and every subtest. */

    virtual void TearDown() {};   /**< This gets called after testMethod() and every subtest.
                                       This is invoked even if testMethod() or a subtest got skipped due to setUp() returned false! */

    virtual void Finalize() {};   /**< This gets called after running the whole tests, use this instead of a dtor. */

    /**
        Adds the given unit-subtest function with the given name to the test.
    */
    void AddSubTest(const char* subTestName, PFNUNITSUBTEST subTestFunc)
    {
        if ( subTestFunc == NULL )
            return;

        TUNITSUBTESTFUNCNAMEPAIR newPair(subTestFunc, std::string(subTestName));
        tSubTests.push_back(newPair);                              
    } // AddSubTest()

private:
    typedef std::pair<PFNUNITSUBTEST, std::string> TUNITSUBTESTFUNCNAMEPAIR;   /**< Unit-subtest function pointer and subtest name pair. */

    // ---------------------------------------------------------------------------

    static UnitTest utInstance;

    std::string sTestName;                             /**< Name of the test. */
    std::string sTestFile;                             /**< Testfile name. */
    std::vector<std::string> sMessages;                /**< Error messages. */
    std::vector<TUNITSUBTESTFUNCNAMEPAIR> tSubTests;   /**< Unit-subtests, not mandatory. */
    int nSucceededSubTests;                            /**< Number of succeeded unit-subtests. */
    bool bTestRan;                                     /**< Did the test run? */

    // ---------------------------------------------------------------------------

    template <class T>
    static std::string toString(const T& value)
    {
        std::stringstream str;
        str << value;
        return str.str();
    }

    static std::string toString(bool value)
    {
        return value ? "TRUE" : "FALSE";
    }

    static std::string getFilename(const std::string& path)
    {
        return path.substr( path.find_last_of('\\') + 1 );
    }

    /**
        Resets the test so it gets into a rerunnable state.
    */
    void Reset()
    {
        bTestRan = false;
        sMessages.clear();
        nSucceededSubTests = 0;
    }

}; // class UnitTest
