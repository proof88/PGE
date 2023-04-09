#pragma once

/*
    ###################################################################################
    PgeOldNewValueTest.h
    Unit test for PgeOldNewValue.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
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
        AddSubTest("testCtor", (PFNUNITSUBTEST)&PgeOldNewValueTest::testCtor);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST)&PgeOldNewValueTest::testCopyCtor);
        AddSubTest("testSet", (PFNUNITSUBTEST)&PgeOldNewValueTest::testSet);
        AddSubTest("testCommit", (PFNUNITSUBTEST)&PgeOldNewValueTest::testCommit);
        AddSubTest("testRevert", (PFNUNITSUBTEST)&PgeOldNewValueTest::testRevert);
        AddSubTest("testOperatorEquals", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorEquals);
        AddSubTest("testOperatorNotEquals", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorNotEquals);
        AddSubTest("testOperatorAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorAssignment);
        AddSubTest("testOperatorAddition", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorAddition);
        AddSubTest("testOperatorAdditionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorAdditionAssignment);
        AddSubTest("testOperatorSubtraction", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorSubtraction);
        AddSubTest("testOperatorSubtractionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorSubtractionAssignment);
        AddSubTest("testOperatorMultiplication", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorMultiplication);
        AddSubTest("testOperatorMultiplicationAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorMultiplicationAssignment);
        AddSubTest("testOperatorDivision", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDivision);
        AddSubTest("testOperatorDivisionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDivisionAssignment);
        AddSubTest("testOperatorModularDivision", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorModularDivision);
        AddSubTest("testOperatorModularDivisionAssignment", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorModularDivisionAssignment);
        AddSubTest("testOperatorIncrementPrefix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorIncrementPrefix);
        AddSubTest("testOperatorIncrementPostfix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorIncrementPostfix);
        AddSubTest("testOperatorDecrementPrefix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDecrementPrefix);
        AddSubTest("testOperatorDecrementPostfix", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorDecrementPostfix);
        AddSubTest("testOperatorNegative", (PFNUNITSUBTEST)&PgeOldNewValueTest::testOperatorNegative);

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
            assertFalse(ov.isDirty(), "dirty");
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
            assertEquals(ov1.isDirty(), ov3.isDirty(), "dirty");
    }

    bool testSet()
    {
        PgeOldNewValue<int> ov(2);

        bool b = assertFalse(ov.set(2), "set 1");
        b &= assertFalse(ov.isDirty(), "dirty 1");

        b &= assertTrue(ov.set(4), "set 2");
        b &= assertTrue(ov.isDirty(), "dirty 2");
        b &= assertNotEquals(ov.getOld(), ov.getNew(), "old vs new");

        return b;
    }

    bool testCommit()
    {
        PgeOldNewValue<int> ov(2);

        bool b = assertTrue(ov.set(4), "set");
        ov.commit();

        return b &
            assertEquals(4, ov.getOld(), "old") &
            assertEquals(4, ov.getNew(), "new") &
            assertFalse(ov.isDirty(), "dirty");
    }

    bool testRevert()
    {
        PgeOldNewValue<int> ov(2);

        bool b = assertTrue(ov.set(4), "set");
        ov.revert();

        return b & 
            assertEquals(2, ov.getOld(), "old") &
            assertEquals(2, ov.getNew(), "new") &
            assertFalse(ov.isDirty(), "dirty");
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
            assertEquals(ov1.isDirty(), ov3.isDirty(), "dirty");
    }

    bool testOperatorAddition()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal + 3;
        bResult &= assertEquals(ovOriginal.getNew() + 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 + ovOriginal;
        bResult &= assertEquals(5 + ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal + ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() + ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3");

        return bResult;
    }

    bool testOperatorAdditionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult += ovOperand;
        bResult &= assertEquals(ovResult.getOld() + ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1");

        ovResult.revert();  // makes dirty flag cleared
        ovResult += 2;
        bResult &= assertEquals(ovResult.getOld() + 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2");

        return bResult;
    }

    bool testOperatorSubtraction()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal - 3;
        bResult &= assertEquals(ovOriginal.getNew() - 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 - ovOriginal;
        bResult &= assertEquals(5 - ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal - ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() - ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3");

        return bResult;
    }

    bool testOperatorSubtractionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult -= ovOperand;
        bResult &= assertEquals(ovResult.getOld() - ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1");

        ovResult.revert();  // makes dirty flag cleared
        ovResult -= 2;
        bResult &= assertEquals(ovResult.getOld() - 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2");

        return bResult;
    }

    bool testOperatorMultiplication()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal * 3;
        bResult &= assertEquals(ovOriginal.getNew() * 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 * ovOriginal;
        bResult &= assertEquals(5 * ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal * ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() * ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3");

        return bResult;
    }

    bool testOperatorMultiplicationAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult *= ovOperand;
        bResult &= assertEquals(ovResult.getOld() * ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1");

        ovResult.revert();  // makes dirty flag cleared
        ovResult *= 2;
        bResult &= assertEquals(ovResult.getOld() * 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2");

        return bResult;
    }

    bool testOperatorDivision()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;

        ovResult = ovOriginal / 3;
        bResult &= assertEquals(ovOriginal.getNew() / 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 / ovOriginal;
        bResult &= assertEquals(5 / ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal / ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() / ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3");

        return bResult;
    }

    bool testOperatorDivisionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;

        ovResult /= ovOperand;
        bResult &= assertEquals(ovResult.getOld() / ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1");

        ovResult.revert();  // makes dirty flag cleared
        ovResult /= 2;
        bResult &= assertEquals(ovResult.getOld() / 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2");

        return bResult;
    }

    bool testOperatorModularDivision()
    {
        const PgeOldNewValue<int> ovOriginal(4);
        PgeOldNewValue<int> ovResult;
        bool bResult = true;
    
        ovResult = ovOriginal % 3;
        bResult &= assertEquals(ovOriginal.getNew() % 3, ovResult.getNew(), "res 1") &
            assertFalse(ovResult.isDirty(), "dirty 1");
    
        ovResult.commit();  // makes dirty flag cleared
        ovResult = 5 % ovOriginal;
        bResult &= assertEquals(5 % ovOriginal.getNew(), ovResult.getNew(), "res 2") &
            assertFalse(ovResult.isDirty(), "dirty 2");

        ovResult.commit();  // makes dirty flag cleared
        ovResult = ovOriginal % ovOriginal;
        bResult &= assertEquals(ovOriginal.getNew() % ovOriginal.getNew(), ovResult.getNew(), "res 3") &
            assertFalse(ovResult.isDirty(), "dirty 3");
    
        return bResult;
    }
    
    bool testOperatorModularDivisionAssignment()
    {
        const PgeOldNewValue<int> ovOperand(2);
        PgeOldNewValue<int> ovResult(4);
        bool bResult = true;
    
        ovResult %= ovOperand;
        bResult &= assertEquals(ovResult.getOld() % ovOperand.getNew(), ovResult.getNew(), "res 1") &
            assertTrue(ovResult.isDirty(), "dirty 1");
    
        ovResult.revert();  // makes dirty flag cleared
        ovResult %= 2;
        bResult &= assertEquals(ovResult.getOld() % 2, ovResult.getNew(), "res 2") &
            assertTrue(ovResult.isDirty(), "dirty 2");
    
        return bResult;
    }

    bool testOperatorIncrementPrefix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = ++var1;
    
        return assertEquals(ovOriginal.getNew() + 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew() + 1, ovResult.getNew(), "b2");
    }
    
    bool testOperatorIncrementPostfix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = var1++;
    
        return assertEquals(ovOriginal.getNew() + 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew(), ovResult.getNew(), "b2");
    }
    
    bool testOperatorDecrementPrefix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = --var1;
    
        return assertEquals(ovOriginal.getNew() - 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew() - 1, ovResult.getNew(), "b2");
    }
    
    bool testOperatorDecrementPostfix()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        PgeOldNewValue<int> var1(ovOriginal);
        const PgeOldNewValue<int> ovResult = var1--;
    
        return assertEquals(ovOriginal.getNew() - 1, var1.getNew(), "b1") &
            assertEquals(ovOriginal.getNew(), ovResult.getNew(), "b2");
    }
    
    bool testOperatorNegative()
    {
        const PgeOldNewValue<int> ovOriginal(2);
        const PgeOldNewValue<int> ovNegated = -ovOriginal;
    
        return assertEquals(-ovOriginal.getNew(), ovNegated.getNew(), "b1") &
            assertFalse(ovNegated.isDirty(), "b2");
    }
};
