/*
    ###################################################################################
    PRREObject3D.cpp
    This file is part of PRRE.
    PRREObject3D class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "PRREObject3DManager.h"
#include "PRREObject3DImpl.h"
#include "../PRREGLextensionFuncs.h"
#include "../PRREGLsnippets.h"
#include "../Hardware/PRREhwInfo.h"

using namespace std;

/*

    Notes

    ***************************

    GeForce 2 cards (and probably GeForce 4MX) only supported 16-bit indexing in HW, so
    if you went over 64Ki vertices then it would have to do the indexing in software.

    Some manual say that glDrawElements()' type parameter must be GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT, while some allow GL_UNSIGNED_INT too.
    Depends on implementation. This also means that we must use the smallest type given to glDrawElements() not only because of
    performance considerations but also in order to make the function work on older implementations.

    glDrawRangeElements() indices and vertices limits:
    http://www.opengl.org/registry/specs/EXT/draw_range_elements.txt
    The specification of glDrawElements does not allow optimal performance for some OpenGL implementations, however. 
    In particular, it has no restrictions on the number of indices given, the number of unique vertices referenced nor a
    direct indication of the set of unique vertices referenced by the given indices.  This forces some OpenGL
    implementations to walk the index data given, building up a separate list of unique vertex references for later use
    in the pipeline. Additionally, since some OpenGL implementations have internal limitations on how many vertices they can
    deal with simultaneously the unbounded nature of glDrawElements requires the implementation to be prepared to segment the
    input data and do multiple passes.  These preprocessing steps can consume a significant amount of time.
    glDrawRangeElementsEXT is designed to avoid the preprocessing steps which may be necessary for glDrawElements.
    As such it does not have the flexibility of glDrawElements but it is sufficiently functional for a large class of applications
    to benefit from its use. glDrawRangeElementsEXT enhances glDrawElements in two ways:
    1. The set of unique vertices referenced by the indices is explicitly indicated via the start and end parameters, removing the
        necessity to determine this through examination of the index data.  The implementation is given a contiguous chunk of vertex
        data that it can immediately begin streaming through the vertex pipeline.
    2. Recommended limits on the amount of data to be processed can be indicated by the implementation through GL_MAX_ELEMENTS_VERTICES_EXT
        and GL_MAX_ELEMENTS_INDICES_EXT. If an application respects these limits it removes the need to split the incoming data into multiple
        chunks since the maximums can be set to the optimal values for the implementation to handle in one pass.

    http://www.opengl.org/discussion_boards/showthread.php/146440-glDrawRangeElements-GL_MAX_ELEMENTS_VERTICES-INDICES
    However the performance effects of these caches has to do with the re-use of indices and
    the relative locality of vertex array contents. These performance effects aren't really
    related to the number of vertices actually processed by a particular glDrawRangeElements as
    the limits above are specified to describe.
    It's tempting to "re-interpret" these limits to be more "relevant" to how modern GPUs work, but
    that is not what the limits are specified to mean.

    http://stackoverflow.com/questions/7549991/questions-about-gldrawrangeelements
    Now that we know how to use glDrawElements, let's talk about glDrawRangeElements.
    When normally using glDrawElements, you specify a location in the source index array to pull from.
    The indices and count parameters tell OpenGL where to find the indices. But the actual indices pulled from
    this array could be anywhere within the boundaries of the source vertex array indices.
    glDrawRangeElements allows you to give OpenGL a range (half-open again) of vertex index values.
    What you are saying is that the indices it gets during this draw call will not exceed that range (half-open).
    This could allow the driver to perform useful optimizations. The start value should be the lowest index value
    that will be gotten from the index array, and the end should be the highest (+1 because it's half-open).
    It should not simply be the index of the first and last vertices.

    http://www.songho.ca/opengl/gl_vertexarray.html
    The additional parameters in glDrawRangeElements() are start and end index, then OpenGL prefetches a limited
    amount of vertices from these values: end - start + 1. And the values in index array must lie in between
    start and end index. Note that not all vertices in range (start, end) must be referenced. But, if you specify a
    sparsely used range, it causes unnecessary process for many unused vertices in that range.

    ***************************

    Regular VAs, compiled VAs, server-side VAs.

    Compiled vertex array: lock-unlock, but only 1 can be locked at a time, so the only use of this may be
    when the same geometry is rendered multiple times like multipass blending if no multitexturing available.
    Some performance gain may be observed also when using indices (drawElements) because shared vertices will be
    transformed and lit only once. This also means there is no such post-TnL cache optimisation with standard drawElements.

    Korai gyártóspecifikus VBO-k:
     - GeForce2: VAR: NV_vertex_array_range, NV_vertex_array_range2
       VRAM or AGP memory. Write is slow. Write is fast only if Fast Writes is supported.
       Rule: allocate static geometry in VRAM, allocate AGP mem for dynamic geometry.
       Modifying AGP memory is slow, so first work in system memory, then block-copy to AGP memory.
     - VAO: ATI_vertex_array_object, ATI_map_object_buffer, ATI_element_array (like ELEMENT_ARRAY_BUFFER_ARB target).
       Can't be AGP mem, only VRAM.

    Usage params:
    - ATI_vertex_array_object: STATIC_ATI, DYNAMIC_ATI
    - NV_vertex_array_range: readFreq, writeFreq, priority
    - ARB_vertex_buffer_object: STATIC, STREAM, DYNAMIC; DRAW, READ, COPY


    BufferObject: a GPU allocated memory buffer
	    Vertex Buffer Object: a BufferObject containing vertices informations (colors, position, custom data used by a shader, ...)
	    Pixel Buffer Object: a BufferObject containing pixel or texel informations. Mainly used to upload textures.
	    Element Buffer Object: a BufferObject containing indices (used by glDrawElements).
    Vertex Array: memory used by gl*Pointer call.
	    Might be host memory or a Vertex Buffer Object if it is bound using glBindBuffer command with GL_ARRAY_BUFFER.
    Element Array: memory used by glDrawElements call.
	    Might be host memory or an Element Buffer Object if it is bound using glBindBuffer command with GL_ELEMENT_ARRAY_BUFFER.


    ***************************

    GL_ATI_pn_triangles: ATI TruForm, Radeon 8500.

    ***************************

*/


/*
   PRREObject3D::PRREObject3DImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Gets the appropriate PRRE blend factor for the given GL enum.

    @return The appropriate PRRE blend factor for the given GL enum.
            PRRE_ZERO for invalid GL enum.
*/
TPRRE_BLENDFACTORS PRREObject3D::PRREObject3DImpl::getPRREblendFromGLblend(GLenum glb)
{
    switch( glb )
    {
    case GL_ZERO                : return PRRE_ZERO;
    case GL_ONE                 : return PRRE_ONE;
    case GL_SRC_COLOR           : return PRRE_SRC_COLOR;
    case GL_ONE_MINUS_SRC_COLOR : return PRRE_ONE_MINUS_SRC_COLOR;
    case GL_DST_COLOR           : return PRRE_DST_COLOR;
    case GL_ONE_MINUS_DST_COLOR : return PRRE_ONE_MINUS_DST_COLOR;
    case GL_SRC_ALPHA           : return PRRE_SRC_ALPHA;
    case GL_ONE_MINUS_SRC_ALPHA : return PRRE_ONE_MINUS_SRC_ALPHA;
    case GL_DST_ALPHA           : return PRRE_DST_ALPHA;         
    case GL_ONE_MINUS_DST_ALPHA : return PRRE_ONE_MINUS_DST_ALPHA;
    case GL_SRC_ALPHA_SATURATE  : return PRRE_SRC_ALPHA_SATURATE;
    default                     : return PRRE_ZERO;
    }
} // getPRREblendFromGLblend()


/**
    Gets the appropriate GL enum for the given PRRE blend factor.

    @return The appropriate GL enum for the given PRRE blend factor.
            GL_ZERO for invalid PRRE blend factor.
*/
GLenum PRREObject3D::PRREObject3DImpl::getGLblendFromPRREblend(TPRRE_BLENDFACTORS bf)
{
    switch( bf )
    {
    case PRRE_ZERO                : return GL_ZERO;
    case PRRE_ONE                 : return GL_ONE;
    case PRRE_SRC_COLOR           : return GL_SRC_COLOR;
    case PRRE_ONE_MINUS_SRC_COLOR : return GL_ONE_MINUS_SRC_COLOR;
    case PRRE_DST_COLOR           : return GL_DST_COLOR;
    case PRRE_ONE_MINUS_DST_COLOR : return GL_ONE_MINUS_DST_COLOR;
    case PRRE_SRC_ALPHA           : return GL_SRC_ALPHA;
    case PRRE_ONE_MINUS_SRC_ALPHA : return GL_ONE_MINUS_SRC_ALPHA;
    case PRRE_DST_ALPHA           : return GL_DST_ALPHA;         
    case PRRE_ONE_MINUS_DST_ALPHA : return GL_ONE_MINUS_DST_ALPHA;
    case PRRE_SRC_ALPHA_SATURATE  : return GL_SRC_ALPHA_SATURATE;
    default                       : return GL_ZERO;
    }
} // getGLblendFromPRREblend()


TPRRE_PRIMITIVE_FORMAT PRREObject3D::PRREObject3DImpl::getPRREprimitiveFromGLprimitive(GLenum glprim)
{
    switch (glprim)
    {
    case GL_TRIANGLES      : return PRRE_PM_TRIANGLES;
    case GL_TRIANGLE_STRIP : return PRRE_PM_TRIANGLE_STRIPS;
    case GL_TRIANGLE_FAN   : return PRRE_PM_TRIANGLE_FANS; 
    case GL_QUADS          : return PRRE_PM_QUADS;
    case GL_QUAD_STRIP     : return PRRE_PM_QUAD_STRIPS;
    case GL_LINES          : return PRRE_PM_LINES;
    case GL_LINE_STRIP     : return PRRE_PM_LINE_STRIPS;
    default                : return PRRE_PM_POINTS;    
    }
} // getPRREprimitiveFromGLprimitive()


GLenum PRREObject3D::PRREObject3DImpl::getGLprimitiveFromPRREprimitive(TPRRE_PRIMITIVE_FORMAT pf)
{
    switch (pf)
    {
    case PRRE_PM_TRIANGLES       : return GL_TRIANGLES;
    case PRRE_PM_TRIANGLE_STRIPS : return GL_TRIANGLE_STRIP;
    case PRRE_PM_TRIANGLE_FANS   : return GL_TRIANGLE_FAN; 
    case PRRE_PM_QUADS           : return GL_QUADS;
    case PRRE_PM_QUAD_STRIPS     : return GL_QUAD_STRIP;
    case PRRE_PM_LINES           : return GL_LINES;
    case PRRE_PM_LINE_STRIPS     : return GL_LINE_STRIP;
    default                      : return GL_POINTS;
    }
} // getGLprimitiveFromPRREprimitive()


PRREObject3D::PRREObject3DImpl::~PRREObject3DImpl()
{
    getConsole().OLnOI("~PRREObject3D() ...");
    
    FreeGLresources();

    free(pVertices);
    free(pVerticesTransf);
    free(pNormals);
    free(pVertexIndices);
    free(pNormalIndices);
    free(pFbBuffer);
    pVertices = PGENULL;
    pVerticesTransf = PGENULL;
    pNormals = PGENULL;
    pVertexIndices = pNormalIndices;  /* TODO: why? Why not set to NULL? */
    pFbBuffer = PGENULL;

    delete pMaterial;
    pMaterial = PGENULL;

    _pOwner->DeleteAll();

    // if we are just a cloned object, above code should have done essentially nothing
    if ( pRefersto )
    {
        // TODO: probably we should decrement reference count in referred object ...
    }

    getConsole().SOLnOO("Done!");
} // ~PRRETexture()


PRREObject3D* PRREObject3D::PRREObject3DImpl::getReferredObject() const
{
    return pRefersto; 
}


TPRRE_PRIMITIVE_FORMAT PRREObject3D::PRREObject3DImpl::getPrimitiveFormat() const
{
    return primitiveFormat;
} // getPrimitiveFormat()


TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::PRREObject3DImpl::getVertexModifyingHabit() const
{
    return BIT(PRRE_VT_VMOD_BIT) & vertexTransferMode;
} // getVertexModifyingHabit()


void PRREObject3D::PRREObject3DImpl::SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    SetVertexTransferMode( PRREObject3DManager::selectVertexTransferMode(vmod, getVertexReferencingMode(), false) );
} // SetVertexModifyingHabit()


TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::PRREObject3DImpl::getVertexReferencingMode() const
{
    return BIT(PRRE_VT_VREF_BIT) & vertexTransferMode;
} // getVertexReferencingMode()

 
void PRREObject3D::PRREObject3DImpl::SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    SetVertexTransferMode( PRREObject3DManager::selectVertexTransferMode(getVertexModifyingHabit(), vref, false) );
} // SetVertexReferencingMode()


TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::PRREObject3DImpl::getVertexTransferMode() const
{
    return vertexTransferMode;
} // getVertexTransferMode()


void PRREObject3D::PRREObject3DImpl::SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    getConsole().OLnOI("PRREObject3D::SetVertexTransferMode()");

    // Note: we don't check if the selected transfer mode is already selected.
    // Reason: selecting an already selected transfer mode is a way to recreate buffers / refresh geometry.
    // Modify geometry in system RAM and simply issue SetVertexTransferMode( getVertexTransferMode() ) to apply changes.

    if ( PRREObject3DManager::isVertexReferencingIndexed(vertexTransferMode) &&
         !PRREObject3DManager::isVertexReferencingIndexed(vtrans) )
    {
        if ( !isSwitchFromIndexedAllowed() )
        {
            getConsole().EOLnOO("ERROR: failed to switch from indexed to direct referencing mode!");
            return;
        }
    }

    if ( !PRREObject3DManager::isVertexTransferModeSelectable(vtrans) )
    {
        getConsole().EOLnOO("ERROR: given mode %d is unavailable!", (int)vtrans);
        return;
    }

    FreeGLresources();   // now it's safe to delete GL resources

    if ( BIT_READ(vtrans, PRRE_VT_SVA_BIT) )
    {
        /* check if we want VBO? */
        CompileIntoVertexBufferObjects(
            PRREObject3DManager::isVertexReferencingIndexed(vtrans),
            PRREObject3DManager::isVertexModifyingDynamic(vtrans) );
    }
    else
    {
        /* check if we want display list? */
        if ( (vtrans == (PRRE_VMOD_STATIC | PRRE_VREF_DIRECT)) ||
            (vtrans == (PRRE_VMOD_STATIC | PRRE_VREF_INDEXED)) )
            CompileIntoDisplayList(PRREObject3DManager::isVertexReferencingIndexed(vtrans));
    }

    /* no other transfer mode needs further preparation */
    vertexTransferMode = vtrans;

    if ( vertexTransferMode == vtrans )
    {   // if succeeded, apply the new transfer mode to the subobjects too
        getConsole().OLn("Ok, applying for subobjects ...");
        for (TPRREint i = 0; i < _pOwner->getCount(); i++)
            ((PRREObject3D*)(_pOwner->getAttachedAt)(i))->SetVertexTransferMode( vertexTransferMode );
        getConsole().OLn("Done!");
    }

    getConsole().OO();
} // SetVertexTransferMode()


TPRREuint PRREObject3D::PRREObject3DImpl::getVerticesCount() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getVerticesCount();
    else
        return nVertices_h;
} // getVerticesCount()


const TXYZ* PRREObject3D::PRREObject3DImpl::getVertices() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getVertices();
    else
        return pVertices;
} // getVertices()


TXYZ* PRREObject3D::PRREObject3DImpl::getVertices()
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getVertices();
    else
        return pVertices;
} // getVertices()


TPRRE_TRANSFORMED_VERTEX* PRREObject3D::PRREObject3DImpl::getTransformedVertices()
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getTransformedVertices();
    else
        return pVerticesTransf;
}


/**
    Gets the nmber of vertex indices.
*/
TPRREuint PRREObject3D::PRREObject3DImpl::getVertexIndicesCount() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getVertexIndicesCount();
    else
        return nVertexIndices_h; 
}


/**
    Gets the pointer to vertex indices.
*/
const void* PRREObject3D::PRREObject3DImpl::getVertexIndices() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getVertexIndices();
    else
        return pVertexIndices;
}


/**
    Gets the type of the indices.
*/
unsigned int PRREObject3D::PRREObject3DImpl::getVertexIndicesType() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getVertexIndicesType();
    else
        return nIndicesType;
}


TPRREuint PRREObject3D::PRREObject3DImpl::getNormalsCount() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getNormalsCount();
    else
        return nNormals_h;
} // getNormalsCount()


const TXYZ* PRREObject3D::PRREObject3DImpl::getNormals() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getNormals();
    else
        return pNormals;
} // getNormals()


PRREVector& PRREObject3D::PRREObject3DImpl::getPosVec()
{
    return vPos;
} // getPosVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getPosVec() const
{
    return vPos;
} // getPosVec()


PRREVector& PRREObject3D::PRREObject3DImpl::getAngleVec()
{
    return vAngle;
} // getAngleVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getAngleVec() const
{
    return vAngle;
} // getAngleVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getSizeVec() const
{
    return vSize;
} // getSizeVec()


PRREVector PRREObject3D::PRREObject3DImpl::getScaledSizeVec() const
{
    const PRREVector tmp(vSize.getX() * vScaling.getX(), vSize.getY() * vScaling.getY(), vSize.getZ() * vScaling.getZ());
    return tmp;
} // getScaledSizeVec()


void PRREObject3D::PRREObject3DImpl::RecalculateSize()
{

    if ( _pOwner->getCount() > 0 )
    {
        // we are parent, ask our subobjects to determine their size and object-local position
        for (TPRREint i = 0; i < _pOwner->getCount(); i++)
            ((PRREObject3D*)_pOwner->getAttachedAt(i))->RecalculateSize();

        // now we can determine our own size as parent based on min and max subobject positions and size
        TXYZ minvals, maxvals;
        minvals.x = ((PRREObject3D*)_pOwner->getAttachedAt(0))->getPosVec().getX() - ((PRREObject3D*)_pOwner->getAttachedAt(0))->getSizeVec().getX() / 2.0f;
        minvals.y = ((PRREObject3D*)_pOwner->getAttachedAt(0))->getPosVec().getY() - ((PRREObject3D*)_pOwner->getAttachedAt(0))->getSizeVec().getY() / 2.0f;
        minvals.z = ((PRREObject3D*)_pOwner->getAttachedAt(0))->getPosVec().getZ() - ((PRREObject3D*)_pOwner->getAttachedAt(0))->getSizeVec().getZ() / 2.0f;
        maxvals.x = -minvals.x;
        maxvals.y = -minvals.y;
        maxvals.z = -minvals.z;

        for (TPRREint i = 1; i < _pOwner->getCount(); i++)
        {
            if ( minvals.x > ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() - ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f )
                minvals.x = ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() - ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f;
            if ( maxvals.x < ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() + ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f )
                maxvals.x = ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() + ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f;
            if ( minvals.y > ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() - ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f )
                minvals.y = ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() - ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f;
            if ( maxvals.y < ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() + ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f )
                maxvals.y = ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() + ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f;
            if ( minvals.z > ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() - ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f )
                minvals.z = ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() - ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f;
            if ( maxvals.z < ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() + ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f )
                maxvals.z = ((PRREObject3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() + ((PRREObject3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f;
        }

        vSize.SetX( abs( minvals.x - maxvals.x ) );
        vSize.SetY( abs( minvals.y - maxvals.y ) );
        vSize.SetZ( abs( minvals.z - maxvals.z ) );

        // since we are parent we dont need to modify our world-space position at all based on our subobjects
    }

    if ( (nVertices_h == 0) || (pVertices == PGENULL ) )
        return;

    // we are subobject, so we need to calculate our own size and object-local position based on min and max vertex positions

    TXYZ minvals, maxvals;
    minvals.x = pVertices[0].x;
    minvals.y = pVertices[0].y;
    minvals.z = pVertices[0].z;
    maxvals.x = minvals.x;
    maxvals.y = minvals.y;
    maxvals.z = minvals.z;

    for (TPRREuint i = 1; i < nVertices_h; i++)
    {
        if ( minvals.x > pVertices[i].x ) minvals.x = pVertices[i].x;
        if ( maxvals.x < pVertices[i].x ) maxvals.x = pVertices[i].x;
        if ( minvals.y > pVertices[i].y ) minvals.y = pVertices[i].y;
        if ( maxvals.y < pVertices[i].y ) maxvals.y = pVertices[i].y;
        if ( minvals.z > pVertices[i].z ) minvals.z = pVertices[i].z;
        if ( maxvals.z < pVertices[i].z ) maxvals.z = pVertices[i].z;
    } 

    vSize.SetX( abs( minvals.x - maxvals.x ) );
    vSize.SetY( abs( minvals.y - maxvals.y ) );
    vSize.SetZ( abs( minvals.z - maxvals.z ) );

    vPos.SetX( minvals.x + vSize.getX() / 2.0f );
    vPos.SetY( minvals.y + vSize.getY() / 2.0f );
    vPos.SetZ( minvals.z + vSize.getZ() / 2.0f );
    
} // RecalculateSize()


const PRREVector& PRREObject3D::PRREObject3DImpl::getScaling() const
{
    return vScaling;
} // getScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(TPRREfloat value)
{
    vScaling.Set(value, value, value);
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(const PRREVector& value)
{
    vScaling = value;
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::Scale(TPRREfloat value)
{
    vScaling.Set( vScaling.getX() * value, vScaling.getY() * value, vScaling.getZ() * value );
} // Scale()


void PRREObject3D::PRREObject3DImpl::Scale(const PRREVector& value)
{
    vScaling.Set( vScaling.getX() * value.getX(), vScaling.getY() * value.getY(), vScaling.getZ() * value.getZ() );
} // Scale()


TPRREbool PRREObject3D::PRREObject3DImpl::isVisible() const
{
    return bVisible;
} // isVisible()


void PRREObject3D::PRREObject3DImpl::SetVisible(TPRREbool state)
{
    bVisible = state;
} // SetVisible()


void PRREObject3D::PRREObject3DImpl::Show()
{
    SetVisible(true);
} // Show()


void PRREObject3D::PRREObject3DImpl::Hide()
{
    SetVisible(false);
} // Hide()


TPRREbool PRREObject3D::PRREObject3DImpl::isColliding_TO_BE_REMOVED() const
{
    return bColliding;
}


void PRREObject3D::PRREObject3DImpl::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    bColliding = value;
}


TPRRE_ROTATION_ORDER PRREObject3D::PRREObject3DImpl::getRotationOrder() const
{
    return rotation;
} // getRotationOrder()


void PRREObject3D::PRREObject3DImpl::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    rotation = value;
} // SetRotationOrder()


TPRREbool PRREObject3D::PRREObject3DImpl::isLit() const
{
    return bAffectedByLights;
} // isLit()


void PRREObject3D::PRREObject3DImpl::SetLit(TPRREbool value)
{
    bAffectedByLights = value;
} // SetLit()


TPRREbool PRREObject3D::PRREObject3DImpl::isDoubleSided() const
{
    return bDoubleSided;
} // isDoubleSided()


void PRREObject3D::PRREObject3DImpl::SetDoubleSided(TPRREbool value)
{
    bDoubleSided = value;
} // SetDoubleSided()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframed() const
{
    return bWireframe;
} // isWireframed()


void PRREObject3D::PRREObject3DImpl::SetWireframed(TPRREbool value)
{
    bWireframe = value;
} // SetWireframed()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframedCulled() const
{
    return bWireframedCull;
} // isWireframedCulled()


void PRREObject3D::PRREObject3DImpl::SetWireframedCulled(TPRREbool value)
{
    bWireframedCull = value;
} // SetWireframedCulled()


TPRREbool PRREObject3D::PRREObject3DImpl::isAffectingZBuffer() const
{
    return bAffectZBuffer;
} // isAffectingZBuffer()


void PRREObject3D::PRREObject3DImpl::SetAffectingZBuffer(TPRREbool value)
{
    bAffectZBuffer = value;
} // SetAffectingZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isTestingAgainstZBuffer() const
{
    return bAllowZTesting;
} // isTestingAgainstZBuffer()


void PRREObject3D::PRREObject3DImpl::SetTestingAgainstZBuffer(TPRREbool value)
{
    bAllowZTesting = value;
} // SetTestingAgainstZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isStickedToScreen() const
{
    return bStickedToScreen;
} // isStickedToScreen()


void PRREObject3D::PRREObject3DImpl::SetStickedToScreen(TPRREbool value)
{
    bStickedToScreen = value;
    SetDoubleSided(true);
    SetLit(false);
    SetTestingAgainstZBuffer(false);
} // SetStickedToScreen()


const PRREMaterial& PRREObject3D::PRREObject3DImpl::getMaterial() const
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getMaterial();
    else
        return *pMaterial;
} // getMaterial()


PRREMaterial& PRREObject3D::PRREObject3DImpl::getMaterial()
{
    if ( (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getMaterial();
    else
        return *pMaterial;
} // getMaterial()


TPRREuint PRREObject3D::PRREObject3DImpl::getUsedSystemMemory() const
{
    return (
        sizeof(*this) +
        nVertices_h * sizeof(TXYZ) +
        nVertexIndices_h * PRREGLsnippets::getSizeofIndexType(nIndicesType) +
        nNormals_h * sizeof(TXYZ) +
        nNormalIndices_h * PRREGLsnippets::getSizeofIndexType(nIndicesType) +
        nFbBuffer_h +
        pMaterial ? pMaterial->getUsedSystemMemory() : 0
    );
} // getUsedSystemMemory()     


void PRREObject3D::PRREObject3DImpl::Draw(bool bLighting)
{
    if ( !bVisible )
        return;

    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        DrawSW();
        return;
    }

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( (_pOwner->getCount() > 0) || pRefersto )
    {
        // Currently 3D objects are 2-level entities:
        // first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.
        
        ApplyTransformations();
        PrepareGLbeforeDraw(bLighting);

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = pRefersto ? pRefersto : _pOwner;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(bLighting);
    }

    // before trying to draw anything, see if we actually have anything to draw ...
    if ( (nVertexIndices_h == 0) && (nVertices_h == 0) && (nVerticesVBO == 0) && (nDispList == 0) )
        return;  // we return here if we are parent or we are cloned object

    // if we reach this point then either our parent is drawing us as its subobject, or a cloned object is drawing us on behalf of our parent
    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        if ( getMaterial().isMultiTextured() )
        {
            // enable blending of 2nd layer
            glEnable(GL_BLEND);
	          glBlendFunc(getGLblendFromPRREblend(getMaterial().getSourceBlendFunc(1)), getGLblendFromPRREblend(getMaterial().getDestinationBlendFunc(1)));
            LoadTextureIntoTMU( pMaterial->getTexture(0), 0 );
            LoadTextureIntoTMU( pMaterial->getTexture(1), 1 );
        }
        else
        {
            // disable blending only if base layer is not blended ...
            if ( ! PRREObject3DManager::isBlendFuncBlends(getMaterial().getSourceBlendFunc(), getMaterial().getDestinationBlendFunc()) )
                glDisable(GL_BLEND);
            LoadTextureIntoTMU( pMaterial->getTexture(0), 0 );
            LoadTextureIntoTMU( PGENULL, 1 );
        }
    }
    else
    {
        // disable blending only if base layer is not blended ...
        if ( ! PRREObject3DManager::isBlendFuncBlends(getMaterial().getSourceBlendFunc(), getMaterial().getDestinationBlendFunc()) )
            glDisable(GL_BLEND);
        LoadTextureIntoTMU( pMaterial->getTexture() );
    }

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(getVertexTransferMode(),PRRE_VT_VENDOR_BITS,3) != 0 )
        return;

    // If you want all vertices to be transformed and catched in feedback mode then dont forget to disable culling and depth testing (maybe only 1 is needed to be disabled).
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);

    if ( pFbBuffer == NULL )
    {
        /* We create this buffer only once at first run. */
        /* We intentionally switch to feedback mode only once since now it is for debugging purposes only, for breakpoint, etc.
           but in the future when HW transform and SW rasterization can be combined, this will be controlled by public API */
       
        /* num of values can be stored in this buffer */
        /* we need to store not only the transformed values but also some extra values for grammar */
        /* in this case, the items placed in this array are polygon items (GL_POLYGON_TOKEN) which look like this:
            GL_POLYGON_TOKEN 3 (value value value value (value value value value) (value value value value))
            So for a standard cube where each side has 2 triangles, it is 12 triangles in total, meaning
            12 polygons. GL_POLYGON_TOKEN itself is 1 value, 3 itself is also 1 value, and then 3*12 values for 3 vertices.
            So the total num of values is: numOfTriangles * (2+3*12)
            Example with some real data is in later comment with debugbuffer[]. */

        nFbBuffer_h = (nVertices_h / 3) * (2+3*12);
        /* TODO: probably in future we should rather use pVerticesTransf here as well since we already have it for that purpose, right? ;) */
        pFbBuffer = (GLfloat*) realloc(pFbBuffer, nFbBuffer_h * sizeof(GLfloat));

        /* unfortunately only the most detailed option GL_4D_COLOR_TEXTURE will give us the w-coord of vertices so we need to use that */
        glFeedbackBuffer(nFbBuffer_h, GL_4D_COLOR_TEXTURE, pFbBuffer);
        
        glDisable(GL_CULL_FACE); /* otherwise only the front facing side of cube would be written to feedback buffer */

        glRenderMode(GL_FEEDBACK);
    }  

    if ( BIT_READ(getVertexTransferMode(), PRRE_VT_VA_BIT) == 0u )
    {
        /* there is no array for geometry, so we either invoke a display list or pass vertices 1-by-1*/

        if ( PRREObject3DManager::isVertexModifyingDynamic( getVertexTransferMode() ) )
            // PRRE_VT_DYN_..._1_BY_1
            ProcessGeometry( PRREObject3DManager::isVertexReferencingIndexed( getVertexTransferMode() ) );
        else
            // PRRE_VT_STA_..._DL
            glCallList( nDispList );
    } // PRRE_VT_VA_BIT == 1u
    else
    {
        /* vertex array either in client or server */
        const bool bServer = BIT_READ(getVertexTransferMode(), PRRE_VT_SVA_BIT) == 1u;

        SetArrayPointers( bServer );

            // There is not much use of the CVA mode in this form, however sometime later
            // the code could be somehow enhanced to do multitexturing with just 1 TMU
            // by drawing the same geometry again with different texture blended between
            // 1 lock-unlock, in that case vertices would be transformed only once!
            // This would be useful when multitexturing is enabled on 1-TMU VPU/GPU.
            // However even TNT had 2 TMUs, so I'm not planning to do this improvement.
            // Just wanted to emphasize that locking the array and drawing it only once
            // does not make much sense.
            if ( BIT_READ(getVertexTransferMode(), PRRE_VT_CVA_BIT) == 1u )
                glLockArraysEXT(0, nVertices_h);

                // Static and dynamic vmods are not differentiated here but when creating the arrays.
                // That is why we don't check for vmod in this code.

                if ( PRREObject3DManager::isVertexReferencingIndexed( getVertexTransferMode() ) )
                {
                    if ( BIT_READ(getVertexTransferMode(), PRRE_VT_RNG_BIT) )
                    {
                        // TODO: should call this in loop, do multiple batches based on the implementation-dependent values
                        // (GL_MAX_ELEMENTS_VERTICES_EXT, GL_MAX_ELEMENTS_INDICES_EXT).
                        glDrawRangeElementsEXT(
                            getGLprimitiveFromPRREprimitive(primitiveFormat),
                            nMinIndex, nMaxIndex, nVertexIndices_h, nIndicesType, bServer ? NULL : pVertexIndices );
                    }
                    else
                    {
                        glDrawElements(
                            getGLprimitiveFromPRREprimitive(primitiveFormat), 
                            nVertexIndices_h, nIndicesType, bServer ? NULL : pVertexIndices );
                    }
                }
                else
                {
                    glDrawArrays( getGLprimitiveFromPRREprimitive(primitiveFormat), 0, nVertices_h );
                }

            if ( BIT_READ(getVertexTransferMode(), PRRE_VT_CVA_BIT) == 1u )
                    glUnlockArraysEXT();

        ResetArrayPointers( bServer );
    } // PRRE_VT_VA_BIT == 0u

    /* The number of values (not vertices) transferred to the feedback buffer. */
    GLint nFbBufferWritten_h = glRenderMode(GL_RENDER);

    GLfloat debugbuffer[500];

    if ( nFbBufferWritten_h > 0 )
    {
        // we are happy
        for (int i = 0; i < min(500,nFbBufferWritten_h); i++)
            debugbuffer[i] = pFbBuffer[i];
        Sleep(1); // suitable for placing breakpoint
        /*
            debugbuffer[0] = GL_POLYGON_TOKEN;
            debugbuffer[1] = 3;
            debugbuffer[ 2.. 5] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[ 6.. 9] = r g b a
            debugbuffer[10..13] = texture coords, probably x y u w
            debugbuffer[14..17] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[18..21] = r g b a
            debugbuffer[22..25] = texture coords, probably x y u w
            debugbuffer[26..29] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[30..33] = r g b a
            debugbuffer[34..37] = texture coords, probably x y u w
            debugbuffer[38] = GL_POLYGON_TOKEN;
            debugbuffer[39] = 3;

            example: gamedata\\models\\cube.obj
            assuming that nearplane is 0.1f, farplane is 100.0f, depthrange is [0, 1], fovy is 80
            box
            i=2 (3. vertex) debugbuffer[26]
                model space        -> world space       -> view/eye space    -> clip space                   -> normalized device coords     -> screen coords
            sw: [-0.5, -0.5, -0.5] -> [-0.5, -0.5, 2.5] -> [-0.5, -0.5, 2.5] -> [-0.446, -0.595, 2.348, 2.5] -> [-0.178, -0.238, 0.939, 2.5] -> [328, 228, 0.961, 2.5]
            hw:                                                                                                                                 [328, 228, 0.961, 2.5]

            i=6 (7. vertex) debugbuffer[?]
                model space        -> world space       -> view/eye space    -> clip space                   -> normalized device coords     -> screen coords
            sw: [ 0.5,  0.5,  0.5] -> [ 0.5,  0.5, 3.5] -> [ 0.5,  0.5, 3.5] -> [ 0.446,  0.595, 3.368, 3.5] -> [ 0.127,  0.170, 0.962, 3.5] -> [451, 351, 0.972, 3.5]
            hw:                                                                                                                                 [451, 351, 0.972, 3.5]

            ...
        */
    }
    else
    {
        // we were already in GL_RENDER (0) mode, OR nothing was returned, OR we were in FEEDBACK but buffer was not enough to hold all values (<0) ...
    }

} // Draw()


// ############################## PROTECTED ##############################



/**
    @param owner                 The public Object3D class instance owning this pimpl object.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREObject3D::PRREObject3DImpl::PRREObject3DImpl(
    PRREObject3D* owner,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory )
{
    _pOwner = owner;
    getConsole().OLnOI("PRREObject3D() ...");

    pRefersto = NULL;
    bAffectedByLights = bAffectZBuffer = bAllowZTesting = true;
    bReadyToDraw = bVisible = true;
    bColliding = bDoubleSided = false;
    bWireframe = bWireframedCull = bStickedToScreen = false;

    vScaling.Set(1.0f, 1.0f, 1.0f);
    rotation = PRRE_YXZ;
    nDispList = 0;
    nVerticesVBO = nColorsVBO = nNormalsVBO = nIndicesVBO = 0;
    // nTexcoordsVBO to be resized by manager outside
    pMaterial = PGENULL;

    primitiveFormat = PRRE_PM_QUADS;
    vertexTransferMode = PRREObject3DManager::selectVertexTransferMode(vmod, vref, bForceUseClientMemory);
    pVertices = PGENULL;
    pVerticesTransf = PGENULL;
    pNormals = PGENULL;
    pVertexIndices = PGENULL;
    pNormalIndices = PGENULL;
    pFbBuffer = PGENULL;

    nMinIndex = UINT_MAX;  // todo: should make macros like PRRE_UINT_MAX into prretypes.h
    nMaxIndex = 0;
    nIndicesType = GL_UNSIGNED_INT;

    nVertices_h = nVertexIndices_h = 0;
    nNormals_h = nNormalIndices_h = 0;
    nFaces_h = 0;
    nFbBuffer_h = 0;

    getConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREObject3D::PRREObject3DImpl::PRREObject3DImpl(const PRREObject3DImpl&)
{

}


PRREObject3D::PRREObject3DImpl& PRREObject3D::PRREObject3DImpl::operator=(const PRREObject3DImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################


/**
    Hack to be able to use CConsole singleton instance instead of owner's protected console instance.
    TODO: maybe the whole getConsole() stuff should be removed from everywhere because CConsole is a singleton class after all.
*/
CConsole& PRREObject3D::PRREObject3DImpl::getConsole() const
{
    return CConsole::getConsoleInstance();
}


/**
    Gets an index value from a given index array.
*/
TPRREuint PRREObject3D::PRREObject3DImpl::getIndexFromArray(const void* arr, TPRREuint index) const
{
    return PRREGLsnippets::getIndexFromArray(arr, index, this->nIndicesType);
}


/**
    Sets an index value in a given index array.
*/
void PRREObject3D::PRREObject3DImpl::SetIndexInArray(void* arr, TPRREuint index, TPRREuint value)
{
    PRREGLsnippets::SetIndexInArray(arr, index, value, this->nIndicesType);
}


/**
    Tells whether it is allowed to switch from indexed to non-indexed vertex referencing mode.
    It is allowed when geometry data is stored redundantly so direct referencing is available.
    Geometry data is stored redundantly when multiple vertices are defined with the same position
    for different faces. In this situation, the number of vertex indices is equal to the number of
    defined vertices so the number of defined vertices is the adequate number for rendering the
    whole geometry. So switching to non-indexed is disallowed only when the number of defined
    vertices is less then the number of indices.
*/
TPRREbool PRREObject3D::PRREObject3DImpl::isSwitchFromIndexedAllowed() const
{
    if ( _pOwner->getCount() > 0 )
        return ((PRREObject3D*)_pOwner->getAttachedAt(0))->p->isSwitchFromIndexedAllowed();
    else
        return (nVertexIndices_h == nVertices_h) && (nVertices_h > 0);
}


/**
    Goes thru vertices and feeds them to OpenGL.
    Used by direct and indexed immediate modes and during display list building.

    @param indexed If true, will go thru vertices by using pVertexIndices for ordering, otherwise won't use it.
*/
void PRREObject3D::PRREObject3DImpl::ProcessGeometry(TPRREbool indexed) const  
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    //const TRGBAFLOAT* const pColors = pMaterial->getColors(0);
    //const void* const pColorIndices = pMaterial->getColorIndices(0);
    const TUVW* const pTexcoords = pMaterial->getTexcoords(0);
    const void* const pTexcoordIndices = pMaterial->getTexcoordIndices(0);
    // following can be NULL if material system supports only 1 level
    const TUVW* const pTexcoords2 = pMaterial->getTexcoords(1);
    const void* const pTexcoordIndices2 = pMaterial->getTexcoordIndices(1);

    glBegin( getGLprimitiveFromPRREprimitive(primitiveFormat) );
        for (TPRREuint i = 0; i < nVertexIndices_h; i++)
        {
            const TXYZ&       vertex = indexed ? pVertices[ getIndexFromArray(pVertexIndices, i) ]    : pVertices[i];
            const TXYZ&       normal = indexed ? pNormals[ getIndexFromArray(pNormalIndices, i) ]     : pNormals[i];
            //const TRGBAFLOAT& color  = indexed ? pColors[ getIndexFromArray(pColorIndices, i) ]       : pColors[i];
            const TUVW&       uvw    = indexed ? pTexcoords[ getIndexFromArray(pTexcoordIndices, i) ] : pTexcoords[i];

            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
            {
                if ( getMaterial().isMultiTextured() )
                {
                    const TUVW& uvw2 = indexed ? pTexcoords2[ getIndexFromArray(pTexcoordIndices2, i) ] : pTexcoords2[i];
                    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, uvw.u, uvw.v);
                    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, uvw2.u, uvw2.v);
                }
                else
                {
                    if ( pMaterial->getTexture() != PGENULL )
                        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, uvw.u, uvw.v);
                }
            }
            else
            {
                if ( pMaterial->getTexture() != PGENULL )
                    glTexCoord2f(uvw.u, uvw.v);
            }
            
            // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere in setarraypointers()
            //glColor4f(color.red, color.green, color.blue, color.alpha);
            if ( bAffectedByLights )
                glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        } // for k
    glEnd();
} // ProcessGeometry()


/**
    Compiles OpenGL drawing commands into display list.

    @param indexed If true, will issue ProcessGeometry() to go thru geometry data by using indices for order.
*/
void PRREObject3D::PRREObject3DImpl::CompileIntoDisplayList(TPRREbool indexed)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( pVertices == NULL )
        return;

    nDispList = glGenLists(1);
    glNewList(nDispList, GL_COMPILE);
        ProcessGeometry( indexed );
    glEndList();
} // CompileIntoDisplayList()


 /**
    Compiles geometry into VBOs.

    @param indexed If true, will go thru geometry data by using indices for order. This will generate an element array buffer.
                   Ignored if vertex indices array is NULL.
    @param dynamic If true, the usage hint when building vertex buffer objects will be dynamic, otherwise static.
 */
void PRREObject3D::PRREObject3DImpl::CompileIntoVertexBufferObjects(TPRREbool indexed, TPRREbool dynamic)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( indexed && (nIndicesVBO == 0) && (pVertexIndices != NULL) )
    {
        glGenBuffersARB(1, &nIndicesVBO);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndicesVBO);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nVertexIndices_h * PRREGLsnippets::getSizeofIndexType(nIndicesType), pVertexIndices, GL_STATIC_DRAW_ARB);
        // Note: we always store indices in static buffer but could be in client memory, too.
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }

    if ( (nVerticesVBO == 0) && (pVertices != NULL) )
    {
        GLenum usage = dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;
        glGenBuffersARB(1, &nVerticesVBO);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nVerticesVBO);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,  nVertices_h * sizeof(TXYZ), pVertices, usage);
 
        glGenBuffersARB(1, &nColorsVBO);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nColorsVBO);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, pMaterial->getColorsCount() * sizeof(TRGBAFLOAT), pMaterial->getColors(), usage);

        glGenBuffersARB(1, &nTexcoordsVBO[0]);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[0]);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, pMaterial->getTexcoordsCount() * sizeof(TUVW), pMaterial->getTexcoords(), usage);

        if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
        {
            if ( getMaterial().isMultiTextured() )
            {
                glGenBuffersARB(1, &nTexcoordsVBO[1]);
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[1]);
                glBufferDataARB(GL_ARRAY_BUFFER_ARB, pMaterial->getTexcoordsCount(1) * sizeof(TUVW), pMaterial->getTexcoords(1), usage);
            }
        }

        glGenBuffersARB(1, &nNormalsVBO);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nNormalsVBO);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, nNormals_h * sizeof(TXYZ), pNormals, usage);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    }

} // CompileIntoVertexBufferObjects()


/**
    Frees up allocated resources from OpenGL.
    Should be called before setting a new Vertex Transfer mode and when deleting the object.
*/
void PRREObject3D::PRREObject3DImpl::FreeGLresources()
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    for (TPRREint i = 0; i < _pOwner->getCount(); i++)
        ((PRREObject3D*)_pOwner->getAttachedAt(i))->p->FreeGLresources();

    if ( nDispList != 0 )
    {
        glDeleteLists(nDispList, 1);
        nDispList = 0;
    }
    if ( nVerticesVBO != 0 )
    {
        glDeleteBuffersARB(1, &nVerticesVBO);
        nVerticesVBO = 0;
    }
    if ( nColorsVBO != 0 )
    {
        glDeleteBuffersARB(1, &nColorsVBO);
        nColorsVBO = 0;
    }
    if ( nNormalsVBO != 0 )
    {
        glDeleteBuffersARB(1, &nNormalsVBO);
        nNormalsVBO = 0;
    }
    if ( nIndicesVBO != 0 )
    {
        glDeleteBuffersARB(1, &nIndicesVBO);
        nIndicesVBO = 0;
    }

    for (std::vector<GLuint>::size_type i = 0; i < nTexcoordsVBO.size(); i++)
    {
        if ( nTexcoordsVBO[i] != 0 )
        {
            glDeleteBuffersARB(1, &nTexcoordsVBO[i]);
            nTexcoordsVBO[i] = 0;
        }
    }
}


/**
    Sets vertex data pointers prior to drawing.
    Can be used for both vertex buffer objects and legacy vertex arrays.

    @param redirectToServer Specify true if geometry data is located in server memory.
*/
void PRREObject3D::PRREObject3DImpl::SetArrayPointers(TPRREbool redirectToServer) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( (PRREObject3DManager::isVertexReferencingIndexed(vertexTransferMode)) && redirectToServer )
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndicesVBO);

    glEnableClientState(GL_VERTEX_ARRAY);
    // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason:
    // as tmcsgfxlib uses a single vertex color for object color key when object is textured,
    // we shouldn't specify any further per-vertex color, otherwise alphablending would be bad with legacy projects.
    // In future, we should rather use glBlendColor() for such objects and then per-vertex colors could be re-enabled again.
    // glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if ( redirectToServer )
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nVerticesVBO);
        glVertexPointer(3, GL_FLOAT, 0, NULL);

        // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere above
        //glBindBufferARB(GL_ARRAY_BUFFER_ARB, nColorsVBO);
        //glColorPointer(4, GL_FLOAT, 0, NULL);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nNormalsVBO);
        glNormalPointer(GL_FLOAT, 0, NULL);
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, 0, pVertices);
        // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere above
        //glColorPointer(4, GL_FLOAT, 0, pMaterial->getColors());
        glNormalPointer(GL_FLOAT, 0, pNormals);
    }

    if ( pMaterial->isTextured() )
    {
        const TPRREuint iMaxTexLayer = pMaterial->isMultiTextured() ? 1 : 0;

        for (TPRREuint i = 0; i <= iMaxTexLayer; i++)
        {
            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
                glClientActiveTextureARB(GL_TEXTURE0_ARB + i);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            if ( redirectToServer )
            {
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[i]);
                glTexCoordPointer(3, GL_FLOAT, 0, NULL);
            }
            else
                glTexCoordPointer(3, GL_FLOAT, 0, pMaterial->getTexcoords(i));
        }
    }

} // SetArrayPointers()


/**
    Resets vertex data pointers after drawing.
    Can be used for both vertex buffer objects and legacy vertex arrays.

    @param redirectToServer Specify true if geometry data is located in server memory.
*/
void PRREObject3D::PRREObject3DImpl::ResetArrayPointers(TPRREbool redirectToServer) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( redirectToServer )
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        if ( PRREObject3DManager::isVertexReferencingIndexed(vertexTransferMode) )
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }
    if ( pMaterial->getTexture() != PGENULL )
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
} // ResetArrayPointers()


/**
    Loads the given texture into the texture mapping unit.

    @param tex  Texture to be loaded. If NULL, texturing will be disabled on the specified texture mapping unit.
    @param iTMU Into which TMU we want to load the texture. Currently it must be either 0 or 1.
*/
void PRREObject3D::PRREObject3DImpl::LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    // TODO: the PR00FPS-style lasttex method is not present here currently ... later it should be implemented.
    // A general TMU manager should record which TMU holds which texture, so that can avoid unnecessary glBindTexture() calls.

    // currently we support only 2 textured layers ...
    if ( iTMU > 1 )
    {
        getConsole().EOLn("ERROR: LoadTextureIntoTMU(tex, %d)", iTMU);
        _ASSERT( false );
    }

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE0_ARB + iTMU);
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
    else
    {
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            if ( isStickedToScreen() )
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            else
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
} // LoadTextureIntoTMU()


/**
    Applies transformations to the current modelview matrix based on the given object.
*/
void PRREObject3D::PRREObject3DImpl::ApplyTransformations() const    
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    glTranslatef(getPosVec().getX(), getPosVec().getY(), getPosVec().getZ());
    
    switch ( getRotationOrder() )
    {
    case PRRE_XYZ:
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        break;
    case PRRE_XZY:
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getY(), 0,1,0);
        break;
    case PRRE_YXZ:
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
         break;
    case PRRE_YZX:
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getX(), 1,0,0);
        break;
    case PRRE_ZXY:
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getY(), 0,1,0);
        break;
    case PRRE_ZYX:
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getX(), 1,0,0);
        break;
    } // switch

    glScalef(getScaling().getX(), getScaling().getY(), getScaling().getZ());
    /*
        Well, mirror-transforming an object (scaling by negative value) makes its faces flip:
        https://www.gamedev.net/forums/topic/640616-negative-scaling-flips-face-winding-affects-backface-culling/
        I wanted to X-mirror objects loaded by tmcsgfxlib-to-prre wrapper for legacy projects, because I noticed they appeared mirrored compared to proofps engine behavior.
        So I noticed when setting negative scale on X axis, faces also flipped, so objects became inside-out.
        This is expected since vertices' winding order also flips because we still feed triangle vertices in same order even though triangles become mirrored.
        But I don't treat this as a legacy-project-only issue, since even a new project with the new engine could come up for any reason with negative scaling for any mirroring reason.
        So, this is rather a feature: we need to detect negative scaling factor and change front face winding order on-the-fly.
    */
    if ( getScaling().getX() * getScaling().getY() * getScaling().getZ() <= (0.0f-PFL::E) )
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);
}


void PRREObject3D::PRREObject3DImpl::PrepareGLbeforeDraw(bool bLighting) const
{
    /*AmbientLightPos[0] =  cam.getX() - obj->getPosVec().getX();
    AmbientLightPos[1] =  cam.getY() - obj->getPosVec().getY();
    AmbientLightPos[2] = -cam.getZ() - obj->getPosVec().getZ();*/
    /*AmbientLightPos[0] = cam.getX();
    AmbientLightPos[1] = cam.getY();
    AmbientLightPos[2] = -cam.getZ()-2.0f;
    AmbientLightPos[3] = 1.0f;
    glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);  */

    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( isDoubleSided() )
    {
        glDisable(GL_CULL_FACE);
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos); 
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    }
    else
    {
        glEnable(GL_CULL_FACE);
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    } // isDoubleSided()

    if ( isWireframed() )
    {
        glDisable(GL_LIGHTING);
        if ( !isWireframedCulled() )
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } // isWireframed()

    if ( isTestingAgainstZBuffer() )
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    // TODO: nope, we shouldnt specify color this way because glColor4f() is called per-vertex anyway later
    // getTextureEnvColor() is used by texture environment mode / function, not blending!
    // we should rather use glBlendColor() for this with GL_*_CONSTANT_* blendfunc, or modify the code not to specify color per-vertex
    
    glColor4f(
        getMaterial().getTextureEnvColor().getRedAsFloat(),
        getMaterial().getTextureEnvColor().getGreenAsFloat(),
        getMaterial().getTextureEnvColor().getBlueAsFloat(),
        getMaterial().getTextureEnvColor().getAlphaAsFloat() );
                
                
    if ( PRREObject3DManager::isBlendFuncBlends(getMaterial().getSourceBlendFunc(), getMaterial().getDestinationBlendFunc()) )
    {
        glEnable(GL_BLEND);
        glBlendFunc(getGLblendFromPRREblend(getMaterial().getSourceBlendFunc()), getGLblendFromPRREblend(getMaterial().getDestinationBlendFunc()));
        glAlphaFunc(GL_GREATER, 0.1f);
        glEnable(GL_ALPHA_TEST);
    }
    else
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
}


void PRREObject3D::PRREObject3DImpl::DrawSW()
{
    if ( !bVisible )
        return;

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( (_pOwner->getCount() > 0) || pRefersto )
    {
        // Currently 3D objects are 2-level things: first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.

        ApplyTransformations();

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = pRefersto ? pRefersto : _pOwner;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(false);
    }

    // before trying to draw anything, see if we actually have anything to draw ...
    if ( (nVertexIndices_h == 0) && (nVertices_h == 0) )
        return;  // we return here if we are parent or we are cloned object

    // actual draw here
    // todo: wtf, we should finally decide if it is renderer's responsibility to actually render an object or object's responsibility?
}


/*
   PRREObject3D
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREObject3D::~PRREObject3D()
{
    delete p;
    p = NULL;
} // ~PRRETexture()


/**
    Gets the original object which was cloned to create this object.
    @return NULL if this is a non-cloned object, otherwise the pointer to the original object which was cloned to create this cloned object.
*/
PRREObject3D* PRREObject3D::getReferredObject() const
{
    return p->getReferredObject();
}


/**
    Gets the primitives' format.
*/
TPRRE_PRIMITIVE_FORMAT PRREObject3D::getPrimitiveFormat() const
{
    return p->getPrimitiveFormat();
} // getPrimitiveFormat()


/**
    Gets vertex modifying habit.
*/
TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::getVertexModifyingHabit() const
{
    return p->getVertexModifyingHabit();
} // getVertexModifyingHabit()


/**
    Sets vertex modifying habit.
*/
void PRREObject3D::SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    p->SetVertexModifyingHabit(vmod);
} // SetVertexModifyingHabit()


/**
    Gets vertex referencing mode.
*/
TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::getVertexReferencingMode() const
{
    return p->getVertexReferencingMode();
} // getVertexReferencingMode()

 
/**
    Sets vertex referencing mode.
*/
void PRREObject3D::SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    p->SetVertexReferencingMode(vref);
} // SetVertexReferencingMode()


/**
    Gets vertex transfer mode.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::getVertexTransferMode() const
{
    return p->getVertexTransferMode();
} // getVertexTransferMode()


/**
    Sets vertex transfer mode.
    Vertex transfer mode gets selected automatically by PPP.
    However, it can be also set manually for custom reasons.
    The setting may not happen if the selected transfer mode is not available for some reason, for
    example, if the selected mode is not supported on the current hardware.
*/
void PRREObject3D::SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    p->SetVertexTransferMode(vtrans);
} // SetVertexTransferMode()


/**
    Gets the number of vertices.

    @return Number of vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            count is the subobject's vertex count. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREObject3D::getVerticesCount() const
{
    return p->getVerticesCount();
} // getVerticesCount()


/**
    Gets the pointer to vertices.

    @return Pointer to vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TXYZ* PRREObject3D::getVertices() const
{
    return p->getVertices();
} // getVertices()


/**
    Gets the pointer to vertices.

    @return Pointer to vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TXYZ* PRREObject3D::getVertices()
{
    return p->getVertices();
} // getVertices()


/**
    Gets the pointer to transformed vertices.
    These are generated after rendering.

    @return Pointer to transformed vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's transformed vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRRE_TRANSFORMED_VERTEX* PRREObject3D::getTransformedVertices()
{
    return p->getTransformedVertices();
} // getVertices()


/**
    Gets the number of vertex indices.
*/
TPRREuint PRREObject3D::getVertexIndicesCount() const
{
    return p->getVertexIndicesCount();
}


/**
    Gets the pointer to vertex indices.
*/
const void* PRREObject3D::getVertexIndices() const
{
    return p->getVertexIndices();
}


/**
    Gets the type of the indices.
*/
unsigned int PRREObject3D::getVertexIndicesType() const
{
    return p->getVertexIndicesType();
}


/**
    Gets the number of normals.

    @return Number of normals. If the object's own normal count is 0 but it has exactly 1 subobject, the returned
            count is the subobject's normal count. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREObject3D::getNormalsCount() const
{
    return p->getNormalsCount();
} // getNormalsCount()


/**
    Gets the pointer to normals.

    @return Pointer to normals. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's normals pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TXYZ* PRREObject3D::getNormals() const
{
    return p->getNormals();
} // getNormals()


/**
    Gets the position.
    @return Position vector.
*/
PRREVector& PRREObject3D::getPosVec()
{
    return p->getPosVec();
} // getPosVec()


/**
    Gets the position.
    @return Position vector.
*/
const PRREVector& PRREObject3D::getPosVec() const
{
    return p->getPosVec();
} // getPosVec()


/**
    Gets the rotation angles.
    @return Rotation angles vector.
*/
PRREVector& PRREObject3D::getAngleVec()
{
    return p->getAngleVec();
} // getAngleVec()


/**
    Gets the rotation angles.
    @return Rotation angles vector.
*/
const PRREVector& PRREObject3D::getAngleVec() const
{
    return p->getAngleVec();
} // getAngleVec()


/**
    Gets the base sizes.
    @return Base sizes vector.
*/
const PRREVector& PRREObject3D::getSizeVec() const
{
    return p->getSizeVec();
} // getSizeVec()


/**
    Gets the real sizes.
    @return Real sizes vector.
*/
PRREVector PRREObject3D::getScaledSizeVec() const
{
    return p->getScaledSizeVec();
} // getScaledSizeVec()


/**
    Recalculates the sizes.
*/
void PRREObject3D::RecalculateSize()
{
    p->RecalculateSize();
} // RecalculateSize()


/**
    Gets the scaling.
    @return Scaling.
*/
const PRREVector& PRREObject3D::getScaling() const
{
    return p->getScaling();
} // getScaling()


/**
    Sets the scaling to given scalar.
*/
void PRREObject3D::SetScaling(TPRREfloat value)
{
    p->SetScaling(value);
} // SetScaling()


/**
    Sets the scaling to given vector.
*/
void PRREObject3D::SetScaling(const PRREVector& value)
{
    p->SetScaling(value);
} // SetScaling()


/**
    Scales by the given scalar value. 
*/
void PRREObject3D::Scale(TPRREfloat value)
{
    p->Scale(value);
} // Scale()


/**
    Scales by the given vector.
*/
void PRREObject3D::Scale(const PRREVector& value)
{
    p->Scale(value);
} // Scale()


/**
    Gets the visibility state.
    @return True, if visible, false otherwise.
*/
TPRREbool PRREObject3D::isVisible() const
{
    return p->isVisible();
} // isVisible()


/**
    Sets the visibility state.
*/
void PRREObject3D::SetVisible(TPRREbool state)
{
    p->SetVisible(state);
} // SetVisible()


/**
    Sets the visibility state to true.
    Equivalent to SetVisible(true).
*/
void PRREObject3D::Show()
{
    p->Show();
} // Show()


/**
    Sets the visibility state to false.
    Equivalent to SetVisible(false).
*/
void PRREObject3D::Hide()
{
    p->Hide();
} // Hide()


/**
    Gets whether colliding is enabled.
    @return True, if colliding is enabled, false otherwise.
*/
TPRREbool PRREObject3D::isColliding_TO_BE_REMOVED() const
{
    return p->isColliding_TO_BE_REMOVED();
}


/**
    Sets whether colliding is enabled.
*/
void PRREObject3D::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    p->SetColliding_TO_BE_REMOVED(value);
}


/**
    Gets the rotation order.
    @return Rotation order.
*/
TPRRE_ROTATION_ORDER PRREObject3D::getRotationOrder() const
{
    return p->getRotationOrder();
} // getRotationOrder()


/**
    Sets the rotation order.
*/
void PRREObject3D::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    p->SetRotationOrder(value);
} // SetRotationOrder()


/**
    Gets the lit state.
    @return True if lit, false otherwise.
*/
TPRREbool PRREObject3D::isLit() const
{
    return p->isLit();
} // isLit()


/**
    Sets the lit state.
*/
void PRREObject3D::SetLit(TPRREbool value)
{
    p->SetLit(value);
} // SetLit()


/**
    Gets the double sided state.
    @return True if double sided, false otherwise.
*/
TPRREbool PRREObject3D::isDoubleSided() const
{
    return p->isDoubleSided();
} // isDoubleSided()


/**
    Sets the double sided state.
*/
void PRREObject3D::SetDoubleSided(TPRREbool value)
{
    p->SetDoubleSided(value);
} // SetDoubleSided()


/**
    Gets the wireframed state.
    @return True if wireframed, false otherwise.
*/
TPRREbool PRREObject3D::isWireframed() const
{
    return p->isWireframed();
} // isWireframed()


/**
    Sets the wireframed state.
*/
void PRREObject3D::SetWireframed(TPRREbool value)
{
    p->SetWireframed(value);
} // SetWireframed()


/**
    Gets the wireframed culling state.
    @return True if culling is enabled in wireframed state, false otherwise.
*/
TPRREbool PRREObject3D::isWireframedCulled() const
{
    return p->isWireframedCulled();
} // isWireframedCulled()


/**
    Sets the wireframed culling state.
*/
void PRREObject3D::SetWireframedCulled(TPRREbool value)
{
    p->SetWireframedCulled(value);
} // SetWireframedCulled()


/**
    Gets whether we write to the Z-Buffer while rendering.
    @return True if we write to Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isAffectingZBuffer() const
{
    return p->isAffectingZBuffer();
} // isAffectingZBuffer()


/**
    Sets whether we write to the Z-Buffer while rendering.
*/
void PRREObject3D::SetAffectingZBuffer(TPRREbool value)
{
    p->SetAffectingZBuffer(value);
} // SetAffectingZBuffer()


/**
    Gets whether we test against the Z-Buffer while rendering.
    @return True if we test against the Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isTestingAgainstZBuffer() const
{
    return p->isTestingAgainstZBuffer();
} // isTestingAgainstZBuffer()


/**
    Sets whether we test against the Z-Buffer while rendering.
*/
void PRREObject3D::SetTestingAgainstZBuffer(TPRREbool value)
{
    p->SetTestingAgainstZBuffer(value);
} // SetTestingAgainstZBuffer()


/**
    Gets the sticked-to-screen state.
    @return True if sticked to screen, false otherwise.
*/
TPRREbool PRREObject3D::isStickedToScreen() const
{
    return p->isStickedToScreen();
} // isStickedToScreen()


/**
    Sets the sticked-to-screen state.
    Sticked to screen means the object is rendered with orthogonal projection and not overlapped by non-sticked objects.
    Suitable for 2D object rendering such as GUI elements.
    Note that this call not only changes sticked state of the object but also calls the following methods:
     - SetDoubleSided(true);
     - SetLit(false);
     - SetTestingAgainstZBuffer(false).
*/
void PRREObject3D::SetStickedToScreen(TPRREbool value)
{
    p->SetStickedToScreen(value);
} // SetStickedToScreen()


/**
    Gets the material.

    @return Material. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned material is the subobject's material.
            This implicit behavior is for convenience for objects storing only 1 subobject like internally created objects.
*/
const PRREMaterial& PRREObject3D::getMaterial() const
{
    return p->getMaterial();
} // getMaterial()


/**
    Gets the material.
    @return Material. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned material is the subobject's material.
            This implicit behavior is for convenience for objects storing only 1 subobject like internally created objects.
*/
PRREMaterial& PRREObject3D::getMaterial()
{
    return p->getMaterial();
} // getMaterial()


/**
    Gets the amount of allocated system memory.
    It includes the allocated Material size as well (getMaterial()).

    @return Amount if allocated system memory in Bytes.
*/
TPRREuint PRREObject3D::getUsedSystemMemory() const
{
    return PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) + sizeof(*this) + p->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Draws the object.
*/
void PRREObject3D::Draw(bool bLighting)
{
    p->Draw(bLighting);
} // Draw()


// ############################## PROTECTED ##############################


/**
    Only PRREObject3DManager creates it.

    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREObject3D::PRREObject3D(
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory )
{
    p = new PRREObject3DImpl(this, vmod, vref, bForceUseClientMemory);
} // PRREObject3D()


PRREObject3D::PRREObject3D(const PRREObject3D&)
{

}


PRREObject3D& PRREObject3D::operator=(const PRREObject3D&)
{
    return *this;
}


// ############################### PRIVATE ###############################


