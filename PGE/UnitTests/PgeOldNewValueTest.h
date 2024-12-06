#pragma once

/*
    ###################################################################################
    PgeOldNewValueTest.h
    Unit test for PgeOldNewValue.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Config/PgeOldNewValue.h"
#include "../PURE/include/external/Math/PureVector.h"

class PgeOldNewValueTest :
    public UnitTest
{
public:

    PgeOldNewValueTest() :
        UnitTest(__FILE__)
    {
        addSubTest("testCtor", (PFNUNITSUBTEST)&PgeOldNewValueTest::testCtor);
        addSubTest("testCopyCtor", (PFNUNITSUBTEST)&PgeOldNewValueTest::testCopyCtor);
        addSubTest("testSet", (PFNUNITSUBTEST)&PgeOldNewValueTest::testSet);
        addSubTest("testCommit", (PFNUNITSUBTEST)&PgeOldNewValueTest::testCommit);
        addSubTest("testRevert", (PFNUNITSUBTEST)&PgeOldNewValueTest::testRevert);
        addSubTest("testOperatorEquals", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorEquals);
        addSubTest("testOperatorNotEquals", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorNotEquals);
        addSubTest("testOperatorAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorAssignment);
        addSubTest("testOperatorConversionToValueType", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorConversionToValueType);
        addSubTest("testOperatorAddition", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorAddition);
        addSubTest("testOperatorAdditionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorAdditionAssignment);
        addSubTest("testOperatorSubtraction", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorSubtraction);
        addSubTest("testOperatorSubtractionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorSubtractionAssignment);
        addSubTest("testOperatorMultiplication", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorMultiplication);
        addSubTest("testOperatorMultiplicationAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorMultiplicationAssignment);
        addSubTest("testOperatorDivision", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDivision);
        addSubTest("testOperatorDivisionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDivisionAssignment);
        addSubTest("testOperatorModularDivision", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorModularDivision);
        addSubTest("testOperatorModularDivisionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorModularDivisionAssignment);
        addSubTest("testOperatorIncrementPrefix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorIncrementPrefix);
        addSubTest("testOperatorIncrementPostfix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorIncrementPostfix);
        addSubTest("testOperatorDecrementPrefix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDecrementPrefix);
        addSubTest("testOperatorDecrementPostfix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDecrementPostfix);
        addSubTest("testOperatorNegative", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorNegative);

    } // PgeOldNewValueTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PgeOldNewValueTest(const PgeOldNewValueTest&)
    {};

    PgeOldNewValueTest& operator=(const PgeOldNewValueTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        PgeOldNewValue<int> ov(2);

        return assertEquals(2, ov.getOld(), "old") &
            assertEquals(2, ov.getNew(), "new") &
            assertFalse(ov.isDirty(), "dirty") &
            assertEquals(0, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged");
    }

    bool testCopyCtor()
    {
        const PureVector vecReference(1, 2, 3);
        PgeOldNewValue<PureVector> ov1(vecReference);
        const PgeOldNewValue<PureVector> ov2(ov1);

        bool b = assertEquals(ov1.isDirty(), ov2.isDirty(), "not dirty");

        ov1.set(PureVector(6, 7, 8));
        const PgeOldNewValue<PureVector> ov3(ov1);

        return b &
            assertEquals(vecReference, ov2.getOld(), "old") &
            assertEquals(vecReference, ov2.getNew(), "new") &
            assertEquals(ov1.isDirty(), ov3.isDirty(), "dirty") &
            assertEquals(0, ov2.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertEquals(ov1.getLastTimeNewValueChanged().time_since_epoch().count(), ov3.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");
    }

    bool testSet()
    {
        PgeOldNewValue<int> ov(2);
        PgeOldNewValue<float> ovFloat(2.5f);

        bool b = assertFalse(ov.set(2), "set 1");
        b &= assertFalse(ov.isDirty(), "dirty 1");
        b &= assertEquals(0, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        b &= assertTrue(ov.set(4), "set 2");
        b &= assertTrue(ov.isDirty(), "dirty 2");
        b &= assertNotEquals(ov.getOld(), ov.getNew(), "old vs new");
        b &= assertNotEquals(0, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2");

        const auto timeLastTimeNewValueChanged = ov.getLastTimeNewValueChanged().time_since_epoch().count();
        // setting it back to original value should clear the dirty flag!
        b &= assertFalse(ov.set(2), "set 3");
        b &= assertFalse(ov.isDirty(), "dirty 3");
        b &= assertNotEquals(0, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");
        b &= assertNotEquals(timeLastTimeNewValueChanged, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 4");

        b &= assertTrue(ovFloat.set(2.6f), "set float 1");
        b &= assertTrue(ovFloat.isDirty(), "float dirty 1");
        b &= assertNotEquals(0, ovFloat.getLastTimeNewValueChanged().time_since_epoch().count(), "ovFloat getLastTimeNewValueChanged 1");

        const auto timeLastTimeFloatNewValueChanged = ovFloat.getLastTimeNewValueChanged().time_since_epoch().count();
        // setting it back to original value should clear the dirty flag!
        b &= assertFalse(ovFloat.set(2.5f), "set float 2");
        b &= assertFalse(ovFloat.isDirty(), "float dirty 2");
        b &= assertNotEquals(0, ovFloat.getLastTimeNewValueChanged().time_since_epoch().count(), "ovFloat getLastTimeNewValueChanged 2");
        b &= assertNotEquals(timeLastTimeFloatNewValueChanged, ovFloat.getLastTimeNewValueChanged().time_since_epoch().count(), "ovFloat getLastTimeNewValueChanged 3");

        b &= assertTrue(ovFloat.set(2.6f), "set float 3");
        b &= assertTrue(ovFloat.isDirty(), "float dirty 3");
        ovFloat.commit();
        b &= assertTrue(ovFloat.set(2.61f), "set float 4");
        b &= assertTrue(ovFloat.isDirty(), "float dirty 4");
        ovFloat.commit();
        b &= assertTrue(ovFloat.set(2.611f), "set float 5");
        b &= assertTrue(ovFloat.isDirty(), "float dirty 5");
        ovFloat.commit();
        b &= assertTrue(ovFloat.set(2.6111f), "set float 6");
        b &= assertTrue(ovFloat.isDirty(), "float dirty 6");
        ovFloat.commit();
        b &= assertTrue(ovFloat.set(2.61111f), "set float 7");
        b &= assertTrue(ovFloat.isDirty(), "float dirty 7");
        ovFloat.commit();

        const auto timeLastTimeFloatNewValueChangedAgain = ovFloat.getLastTimeNewValueChanged().time_since_epoch().count();
        b &= assertFalse(ovFloat.set(2.611111f), "set float 8");
        b &= assertFalse(ovFloat.isDirty(), "float dirty 8");
        b &= assertEquals(timeLastTimeFloatNewValueChangedAgain, ovFloat.getLastTimeNewValueChanged().time_since_epoch().count(), "ovFloat getLastTimeNewValueChanged x");

        return b;
    }

    bool testCommit()
    {
        PgeOldNewValue<int> ov(2);

        bool b = assertTrue(ov.set(4), "set");
        const auto timeLastTimeFloatNewValueChangedAgain = ov.getLastTimeNewValueChanged().time_since_epoch().count();
        ov.commit();

        return b &
            assertEquals(4, ov.getOld(), "old") &
            assertEquals(4, ov.getNew(), "new") &
            assertFalse(ov.isDirty(), "dirty") &
            assertEquals(timeLastTimeFloatNewValueChangedAgain, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged");
    }

    bool testRevert()
    {
        PgeOldNewValue<int> ov(2);

        bool b = assertTrue(ov.set(4), "set");
        const auto timeLastTimeFloatNewValueChangedAgain = ov.getLastTimeNewValueChanged().time_since_epoch().count();
        ov.revert();

        return b & 
            assertEquals(2, ov.getOld(), "old") &
            assertEquals(2, ov.getNew(), "new") &
            assertFalse(ov.isDirty(), "dirty") &
            assertEquals(timeLastTimeFloatNewValueChangedAgain, ov.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged");
    }

    bool testOperatorEquals()
    {
        const PgeOldNewValue<int> ov1(4);
        const PgeOldNewValue<int> ov2(2);

        return assertTrue(ov1 == ov1, "1") &
            assertFalse(ov1 == ov2, "2") &
            assertFalse(ov2 == ov1, "3") &
            assertTrue(ov1 == 4, "4") &
            assertTrue(4 == ov1, "5");
    }

    bool testOperatorNotEquals()
    {
        const PgeOldNewValue<int> ov1(4);
        const PgeOldNewValue<int> ov2(2);

        return assertFalse(ov1 != ov1, "1") &
            assertTrue(ov1 != ov2, "2") &
            assertTrue(ov2 != ov1, "3") &
            assertFalse(ov1 != 4, "4") &
            assertFalse(4 != ov1, "5");
    }

    bool testOperatorAssignment()
    {
        const PureVector vecReference(1, 2, 3);
        PgeOldNewValue<PureVector> ov1(vecReference);
        const PgeOldNewValue<PureVector> ov2 = ov1;

        bool b = assertEquals(ov1.isDirty(), ov2.isDirty(), "not dirty");

        ov1.set(PureVector(6, 7, 8));
        const PgeOldNewValue<PureVector> ov3 = ov1;

        return b &
            assertEquals(vecReference, ov2.getOld(), "old") &
            assertEquals(vecReference, ov2.getNew(), "new") &
            assertEquals(ov1.isDirty(), ov3.isDirty(), "dirty") &
            assertEquals(0, ov2.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertEquals(ov1.getLastTimeNewValueChanged().time_since_epoch().count(), ov3.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");
    }

    bool testOperatorConversionToValueType()
    {
        const PgeOldNewValue<int> ov1(4);
        const int nv1 = ov1;

        return assertEquals(ov1.getNew(), nv1);
    }

    bool testOperatorAddition()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal + 3;
        bResult &= assertEquals(ovOriginal.getNew() + 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 + ovOriginal;
        bResult &= assertEquals(5 + ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal + ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() + ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorAdditionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult += ovOperand;
        bResult &= assertEquals(ovResult.getOld() + ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.revert();  // makes dirty flag cleared
        const auto timeLastTimeFloatNewValueChanged = ovResult.getLastTimeNewValueChanged().time_since_epoch().count();
        ovResult += 2;
        bResult &= assertEquals(ovResult.getOld() + 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertNotEquals(timeLastTimeFloatNewValueChanged, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorSubtraction()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal - 3;
        bResult &= assertEquals(ovOriginal.getNew() - 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 - ovOriginal;
        bResult &= assertEquals(5 - ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal - ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() - ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorSubtractionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult -= ovOperand;
        bResult &= assertEquals(ovResult.getOld() - ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.revert();  // makes dirty flag cleared
        const auto timeLastTimeFloatNewValueChanged = ovResult.getLastTimeNewValueChanged().time_since_epoch().count();
        ovResult -= 2;
        bResult &= assertEquals(ovResult.getOld() - 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertNotEquals(timeLastTimeFloatNewValueChanged, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorMultiplication()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal * 3;
        bResult &= assertEquals(ovOriginal.getNew() * 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 * ovOriginal;
        bResult &= assertEquals(5 * ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal * ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() * ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorMultiplicationAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult *= ovOperand;
        bResult &= assertEquals(ovResult.getOld() * ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.revert();  // makes dirty flag cleared
        const auto timeLastTimeFloatNewValueChanged = ovResult.getLastTimeNewValueChanged().time_since_epoch().count();
        ovResult *= 2;
        bResult &= assertEquals(ovResult.getOld() * 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertNotEquals(timeLastTimeFloatNewValueChanged, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorDivision()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal / 3;
        bResult &= assertEquals(ovOriginal.getNew() / 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 / ovOriginal;
        bResult &= assertEquals(5 / ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal / ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() / ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorDivisionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult /= ovOperand;
        bResult &= assertEquals(ovResult.getOld() / ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");

        ovResult.revert();  // makes dirty flag cleared
        const auto timeLastTimeFloatNewValueChanged = ovResult.getLastTimeNewValueChanged().time_since_epoch().count();
        ovResult /= 2;
        bResult &= assertEquals(ovResult.getOld() / 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertNotEquals(timeLastTimeFloatNewValueChanged, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");

        return bResult;
    }

    bool testOperatorModularDivision()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;
    
        ovResult = ovOriginal % 3;
        bResult &= assertEquals(ovOriginal.getNew() % 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");
    
        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 % ovOriginal;
        bResult &= assertEquals(5 % ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal % ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() % ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");
    
        return bResult;
    }
    
    bool testOperatorModularDivisionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;
    
        ovResult %= ovOperand;
        bResult &= assertEquals(ovResult.getOld() % ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 1");
    
        ovResult.revert();  // makes dirty flag cleared
        const auto timeLastTimeFloatNewValueChanged = ovResult.getLastTimeNewValueChanged().time_since_epoch().count();
        ovResult %= 2;
        bResult &= assertEquals(ovResult.getOld() % 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2") &
            assertNotEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 2") &
            assertNotEquals(timeLastTimeFloatNewValueChanged, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "getLastTimeNewValueChanged 3");
    
        return bResult;
    }

    bool testOperatorIncrementPrefix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = ++var1;
    
        return assertEquals(ovOriginal.getNew() + 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew() + 1, ovResult.getNew(), "b2") &
            assertNotEquals(0, var1.getLastTimeNewValueChanged().time_since_epoch().count(), "var1 getLastTimeNewValueChanged") &
            assertEquals(0, ovOriginal.getLastTimeNewValueChanged().time_since_epoch().count(), "ovOriginal getLastTimeNewValueChanged") &
            assertEquals(var1.getLastTimeNewValueChanged().time_since_epoch().count(), ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "ovResult getLastTimeNewValueChanged");
    }
    
    bool testOperatorIncrementPostfix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = var1++;
    
        return assertEquals(ovOriginal.getNew() + 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew(), ovResult.getNew(), "b2") &
            assertNotEquals(0, var1.getLastTimeNewValueChanged().time_since_epoch().count(), "var1 getLastTimeNewValueChanged") &
            assertEquals(0, ovOriginal.getLastTimeNewValueChanged().time_since_epoch().count(), "ovOriginal getLastTimeNewValueChanged") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "ovResult getLastTimeNewValueChanged");
    }
    
    bool testOperatorDecrementPrefix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = --var1;
    
        return assertEquals(ovOriginal.getNew() - 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew() - 1, ovResult.getNew(), "b2") &
            assertNotEquals(0, var1.getLastTimeNewValueChanged().time_since_epoch().count(), "var1 getLastTimeNewValueChanged") &
            assertEquals(0, ovOriginal.getLastTimeNewValueChanged().time_since_epoch().count(), "ovOriginal getLastTimeNewValueChanged") &
            assertEquals(var1.getLastTimeNewValueChanged().time_since_epoch().count(), ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "ovResult getLastTimeNewValueChanged");
    }
    
    bool testOperatorDecrementPostfix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = var1--;
    
        return assertEquals(ovOriginal.getNew() - 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew(), ovResult.getNew(), "b2") &
            assertNotEquals(0, var1.getLastTimeNewValueChanged().time_since_epoch().count(), "var1 getLastTimeNewValueChanged") &
            assertEquals(0, ovOriginal.getLastTimeNewValueChanged().time_since_epoch().count(), "ovOriginal getLastTimeNewValueChanged") &
            assertEquals(0, ovResult.getLastTimeNewValueChanged().time_since_epoch().count(), "ovResult getLastTimeNewValueChanged");
    }
    
    bool testOperatorNegative()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        const PgeOldNewValue<int> ovNegated = -ovOriginal;
    
        return assertEquals(-ovOriginal.getNew(), ovNegated.getNew(), "b1") &
            assertFalse(ovNegated.isDirty(), "b2") &
            assertEquals(0, ovOriginal.getLastTimeNewValueChanged().time_since_epoch().count(), "ovOriginal getLastTimeNewValueChanged") &
            assertEquals(0, ovNegated.getLastTimeNewValueChanged().time_since_epoch().count(), "ovNegated getLastTimeNewValueChanged");
    }
};
