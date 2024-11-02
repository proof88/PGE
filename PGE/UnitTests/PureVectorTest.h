#pragma once

/*
    ###################################################################################
    PureVectorTest.h
    Unit test for PureVector.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Math/PureVector.h"
#include <set>
#include <sstream>

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureVectorTest :
    public UnitTest
{
public:

    PureVectorTest() :
        UnitTest(__FILE__)
    {
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureVectorTest::testCtor);
        addSubTest("testCopyCtor", (PFNUNITSUBTEST) &PureVectorTest::testCopyCtor);
        addSubTest("testCtor2", (PFNUNITSUBTEST) &PureVectorTest::testCtor2);
        addSubTest("testGetX", (PFNUNITSUBTEST) &PureVectorTest::testGetX);
        addSubTest("testGetY", (PFNUNITSUBTEST) &PureVectorTest::testGetY);
        addSubTest("testGetZ", (PFNUNITSUBTEST) &PureVectorTest::testGetZ);
        addSubTest("testGet", (PFNUNITSUBTEST) &PureVectorTest::testGet);
        addSubTest("testSetX", (PFNUNITSUBTEST) &PureVectorTest::testSetX);
        addSubTest("testSetY", (PFNUNITSUBTEST) &PureVectorTest::testSetY);
        addSubTest("testSetZ", (PFNUNITSUBTEST) &PureVectorTest::testSetZ);
        addSubTest("testSet1", (PFNUNITSUBTEST) &PureVectorTest::testSet1);
        addSubTest("testSet2", (PFNUNITSUBTEST) &PureVectorTest::testSet2);
        addSubTest("testIsZero", (PFNUNITSUBTEST) &PureVectorTest::testIsZero);
        addSubTest("testSetZero", (PFNUNITSUBTEST) &PureVectorTest::testSetZero);
        addSubTest("testGetLength", (PFNUNITSUBTEST) &PureVectorTest::testGetLength);
        addSubTest("testNormalize", (PFNUNITSUBTEST) &PureVectorTest::testNormalize);
        addSubTest("testGetDotProduct1", (PFNUNITSUBTEST) &PureVectorTest::testGetDotProduct1);
        addSubTest("testGetDotProduct2", (PFNUNITSUBTEST) &PureVectorTest::testGetDotProduct2);
        addSubTest("testGetCrossProduct1", (PFNUNITSUBTEST) &PureVectorTest::testGetCrossProduct1);
        addSubTest("testGetCrossProduct2", (PFNUNITSUBTEST) &PureVectorTest::testGetCrossProduct2);
        addSubTest("testGetCrossProduct3", (PFNUNITSUBTEST) &PureVectorTest::testGetCrossProduct3);
        addSubTest("testOperatorEquals", (PFNUNITSUBTEST) &PureVectorTest::testOperatorEquals);
        addSubTest("testOperatorNotEquals", (PFNUNITSUBTEST) &PureVectorTest::testOperatorNotEquals);
        addSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PureVectorTest::testOperatorAssignment);
        addSubTest("testOperatorAddition", (PFNUNITSUBTEST) &PureVectorTest::testOperatorAddition);
        addSubTest("testOperatorAdditionAssignment", (PFNUNITSUBTEST) &PureVectorTest::testOperatorAdditionAssignment);
        addSubTest("testOperatorSubtraction", (PFNUNITSUBTEST) &PureVectorTest::testOperatorSubtraction);
        addSubTest("testOperatorSubtractionAssignment", (PFNUNITSUBTEST) &PureVectorTest::testOperatorSubtractionAssignment);
        addSubTest("testOperatorMultiplicationByScalar1", (PFNUNITSUBTEST) &PureVectorTest::testOperatorMultiplicationByScalar1);
        addSubTest("testOperatorMultiplicationByScalar2", (PFNUNITSUBTEST) &PureVectorTest::testOperatorMultiplicationByScalar2);
        addSubTest("testOperatorMultiplicationAssignmentByScalar", (PFNUNITSUBTEST) &PureVectorTest::testOperatorMultiplicationAssignmentByScalar);
        addSubTest("testOperatorDivisionByScalar1", (PFNUNITSUBTEST)&PureVectorTest::testOperatorDivisionByScalar1);
        addSubTest("testOperatorDivisionAssignmentByScalar", (PFNUNITSUBTEST)&PureVectorTest::testOperatorDivisionAssignmentByScalar);
        addSubTest("testOperatorDotProduct1", (PFNUNITSUBTEST) &PureVectorTest::testOperatorDotProduct1);
        addSubTest("testOperatorDotProduct2", (PFNUNITSUBTEST) &PureVectorTest::testOperatorDotProduct2);
        addSubTest("testOperatorCrossProduct1", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCrossProduct1);
        addSubTest("testOperatorCrossProduct2", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCrossProduct2);
        addSubTest("testOperatorCrossProduct3", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCrossProduct3);
        addSubTest("testOperatorAccessElement1", (PFNUNITSUBTEST) &PureVectorTest::testOperatorAccessElement1);
        addSubTest("testOperatorAccessElement2", (PFNUNITSUBTEST) &PureVectorTest::testOperatorAccessElement2);
        addSubTest("testOperatorNegative", (PFNUNITSUBTEST) &PureVectorTest::testOperatorNegative);
        addSubTest("testOperatorCrossProductAssignment1", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCrossProductAssignment1);
        addSubTest("testOperatorCompareLess", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCompareLess);
        addSubTest("testOperatorCompareLessOrEqual", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCompareLessOrEqual);
        addSubTest("testOperatorCompareGreater", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCompareGreater);
        addSubTest("testOperatorCompareGreaterOrEqual", (PFNUNITSUBTEST) &PureVectorTest::testOperatorCompareGreaterOrEqual);
        addSubTest("testOperatorWriteToStream", (PFNUNITSUBTEST) &PureVectorTest::testOperatorWriteToStream);
    } // PureVectorTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PureVectorTest(const PureVectorTest&)
    {};         

    PureVectorTest& operator=(const PureVectorTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureVector vec;
        return assertEquals(0, vec[0], "[0]") &
            assertEquals(0, vec[1], "[1]") &
            assertEquals(0, vec[2], "[2]");
    }

    bool testCopyCtor()
    {
        PureVector vecSrc;
        vecSrc[0] = 1;
        vecSrc[1] = 2;
        vecSrc[2] = 3;
        vecSrc[3] = 4;
        const PureVector vecDst(vecSrc);

        return assertEquals(vecSrc, vecDst);
    }

    bool testCtor2()
    {
        const PureVector vec(1, 2, 3);
        return assertEquals(1, vec[0], "[0]") &
            assertEquals(2, vec[1], "[1]") &
            assertEquals(3, vec[2], "[2]");
    }

    bool testGetX()
    {
        const PureVector vec(1, 2, 3);
        return assertEquals(1, vec.getX());
    }

    bool testGetY()
    {
        const PureVector vec(1, 2, 3);
        return assertEquals(2, vec.getY());
    }

    bool testGetZ()
    {
        const PureVector vec(1, 2, 3);
        return assertEquals(3, vec.getZ());
    }

    bool testGet()
    {
        const PureVector vec(1, 2, 3);

        return assertEquals(1, vec.get(0), "get(0)") &
            assertEquals(2, vec.get(1), "get(1)") &
            assertEquals(3, vec.get(2), "get(2)");
    }

    bool testSetX()
    {
        PureVector vec;
        vec.SetX(5);

        return assertEquals(5, vec.getX());
    }

    bool testSetY()
    {
        PureVector vec;
        vec.SetY(5);

        return assertEquals(5, vec.getY());
    }

    bool testSetZ()
    {
        PureVector vec;
        vec.SetZ(5);

        return assertEquals(5, vec.getZ());
    }

    bool testSet1()
    {
        PureVector vec;
        vec.Set(0, 1);
        vec.Set(1, 2);
        vec.Set(2, 3);

        return assertEquals(1, vec[0], "[0]") &
            assertEquals(2, vec[1], "[1]") &
            assertEquals(3, vec[2], "[2]");
    }

    bool testSet2()
    {
        PureVector vec;
        vec.Set(1, 2, 3);

        return assertEquals(1, vec[0], "[0]") &
            assertEquals(2, vec[1], "[1]") &
            assertEquals(3, vec[2], "[2]");
    }

    bool testIsZero()
    {
        PureVector vec1, vec2;
        vec2.SetX(1);

        return assertTrue(vec1.isZero(), "vec1.isZero()") &
            assertFalse(vec2.isZero(), "vec2.isZero()");
    }

    bool testSetZero()
    {
        PureVector vec(1,2,3);
        vec.SetZero();

        return assertTrue(vec.isZero());
    }

    bool testGetLength()
    {
        const PureVector vec(1, 2, 3);
        return assertEquals(sqrt(vec.getX()*vec.getX() + vec.getY()*vec.getY() + vec.getZ()*vec.getZ()), vec.getLength(), E);
    }

    bool testNormalize()
    {
        PureVector vec1;
        PureVector vec2(1, 2, 3);

        vec1.Normalize();
        vec2.Normalize();

        return assertLequals(vec1.getX(), 1.f, "vec1.getX()") &
            assertLequals(vec1.getY(), 1.f, "vec1.getY()") &
            assertLequals(vec1.getZ(), 1.f, "vec1.getZ()") &
            assertLequals(vec2.getX(), 1.f, "vec2.getX()") &
            assertLequals(vec2.getY(), 1.f, "vec2.getY()") &
            assertLequals(vec2.getZ(), 1.f, "vec2.getZ()");
    }

    bool testGetDotProduct1()
    {
        const PureVector vec1(1, 2, 3);
        const PureVector vec2(4, 5, 6);

        const float dotProduct1 = vec1.getDotProduct(vec2);
        const float dotProduct2 = vec2.getDotProduct(vec1);

        return assertEquals(
            vec1.getX() * vec2.getX() + vec1.getY() * vec2.getY() + vec1.getZ() * vec2.getZ(), dotProduct1, "dotProduct1") &
               assertEquals(dotProduct1, dotProduct2, "dotProduct2") /* dot product is commutative */;
    }

    bool testGetDotProduct2()
    {
        const PureVector vec1( 0,  0,  0);
        const PureVector vec2( 1,  0,  0);
        const PureVector vec3( 0,  1,  0);
        const PureVector vec4( 0,  0,  1);
        const PureVector vec5( 1,  1,  0);
        const PureVector vec6( 1,  0,  1);
        const PureVector vec7( 0,  1,  1);
        const PureVector vec8( 1,  1,  1);
        const PureVector vecA(-1,  0,  0);
        const PureVector vecB( 0, -1,  0);
        const PureVector vecC( 0,  0, -1);
        const PureVector vecD(-1, -1,  0);
        const PureVector vecE(-1,  0, -1);
        const PureVector vecF( 0, -1, -1);
        const PureVector vecG(-1, -1, -1);

        /* 0 result of dotProduct of 2 vectors means the vectors are perpendicular to each other */
        return assertEquals(0, vec1.getDotProduct(vec2), E, "vec1.dot(vec2)") &
            assertEquals(0, vec1.getDotProduct(vec3), E, "vec1.dot(vec3)") &
            assertEquals(0, vec1.getDotProduct(vec4), E, "vec1.dot(vec4)") &
            assertEquals(0, vec1.getDotProduct(vec5), E, "vec1.dot(vec5)") &
            assertEquals(0, vec1.getDotProduct(vec6), E, "vec1.dot(vec6)") &
            assertEquals(0, vec1.getDotProduct(vec7), E, "vec1.dot(vec7)") &
            assertEquals(0, vec1.getDotProduct(vec8), E, "vec1.dot(vec8)") &
            assertEquals(0, vec1.getDotProduct(vecA), E, "vec1.dot(vecA)") &
            assertEquals(0, vec1.getDotProduct(vecB), E, "vec1.dot(vecB)") &
            assertEquals(0, vec1.getDotProduct(vecC), E, "vec1.dot(vecC)") &
            assertEquals(0, vec1.getDotProduct(vecD), E, "vec1.dot(vecD)") &
            assertEquals(0, vec1.getDotProduct(vecE), E, "vec1.dot(vecE)") &
            assertEquals(0, vec1.getDotProduct(vecF), E, "vec1.dot(vecF)") &
            assertEquals(0, vec1.getDotProduct(vecG), E, "vec1.dot(vecG)") &
            assertEquals(0, vec2.getDotProduct(vec3), E, "vec2.dot(vec3)") &
            assertEquals(0, vec2.getDotProduct(vec4), E, "vec2.dot(vec4)") &
            assertNotEquals(0, vec2.getDotProduct(vec5), E, "vec2.dot(vec5)") &
            assertNotEquals(0, vec2.getDotProduct(vec6), E, "vec2.dot(vec6)") &
            assertEquals(0, vec2.getDotProduct(vec7), E, "vec2.dot(vec7)") &
            assertNotEquals(0, vec2.getDotProduct(vec8), E, "vec2.dot(vec8)") &
            assertNotEquals(0, vec2.getDotProduct(vecA), E, "vec2.dot(vecA)") &
            assertEquals(0, vec2.getDotProduct(vecB), E, "vec2.dot(vecB)") &
            assertEquals(0, vec2.getDotProduct(vecC), E, "vec2.dot(vecC)") &
            assertNotEquals(0, vec2.getDotProduct(vecD), E, "vec2.dot(vecD)") &
            assertNotEquals(0, vec2.getDotProduct(vecE), E, "vec2.dot(vecE)") &
            assertEquals(0, vec2.getDotProduct(vecF), E, "vec2.dot(vecF)") &
            assertNotEquals(0, vec2.getDotProduct(vecG), E, "vec2.dot(vecG)") &
            assertEquals(0, vec3.getDotProduct(vec4), E, "vec3.dot(vec4)") &
            assertNotEquals(0, vec3.getDotProduct(vec5), E, "vec3.dot(vec5)") &
            assertEquals(0, vec3.getDotProduct(vec6), E, "vec3.dot(vec6)") &
            assertNotEquals(0, vec3.getDotProduct(vec7), E, "vec3.dot(vec7)") &
            assertNotEquals(0, vec3.getDotProduct(vec8), E, "vec3.dot(vec8)") &
            assertEquals(0, vec3.getDotProduct(vecA), E, "vec3.dot(vecA)") &
            assertNotEquals(0, vec3.getDotProduct(vecB), E, "vec3.dot(vecB)") &
            assertEquals(0, vec3.getDotProduct(vecC), E, "vec3.dot(vecC)") &
            assertNotEquals(0, vec3.getDotProduct(vecD), E, "vec3.dot(vecD)") &
            assertEquals(0, vec3.getDotProduct(vecE), E, "vec3.dot(vecE)") &
            assertNotEquals(0, vec3.getDotProduct(vecF), E, "vec3.dot(vecF)") &
            assertNotEquals(0, vec3.getDotProduct(vecG), E, "vec3.dot(vecG)") &
            assertEquals(0, vec4.getDotProduct(vec5), E, "vec4.dot(vec5)") &
            assertNotEquals(0, vec4.getDotProduct(vec6), E, "vec4.dot(vec6)") &
            assertNotEquals(0, vec4.getDotProduct(vec7), E, "vec4.dot(vec7)") &
            assertNotEquals(0, vec4.getDotProduct(vec8), E, "vec4.dot(vec8)") &
            assertEquals(0, vec4.getDotProduct(vecA), E, "vec4.dot(vecA)") &
            assertEquals(0, vec4.getDotProduct(vecB), E, "vec4.dot(vecB)") &
            assertNotEquals(0, vec4.getDotProduct(vecC), E, "vec4.dot(vecC)") &
            assertEquals(0, vec4.getDotProduct(vecD), E, "vec4.dot(vecD)") &
            assertNotEquals(0, vec4.getDotProduct(vecE), E, "vec4.dot(vecE)") &
            assertNotEquals(0, vec4.getDotProduct(vecF), E, "vec4.dot(vecF)") &
            assertNotEquals(0, vec4.getDotProduct(vecG), E, "vec4.dot(vecG)") &
            assertNotEquals(0, vec5.getDotProduct(vec6), E, "vec5.dot(vec6)") &
            assertNotEquals(0, vec5.getDotProduct(vec7), E, "vec5.dot(vec7)") &
            assertNotEquals(0, vec5.getDotProduct(vec8), E, "vec5.dot(vec8)") &
            assertNotEquals(0, vec5.getDotProduct(vecA), E, "vec5.dot(vecA)") &
            assertNotEquals(0, vec5.getDotProduct(vecB), E, "vec5.dot(vecB)") &
            assertEquals(0, vec5.getDotProduct(vecC), E, "vec5.dot(vecC)") &
            assertNotEquals(0, vec5.getDotProduct(vecD), E, "vec5.dot(vecD)") &
            assertNotEquals(0, vec5.getDotProduct(vecE), E, "vec5.dot(vecE)") &
            assertNotEquals(0, vec5.getDotProduct(vecF), E, "vec5.dot(vecF)") &
            assertNotEquals(0, vec5.getDotProduct(vecG), E, "vec5.dot(vecG)") &
            assertNotEquals(0, vec6.getDotProduct(vec7), E, "vec6.dot(vec7)") &
            assertNotEquals(0, vec6.getDotProduct(vec8), E, "vec6.dot(vec8)") &
            assertNotEquals(0, vec6.getDotProduct(vecA), E, "vec6.dot(vecA)") &
            assertEquals(0, vec6.getDotProduct(vecB), E, "vec6.dot(vecB)") &
            assertNotEquals(0, vec6.getDotProduct(vecC), E, "vec6.dot(vecC)") &
            assertNotEquals(0, vec6.getDotProduct(vecD), E, "vec6.dot(vecD)") &
            assertNotEquals(0, vec6.getDotProduct(vecE), E, "vec6.dot(vecE)") &
            assertNotEquals(0, vec6.getDotProduct(vecF), E, "vec6.dot(vecF)") &
            assertNotEquals(0, vec6.getDotProduct(vecG), E, "vec6.dot(vecG)") &
            assertNotEquals(0, vec7.getDotProduct(vec8), E, "vec7.dot(vec8)") &
            assertEquals(0, vec7.getDotProduct(vecA), E, "vec7.dot(vecA)") &
            assertNotEquals(0, vec7.getDotProduct(vecB), E, "vec7.dot(vecB)") &
            assertNotEquals(0, vec7.getDotProduct(vecC), E, "vec7.dot(vecC)") &
            assertNotEquals(0, vec7.getDotProduct(vecD), E, "vec7.dot(vecD)") &
            assertNotEquals(0, vec7.getDotProduct(vecE), E, "vec7.dot(vecE)") &
            assertNotEquals(0, vec7.getDotProduct(vecF), E, "vec7.dot(vecF)") &
            assertNotEquals(0, vec7.getDotProduct(vecG), E, "vec7.dot(vecG)");
    }

    bool testGetCrossProduct1()
    {
        const PureVector vec1(1, 0, 0);
        const PureVector vec2(0, 1, 0);
        const PureVector vec3 = vec1.getCrossProduct(vec2);

        return assertEquals(0, vec3.getX(), E, "x") &
            assertEquals(0, vec3.getY(), E, "y") &
            assertEquals(1, vec3.getZ(), E, "z");
    }  

    bool testGetCrossProduct2()
    {
        const PureVector vec1(1, 2, 3);
        const PureVector vec2(4, 5, 6);
        const PureVector vec3 = vec1.getCrossProduct(vec2);

        return assertEquals(-vec2.getCrossProduct(vec1), vec3, "not antiCommutative") &
            assertEquals(vec1.getCrossProduct(vec2+vec3), vec1.getCrossProduct(vec2) + vec1.getCrossProduct(vec3),
                "not distributive on addition");
    }  

    bool testGetCrossProduct3()
    {
        const PureVector vec1( 0,  0,  0);
        const PureVector vec2( 1,  0,  0);
        const PureVector vec3( 0,  1,  0);
        const PureVector vec4( 0,  0,  1);
        const PureVector vec5( 1,  1,  0);
        const PureVector vec6( 1,  0,  1);
        const PureVector vec7( 0,  1,  1);
        const PureVector vec8( 1,  1,  1);
        const PureVector vecA(-1,  0,  0);
        const PureVector vecB( 0, -1,  0);
        const PureVector vecC( 0,  0, -1);
        const PureVector vecD(-1, -1,  0);
        const PureVector vecE(-1,  0, -1);
        const PureVector vecF( 0, -1, -1);
        const PureVector vecG(-1, -1, -1);

        return assertTrue(vec2.getCrossProduct(vec2).isZero(), "vec2.cross(vec2)") &
            assertTrue(vec2.getCrossProduct(vecA).isZero(), "vec2.cross(vecA)") &
            assertTrue(vecA.getCrossProduct(vecA).isZero(), "vecA.cross(vecA)") &
            assertTrue(vec3.getCrossProduct(vec3).isZero(), "vec3.cross(vec3)") &
            assertTrue(vec3.getCrossProduct(vecB).isZero(), "vec3.cross(vecB)") &
            assertTrue(vecB.getCrossProduct(vecB).isZero(), "vecB.cross(vecB)") &
            assertTrue(vec4.getCrossProduct(vec4).isZero(), "vec4.cross(vec4)") &
            assertTrue(vec4.getCrossProduct(vecC).isZero(), "vec4.cross(vecC)") &
            assertTrue(vecC.getCrossProduct(vecC).isZero(), "vecC.cross(vecC)") &
            assertTrue(vec5.getCrossProduct(vec5).isZero(), "vec5.cross(vec5)") &
            assertTrue(vec5.getCrossProduct(vecD).isZero(), "vec5.cross(vecD)") &
            assertTrue(vecD.getCrossProduct(vecD).isZero(), "vecD.cross(vecD)") &
            assertTrue(vec6.getCrossProduct(vec6).isZero(), "vec6.cross(vec6)") &
            assertTrue(vec6.getCrossProduct(vecE).isZero(), "vec6.cross(vecE)") &
            assertTrue(vecE.getCrossProduct(vecE).isZero(), "vecE.cross(vecE)") &
            assertTrue(vec7.getCrossProduct(vec7).isZero(), "vec7.cross(vec7)") &
            assertTrue(vec7.getCrossProduct(vecF).isZero(), "vec7.cross(vecF)") &
            assertTrue(vecF.getCrossProduct(vecF).isZero(), "vecF.cross(vecF)") &
            assertTrue(vec8.getCrossProduct(vec8).isZero(), "vec8.cross(vec8)") &
            assertTrue(vec8.getCrossProduct(vecG).isZero(), "vec8.cross(vecG)") &
            assertTrue(vecG.getCrossProduct(vecG).isZero(), "vecG.cross(vecG)");
    }

    bool testOperatorEquals()
    {
        const PureVector vec1;
        const PureVector vec2;

        return assertEquals(vec1, vec2, "vec1 == vec2") &
            assertEquals(vec1, vec1, "vec1 == vec1");
    }

    bool testOperatorNotEquals()
    {
        const PureVector vec1;
        const PureVector vec2;

        return assertFalse(vec1 != vec2, "vec1 != vec2") &
            assertFalse(vec1 != vec1, "vec1 != vec1");
    }

    bool testOperatorAssignment()
    {
        const PureVector vec1(1, 2, 3);
        const PureVector vec2 = vec1;

        return assertEquals(vec1, vec2);
    }

    bool testOperatorAddition()
    {
        const PureVector vecSrc(1, 2, 3);
        const PureVector vecDst = vecSrc + vecSrc;

        return assertEquals(2, vecDst.getX(), E, "getX()") &
            assertEquals(4, vecDst.getY(), E, "getY()") &
            assertEquals(6, vecDst.getZ(), E, "getZ()");
    }

    bool testOperatorAdditionAssignment()
    {
        PureVector vecSrc(1, 2, 3);
        vecSrc += vecSrc;

        return assertEquals(2, vecSrc.getX(), E, "getX()") &
            assertEquals(4, vecSrc.getY(), E, "getY()") &
            assertEquals(6, vecSrc.getZ(), E, "getZ()");
    }

    bool testOperatorSubtraction()
    {
        const PureVector vecSrc(1, 2, 3);
        const PureVector vecDst = vecSrc - vecSrc;

        return assertTrue(vecDst.isZero());
    }

    bool testOperatorSubtractionAssignment()
    {
        PureVector vecSrc(1, 2, 3);
        vecSrc -= vecSrc;

        return assertTrue(vecSrc.isZero());
    }

    bool testOperatorMultiplicationByScalar1()
    {
        PureVector vec(1, 2, 3);
        vec = vec * 2;

        return assertEquals(2, vec.getX(), E, "getX()") &
            assertEquals(4, vec.getY(), E, "getY()") &
            assertEquals(6, vec.getZ(), E, "getZ()");
    }

    bool testOperatorMultiplicationByScalar2()
    {
        PureVector vec(1, 2, 3);
        vec = 2 * vec;

        return assertEquals(2, vec.getX(), E, "getX()") &
            assertEquals(4, vec.getY(), E, "getY()") &
            assertEquals(6, vec.getZ(), E, "getZ()");
    }

    bool testOperatorMultiplicationAssignmentByScalar()
    {
        PureVector vec(1, 2, 3);
        vec *= 2;

        return assertEquals(2, vec.getX(), E, "getX()") &
            assertEquals(4, vec.getY(), E, "getY()") &
            assertEquals(6, vec.getZ(), E, "getZ()");
    }

    bool testOperatorDivisionByScalar1()
    {
        PureVector vec(2, 4, 6);
        vec = vec / 2;

        return assertEquals(1, vec.getX(), E, "getX()") &
            assertEquals(2, vec.getY(), E, "getY()") &
            assertEquals(3, vec.getZ(), E, "getZ()");
    }

    bool testOperatorDivisionAssignmentByScalar()
    {
        PureVector vec(2, 4, 6);
        vec /= 2;

        return assertEquals(1, vec.getX(), E, "getX()") &
            assertEquals(2, vec.getY(), E, "getY()") &
            assertEquals(3, vec.getZ(), E, "getZ()");
    }

    bool testOperatorDotProduct1()
    {
        const PureVector vec1(1, 2, 3);
        const PureVector vec2(4, 5, 6);

        const float dotProduct1 = vec1 * vec2;
        const float dotProduct2 = vec2 * vec1;

        return assertEquals(vec1.getX() * vec2.getX() + vec1.getY() * vec2.getY() + vec1.getZ() * vec2.getZ(), dotProduct1, "dotProduct1") &
            assertEquals(dotProduct1, dotProduct2, "dotProduct2");
    }

    bool testOperatorDotProduct2()
    {
        const PureVector vec1( 0,  0,  0);
        const PureVector vec2( 1,  0,  0);
        const PureVector vec3( 0,  1,  0);
        const PureVector vec4( 0,  0,  1);
        const PureVector vec5( 1,  1,  0);
        const PureVector vec6( 1,  0,  1);
        const PureVector vec7( 0,  1,  1);
        const PureVector vec8( 1,  1,  1);
        const PureVector vecA(-1,  0,  0);
        const PureVector vecB( 0, -1,  0);
        const PureVector vecC( 0,  0, -1);
        const PureVector vecD(-1, -1,  0);
        const PureVector vecE(-1,  0, -1);
        const PureVector vecF( 0, -1, -1);
        const PureVector vecG(-1, -1, -1);

        return assertEquals(0, vec1 * vec2, E, "vec1 * vec2") &
            assertEquals(0, vec1 * vec3, E, "vec1 * vec3") &
            assertEquals(0, vec1 * vec4, E, "vec1 * vec4") &
            assertEquals(0, vec1 * vec5, E, "vec1 * vec5") &
            assertEquals(0, vec1 * vec6, E, "vec1 * vec6") &
            assertEquals(0, vec1 * vec7, E, "vec1 * vec7") &
            assertEquals(0, vec1 * vec8, E, "vec1 * vec8") &
            assertEquals(0, vec1 * vecA, E, "vec1 * vecA") &
            assertEquals(0, vec1 * vecB, E, "vec1 * vecB") &
            assertEquals(0, vec1 * vecC, E, "vec1 * vecC") &
            assertEquals(0, vec1 * vecD, E, "vec1 * vecD") &
            assertEquals(0, vec1 * vecE, E, "vec1 * vecE") &
            assertEquals(0, vec1 * vecF, E, "vec1 * vecF") &
            assertEquals(0, vec1 * vecG, E, "vec1 * vecG") &
            assertEquals(0, vec2 * vec3, E, "vec2 * vec3") &
            assertEquals(0, vec2 * vec4, E, "vec2 * vec4") &
            assertNotEquals(0, vec2 * vec5, E, "vec2 * vec5") &
            assertNotEquals(0, vec2 * vec6, E, "vec2 * vec6") &
            assertEquals(0, vec2 * vec7, E, "vec2 * vec7") &
            assertNotEquals(0, vec2 * vec8, E, "vec2 * vec8") &
            assertNotEquals(0, vec2 * vecA, E, "vec2 * vecA") &
            assertEquals(0, vec2 * vecB, E, "vec2 * vecB") &
            assertEquals(0, vec2 * vecC, E, "vec2 * vecC") &
            assertNotEquals(0, vec2 * vecD, E, "vec2 * vecD") &
            assertNotEquals(0, vec2 * vecE, E, "vec2 * vecE") &
            assertEquals(0, vec2 * vecF, E, "vec2 * vecF") &
            assertNotEquals(0, vec2 * vecG, E, "vec2 * vecG") &
            assertEquals(0, vec3 * vec4, E, "vec3 * vec4") &
            assertNotEquals(0, vec3 * vec5, E, "vec3 * vec5") &
            assertEquals(0, vec3 * vec6, E, "vec3 * vec6") &
            assertNotEquals(0, vec3 * vec7, E, "vec3 * vec7") &
            assertNotEquals(0, vec3 * vec8, E, "vec3 * vec8") &
            assertEquals(0, vec3 * vecA, E, "vec3 * vecA") &
            assertNotEquals(0, vec3 * vecB, E, "vec3 * vecB") &
            assertEquals(0, vec3 * vecC, E, "vec3 * vecC") &
            assertNotEquals(0, vec3 * vecD, E, "vec3 * vecD") &
            assertEquals(0, vec3 * vecE, E, "vec3 * vecE") &
            assertNotEquals(0, vec3 * vecF, E, "vec3 * vecF") &
            assertNotEquals(0, vec3 * vecG, E, "vec3 * vecG") &
            assertEquals(0, vec4 * vec5, E, "vec4 * vec5") &
            assertNotEquals(0, vec4 * vec6, E, "vec4 * vec6") &
            assertNotEquals(0, vec4 * vec7, E, "vec4 * vec7") &
            assertNotEquals(0, vec4 * vec8, E, "vec4 * vec8") &
            assertEquals(0, vec4 * vecA, E, "vec4 * vecA") &
            assertEquals(0, vec4 * vecB, E, "vec4 * vecB") &
            assertNotEquals(0, vec4 * vecC, E, "vec4 * vecC") &
            assertEquals(0, vec4 * vecD, E, "vec4 * vecD") &
            assertNotEquals(0, vec4 * vecE, E, "vec4 * vecE") &
            assertNotEquals(0, vec4 * vecF, E, "vec4 * vecF") &
            assertNotEquals(0, vec4 * vecG, E, "vec4 * vecG") &
            assertNotEquals(0, vec5 * vec6, E, "vec5 * vec6") &
            assertNotEquals(0, vec5 * vec7, E, "vec5 * vec7") &
            assertNotEquals(0, vec5 * vec8, E, "vec5 * vec8") &
            assertNotEquals(0, vec5 * vecA, E, "vec5 * vecA") &
            assertNotEquals(0, vec5 * vecB, E, "vec5 * vecB") &
            assertEquals(0, vec5 * vecC, E, "vec5 * vecC") &
            assertNotEquals(0, vec5 * vecD, E, "vec5 * vecD") &
            assertNotEquals(0, vec5 * vecE, E, "vec5 * vecE") &
            assertNotEquals(0, vec5 * vecF, E, "vec5 * vecF") &
            assertNotEquals(0, vec5 * vecG, E, "vec5 * vecG") &
            assertNotEquals(0, vec6 * vec7, E, "vec6 * vec7") &
            assertNotEquals(0, vec6 * vec8, E, "vec6 * vec8") &
            assertNotEquals(0, vec6 * vecA, E, "vec6 * vecA") &
            assertEquals(0, vec6 * vecB, E, "vec6 * vecB") &
            assertNotEquals(0, vec6 * vecC, E, "vec6 * vecC") &
            assertNotEquals(0, vec6 * vecD, E, "vec6 * vecD") &
            assertNotEquals(0, vec6 * vecE, E, "vec6 * vecE") &
            assertNotEquals(0, vec6 * vecF, E, "vec6 * vecF") &
            assertNotEquals(0, vec6 * vecG, E, "vec6 * vecG") &
            assertNotEquals(0, vec7 * vec8, E, "vec7 * vec8") &
            assertEquals(0, vec7 * vecA, E, "vec7 * vecA") &
            assertNotEquals(0, vec7 * vecB, E, "vec7 * vecB") &
            assertNotEquals(0, vec7 * vecC, E, "vec7 * vecC") &
            assertNotEquals(0, vec7 * vecD, E, "vec7 * vecD") &
            assertNotEquals(0, vec7 * vecE, E, "vec7 * vecE") &
            assertNotEquals(0, vec7 * vecF, E, "vec7 * vecF") &
            assertNotEquals(0, vec7 * vecG, E, "vec7 * vecG");
    }

    bool testOperatorCrossProduct1()
    {
        const PureVector vec1(1, 0, 0);
        const PureVector vec2(0, 1, 0);

        PureVector vec3 = vec1 ^ vec2;

        return assertEquals(0, vec3.getX(), E, "x") &
            assertEquals(0, vec3.getY(), E, "y") &
            assertEquals(1, vec3.getZ(), E, "z");
    }

    bool testOperatorCrossProduct2()
    {
        const PureVector vec1(1, 2, 3);
        const PureVector vec2(4, 5, 6);

        PureVector vec3 = vec1 ^ vec2;

        return assertEquals(-vec2 ^ vec1, vec3, "not antiCommutative") &
            assertEquals(vec1 ^ (vec2+vec3), (vec1 ^ vec2) + (vec1 ^ vec3), "not distributive on addition");
    }

    bool testOperatorCrossProduct3()
    {
        const PureVector vec1( 0,  0,  0);
        const PureVector vec2( 1,  0,  0);
        const PureVector vec3( 0,  1,  0);
        const PureVector vec4( 0,  0,  1);
        const PureVector vec5( 1,  1,  0);
        const PureVector vec6( 1,  0,  1);
        const PureVector vec7( 0,  1,  1);
        const PureVector vec8( 1,  1,  1);
        const PureVector vecA(-1,  0,  0);
        const PureVector vecB( 0, -1,  0);
        const PureVector vecC( 0,  0, -1);
        const PureVector vecD(-1, -1,  0);
        const PureVector vecE(-1,  0, -1);
        const PureVector vecF( 0, -1, -1);
        const PureVector vecG(-1, -1, -1);

        return assertTrue((vec2 ^ vec2).isZero(), "vec2 ^ vec2") &
            assertTrue((vec2 ^ vecA).isZero(), "vec2 ^ vecA") &
            assertTrue((vecA ^ vecA).isZero(), "vecA ^ vecA") &
            assertTrue((vec3 ^ vec3).isZero(), "vec3 ^ vec3") &
            assertTrue((vec3 ^ vecB).isZero(), "vec3 ^ vecB") &
            assertTrue((vecB ^ vecB).isZero(), "vecB ^ vecB") &
            assertTrue((vec4 ^ vec4).isZero(), "vec4 ^ vec4") &
            assertTrue((vec4 ^ vecC).isZero(), "vec4 ^ vecC") &
            assertTrue((vecC ^ vecC).isZero(), "vecC ^ vecC") &
            assertTrue((vec5 ^ vec5).isZero(), "vec5 ^ vec5") &
            assertTrue((vec5 ^ vecD).isZero(), "vec5 ^ vecD") &
            assertTrue((vecD ^ vecD).isZero(), "vecD ^ vecD") &
            assertTrue((vec6 ^ vec6).isZero(), "vec6 ^ vec6") &
            assertTrue((vec6 ^ vecE).isZero(), "vec6 ^ vecE") &
            assertTrue((vecE ^ vecE).isZero(), "vecE ^ vecE") &
            assertTrue((vec7 ^ vec7).isZero(), "vec7 ^ vec7") &
            assertTrue((vec7 ^ vecF).isZero(), "vec7 ^ vecF") &
            assertTrue((vecF ^ vecF).isZero(), "vecF ^ vecF") &
            assertTrue((vec8 ^ vec8).isZero(), "vec8 ^ vec8") &
            assertTrue((vec8 ^ vecG).isZero(), "vec8 ^ vecG") &
            assertTrue((vecG ^ vecG).isZero(), "vecG ^ vecG");
    }

    bool testOperatorAccessElement1()
    {
        PureVector vec;

        vec(0) = 1;
        vec(1) = 2;
        vec(2) = 3;

        return assertEquals(1, vec(0), E, "(0)") &
            assertEquals(2, vec(1), E, "(1)") &
            assertEquals(3, vec(2), E, "(2)");
    }

    bool testOperatorAccessElement2()
    {
        PureVector vec;

        vec[0] = 1;
        vec[1] = 2;
        vec[2] = 3;

        return assertEquals(1, vec[0], E, "[0]") &
            assertEquals(2, vec[1], E, "[1]") &
            assertEquals(3, vec[2], E, "[2]");
    }

    bool testOperatorNegative()
    {
        PureVector vec(1, 2, 3);

        vec = -vec;

        return assertEquals(-1, vec(0), E, "(0)") &
            assertEquals(-2, vec(1), E, "(1)") &
            assertEquals(-3, vec(2), E, "(2)");
    }

    bool testOperatorCrossProductAssignment1()
    {
        PureVector vec1(1, 0, 0);
        const PureVector vec2(0, 1, 0);

        vec1 ^= vec2;

        return assertEquals(0, vec1.getX(), E, "x") &
            assertEquals(0, vec1.getY(), E, "y") &
            assertEquals(1, vec1.getZ(), E, "z");
    }

    bool testOperatorCompareLess()
    {
        const PureVector nullvec1;
        const PureVector nullvec2;

        bool b = assertFalse(nullvec1 < nullvec1, "nullvec self");
        b &= assertFalse(nullvec1 < nullvec2, "nullvec 1") & assertFalse(nullvec2 < nullvec1, "nullvec 2");

        const PureVector vecX(1, 0, 0);
        const PureVector vecY(0, 1, 0);
        const PureVector vecZ(0, 0, 1);

        b &= assertFalse(vecX < vecX, "vecX self");
        b &= assertFalse(vecY < vecY, "vecY self");
        b &= assertFalse(vecZ < vecZ, "vecZ self");

        b &= assertFalse(vecX < vecY, "vecX < vecY") & assertTrue(vecY < vecX, "vecY < vecX");
        b &= assertFalse(vecX < vecZ, "vecX < vecZ") & assertTrue(vecZ < vecX, "vecZ < vecX");
        b &= assertFalse(vecY < vecZ, "vecY < vecZ") & assertTrue(vecZ < vecY, "vecZ < vecY");

        const PureVector vecXY(1, 1, 0);
        const PureVector vecXZ(1, 0, 1);
        const PureVector vecYZ(0, 1, 1);

        b &= assertFalse(vecXY < vecXY, "vecXY self");
        b &= assertFalse(vecXZ < vecXZ, "vecXZ self");
        b &= assertFalse(vecYZ < vecYZ, "vecYZ self");

        b &= assertFalse(vecXY < vecXZ, "vecXY < vecXZ") & assertTrue(vecXZ < vecXY, "vecXZ < vecXY");
        b &= assertFalse(vecXY < vecYZ, "vecXY < vecYZ") & assertTrue(vecYZ < vecXY, "vecYZ < vecXY");
        b &= assertFalse(vecXZ < vecYZ, "vecXZ < vecYZ") & assertTrue(vecYZ < vecXZ, "vecYZ < vecXZ");

        const PureVector vecX2YZ(1, 2, 1);
        const PureVector vecX3YZ(1, 3, 1);
        const PureVector vecX4YZ(1, 4, 1);

        b &= assertTrue(vecX2YZ < vecX3YZ, "vecX2YZ < vecX3YZ") & assertFalse(vecX3YZ < vecX2YZ, "vecX3YZ < vecX2YZ");
        b &= assertTrue(vecX3YZ < vecX4YZ, "vecX3YZ < vecX4YZ") & assertFalse(vecX4YZ < vecX3YZ, "vecX4YZ < vecX3YZ");
        b &= assertTrue(vecX2YZ < vecX4YZ, "vecX2YZ < vecX4YZ") & assertFalse(vecX4YZ < vecX2YZ, "vecX4YZ < vecX2YZ");

        // std container ordering uses operator< of object, so let's see if auto ordering by set works as we expect

        // TODO: with cpp11 initializer list, this vector can be const
        std::vector<PureVector> expectedOrder;
        expectedOrder.push_back(vecZ);
        expectedOrder.push_back(vecY);
        expectedOrder.push_back(vecYZ);
        expectedOrder.push_back(vecX);
        expectedOrder.push_back(vecXZ);
        expectedOrder.push_back(vecXY);
        expectedOrder.push_back(vecX2YZ);
        expectedOrder.push_back(vecX3YZ);
        expectedOrder.push_back(vecX4YZ);
        
        std::set<PureVector> actualOrder;
        actualOrder.insert(vecX);
        actualOrder.insert(vecY);
        actualOrder.insert(vecZ);
        actualOrder.insert(vecXY);
        actualOrder.insert(vecXZ);
        actualOrder.insert(vecYZ);
        actualOrder.insert(vecX2YZ);
        actualOrder.insert(vecX3YZ);
        actualOrder.insert(vecX4YZ);

        b &= assertEquals(expectedOrder.size(), actualOrder.size(), "container sizes");
        if ( b )
        {
            auto expIt = expectedOrder.begin();
            auto actualIt = actualOrder.begin();
            while ( b && (expIt != expectedOrder.end()) )
            {
                b &= assertEquals(*expIt, *actualIt, "elem order");
                expIt++;
                actualIt++;
            }
        }

        return b;
    }

    bool testOperatorCompareLessOrEqual()
    {
        const PureVector nullvec1;
        const PureVector nullvec2;

        bool b = assertTrue(nullvec1 <= nullvec1, "nullvec self");
        b &= assertTrue(nullvec1 <= nullvec2, "nullvec 1") & assertTrue(nullvec2 <= nullvec1, "nullvec 2");

        const PureVector vecX(1, 0, 0);
        const PureVector vecY(0, 1, 0);
        const PureVector vecZ(0, 0, 1);

        b &= assertTrue(vecX <= vecX, "vecX self");
        b &= assertTrue(vecY <= vecY, "vecY self");
        b &= assertTrue(vecZ <= vecZ, "vecZ self");

        b &= assertFalse(vecX <= vecY, "vecX < vecY") & assertTrue(vecY <= vecX, "vecY < vecX");
        b &= assertFalse(vecX <= vecZ, "vecX < vecZ") & assertTrue(vecZ <= vecX, "vecZ < vecX");
        b &= assertFalse(vecY <= vecZ, "vecY < vecZ") & assertTrue(vecZ <= vecY, "vecZ < vecY");

        const PureVector vecXY(1, 1, 0);
        const PureVector vecXZ(1, 0, 1);
        const PureVector vecYZ(0, 1, 1);

        b &= assertTrue(vecXY <= vecXY, "vecXY self");
        b &= assertTrue(vecXZ <= vecXZ, "vecXZ self");
        b &= assertTrue(vecYZ <= vecYZ, "vecYZ self");

        b &= assertFalse(vecXY <= vecXZ, "vecXY < vecXZ") & assertTrue(vecXZ <= vecXY, "vecXZ < vecXY");
        b &= assertFalse(vecXY <= vecYZ, "vecXY < vecYZ") & assertTrue(vecYZ <= vecXY, "vecYZ < vecXY");
        b &= assertFalse(vecXZ <= vecYZ, "vecXZ < vecYZ") & assertTrue(vecYZ <= vecXZ, "vecYZ < vecXZ");

        return b;
    }

    bool testOperatorCompareGreater()
    {
        const PureVector nullvec1;
        const PureVector nullvec2;

        bool b = assertFalse(nullvec1 > nullvec1, "nullvec self");
        b &= assertFalse(nullvec1 > nullvec2, "nullvec 1") & assertFalse(nullvec2 > nullvec1, "nullvec 2");

        const PureVector vecX(1, 0, 0);
        const PureVector vecY(0, 1, 0);
        const PureVector vecZ(0, 0, 1);

        b &= assertFalse(vecX > vecX, "vecX self");
        b &= assertFalse(vecY > vecY, "vecY self");
        b &= assertFalse(vecZ > vecZ, "vecZ self");

        b &= assertTrue(vecX > vecY, "vecX > vecY") & assertFalse(vecY > vecX, "vecY > vecX");
        b &= assertTrue(vecX > vecZ, "vecX > vecZ") & assertFalse(vecZ > vecX, "vecZ > vecX");
        b &= assertTrue(vecY > vecZ, "vecY > vecZ") & assertFalse(vecZ > vecY, "vecZ > vecY");

        const PureVector vecXY(1, 1, 0);
        const PureVector vecXZ(1, 0, 1);
        const PureVector vecYZ(0, 1, 1);

        b &= assertFalse(vecXY > vecXY, "vecXY self");
        b &= assertFalse(vecXZ > vecXZ, "vecXZ self");
        b &= assertFalse(vecYZ > vecYZ, "vecYZ self");

        b &= assertTrue(vecXY > vecXZ, "vecXY > vecXZ") & assertFalse(vecXZ > vecXY, "vecXZ > vecXY");
        b &= assertTrue(vecXY > vecYZ, "vecXY > vecYZ") & assertFalse(vecYZ > vecXY, "vecYZ > vecXY");
        b &= assertTrue(vecXZ > vecYZ, "vecXZ > vecYZ") & assertFalse(vecYZ > vecXZ, "vecYZ > vecXZ");

        return b;
    }

    bool testOperatorCompareGreaterOrEqual()
    {
        const PureVector nullvec1;
        const PureVector nullvec2;

        bool b = assertTrue(nullvec1 >= nullvec1, "nullvec self");
        b &= assertTrue(nullvec1 >= nullvec2, "nullvec 1") & assertTrue(nullvec2 >= nullvec1, "nullvec 2");

        const PureVector vecX(1, 0, 0);
        const PureVector vecY(0, 1, 0);
        const PureVector vecZ(0, 0, 1);

        b &= assertTrue(vecX >= vecX, "vecX self");
        b &= assertTrue(vecY >= vecY, "vecY self");
        b &= assertTrue(vecZ >= vecZ, "vecZ self");

        b &= assertTrue(vecX >= vecY, "vecX > vecY") & assertFalse(vecY >= vecX, "vecY > vecX");
        b &= assertTrue(vecX >= vecZ, "vecX > vecZ") & assertFalse(vecZ >= vecX, "vecZ > vecX");
        b &= assertTrue(vecY >= vecZ, "vecY > vecZ") & assertFalse(vecZ >= vecY, "vecZ > vecY");

        const PureVector vecXY(1, 1, 0);
        const PureVector vecXZ(1, 0, 1);
        const PureVector vecYZ(0, 1, 1);

        b &= assertTrue(vecXY >= vecXY, "vecXY self");
        b &= assertTrue(vecXZ >= vecXZ, "vecXZ self");
        b &= assertTrue(vecYZ >= vecYZ, "vecYZ self");

        b &= assertTrue(vecXY >= vecXZ, "vecXY > vecXZ") & assertFalse(vecXZ >= vecXY, "vecXZ > vecXY");
        b &= assertTrue(vecXY >= vecYZ, "vecXY > vecYZ") & assertFalse(vecYZ >= vecXY, "vecYZ > vecXY");
        b &= assertTrue(vecXZ >= vecYZ, "vecXZ > vecYZ") & assertFalse(vecYZ >= vecXZ, "vecYZ > vecXZ");

        return b;
    }

    bool testOperatorWriteToStream()
    {
        PureVector vec1(1, 2, 3);
        std::stringstream ssExpected, ssTested;
        ssExpected << "[1,2,3,1]";
        ssTested << vec1;

        return assertEquals(ssExpected.str(), ssTested.str());
    }

}; // class PureVectorTest