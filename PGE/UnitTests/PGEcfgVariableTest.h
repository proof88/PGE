#pragma once

/*
    ###################################################################################
    PGEcfgVariableTest.h
    Unit test for PGEcfgVariable.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Config/PGEcfgVariable.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PGEcfgVariableTest :
    public UnitTest
{
public:

    PGEcfgVariableTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PGEcfgVariableTest::testCtor);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PGEcfgVariableTest::testCopyCtor);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PGEcfgVariableTest::testCtor2);
        AddSubTest("testCtor3", (PFNUNITSUBTEST) &PGEcfgVariableTest::testCtor3);
        AddSubTest("testCtor4", (PFNUNITSUBTEST) &PGEcfgVariableTest::testCtor4);
        AddSubTest("testCtor5", (PFNUNITSUBTEST) &PGEcfgVariableTest::testCtor5);
        AddSubTest("testGetAsInt", (PFNUNITSUBTEST) &PGEcfgVariableTest::testGetAsInt);
        AddSubTest("testGetAsFloat", (PFNUNITSUBTEST) &PGEcfgVariableTest::testGetAsFloat);
        AddSubTest("testGetAsBool", (PFNUNITSUBTEST) &PGEcfgVariableTest::testGetAsBool);
        AddSubTest("testGetAsString", (PFNUNITSUBTEST) &PGEcfgVariableTest::testGetAsString);
        AddSubTest("testSet1", (PFNUNITSUBTEST) &PGEcfgVariableTest::testSet1);
        AddSubTest("testSet2", (PFNUNITSUBTEST) &PGEcfgVariableTest::testSet2);
        AddSubTest("testSet3", (PFNUNITSUBTEST) &PGEcfgVariableTest::testSet3);
        AddSubTest("testSet4", (PFNUNITSUBTEST) &PGEcfgVariableTest::testSet4);
        AddSubTest("testOperatorEquals1", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorEquals1);
        AddSubTest("testOperatorEquals2", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorEquals2);
        AddSubTest("testOperatorEquals3", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorEquals3);
        AddSubTest("testOperatorEquals4", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorEquals4);
        AddSubTest("testOperatorNotEquals1", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorNotEquals1);
        AddSubTest("testOperatorNotEquals2", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorNotEquals2);
        AddSubTest("testOperatorNotEquals3", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorNotEquals3);
        AddSubTest("testOperatorNotEquals4", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorNotEquals4);
        AddSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorAssignment);
        AddSubTest("testOperatorAddition", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorAddition);
        AddSubTest("testOperatorAdditionAssignment", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorAdditionAssignment);
        AddSubTest("testOperatorSubtraction", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorSubtraction);
        AddSubTest("testOperatorSubtractionAssignment", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorSubtractionAssignment);
        AddSubTest("testOperatorMultiplicationByScalar1", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorMultiplicationByScalar);
        AddSubTest("testOperatorMultiplicationAssignmentByScalar", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorMultiplicationAssignmentByScalar);
        AddSubTest("testOperatorDivisionByScalar", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorDivisionByScalar);
        AddSubTest("testOperatorDivisionAssignmentByScalar", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorDivisionAssignmentByScalar);
        AddSubTest("testOperatorModularDivisionByScalar", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorModularDivisionByScalar);
        AddSubTest("testOperatorModularDivisionAssignmentByScalar", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorModularDivisionAssignmentByScalar);
        AddSubTest("testOperatorIncrementPrefix", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorIncrementPrefix);
        AddSubTest("testOperatorIncrementPostfix", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorIncrementPostfix);
        AddSubTest("testOperatorDecrementPrefix", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorDecrementPrefix);
        AddSubTest("testOperatorDecrementPostfix", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorDecrementPostfix);
        AddSubTest("testOperatorNegative", (PFNUNITSUBTEST) &PGEcfgVariableTest::testOperatorNegative);
    } // PGEcfgVariableTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PGEcfgVariableTest(const PGEcfgVariableTest&)
    {};         

    PGEcfgVariableTest& operator=(const PGEcfgVariableTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PGEcfgVariable var;

        return assertEquals(0, var.getAsInt(), "int") &
            assertEquals(0.0f, var.getAsFloat(), E, "float") &
            assertFalse(var.getAsBool(), "bool") &
            assertTrue(var.getAsString().empty(), "string") &
            assertEquals(var.getType(), PGE_CVAR_STRING, "type");
    }

    bool testCopyCtor()
    {
        const PGEcfgVariable var("1");
        const PGEcfgVariable var2(var);
        const PGEcfgVariable var3(2.6f);
        const PGEcfgVariable var4(var3);

        return assertTrue(var == var2) & assertTrue(var3 == var4);
    }

    bool testCtor2()
    {
        const PGEcfgVariable var("alma");

        return assertEquals("alma", var.getAsString(), "string") &
            assertEquals(var.getType(), PGE_CVAR_STRING, "type");
    }

    bool testCtor3()
    {
        const PGEcfgVariable var(1);

        return assertEquals(1, var.getAsInt(), "int") &
            assertEquals(var.getType(), PGE_CVAR_INT, "type");
    }

    bool testCtor4()
    {
        const PGEcfgVariable var(1.5f);

        return assertEquals(1.5f, var.getAsFloat(), E, "float") &
            assertEquals(var.getType(), PGE_CVAR_FLOAT, "type");
    }

    bool testCtor5()
    {
        const PGEcfgVariable var(true);

        return assertTrue(var.getAsBool(), "bool") &
            assertEquals(var.getType(), PGE_CVAR_BOOL, "type");
    }

    bool testGetAsInt()
    {
        PGEcfgVariable var("2");
        bool b = assertEquals(2, var.getAsInt(), "b1");

        var.Set("alma");
        b &= assertEquals(0, var.getAsInt(), "b2");

        var.Set(true);
        b &= assertEquals(1, var.getAsInt(), "b3");

        var.Set(false);
        b &= assertEquals(0, var.getAsInt(), "b4");

        var.Set(2.5f);
        b &= assertEquals(2, var.getAsInt(), "b5");

        return b;
    }

    bool testGetAsFloat()
    {
        PGEcfgVariable var("2.23");
        bool b = assertEquals(2.23f, var.getAsFloat(), E, "b1");

        var.Set("alma");
        b &= assertEquals(0.0f, var.getAsFloat(), E, "b2");

        var.Set(true);
        b &= assertEquals(1, var.getAsFloat(), E, "b3");

        var.Set(false);
        b &= assertEquals(0, var.getAsFloat(), E, "b4");

        return b;
    }

    bool testGetAsBool()
    {
        PGEcfgVariable var("2");
        bool b = assertTrue(var.getAsBool(), "b1");

        var.Set("true");
        b &= assertTrue(var.getAsBool(), "b2");

        var.Set("tRuE");
        b &= assertTrue(var.getAsBool(), "b3");

        var.Set("false");
        b &= assertFalse(var.getAsBool(), "b4");

        var.Set("fAlSe");
        b &= assertFalse(var.getAsBool(), "b5");

        var.Set(0);
        b &= assertFalse(var.getAsBool(), "b6");

        var.Set(-4);
        b &= assertTrue(var.getAsBool(), "b7");

        var.Set(5.6f);
        b &= assertTrue(var.getAsBool(), "b8");

        var.Set(-0.0f);
        b &= assertFalse(var.getAsBool(), "b9");

        var.Set(true);
        b &= assertTrue(var.getAsBool(), "b10");

        var.Set(false);
        b &= assertFalse(var.getAsBool(), "b11");

        return b;
    }

    bool testGetAsString()
    {
        PGEcfgVariable var;
        bool b = assertTrue(var.getAsString().empty(), "b1");

        var.Set("alma");
        b &= assertEquals("alma", var.getAsString(), "b2");

        var.Set(2);
        b &= assertEquals("2", var.getAsString(), "b3");

        var.Set(2.45f);
        b &= assertEquals("2.45", var.getAsString(), "b4");

        var.Set(true);
        b &= assertEquals("true", var.getAsString(), "b5");

        var.Set(false);
        b &= assertEquals("false", var.getAsString(), "b6");

        return b;
    }

    bool testSet1()
    {
        PGEcfgVariable var;
        var.Set("alma");
        
        return assertEquals("alma", var.getAsString(), "string") &
            assertEquals(var.getType(), PGE_CVAR_STRING, "type");
    }

    bool testSet2()
    {
        PGEcfgVariable var;
        var.Set(1);

        return assertEquals(1, var.getAsInt(), "int") &
            assertEquals(var.getType(), PGE_CVAR_INT, "type");
    }

    bool testSet3()
    {
        PGEcfgVariable var;
        var.Set(1.5f);

        return assertEquals(1.5f, var.getAsFloat(), E, "float") &
            assertEquals(var.getType(), PGE_CVAR_FLOAT, "type");
    }

    bool testSet4()
    {
        PGEcfgVariable var;
        var.Set(true);

        return assertTrue(var.getAsBool(), "bool") &
            assertEquals(var.getType(), PGE_CVAR_BOOL, "type");
    }

    bool testOperatorEquals1()
    {
        // test for int-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");

        const bool bCommutativeTest0 = ( varIntType == varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varIntType == varIntType2.getAsInt() );
        const bool bCommutativeTest2 = ( varIntType == varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varIntType == varFloatType2.getAsFloat() );
        const bool bCommutativeTest4 = ( varIntType == varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varIntType == varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varIntType == varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varIntType == varStringType2.getAsString() );

        return assertTrue(varIntType == varIntType, "b1") &
            assertFalse(varIntType == varIntType2, "b2") &
            assertFalse(varIntType == varFloatType, "b3") &
            assertFalse(varIntType == varFloatType2, "b4") &
            assertFalse(varIntType == varBoolType, "b5") &
            assertFalse(varIntType == varBoolType2, "b6") &
            assertFalse(varIntType == varStringType, "b7") &
            assertFalse(varIntType == varStringType2, "b8") &
            assertTrue(bCommutativeTest0, "b9") &
            assertFalse(bCommutativeTest1, "b10") &
            assertTrue(bCommutativeTest2, "b11") &
            assertFalse(bCommutativeTest3, "b12") &
            assertTrue(bCommutativeTest4, "b13") &
            assertFalse(bCommutativeTest5, "b14") &
            assertTrue(bCommutativeTest6, "b15") &
            assertFalse(bCommutativeTest7, "b16") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() == varIntType), "b17") &
            assertEquals(bCommutativeTest1, (varIntType2.getAsInt() == varIntType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() == varIntType), "b19") &
            assertEquals(bCommutativeTest3, (varFloatType2.getAsFloat() == varIntType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() == varIntType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() == varIntType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() == varIntType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() == varIntType), "b24");
    }

    bool testOperatorEquals2()
    {
        // test for float-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");

        const bool bCommutativeTest0 = ( varFloatType == varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varFloatType == varIntType2.getAsInt() );
        const bool bCommutativeTest2 = ( varFloatType == varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varFloatType == varFloatType2.getAsFloat() );
        const bool bCommutativeTest4 = ( varFloatType == varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varFloatType == varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varFloatType == varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varFloatType == varStringType2.getAsString() );

        return assertFalse(varFloatType == varIntType, "b1") &
            assertFalse(varFloatType == varIntType2, "b2") &
            assertTrue(varFloatType == varFloatType, "b3") &
            assertFalse(varFloatType == varFloatType2, "b4") &
            assertFalse(varFloatType == varBoolType, "b5") &
            assertFalse(varFloatType == varBoolType2, "b6") &
            assertFalse(varFloatType == varStringType, "b7") &
            assertFalse(varFloatType == varStringType2, "b8") &
            assertTrue(bCommutativeTest0, "b9") &
            assertFalse(bCommutativeTest1, "b10") &
            assertTrue(bCommutativeTest2, "b11") &
            assertFalse(bCommutativeTest3, "b12") &
            assertTrue(bCommutativeTest4, "b13") &
            assertFalse(bCommutativeTest5, "b14") &
            assertTrue(bCommutativeTest6, "b15") &
            assertFalse(bCommutativeTest7, "b16") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() == varFloatType), "b17") &
            assertEquals(bCommutativeTest1, (varIntType2.getAsInt() == varFloatType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() == varFloatType), "b19") &
            assertEquals(bCommutativeTest3, (varFloatType2.getAsFloat() == varFloatType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() == varFloatType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() == varFloatType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() == varFloatType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() == varFloatType), "b24");
    }

    bool testOperatorEquals3()
    {
        // test for bool-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");

        const bool bCommutativeTest0 = ( varBoolType == varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varBoolType == 0 );
        const bool bCommutativeTest2 = ( varBoolType == varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varBoolType == 0.0f );
        const bool bCommutativeTest4 = ( varBoolType == varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varBoolType == varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varBoolType == varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varBoolType == varStringType2.getAsString() );
        const bool bCommutativeTest8 = ( varBoolType == std::string("true") );
        const bool bCommutativeTest9 = ( varBoolType == std::string("false") );

        return assertFalse(varBoolType == varIntType, "b1") &
            assertFalse(varBoolType == varIntType2, "b2") &
            assertFalse(varBoolType == varFloatType, "b3") &
            assertFalse(varBoolType == varFloatType2, "b4") &
            assertTrue(varBoolType == varBoolType, "b5") &
            assertFalse(varBoolType == varBoolType2, "b6") &
            assertFalse(varBoolType == varStringType, "b7") &
            assertFalse(varBoolType == varStringType2, "b8") &
            assertTrue(bCommutativeTest0, "b9") &
            assertFalse(bCommutativeTest1, "b10") &
            assertTrue(bCommutativeTest2, "b11") &
            assertFalse(bCommutativeTest3, "b12") &
            assertTrue(bCommutativeTest4, "b13") &
            assertFalse(bCommutativeTest5, "b14") &
            assertTrue(bCommutativeTest6, "b15") &
            assertFalse(bCommutativeTest7, "b16") &
            assertTrue(bCommutativeTest8, "b15A") &
            assertFalse(bCommutativeTest9, "b16A") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() == varBoolType), "b17") &
            assertEquals(bCommutativeTest1, (0 == varBoolType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() == varBoolType), "b19") &
            assertEquals(bCommutativeTest3, (0.0f == varBoolType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() == varBoolType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() == varBoolType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() == varBoolType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() == varBoolType), "b24") &
            assertEquals(bCommutativeTest8, (std::string("true") == varBoolType), "b25") &
            assertEquals(bCommutativeTest9, (std::string("false") == varBoolType), "b26");
    }

    bool testOperatorEquals4()
    {
        // test for string-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");
        const PGEcfgVariable varStringType3("true");
        const PGEcfgVariable varStringType4("false");

        const bool bCommutativeTest0 = ( varStringType == varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varStringType == varIntType2.getAsInt() );
        const bool bCommutativeTest2 = ( varStringType == varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varStringType == varFloatType2.getAsFloat() );
        const bool bCommutativeTest4 = ( varStringType == varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varStringType == varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varStringType == varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varStringType == varStringType2.getAsString() );
        const bool bCommutativeTest8 = ( varStringType3 == varBoolType.getAsBool() );
        const bool bCommutativeTest9 = ( varStringType4 == varBoolType.getAsBool() );

        return assertFalse(varStringType == varIntType, "b1") &
            assertFalse(varStringType == varIntType2, "b2") &
            assertFalse(varStringType == varFloatType, "b3") &
            assertFalse(varStringType == varFloatType2, "b4") &
            assertFalse(varStringType == varBoolType, "b5") &
            assertFalse(varStringType == varBoolType2, "b6") &
            assertTrue(varStringType == varStringType, "b7") &
            assertFalse(varStringType == varStringType2, "b8") &
            assertTrue(bCommutativeTest0, "b9") &
            assertFalse(bCommutativeTest1, "b10") &
            assertTrue(bCommutativeTest2, "b11") &
            assertFalse(bCommutativeTest3, "b12") &
            assertTrue(bCommutativeTest4, "b13") &
            assertFalse(bCommutativeTest5, "b14") &
            assertTrue(bCommutativeTest6, "b15") &
            assertFalse(bCommutativeTest7, "b16") &
            assertTrue(bCommutativeTest8, "b15A") &
            assertFalse(bCommutativeTest9, "b16A") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() == varStringType), "b17") &
            assertEquals(bCommutativeTest1, (varIntType2.getAsInt() == varStringType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() == varStringType), "b19") &
            assertEquals(bCommutativeTest3, (varFloatType2.getAsFloat() == varStringType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() == varStringType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() == varStringType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() == varStringType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() == varStringType), "b24") &
            assertEquals(bCommutativeTest8, (varBoolType.getAsBool() == varStringType3), "b25") &
            assertEquals(bCommutativeTest9, (varBoolType.getAsBool() == varStringType4), "b26");
    }

    bool testOperatorNotEquals1()
    {
        // test for int-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");

        const bool bCommutativeTest0 = ( varIntType != varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varIntType != varIntType2.getAsInt() );
        const bool bCommutativeTest2 = ( varIntType != varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varIntType != varFloatType2.getAsFloat() );
        const bool bCommutativeTest4 = ( varIntType != varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varIntType != varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varIntType != varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varIntType != varStringType2.getAsString() );

        return assertFalse(varIntType != varIntType, "b1") &
            assertTrue(varIntType != varIntType2, "b2") &
            assertTrue(varIntType != varFloatType, "b3") &
            assertTrue(varIntType != varFloatType2, "b4") &
            assertTrue(varIntType != varBoolType, "b5") &
            assertTrue(varIntType != varBoolType2, "b6") &
            assertTrue(varIntType != varStringType, "b7") &
            assertTrue(varIntType != varStringType2, "b8") &
            assertFalse(bCommutativeTest0, "b9") &
            assertTrue(bCommutativeTest1, "b10") &
            assertFalse(bCommutativeTest2, "b11") &
            assertTrue(bCommutativeTest3, "b12") &
            assertFalse(bCommutativeTest4, "b13") &
            assertTrue(bCommutativeTest5, "b14") &
            assertFalse(bCommutativeTest6, "b15") &
            assertTrue(bCommutativeTest7, "b16") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() != varIntType), "b17") &
            assertEquals(bCommutativeTest1, (varIntType2.getAsInt() != varIntType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() != varIntType), "b19") &
            assertEquals(bCommutativeTest3, (varFloatType2.getAsFloat() != varIntType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() != varIntType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() != varIntType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() != varIntType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() != varIntType), "b24");
    }

    bool testOperatorNotEquals2()
    {
        // test for float-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");

        const bool bCommutativeTest0 = ( varFloatType != varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varFloatType != varIntType2.getAsInt() );
        const bool bCommutativeTest2 = ( varFloatType != varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varFloatType != varFloatType2.getAsFloat() );
        const bool bCommutativeTest4 = ( varFloatType != varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varFloatType != varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varFloatType != varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varFloatType != varStringType2.getAsString() );

        return assertTrue(varFloatType != varIntType, "b1") &
            assertTrue(varFloatType != varIntType2, "b2") &
            assertFalse(varFloatType != varFloatType, "b3") &
            assertTrue(varFloatType != varFloatType2, "b4") &
            assertTrue(varFloatType != varBoolType, "b5") &
            assertTrue(varFloatType != varBoolType2, "b6") &
            assertTrue(varFloatType != varStringType, "b7") &
            assertTrue(varFloatType != varStringType2, "b8") &
            assertFalse(bCommutativeTest0, "b9") &
            assertTrue(bCommutativeTest1, "b10") &
            assertFalse(bCommutativeTest2, "b11") &
            assertTrue(bCommutativeTest3, "b12") &
            assertFalse(bCommutativeTest4, "b13") &
            assertTrue(bCommutativeTest5, "b14") &
            assertFalse(bCommutativeTest6, "b15") &
            assertTrue(bCommutativeTest7, "b16") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() != varFloatType), "b17") &
            assertEquals(bCommutativeTest1, (varIntType2.getAsInt() != varFloatType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() != varFloatType), "b19") &
            assertEquals(bCommutativeTest3, (varFloatType2.getAsFloat() != varFloatType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() != varFloatType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() != varFloatType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() != varFloatType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() != varFloatType), "b24");
    }

    bool testOperatorNotEquals3()
    {
        // test for bool-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");

        const bool bCommutativeTest0 = ( varBoolType != varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varBoolType != 0 );
        const bool bCommutativeTest2 = ( varBoolType != varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varBoolType != 0.0f );
        const bool bCommutativeTest4 = ( varBoolType != varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varBoolType != varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varBoolType != varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varBoolType != varStringType2.getAsString() );
        const bool bCommutativeTest8 = ( varBoolType != std::string("true") );
        const bool bCommutativeTest9 = ( varBoolType != std::string("false") );

        return assertTrue(varBoolType != varIntType, "b1") &
            assertTrue(varBoolType != varIntType2, "b2") &
            assertTrue(varBoolType != varFloatType, "b3") &
            assertTrue(varBoolType != varFloatType2, "b4") &
            assertFalse(varBoolType != varBoolType, "b5") &
            assertTrue(varBoolType != varBoolType2, "b6") &
            assertTrue(varBoolType != varStringType, "b7") &
            assertTrue(varBoolType != varStringType2, "b8") &
            assertFalse(bCommutativeTest0, "b9") &
            assertTrue(bCommutativeTest1, "b10") &
            assertFalse(bCommutativeTest2, "b11") &
            assertTrue(bCommutativeTest3, "b12") &
            assertFalse(bCommutativeTest4, "b13") &
            assertTrue(bCommutativeTest5, "b14") &
            assertFalse(bCommutativeTest6, "b15") &
            assertTrue(bCommutativeTest7, "b16") &
            assertFalse(bCommutativeTest8, "b15A") &
            assertTrue(bCommutativeTest9, "b16A") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() != varBoolType), "b17") &
            assertEquals(bCommutativeTest1, (0 != varBoolType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() != varBoolType), "b19") &
            assertEquals(bCommutativeTest3, (0.0f != varBoolType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() != varBoolType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() != varBoolType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() != varBoolType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() != varBoolType), "b24") &
            assertEquals(bCommutativeTest8, (std::string("true") != varBoolType), "b25") &
            assertEquals(bCommutativeTest9, (std::string("false") != varBoolType), "b26");
    }

    bool testOperatorNotEquals4()
    {
        // test for string-typed cvar
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varIntType2(4);
        const PGEcfgVariable varFloatType(2.0f);
        const PGEcfgVariable varFloatType2(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varBoolType2(false);
        const PGEcfgVariable varStringType("2");
        const PGEcfgVariable varStringType2("asd");
        const PGEcfgVariable varStringType3("true");
        const PGEcfgVariable varStringType4("false");

        const bool bCommutativeTest0 = ( varStringType != varIntType.getAsInt() );
        const bool bCommutativeTest1 = ( varStringType != varIntType2.getAsInt() );
        const bool bCommutativeTest2 = ( varStringType != varFloatType.getAsFloat() );
        const bool bCommutativeTest3 = ( varStringType != varFloatType2.getAsFloat() );
        const bool bCommutativeTest4 = ( varStringType != varBoolType.getAsBool() );
        const bool bCommutativeTest5 = ( varStringType != varBoolType2.getAsBool() );
        const bool bCommutativeTest6 = ( varStringType != varStringType.getAsString() );
        const bool bCommutativeTest7 = ( varStringType != varStringType2.getAsString() );
        const bool bCommutativeTest8 = ( varStringType3 != varBoolType.getAsBool() );
        const bool bCommutativeTest9 = ( varStringType4 != varBoolType.getAsBool() );

        return assertTrue(varStringType != varIntType, "b1") &
            assertTrue(varStringType != varIntType2, "b2") &
            assertTrue(varStringType != varFloatType, "b3") &
            assertTrue(varStringType != varFloatType2, "b4") &
            assertTrue(varStringType != varBoolType, "b5") &
            assertTrue(varStringType != varBoolType2, "b6") &
            assertFalse(varStringType != varStringType, "b7") &
            assertTrue(varStringType != varStringType2, "b8") &
            assertFalse(bCommutativeTest0, "b9") &
            assertTrue(bCommutativeTest1, "b10") &
            assertFalse(bCommutativeTest2, "b11") &
            assertTrue(bCommutativeTest3, "b12") &
            assertFalse(bCommutativeTest4, "b13") &
            assertTrue(bCommutativeTest5, "b14") &
            assertFalse(bCommutativeTest6, "b15") &
            assertTrue(bCommutativeTest7, "b16") &
            assertFalse(bCommutativeTest8, "b15A") &
            assertTrue(bCommutativeTest9, "b16A") &
            assertEquals(bCommutativeTest0, (varIntType.getAsInt() != varStringType), "b17") &
            assertEquals(bCommutativeTest1, (varIntType2.getAsInt() != varStringType), "b18") &
            assertEquals(bCommutativeTest2, (varFloatType.getAsFloat() != varStringType), "b19") &
            assertEquals(bCommutativeTest3, (varFloatType2.getAsFloat() != varStringType), "b20") &
            assertEquals(bCommutativeTest4, (varBoolType.getAsBool() != varStringType), "b21") &
            assertEquals(bCommutativeTest5, (varBoolType2.getAsBool() != varStringType), "b22") &
            assertEquals(bCommutativeTest6, (varStringType.getAsString() != varStringType), "b23") &
            assertEquals(bCommutativeTest7, (varStringType2.getAsString() != varStringType), "b24") &
            assertEquals(bCommutativeTest8, (varBoolType.getAsBool() != varStringType3), "b25") &
            assertEquals(bCommutativeTest9, (varBoolType.getAsBool() != varStringType4), "b26");
    }

    bool testOperatorAssignment()
    {
        PGEcfgVariable varIntType(2);
        PGEcfgVariable varFloatType(2.5f);
        PGEcfgVariable varBoolType(true);
        PGEcfgVariable varStringType("alma");

        PGEcfgVariable var1 = varIntType;
        PGEcfgVariable var2 = varFloatType;
        PGEcfgVariable var3 = varBoolType;
        PGEcfgVariable var4 = varStringType;
        PGEcfgVariable var5 = 1;
        PGEcfgVariable var6 = 1.5f;
        PGEcfgVariable var7 = true;
        PGEcfgVariable var8 = "asd";

        return assertEquals(varIntType.getAsInt(), var1.getAsInt(), "b1") &
            assertEquals(varIntType.getType(), var1.getType(), "b2") &
            assertEquals(varFloatType.getAsFloat(), var2.getAsFloat(), "b3") &
            assertEquals(varFloatType.getType(), var2.getType(), "b4") &
            assertEquals(varBoolType.getAsBool(), var3.getAsBool(), "b5") &
            assertEquals(varBoolType.getType(), var3.getType(), "b6") &
            assertEquals(varStringType.getAsString(), var4.getAsString(), "b7") &
            assertEquals(varStringType.getType(), var4.getType(), "b8") &
            assertEquals(1, var5.getAsInt(), "b9") &
            assertEquals(PGE_CVAR_INT, var5.getType(), "b10") &
            assertEquals(1.5f, var6.getAsFloat(), E, "b11") &
            assertEquals(PGE_CVAR_FLOAT, var6.getType(), "b12") &
            assertEquals(true, var7.getAsBool(), "b13") &
            assertEquals(PGE_CVAR_BOOL, var7.getType(), "b14") &
            assertEquals(std::string("asd"), var8.getAsString(), "b15") &
            assertEquals(PGE_CVAR_STRING, var8.getType(), "b16");
    }

    bool testOperatorAddition()
    {
        const PGEcfgVariable varIntType(1);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        PGEcfgVariable varResult;
        bool bResult = true;

        varResult = varIntType + 2;
        bResult &= assertEquals(varIntType.getAsInt() + 2, varResult.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b1type");

        varResult = 2 + varIntType;
        bResult &= assertEquals(2 + varIntType.getAsInt(), varResult.getAsInt(), "b2res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b2type");

        varResult = varFloatType + 2;
        bResult &= assertEquals(varFloatType.getAsFloat() + 2, varResult.getAsFloat(), E, "b3res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b3type");

        varResult = 2 + varFloatType;
        bResult &= assertEquals(2 + varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b4res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b4type");

        varResult = varIntType + 2.5f;
        bResult &= assertEquals(varIntType.getAsInt() + 2.5f, varResult.getAsFloat(), E, "b5res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b5type");

        varResult = 2.5f + varIntType;
        bResult &= assertEquals(2.5f + varIntType.getAsInt(), varResult.getAsFloat(), E, "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b6type");

        varResult = varStringType + 2;
        bResult &= assertEquals("alma2", varResult.getAsString(), "b7res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b7type");

        varResult = 2 + varStringType;
        bResult &= assertEquals("2alma", varResult.getAsString(), "b8res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b8type");

        varResult = varStringType + 2.5f;
        bResult &= assertEquals("alma2.5", varResult.getAsString(), "b9res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b9type");

        varResult = 2.5f + varStringType;
        bResult &= assertEquals("2.5alma", varResult.getAsString(), "b10res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b10type");

        varResult = varStringType + "fa";
        bResult &= assertEquals("almafa", varResult.getAsString(), "b11res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b11type");

        varResult = "fa" + varStringType;
        bResult &= assertEquals("faalma", varResult.getAsString(), "b12res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b12type");

        varResult = varFloatType + "fa";
        bResult &= assertEquals("2.5fa", varResult.getAsString(), "b13res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b13type");

        varResult = "fa" + varFloatType;
        bResult &= assertEquals("fa2.5", varResult.getAsString(), "b14res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b14type");

        varResult = varIntType + "fa";
        bResult &= assertEquals("1fa", varResult.getAsString(), "b15res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b15ype");

        varResult = "fa" + varIntType;
        bResult &= assertEquals("fa1", varResult.getAsString(), "b16res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b16type");

        varResult = "fa" + varBoolType;
        bResult &= assertEquals("fatrue", varResult.getAsString(), "b17res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b17type");

        varResult = varBoolType + "fa";
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b18type");

        varResult = varBoolType + 2;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b19type");

        varResult = varBoolType + 2.5f;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b20type");

        varResult = varBoolType + false;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b21type");

        varResult = varIntType + false;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b22res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b22type");

        varResult = varFloatType + false;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b23res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b23type");

        varResult = varStringType + false;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b24res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b24type");

        return bResult;
    }

    bool testOperatorAdditionAssignment()
    {
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalResultIntType(3);
        const PGEcfgVariable varOriginalResultFloatType(1.5f);
        const PGEcfgVariable varOriginalResultBoolType(false);
        const PGEcfgVariable varOriginalResultStringType("asd");
        PGEcfgVariable varResultIntType = varOriginalResultIntType;
        PGEcfgVariable varResultFloatType = varOriginalResultFloatType;
        PGEcfgVariable varResultBoolType = varOriginalResultBoolType;
        PGEcfgVariable varResultStringType = varOriginalResultStringType;
        bool bResult = true;

        // incrementing int-typed cvar

        varResultIntType += varIntType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() + varIntType.getAsInt(), varResultIntType.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b1type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += varFloatType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() + varFloatType.getAsFloat(), varResultIntType.getAsFloat(), E, "b2res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b2type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += varBoolType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b3res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b3type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += varStringType;
        bResult &= assertEquals(varOriginalResultIntType.getAsString() + varStringType.getAsString(), varResultIntType.getAsString(), "b4res") &
            assertEquals(PGE_CVAR_STRING, varResultIntType.getType(), "b4type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += 2;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() + 2, varResultIntType.getAsInt(), "b5res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += 2.5f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() + 2.5f, varResultIntType.getAsFloat(), E, "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b6type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += false;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b7res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b7type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType += "asd";
        bResult &= assertEquals(varOriginalResultIntType.getAsString() + "asd", varResultIntType.getAsString(), "b8res") &
            assertEquals(PGE_CVAR_STRING, varResultIntType.getType(), "b8type");

        // incrementing float-typed cvar

        varResultFloatType += varIntType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() + varIntType.getAsInt(), varResultFloatType.getAsFloat(), E, "b9res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b9type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += varFloatType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() + varFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b10res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b10type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += varBoolType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b11res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b11type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += varStringType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsString() + varStringType.getAsString(), varResultFloatType.getAsString(), "b12res") &
            assertEquals(PGE_CVAR_STRING, varResultFloatType.getType(), "b12type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += 2;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() + 2, varResultFloatType.getAsFloat(), E, "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += 2.5f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() + 2.5f, varResultFloatType.getAsFloat(), E, "b14res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += false;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b15res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b15type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType += "asd";
        bResult &= assertEquals(varOriginalResultFloatType.getAsString() + "asd", varResultFloatType.getAsString(), "b16res") &
            assertEquals(PGE_CVAR_STRING, varResultFloatType.getType(), "b16type");

        // incrementing bool-typed cvar

        varResultBoolType += varIntType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b17res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b17type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += varFloatType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b18type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += varBoolType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b19type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += varStringType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b20type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += 2;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += 2.5f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += false;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b23res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b23type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType += "asd";
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b24res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b24type");

        // incrementing string-typed cvar

        varResultStringType += varIntType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString() + varIntType.getAsString(), varResultStringType.getAsString(), "b25res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b25type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += varFloatType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString() + varFloatType.getAsString(), varResultStringType.getAsString(), "b26res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b26type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += varBoolType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b27res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b27type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += varStringType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString() + varStringType.getAsString(), varResultStringType.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b28type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += 2;
        bResult &= assertEquals(varOriginalResultStringType.getAsString() + "2", varResultStringType.getAsString(), "b29res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += 2.5f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString() + "2.5", varResultStringType.getAsString(), "b30res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += false;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b31res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b31type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType += "asd";
        bResult &= assertEquals(varOriginalResultStringType.getAsString() + "asd", varResultStringType.getAsString(), "b32res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b32type");

        return bResult;
    }

    bool testOperatorSubtraction()
    {
        const PGEcfgVariable varIntType(1);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        PGEcfgVariable varResult;
        bool bResult = true;

        varResult = varIntType - 2;
        bResult &= assertEquals(varIntType.getAsInt() - 2, varResult.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b1type");

        varResult = 2 - varIntType;
        bResult &= assertEquals(2 - varIntType.getAsInt(), varResult.getAsInt(), "b2res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b2type");

        varResult = varFloatType - 2;
        bResult &= assertEquals(varFloatType.getAsFloat() - 2, varResult.getAsFloat(), E, "b3res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b3type");

        varResult = 2 - varFloatType;
        bResult &= assertEquals(2 - varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b4res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b4type");

        varResult = varIntType - 2.5f;
        bResult &= assertEquals(varIntType.getAsInt() - 2.5f, varResult.getAsFloat(), E, "b5res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b5type");

        varResult = 2.5f - varIntType;
        bResult &= assertEquals(2.5f - varIntType.getAsInt(), varResult.getAsFloat(), E, "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b6type");

        varResult = varStringType - 2;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b7res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b7type");

        varResult = 2 - varStringType;
        bResult &= assertEquals(2, varResult.getAsInt(), "b8res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b8type");

        varResult = varStringType - 2.5f;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b9res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b9type");

        varResult = 2.5f - varStringType;
        bResult &= assertEquals(2.5f, varResult.getAsFloat(), E, "b10res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b10type");

        varResult = varStringType - "fa";
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b11res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b11type");

        varResult = varFloatType - "fa";
        bResult &= assertEquals(2.5f, varResult.getAsFloat(), "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b13type");

        varResult = varIntType - "fa";
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b15res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b15ype");

        varResult = varBoolType - "fa";
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b18type");

        varResult = varBoolType - 2;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b19type");

        varResult = varBoolType - 2.5f;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b20type");

        varResult = varBoolType - false;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b21type");

        varResult = varIntType - false;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b22res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b22type");

        varResult = varFloatType - false;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b23res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b23type");

        varResult = varStringType - false;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b24res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b24type");

        return bResult;
    }

    bool testOperatorSubtractionAssignment()
    {
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalResultIntType(3);
        const PGEcfgVariable varOriginalResultFloatType(1.5f);
        const PGEcfgVariable varOriginalResultBoolType(false);
        const PGEcfgVariable varOriginalResultStringType("asd");
        PGEcfgVariable varResultIntType = varOriginalResultIntType;
        PGEcfgVariable varResultFloatType = varOriginalResultFloatType;
        PGEcfgVariable varResultBoolType = varOriginalResultBoolType;
        PGEcfgVariable varResultStringType = varOriginalResultStringType;
        bool bResult = true;

        // decrementing int-typed cvar

        varResultIntType -= varIntType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() - varIntType.getAsInt(), varResultIntType.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b1type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= varFloatType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() - varFloatType.getAsFloat(), varResultIntType.getAsFloat(), E, "b2res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b2type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= varBoolType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b3res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b3type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= varStringType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b4res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b4type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= 2;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() - 2, varResultIntType.getAsInt(), "b5res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= 2.5f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() - 2.5f, varResultIntType.getAsFloat(), E, "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b6type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= false;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b7res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b7type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType -= "asd";
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b8res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b8type");

        // decrementing float-typed cvar

        varResultFloatType -= varIntType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() - varIntType.getAsInt(), varResultFloatType.getAsFloat(), E, "b9res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b9type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= varFloatType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() - varFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b10res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b10type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= varBoolType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b11res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b11type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= varStringType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b12res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b12type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= 2;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() - 2, varResultFloatType.getAsFloat(), E, "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= 2.5f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() - 2.5f, varResultFloatType.getAsFloat(), E, "b14res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= false;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b15res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b15type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType -= "asd";
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), E, "b16res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b16type");

        // decrementing bool-typed cvar

        varResultBoolType -= varIntType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b17res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b17type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= varFloatType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b18type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= varBoolType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b19type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= varStringType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b20type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= 2;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= 2.5f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= false;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b23res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b23type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType -= "asd";
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b24res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b24type");

        // decrementing string-typed cvar

        varResultStringType -= varIntType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b25res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b25type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= varFloatType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b26res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b26type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= varBoolType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b27res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b27type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= varStringType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b28type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= 2;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b29res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= 2.5f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b30res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= false;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b31res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b31type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType -= "asd";
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b32res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b32type");

        return bResult;
    }

    bool testOperatorMultiplicationByScalar()
    {
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        PGEcfgVariable varResult;
        bool bResult = true;

        varResult = varIntType * 2;
        bResult &= assertEquals(varIntType.getAsInt() * 2, varResult.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b1type");

        varResult = 2 * varIntType;
        bResult &= assertEquals(2 * varIntType.getAsInt(), varResult.getAsInt(), "b2res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b2type");

        varResult = varFloatType * 2;
        bResult &= assertEquals(varFloatType.getAsFloat() * 2, varResult.getAsFloat(), E, "b3res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b3type");

        varResult = 2 * varFloatType;
        bResult &= assertEquals(2 * varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b4res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b4type");

        varResult = varIntType * 2.5f;
        bResult &= assertEquals(varIntType.getAsInt() * 2.5f, varResult.getAsFloat(), E, "b5res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b5type");

        varResult = 2.5f * varIntType;
        bResult &= assertEquals(2.5f * varIntType.getAsInt(), varResult.getAsFloat(), E, "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b6type");

        varResult = varStringType * 2;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b7res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b7type");

        varResult = varStringType * 2.5f;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b9res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b9type");

        varResult = varStringType * "fa";
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b11res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b11type");

        varResult = varFloatType * "fa";
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b13type");

        varResult = varIntType * "fa";
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b15res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b15ype");

        varResult = varBoolType * "fa";
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b18type");

        varResult = varBoolType * 2;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b19type");

        varResult = varBoolType * 2.5f;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b20type");

        varResult = varBoolType * false;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b21type");

        varResult = varIntType * false;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b22res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b22type");

        varResult = varFloatType * false;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b23res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b23type");

        varResult = varStringType * false;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b24res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b24type");

        return bResult;
    }

    bool testOperatorMultiplicationAssignmentByScalar()
    {
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalResultIntType(3);
        const PGEcfgVariable varOriginalResultFloatType(1.5f);
        const PGEcfgVariable varOriginalResultBoolType(false);
        const PGEcfgVariable varOriginalResultStringType("asd");
        PGEcfgVariable varResultIntType = varOriginalResultIntType;
        PGEcfgVariable varResultFloatType = varOriginalResultFloatType;
        PGEcfgVariable varResultBoolType = varOriginalResultBoolType;
        PGEcfgVariable varResultStringType = varOriginalResultStringType;
        bool bResult = true;

        // multiplying int-typed cvar

        varResultIntType *= varIntType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() * varIntType.getAsInt(), varResultIntType.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b1type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= varFloatType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() * varFloatType.getAsFloat(), varResultIntType.getAsFloat(), "b2res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b2type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= varBoolType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b3res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b3type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= varStringType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b4res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b4type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= 2;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() * 2, varResultIntType.getAsInt(), "b5res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= 2.5f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() * 2.5f, varResultIntType.getAsFloat(), "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b6type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= false;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b7res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b7type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType *= "asd";
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b8res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b8type");

        // multiplying float-typed cvar

        varResultFloatType *= varIntType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() * varIntType.getAsInt(), varResultFloatType.getAsInt(), "b9res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b9type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= varFloatType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() * varFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b10res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b10type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= varBoolType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b11res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b11type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= varStringType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b12res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b12type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= 2;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() * 2, varResultFloatType.getAsFloat(), "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= 2.5f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() * 2.5f, varResultFloatType.getAsFloat(), "b14res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= false;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b15res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b15type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType *= "asd";
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b16res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b16type");

        // multiplying bool-typed cvar

        varResultBoolType *= varIntType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b17res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b17type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= varFloatType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b18type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= varBoolType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b19type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= varStringType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b20type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= 2;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= 2.5f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= false;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b23res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b23type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType *= "asd";
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b24res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b24type");

        // multiplying string-typed cvar

        varResultStringType *= varIntType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b25res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b25type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= varFloatType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b26res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b26type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= varBoolType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b27res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b27type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= varStringType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b28type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= 2;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b29res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= 2.5f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b30res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= false;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b31res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b31type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType *= "asd";
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b32res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b32type");

        return bResult;
    }

    bool testOperatorDivisionByScalar()
    {
        const PGEcfgVariable varIntType(4);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        PGEcfgVariable varResult;
        bool bResult = true;

        varResult = varIntType / 2;
        bResult &= assertEquals(varIntType.getAsInt() / 2, varResult.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b1type");

        varResult = 8 / varIntType;
        bResult &= assertEquals(8 / varIntType.getAsInt(), varResult.getAsInt(), "b2res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b2type");

        varResult = varFloatType / 2;
        bResult &= assertEquals(varFloatType.getAsFloat() / 2, varResult.getAsFloat(), E, "b3res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b3type");

        varResult = 2 / varFloatType;
        bResult &= assertEquals(2 / varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b4res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b4type");

        varResult = varIntType / 1.2f;
        bResult &= assertEquals(varIntType.getAsInt() / 1.2f, varResult.getAsFloat(), E, "b5res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b5type");

        varResult = 10.0f / varIntType;
        bResult &= assertEquals(10.0f / varIntType.getAsInt(), varResult.getAsFloat(), E, "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b6type");

        varResult = varStringType / 2;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b7res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b7type");

        varResult = varStringType / 2.5f;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b9res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b9type");

        varResult = varStringType / "fa";
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b11res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b11type");

        varResult = varFloatType / "fa";
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b13type");

        varResult = varIntType / "fa";
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b15res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b15ype");

        varResult = varBoolType / "fa";
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b18type");

        varResult = varBoolType / 2;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b19type");

        varResult = varBoolType / 2.5f;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b20type");

        varResult = varBoolType / false;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b21type");

        varResult = varIntType / false;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b22res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b22type");

        varResult = varFloatType / false;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b23res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b23type");

        varResult = varStringType / false;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b24res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b24type");

        varResult = varBoolType / 0;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b25res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b25type");

        varResult = varIntType / 0;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b26res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b26type");

        varResult = varFloatType / 0;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b27res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b27type");

        varResult = varStringType / 0;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b28type");

        return bResult;
    }

    bool testOperatorDivisionAssignmentByScalar()
    {
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalResultIntType(3);
        const PGEcfgVariable varOriginalResultFloatType(1.5f);
        const PGEcfgVariable varOriginalResultBoolType(false);
        const PGEcfgVariable varOriginalResultStringType("asd");
        PGEcfgVariable varResultIntType = varOriginalResultIntType;
        PGEcfgVariable varResultFloatType = varOriginalResultFloatType;
        PGEcfgVariable varResultBoolType = varOriginalResultBoolType;
        PGEcfgVariable varResultStringType = varOriginalResultStringType;
        bool bResult = true;

        // dividing int-typed cvar

        varResultIntType /= varIntType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() / varIntType.getAsInt(), varResultIntType.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b1type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= varFloatType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() / varFloatType.getAsFloat(), varResultIntType.getAsFloat(), "b2res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b2type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= varBoolType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b3res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b3type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= varStringType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b4res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b4type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= 2;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() / 2, varResultIntType.getAsInt(), "b5res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= 0;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b5Bres") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5Btype");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= 2.5f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() / 2.5f, varResultIntType.getAsFloat(), "b6res") &
            assertEquals(PGE_CVAR_FLOAT, varResultIntType.getType(), "b6type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= 0.0f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b6Bres") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b6Btype");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= false;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b7res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b7type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType /= "asd";
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b8res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b8type");

        // dividing float-typed cvar

        varResultFloatType /= varIntType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() / varIntType.getAsInt(), varResultFloatType.getAsFloat(), "b9res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b9type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= varFloatType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() / varFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b10res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b10type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= varBoolType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b11res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b11type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= varStringType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b12res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b12type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= 2;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() / 2, varResultFloatType.getAsFloat(), "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= 0;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b13Bres") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13Btype");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= 2.5f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat() / 2.5f, varResultFloatType.getAsFloat(), "b14res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= 0.0f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b14Bres") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14Btype");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= false;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b15res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b15type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType /= "asd";
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b16res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b16type");

        // dividing bool-typed cvar

        varResultBoolType /= varIntType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b17res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b17type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= varFloatType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b18type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= varBoolType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b19type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= varStringType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b20type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= 2;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= 0;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21Bres") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21Btype");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= 2.5f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= 0.0f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22Bres") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22Btype");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= false;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b23res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b23type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType /= "asd";
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b24res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b24type");

        // dividing string-typed cvar

        varResultStringType /= varIntType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b25res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b25type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= varFloatType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b26res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b26type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= varBoolType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b27res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b27type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= varStringType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b28type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= 2;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b29res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= 0;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b29Bres") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29Btype");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= 2.5f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b30res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= 0.0f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b30Bres") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30Btype");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= false;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b31res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b31type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType /= "asd";
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b32res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b32type");

        return bResult;
    }

    bool testOperatorModularDivisionByScalar()
    {
        const PGEcfgVariable varIntType(4);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        PGEcfgVariable varResult;
        bool bResult = true;

        varResult = varIntType % 3;
        bResult &= assertEquals(varIntType.getAsInt() % 3, varResult.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b1type");

        varResult = 5 % varIntType;
        bResult &= assertEquals(5 % varIntType.getAsInt(), varResult.getAsInt(), "b2res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b2type");

        varResult = varFloatType % 2;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b3res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b3type");

        varResult = varIntType % 1.2f;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b5res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b5type");

        varResult = varStringType % 2;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b7res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b7type");

        varResult = varStringType % 2.5f;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b9res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b9type");

        varResult = varStringType % "fa";
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b11res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b11type");

        varResult = varFloatType % "fa";
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b13type");

        varResult = varIntType % "fa";
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b15res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b15ype");

        varResult = varBoolType % "fa";
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b18type");

        varResult = varBoolType % 2;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b19type");

        varResult = varBoolType % 2.5f;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b20type");

        varResult = varBoolType % false;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b21type");

        varResult = varIntType % false;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b22res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b22type");

        varResult = varFloatType % false;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b23res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b23type");

        varResult = varStringType % false;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b24res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b24type");

        varResult = varBoolType % 0;
        bResult &= assertEquals(varBoolType.getAsBool(), varResult.getAsBool(), "b25res") &
            assertEquals(PGE_CVAR_BOOL, varResult.getType(), "b25type");

        varResult = varIntType % 0;
        bResult &= assertEquals(varIntType.getAsInt(), varResult.getAsInt(), "b26res") &
            assertEquals(PGE_CVAR_INT, varResult.getType(), "b26type");

        varResult = varFloatType % 0;
        bResult &= assertEquals(varFloatType.getAsFloat(), varResult.getAsFloat(), E, "b27res") &
            assertEquals(PGE_CVAR_FLOAT, varResult.getType(), "b27type");

        varResult = varStringType % 0;
        bResult &= assertEquals(varStringType.getAsString(), varResult.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResult.getType(), "b28type");

        return bResult;
    }

    bool testOperatorModularDivisionAssignmentByScalar()
    {
        const PGEcfgVariable varIntType(2);
        const PGEcfgVariable varFloatType(2.5f);
        const PGEcfgVariable varBoolType(true);
        const PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalResultIntType(3);
        const PGEcfgVariable varOriginalResultFloatType(1.5f);
        const PGEcfgVariable varOriginalResultBoolType(false);
        const PGEcfgVariable varOriginalResultStringType("asd");
        PGEcfgVariable varResultIntType = varOriginalResultIntType;
        PGEcfgVariable varResultFloatType = varOriginalResultFloatType;
        PGEcfgVariable varResultBoolType = varOriginalResultBoolType;
        PGEcfgVariable varResultStringType = varOriginalResultStringType;
        bool bResult = true;

        // modulo dividing int-typed cvar

        varResultIntType %= varIntType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() % varIntType.getAsInt(), varResultIntType.getAsInt(), "b1res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b1type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= varFloatType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b2res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b2type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= varBoolType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b3res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b3type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= varStringType;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b4res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b4type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= 2;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt() % 2, varResultIntType.getAsInt(), "b5res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= 0;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b5Bres") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b5Btype");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= 2.5f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b6res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b6type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= 0.0f;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b6Bres") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b6Btype");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= false;
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b7res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b7type");

        varResultIntType = varOriginalResultIntType; // reinit
        varResultIntType %= "asd";
        bResult &= assertEquals(varOriginalResultIntType.getAsInt(), varResultIntType.getAsInt(), "b8res") &
            assertEquals(PGE_CVAR_INT, varResultIntType.getType(), "b8type");

        // modulo dividing float-typed cvar

        varResultFloatType %= varIntType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b9res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b9type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= varFloatType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b10res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b10type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= varBoolType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b11res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b11type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= varStringType;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b12res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b12type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= 2;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b13res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= 0;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b13Bres") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b13Btype");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= 2.5f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b14res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= 0.0f;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b14Bres") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b14Btype");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= false;
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b15res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b15type");

        varResultFloatType = varOriginalResultFloatType; // reinit
        varResultFloatType %= "asd";
        bResult &= assertEquals(varOriginalResultFloatType.getAsFloat(), varResultFloatType.getAsFloat(), "b16res") &
            assertEquals(PGE_CVAR_FLOAT, varResultFloatType.getType(), "b16type");

        // modulo dividing bool-typed cvar

        varResultBoolType %= varIntType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b17res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b17type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= varFloatType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b18res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b18type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= varBoolType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b19res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b19type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= varStringType;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b20res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b20type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= 2;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= 0;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b21Bres") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b21Btype");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= 2.5f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= 0.0f;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b22Bres") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b22Btype");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= false;
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b23res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b23type");

        varResultBoolType = varOriginalResultBoolType; // reinit
        varResultBoolType %= "asd";
        bResult &= assertEquals(varOriginalResultBoolType.getAsBool(), varResultBoolType.getAsBool(), "b24res") &
            assertEquals(PGE_CVAR_BOOL, varResultBoolType.getType(), "b24type");

        // modulo dividing string-typed cvar

        varResultStringType %= varIntType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b25res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b25type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= varFloatType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b26res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b26type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= varBoolType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b27res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b27type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= varStringType;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b28res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b28type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= 2;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b29res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= 0;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b29Bres") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b29Btype");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= 2.5f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b30res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= 0.0f;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b30Bres") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b30Btype");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= false;
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b31res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b31type");

        varResultStringType = varOriginalResultStringType; // reinit
        varResultStringType %= "asd";
        bResult &= assertEquals(varOriginalResultStringType.getAsString(), varResultStringType.getAsString(), "b32res") &
            assertEquals(PGE_CVAR_STRING, varResultStringType.getType(), "b32type");

        return bResult;
    }

    bool testOperatorIncrementPrefix()
    {
        PGEcfgVariable varIntType(2);
        PGEcfgVariable varFloatType(2.5f);
        PGEcfgVariable varBoolType(true);
        PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalIntType( varIntType );
        const PGEcfgVariable varOriginalFloatType( varFloatType );
        const PGEcfgVariable varOriginalBoolType( varBoolType );
        const PGEcfgVariable varOriginalStringType( varStringType );

        PGEcfgVariable var1 = ++varIntType;
        PGEcfgVariable var2 = ++varFloatType;
        PGEcfgVariable var3 = ++varBoolType;
        PGEcfgVariable var4 = ++varStringType;

        return assertEquals(varOriginalIntType.getAsInt() + 1, var1.getAsInt(), "b1") &
            assertEquals(varOriginalIntType.getAsInt() + 1, varIntType.getAsInt(), "b2") &
            assertEquals(PGE_CVAR_INT, var1.getType(), "b3") &
            assertEquals(varOriginalFloatType.getAsFloat(), var2.getAsFloat(), "b4") &
            assertEquals(varOriginalFloatType.getAsFloat(), varFloatType.getAsFloat(), "b5") &
            assertEquals(PGE_CVAR_FLOAT, var2.getType(), "b6") &
            assertEquals(varOriginalBoolType.getAsBool(), var3.getAsBool(), "b7") &
            assertEquals(varOriginalBoolType.getAsBool(), varBoolType.getAsBool(), "b8") &
            assertEquals(PGE_CVAR_BOOL, var3.getType(), "b9") &
            assertEquals(varOriginalStringType.getAsString(), var4.getAsString(), "b10") &
            assertEquals(varOriginalStringType.getAsString(), varStringType.getAsString(), "b11") &
            assertEquals(PGE_CVAR_STRING, var4.getType(), "b12");
    }

    bool testOperatorIncrementPostfix()
    {
        PGEcfgVariable varIntType(2);
        PGEcfgVariable varFloatType(2.5f);
        PGEcfgVariable varBoolType(true);
        PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalIntType( varIntType );
        const PGEcfgVariable varOriginalFloatType( varFloatType );
        const PGEcfgVariable varOriginalBoolType( varBoolType );
        const PGEcfgVariable varOriginalStringType( varStringType );

        PGEcfgVariable var1 = varIntType++;
        PGEcfgVariable var2 = varFloatType++;
        PGEcfgVariable var3 = varBoolType++;
        PGEcfgVariable var4 = varStringType++;

        return assertEquals(varOriginalIntType.getAsInt(), var1.getAsInt(), "b1") &
            assertEquals(varOriginalIntType.getAsInt() + 1, varIntType.getAsInt(), "b2") &
            assertEquals(PGE_CVAR_INT, var1.getType(), "b3") &
            assertEquals(varOriginalFloatType.getAsFloat(), var2.getAsFloat(), "b4") &
            assertEquals(varOriginalFloatType.getAsFloat(), varFloatType.getAsFloat(), "b5") &
            assertEquals(PGE_CVAR_FLOAT, var2.getType(), "b6") &
            assertEquals(varOriginalBoolType.getAsBool(), var3.getAsBool(), "b7") &
            assertEquals(varOriginalBoolType.getAsBool(), varBoolType.getAsBool(), "b8") &
            assertEquals(PGE_CVAR_BOOL, var3.getType(), "b9") &
            assertEquals(varOriginalStringType.getAsString(), var4.getAsString(), "b10") &
            assertEquals(varOriginalStringType.getAsString(), varStringType.getAsString(), "b11") &
            assertEquals(PGE_CVAR_STRING, var4.getType(), "b12");
    }

    bool testOperatorDecrementPrefix()
    {
        PGEcfgVariable varIntType(2);
        PGEcfgVariable varFloatType(2.5f);
        PGEcfgVariable varBoolType(true);
        PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalIntType( varIntType );
        const PGEcfgVariable varOriginalFloatType( varFloatType );
        const PGEcfgVariable varOriginalBoolType( varBoolType );
        const PGEcfgVariable varOriginalStringType( varStringType );

        PGEcfgVariable var1 = --varIntType;
        PGEcfgVariable var2 = --varFloatType;
        PGEcfgVariable var3 = --varBoolType;
        PGEcfgVariable var4 = --varStringType;

        return assertEquals(varOriginalIntType.getAsInt() - 1, var1.getAsInt(), "b1") &
            assertEquals(varOriginalIntType.getAsInt() - 1, varIntType.getAsInt(), "b2") &
            assertEquals(PGE_CVAR_INT, var1.getType(), "b3") &
            assertEquals(varOriginalFloatType.getAsFloat(), var2.getAsFloat(), "b4") &
            assertEquals(varOriginalFloatType.getAsFloat(), varFloatType.getAsFloat(), "b5") &
            assertEquals(PGE_CVAR_FLOAT, var2.getType(), "b6") &
            assertEquals(varOriginalBoolType.getAsBool(), var3.getAsBool(), "b7") &
            assertEquals(varOriginalBoolType.getAsBool(), varBoolType.getAsBool(), "b8") &
            assertEquals(PGE_CVAR_BOOL, var3.getType(), "b9") &
            assertEquals(varOriginalStringType.getAsString(), var4.getAsString(), "b10") &
            assertEquals(varOriginalStringType.getAsString(), varStringType.getAsString(), "b11") &
            assertEquals(PGE_CVAR_STRING, var4.getType(), "b12");
    }

    bool testOperatorDecrementPostfix()
    {
        PGEcfgVariable varIntType(2);
        PGEcfgVariable varFloatType(2.5f);
        PGEcfgVariable varBoolType(true);
        PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalIntType( varIntType );
        const PGEcfgVariable varOriginalFloatType( varFloatType );
        const PGEcfgVariable varOriginalBoolType( varBoolType );
        const PGEcfgVariable varOriginalStringType( varStringType );

        PGEcfgVariable var1 = varIntType--;
        PGEcfgVariable var2 = varFloatType--;
        PGEcfgVariable var3 = varBoolType--;
        PGEcfgVariable var4 = varStringType--;

        return assertEquals(varOriginalIntType.getAsInt(), var1.getAsInt(), "b1") &
            assertEquals(varOriginalIntType.getAsInt() - 1, varIntType.getAsInt(), "b2") &
            assertEquals(PGE_CVAR_INT, var1.getType(), "b3") &
            assertEquals(varOriginalFloatType.getAsFloat(), var2.getAsFloat(), "b4") &
            assertEquals(varOriginalFloatType.getAsFloat(), varFloatType.getAsFloat(), "b5") &
            assertEquals(PGE_CVAR_FLOAT, var2.getType(), "b6") &
            assertEquals(varOriginalBoolType.getAsBool(), var3.getAsBool(), "b7") &
            assertEquals(varOriginalBoolType.getAsBool(), varBoolType.getAsBool(), "b8") &
            assertEquals(PGE_CVAR_BOOL, var3.getType(), "b9") &
            assertEquals(varOriginalStringType.getAsString(), var4.getAsString(), "b10") &
            assertEquals(varOriginalStringType.getAsString(), varStringType.getAsString(), "b11") &
            assertEquals(PGE_CVAR_STRING, var4.getType(), "b12");
    }

    bool testOperatorNegative()
    {
        PGEcfgVariable varIntType(2);
        PGEcfgVariable varFloatType(2.5f);
        PGEcfgVariable varBoolType(true);
        PGEcfgVariable varStringType("alma");
        const PGEcfgVariable varOriginalIntType( varIntType );
        const PGEcfgVariable varOriginalFloatType( varFloatType );
        const PGEcfgVariable varOriginalBoolType( varBoolType );
        const PGEcfgVariable varOriginalStringType( varStringType );

        PGEcfgVariable var1 = -varIntType;
        PGEcfgVariable var2 = -varFloatType;
        PGEcfgVariable var3 = -varBoolType;
        PGEcfgVariable var4 = -varStringType;

        return assertEquals(-varOriginalIntType.getAsInt(), var1.getAsInt(), "b1") &
            assertEquals(varOriginalIntType.getAsInt(), varIntType.getAsInt(), "b2") &
            assertEquals(PGE_CVAR_INT, var1.getType(), "b3") &
            assertEquals(-varOriginalFloatType.getAsFloat(), var2.getAsFloat(), "b4") &
            assertEquals(varOriginalFloatType.getAsFloat(), varFloatType.getAsFloat(), "b5") &
            assertEquals(PGE_CVAR_FLOAT, var2.getType(), "b6") &
            assertEquals(varOriginalBoolType.getAsBool(), var3.getAsBool(), "b7") &
            assertEquals(varOriginalBoolType.getAsBool(), varBoolType.getAsBool(), "b8") &
            assertEquals(PGE_CVAR_BOOL, var3.getType(), "b9") &
            assertEquals(varOriginalStringType.getAsString(), var4.getAsString(), "b10") &
            assertEquals(varOriginalStringType.getAsString(), varStringType.getAsString(), "b11") &
            assertEquals(PGE_CVAR_STRING, var4.getType(), "b12");
    }

}; 