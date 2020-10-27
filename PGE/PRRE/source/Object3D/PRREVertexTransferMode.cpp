/*
    ###################################################################################
    PRREVertexTransferMode.cpp
    This file is part of PRRE.
    PRREVertexTransferMode class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PRREVertexTransferMode.h"
#include "../../include/internal/Object3D/PRREVertexTransferModeImpl.h"
#include <cassert>
#include "../../include/external/Hardware/PRREhwInfo.h"
#include "../../include/external/Material/PRREMaterialManager.h"
#include "../../include/internal/PRREGLextensionFuncs.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../../../../PFL/PFL/PFL.h"

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

    Early vendor-specific "VBO"s:
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
   PRREVertexTransfer::PRREVertexTransferImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


TPRRE_PRIMITIVE_FORMAT PRREVertexTransfer::PRREVertexTransferImpl::getPRREprimitiveFromGLprimitive(GLenum glprim)
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


GLenum PRREVertexTransfer::PRREVertexTransferImpl::getGLprimitiveFromPRREprimitive(TPRRE_PRIMITIVE_FORMAT pf)
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


PRREVertexTransfer::PRREVertexTransferImpl::~PRREVertexTransferImpl()
{
    getConsole().OLnOI("~PRREVertexTransferImpl() ...");
    
    FreeGLresources();
    _pOwner->DeleteAll();

    getConsole().SOLnOO("Done!");
} // ~PRRETexture()


TPRRE_VERTEX_MODIFYING_HABIT PRREVertexTransfer::PRREVertexTransferImpl::getVertexModifyingHabit() const
{
    return BIT(PRRE_VT_VMOD_BIT) & vertexTransferMode;
} // getVertexModifyingHabit()


void PRREVertexTransfer::PRREVertexTransferImpl::SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    SetVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(vmod, getVertexReferencingMode(), false) );
} // SetVertexModifyingHabit()


TPRRE_VERTEX_REFERENCING_MODE PRREVertexTransfer::PRREVertexTransferImpl::getVertexReferencingMode() const
{
    return BIT(PRRE_VT_VREF_BIT) & vertexTransferMode;
} // getVertexReferencingMode()

 
void PRREVertexTransfer::PRREVertexTransferImpl::SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    SetVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(getVertexModifyingHabit(), vref, false) );
} // SetVertexReferencingMode()


TPRRE_VERTEX_TRANSFER_MODE PRREVertexTransfer::PRREVertexTransferImpl::getVertexTransferMode() const
{
    return vertexTransferMode;
} // getVertexTransferMode()


void PRREVertexTransfer::PRREVertexTransferImpl::SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    getConsole().OLnOI("PRREVertexTransfer::SetVertexTransferMode()");

    if ( _pOwner->isLevel2() )
    {
        if ( !_pOwner->getManager() )
        {
            // being level-2 without a manager is unusual, but can be achieved when:
            // either user invokes Detach() for a submesh of a complete level-1 mesh,
            // or when we are still creating a mesh and it is not yet attached or it does not have any children.
            // any case, we should IGNORE execution of this function.
            getConsole().EOLnOO("SetVertexTransferMode() of submesh called WITHOUT existing level-1 parent mesh, rejecting!");
            return;
        }
        
        // submesh must reject if its SetVertexTransferMode() was not called by its parent level-1 mesh but someone else from outside ...
        // note: our manager in this case is a level-1 VertexTransfer (Mesh3D) mesh
        if ( !((PRREVertexTransfer*)_pOwner->getManager())->pImpl->bParentInitiatedOperation )
        {
            getConsole().EOLnOO("SetVertexTransferMode() of submesh called outside of its level-1 parent mesh, rejecting!");
            return;
        }
    }  

    // Note: we don't check if the selected transfer mode is already selected.
    // Reason: selecting an already selected transfer mode is a way to recreate buffers / refresh geometry on server side.
    // Modify geometry in system RAM and simply issue SetVertexTransferMode( getVertexTransferMode() ) to apply changes.

    if ( PRREVertexTransfer::isVertexReferencingIndexed(vertexTransferMode) &&
         !PRREVertexTransfer::isVertexReferencingIndexed(vtrans) )
    {
        if ( !isSwitchFromIndexedAllowed() )
        {
            getConsole().EOLnOO("ERROR: failed to switch from indexed to direct referencing mode!");
            return;
        }
    }

    if ( !PRREVertexTransfer::isVertexTransferModeSelectable(vtrans) )
    {
        getConsole().EOLnOO("ERROR: given mode %d is unavailable!", (int)vtrans);
        return;
    }

    FreeGLresources();   // now it's safe to delete GL resources

    if ( BIT_READ(vtrans, PRRE_VT_SVA_BIT) )
    {
        /* check if we want VBO? */
        CompileIntoVertexBufferObjects(
            PRREVertexTransfer::isVertexReferencingIndexed(vtrans),
            PRREVertexTransfer::isVertexModifyingDynamic(vtrans) );
    }
    else
    {
        /* check if we want display list? */
        if ( (vtrans == (PRRE_VMOD_STATIC | PRRE_VREF_DIRECT)) ||
            (vtrans == (PRRE_VMOD_STATIC | PRRE_VREF_INDEXED)) )
            CompileIntoDisplayList(PRREVertexTransfer::isVertexReferencingIndexed(vtrans));
    }

    /* no other transfer mode needs further preparation */
    vertexTransferMode = vtrans;

    if ( _pOwner->isLevel1() )
    {
        bParentInitiatedOperation = true;
        getConsole().OLn("Ok, applying for subobjects ...");
        for (TPRREint i = 0; i < _pOwner->getCount(); i++)
            ((PRREVertexTransfer*)(_pOwner->getAttachedAt)(i))->SetVertexTransferMode( vertexTransferMode );
        getConsole().OLn("Done!");
        bParentInitiatedOperation = false;
    }

    getConsole().OO();
} // SetVertexTransferMode()


TPRREuint PRREVertexTransfer::PRREVertexTransferImpl::getUsedSystemMemory() const
{
    return sizeof(*this);
} // getUsedSystemMemory() 


// ############################## PROTECTED ##############################



/**
    @param pOwner                The public Mesh3D class instance owning this pimpl mesh.
    @param matMgr                A MaterialManager instance to be used for knowing texture unit / layers count.
    @param vmod                  What vertex modifying habit to be set for the new Mesh3D instance.
    @param vref                  What vertex referencing mode to be set for the new Mesh3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREVertexTransfer::PRREVertexTransferImpl::PRREVertexTransferImpl(
    PRREVertexTransfer* pOwner,
    PRREMaterialManager& matMgr,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory ) : materialMgr(matMgr)
{
    _pOwner = pOwner;
    getConsole().OLnOI("PRREVertexTransferImpl() ...");

    bParentInitiatedOperation = false;
    nDispList = 0;
    nVerticesVBO = nColorsVBO = nNormalsVBO = nIndicesVBO = 0;
    nTexcoordsVBO.resize( materialMgr.getMaximumLayerCount() );

    vertexTransferMode = PRREVertexTransfer::selectVertexTransferMode(vmod, vref, bForceUseClientMemory);

    getConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREVertexTransfer::PRREVertexTransferImpl::PRREVertexTransferImpl(const PRREVertexTransferImpl&) : materialMgr(materialMgr)
{

}


PRREVertexTransfer::PRREVertexTransferImpl& PRREVertexTransfer::PRREVertexTransferImpl::operator=(const PRREVertexTransferImpl&)
{
    return *this;
}


void PRREVertexTransfer::PRREVertexTransferImpl::TransferVertices()
{
    if ( BIT_READ(getVertexTransferMode(), PRRE_VT_VA_BIT) == 0u )
    {
        /* there is no array for geometry, so we either invoke a display list or pass vertices 1-by-1 */

        if ( PRREVertexTransfer::isVertexModifyingDynamic( getVertexTransferMode() ) )
            // PRRE_VT_DYN_..._1_BY_1
            ProcessGeometry( PRREVertexTransfer::isVertexReferencingIndexed( getVertexTransferMode() ) );
        else
            // PRRE_VT_STA_..._DL
            glCallList( nDispList );

        return;
    } // PRRE_VT_VA_BIT == 0u

    /* vertex array either in client or server */
    const bool bServer = BIT_READ(getVertexTransferMode(), PRRE_VT_SVA_BIT) == 1u;

    SetArrayPointers( bServer );

        // 1 big benefit of using CVA is when we want to do multitexturing with only
        // 1 TMU, meaning that we need to do it in at least 2 passes, specifying
        // vertex geometry twice! However even TNT had 2 TMUs, so I'm not planning to do this improvement.
        // Another big benefit is when we have shared vertices, in indexed rendering
        // all vertices are transformed if lock is enabled, thus shared vertices
        // can be used multiple times EVEN THOUGH THEY ARE TRANSFORMED ONLY ONCE!
        // Too bad we currently create redundant vertex geometry when loading OBJ mesh,
        // however we have backlog item for that ...
        if ( BIT_READ(getVertexTransferMode(), PRRE_VT_CVA_BIT) == 1u )
            glLockArraysEXT(0, _pOwner->getVerticesCount(false));

        // Static and dynamic vmods are not differentiated here but when creating the arrays.
        // That is why we don't check for vmod in this code.

        if ( PRREVertexTransfer::isVertexReferencingIndexed( getVertexTransferMode() ) )
        {
            assert(_pOwner->getVertexIndicesCount(false) != 0);
            if ( BIT_READ(getVertexTransferMode(), PRRE_VT_RNG_BIT) )
            {
                // TODO: should call this in loop, do multiple batches based on the implementation-dependent values
                // (GL_MAX_ELEMENTS_VERTICES_EXT, GL_MAX_ELEMENTS_INDICES_EXT).
                glDrawRangeElementsEXT(
                    getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()),
                    _pOwner->getMinVertexIndex(false), _pOwner->getMaxVertexIndex(false),
                    _pOwner->getVertexIndicesCount(false), _pOwner->getVertexIndicesType(false), bServer ? NULL : _pOwner->getVertexIndices(false) );
            }
            else
            {
                glDrawElements(
                    getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()), 
                    _pOwner->getVertexIndicesCount(false), _pOwner->getVertexIndicesType(false), bServer ? NULL : _pOwner->getVertexIndices(false) );
            }
        }
        else
        {
            assert(_pOwner->getVerticesCount(false) != 0);
            glDrawArrays( getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()), 0, _pOwner->getVerticesCount(false) );
        }

        if ( BIT_READ(getVertexTransferMode(), PRRE_VT_CVA_BIT) == 1u )
            glUnlockArraysEXT();

    ResetArrayPointers( bServer );
}


// ############################### PRIVATE ###############################


/**
    Hack to be able to use CConsole singleton instance instead of owner's protected console instance.
    TODO: maybe the whole getConsole() stuff should be removed from everywhere because CConsole is a singleton class after all.
*/
CConsole& PRREVertexTransfer::PRREVertexTransferImpl::getConsole() const
{
    return CConsole::getConsoleInstance();
}


/**
    Tells whether it is allowed to switch from indexed to non-indexed vertex referencing mode.
    It is allowed when geometry data is stored redundantly so direct referencing is available.
    Geometry data is stored redundantly when multiple vertices are defined with the same position
    for different faces. In this situation, the number of vertex indices is equal to the number of
    defined vertices so the number of defined vertices is the adequate number for rendering the
    whole geometry even without using indices.
    So switching to non-indexed is disallowed only when the number of defined vertices is less
    than the number of indices.

    @return True if swithing from indexed to non-indexed vertex referencing mode is allowed, false otherwise.
*/
TPRREbool PRREVertexTransfer::PRREVertexTransferImpl::isSwitchFromIndexedAllowed() const
{
    if ( _pOwner->isLevel1() )
    {
        for (TPRREint i = 0; i < _pOwner->getCount(); i++)
        {
            const PRREVertexTransfer& submesh = (const PRREVertexTransfer&) *(_pOwner->getAttachedAt(i));
            if ( !submesh.pImpl->isSwitchFromIndexedAllowed() )
            {
                return false;
            }
        }
        return true;
    }
    else
        return (_pOwner->getVertexIndicesCount(false) == _pOwner->getVerticesCount(false)) && (_pOwner->getVerticesCount(false) > 0);
}


/**
    Goes thru vertices and feeds them to OpenGL.
    Used by any vertex transfer mode not utilizing vertex arrays, such as:
     - immediate mode;
     - display lists.
    Handles both direct and indexed vertex reference modes.

    @param indexed If true, will go thru vertices by using pVertexIndices for ordering, otherwise won't use it.
*/
void PRREVertexTransfer::PRREVertexTransferImpl::ProcessGeometry(TPRREbool indexed) const  
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    //const TRGBAFLOAT* const pColors = _pOwner->getMaterial(false).getColors(0);
    //const void* const pColorIndices = _pOwner->getMaterial(false).getColorIndices(0);
    const TUVW* const pTexcoords = _pOwner->getMaterial(false).getTexcoords(0);
    // following can be NULL if material system supports only 1 level
    const TUVW* const pTexcoords2 = _pOwner->getMaterial(false).getTexcoords(1);
    //const void* const pTexcoordIndices2 = _pOwner->getMaterial(false).getTexcoordIndices(1);

    glBegin( getGLprimitiveFromPRREprimitive(_pOwner->getPrimitiveFormat()) );
        for (TPRREuint i = 0; i < _pOwner->getVertexIndicesCount(false); i++)
        {
            const TXYZ&       vertex = indexed ? _pOwner->getVertices(false)[ _pOwner->getVertexIndex(i) ]    : _pOwner->getVertices(false)[i];
            const TXYZ&       normal = indexed ? _pOwner->getNormals(false)[ _pOwner->getVertexIndex(i) ]     : _pOwner->getNormals(false)[i];
            //const TRGBAFLOAT& color  = indexed ? pColors[ _pOwner->getVertexIndex(pColorIndices, i) ]       : pColors[i];
            const TUVW&       uvw    = indexed ? pTexcoords[ _pOwner->getVertexIndex(i) ] : pTexcoords[i];

            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
            {
                if ( _pOwner->getMaterial(false).isMultiTextured() )
                {
                    const TUVW& uvw2 = indexed ? pTexcoords2[ _pOwner->getVertexIndex(i) ] : pTexcoords2[i];
                    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, uvw.u, uvw.v);
                    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, uvw2.u, uvw2.v);
                }
                else
                {
                    if ( _pOwner->getMaterial(false).getTexture() != PGENULL )
                        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, uvw.u, uvw.v);
                }
            }
            else
            {
                if ( _pOwner->getMaterial(false).getTexture() != PGENULL )
                    glTexCoord2f(uvw.u, uvw.v);
            }
            
            // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere in setarraypointers()
            //glColor4f(color.red, color.green, color.blue, color.alpha);
            
            // in the past, we sent normal data only if bAffectedLights was true
            // however, probably it should be solved in a different way, like enable/disable specific lights like LIGHT0 before rendering object,
            // anyway I've commented out this condition since after refactor, VertexTransfer does not know such thing like bAffectedByLights.
            //if ( bAffectedByLights )
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        } // for k
    glEnd();
} // ProcessGeometry()


/**
    Compiles OpenGL drawing commands into display list.

    @param indexed If true, will issue ProcessGeometry() to go thru geometry data by using indices for order.
*/
void PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoDisplayList(TPRREbool indexed)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( _pOwner->getVertices(false) == NULL )
    {
        getConsole().EOLnOO("ERROR: PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoDisplayList() getVertices() is NULL!");
        return;
    }

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
void PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoVertexBufferObjects(TPRREbool indexed, TPRREbool dynamic)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( nVerticesVBO != 0 )
    {
        getConsole().EOLnOO("ERROR: PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoVertexBufferObjects() nVerticesVBO != 0!");
        return;
    }
    if ( _pOwner->getVertices(false) == NULL )
    {
        getConsole().EOLnOO("ERROR: PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoVertexBufferObjects() getVertices() is NULL!");
        return;
    }
    if ( _pOwner->getNormals(false) == NULL )
    {
        getConsole().EOLnOO("ERROR: PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoVertexBufferObjects() getNormals() is NULL!");
        return;
    }

    if ( indexed )
    {
        if ( nIndicesVBO != 0 )
        {
            getConsole().EOLnOO("ERROR: PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoVertexBufferObjects() nIndicesVBO != 0!");
            return;
        }
        if ( _pOwner->getVertexIndices(false) == NULL )
        {
            getConsole().EOLnOO("ERROR: PRREVertexTransfer::PRREVertexTransferImpl::CompileIntoVertexBufferObjects() getVertexIndices() is NULL!");
            return;
        }
        glGenBuffersARB(1, &nIndicesVBO);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndicesVBO);
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pOwner->getVertexIndicesCount(false) * PRREGLsnippets::getSizeofIndexType(_pOwner->getVertexIndicesType()), _pOwner->getVertexIndices(false), GL_STATIC_DRAW_ARB);
        // Note: we always store indices in static buffer but could be in client memory, too.
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }

    GLenum usage = dynamic ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW_ARB;
    glGenBuffersARB(1, &nVerticesVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, nVerticesVBO);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,  _pOwner->getVerticesCount(false) * sizeof(TXYZ), _pOwner->getVertices(false), usage);
 
    glGenBuffersARB(1, &nColorsVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, nColorsVBO);
    assert(_pOwner->getMaterial(false).getColors() != NULL);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT), _pOwner->getMaterial(false).getColors(), usage);

    glGenBuffersARB(1, &nTexcoordsVBO[0]);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[0]);
    assert(_pOwner->getMaterial(false).getTexcoords() != NULL);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getMaterial(false).getTexcoordsCount() * sizeof(TUVW), _pOwner->getMaterial(false).getTexcoords(), usage);

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        if ( _pOwner->getMaterial(false).isMultiTextured() )
        {
            glGenBuffersARB(1, &nTexcoordsVBO[1]);
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[1]);
            assert(_pOwner->getMaterial(false).getTexcoords(1) != NULL);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getMaterial(false).getTexcoordsCount(1) * sizeof(TUVW), _pOwner->getMaterial(false).getTexcoords(1), usage);
        }
    }

    glGenBuffersARB(1, &nNormalsVBO);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, nNormalsVBO);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, _pOwner->getVerticesCount(false) * sizeof(TXYZ), _pOwner->getNormals(false), usage);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

} // CompileIntoVertexBufferObjects()


/**
    Frees up allocated resources from OpenGL.
    Should be called before setting a new Vertex Transfer mode and when deleting the mesh.
*/
void PRREVertexTransfer::PRREVertexTransferImpl::FreeGLresources()
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    for (TPRREint i = 0; i < _pOwner->getCount(); i++)
        ((PRREVertexTransfer*)_pOwner->getAttachedAt(i))->pImpl->FreeGLresources();

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
void PRREVertexTransfer::PRREVertexTransferImpl::SetArrayPointers(TPRREbool redirectToServer) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    // since this is called every render frame, I dont check for values with if, instead I use only assertions!

    if ( (PRREVertexTransfer::isVertexReferencingIndexed(vertexTransferMode)) && redirectToServer )
    {
        assert(nIndicesVBO != 0);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nIndicesVBO);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason:
    // as tmcsgfxlib uses a single vertex color for object color key when object is textured,
    // we shouldn't specify any further per-vertex color, otherwise alphablending would be bad with legacy projects.
    // In future, we should rather use glBlendColor() for such objects and then per-vertex colors could be re-enabled again.
    // glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    if ( redirectToServer )
    {
        assert(nVerticesVBO != 0);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nVerticesVBO);
        glVertexPointer(3, GL_FLOAT, 0, NULL);

        // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere above
        //glBindBufferARB(GL_ARRAY_BUFFER_ARB, nColorsVBO);
        //glColorPointer(4, GL_FLOAT, 0, NULL);

        assert(nNormalsVBO != 0);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, nNormalsVBO);
        glNormalPointer(GL_FLOAT, 0, NULL);
    }
    else
    {
        assert(_pOwner->getVertices(false) != NULL);
        glVertexPointer(3, GL_FLOAT, 0, _pOwner->getVertices(false));
        // per-vertex color is disabled for compatibility with tmcsgfxlib wrapper, reason somewhere above
        //glColorPointer(4, GL_FLOAT, 0, _pOwner->getMaterial(false).getColors());
        assert(_pOwner->getNormals(false) != NULL);
        glNormalPointer(GL_FLOAT, 0, _pOwner->getNormals(false));
    }

    if ( _pOwner->getMaterial(false).isTextured() )
    {
        const TPRREuint iMaxTexLayer = _pOwner->getMaterial(false).isMultiTextured() ? 1 : 0;
        for (TPRREuint i = 0; i <= iMaxTexLayer; i++)
        {
            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
                glClientActiveTextureARB(GL_TEXTURE0_ARB + i);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            if ( redirectToServer )
            {
                assert(nTexcoordsVBO[i] != 0);
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, nTexcoordsVBO[i]);
                glTexCoordPointer(3, GL_FLOAT, 0, NULL);
            }
            else
            {
                assert(_pOwner->getMaterial(false).getTexcoords(i) != NULL);
                glTexCoordPointer(3, GL_FLOAT, 0, _pOwner->getMaterial(false).getTexcoords(i));
            }
        }
    }
} // SetArrayPointers()


/**
    Resets vertex data pointers after drawing.
    Can be used for both vertex buffer objects and legacy vertex arrays.

    @param redirectToServer Specify true if geometry data is located in server memory.
*/
void PRREVertexTransfer::PRREVertexTransferImpl::ResetArrayPointers(TPRREbool redirectToServer) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( redirectToServer )
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        if ( PRREVertexTransfer::isVertexReferencingIndexed(vertexTransferMode) )
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }
    if ( _pOwner->getMaterial(false).isTextured() )
    {
        const TPRREuint iMaxTexLayer = _pOwner->getMaterial(false).isMultiTextured() ? 1 : 0;
        for (TPRREuint i = 0; i <= iMaxTexLayer; i++)
        {
            if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
                glClientActiveTextureARB(GL_TEXTURE0_ARB + i);

            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
} // ResetArrayPointers()


/*
   PRREVertexTransfer
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Tells whether the given Vertex Transfer Mode is available on the current hardware.
    The following modes are always available with accelerated renderer:
    - dynamic modifying:
      - direct referencing:  PRRE_VT_DYN_DIR_1_BY_1, PRRE_VT_DYN_DIR_RVA
      - indexed referencing: PRRE_VT_DYN_IND_1_BY_1, PRRE_VT_DYN_IND_RVA
    - static modifying:
      - direct referencing:  PRRE_VT_STA_DIR_DL
      - indexed referencing: PRRE_VT_STA_IND_DL

    The following modes are always available with non-accelerated renderer:
    - dynamic modifying:
      - direct referencing:  PRRE_VT_DYN_DIR_1_BY_1
      - indexed referencing: PRRE_VT_DYN_IND_1_BY_1
    - static modifying:
      - direct referencing:  same as for dynamic modifying above
      - indexed referencing: same as for dynamic modifying above

    @return True if the given transfer mode is available on the current hardware, false otherwise.
*/
TPRREbool PRREVertexTransfer::isVertexTransferModeSelectable(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {   /* only these 2 modes are supported in non-accelerated mode */
        if ( (vtrans == (PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT)) ||
             (vtrans == (PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED)) )
           return true;
        else
           return false;
    }

    /* at this point we are accelerated */

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(vtrans,PRRE_VT_VENDOR_BITS,3) != 0 )
        return false;

    /* first need to check for CVA and RNG capability, as both may apply to VBO */

    if ( BIT_READ(vtrans, PRRE_VT_RNG_BIT) && !PRREhwInfo::get().getVideo().isDrawRangeElementsSupported()  )
        return false;

    if ( BIT_READ(vtrans, PRRE_VT_CVA_BIT) && !PRREhwInfo::get().getVideo().isCompiledVertexArraySupported()  )
        return false;

    if ( BIT_READ(vtrans, PRRE_VT_SVA_BIT) && !PRREhwInfo::get().getVideo().isVertexBufferObjectSupported()  )
        return false;

    return true;
} // isVertexTransferModeSelectable()


/**
    Tells whether the given Vertex Transfer Mode references vertices by indexing.
    @return True if the given mode references vertices by indexing.
*/
TPRREbool PRREVertexTransfer::isVertexReferencingIndexed(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return BIT_READ(vtrans, PRRE_VT_VREF_BIT) == 1u;
} // isVertexReferencingIndexed()


/**
    Tells whether the given Vertex Transfer Mode means dynamic modifying habit.
    @return True if the given mode means dynamic modifying habit.
*/
TPRREbool PRREVertexTransfer::isVertexModifyingDynamic(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return BIT_READ(vtrans, PRRE_VT_VMOD_BIT) == 1u;
} // isVertexModifyingDynamic()


/**
    Selects a suitable vertex transfer mode.
    The selected mode is compatible with the current hardware and complies with the given arguments.
    Please note that bForceUseClientMemory = true is considered only if dynamic modifying habit is specified.
    Specifying static modifying habit will always select a mode which places geometry data into server memory.
    
    @return A suitable vertex transfer mode selected by PPP based on the given arguments and
            the current hardware capabilities.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREVertexTransfer::selectVertexTransferMode(
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory)
{
    TPRRE_VERTEX_TRANSFER_MODE ret = 0;

    if ( vref == PRRE_VREF_INDEXED )
        ret = ret | PRRE_VREF_INDEXED;

    // dont go further if CPU is rendering
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return ret | PRRE_VMOD_DYNAMIC;

    if ( vmod == PRRE_VMOD_DYNAMIC )
        ret = ret | PRRE_VMOD_DYNAMIC;

    if ( PRREhwInfo::get().getVideo().isDrawRangeElementsSupported() )
        ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_RNG_BIT);

    if ( PRREhwInfo::get().getVideo().isCompiledVertexArraySupported() )
        ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_CVA_BIT);

    // bForceUseClientMemory is considered only if modifying habit is dynamic
    if ( bForceUseClientMemory && (vmod == PRRE_VMOD_DYNAMIC) )
        return ret | BIT(PRRE_VT_VA_BIT); // client-side should use an array, even if none of RNG and CVA bits are available, to avoid selecting immediate mode

    // At this point we want to use host/server memory, but it could be still either client- or server-side array, or display list or immediate mode, so
    // we should set PRRE_VT_VA_BIT only in case of some arraying.

    // if general VBO is supported, return with that immediately as we don't want to stick to any vendor-specific magical stuff.
    if ( PRREhwInfo::get().getVideo().isVertexBufferObjectSupported() )
        return ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);

    if ( PRREhwInfo::get().getVideo().isATIVertexArrayObjectSupported() )
    {
        if ( PRREhwInfo::get().getVideo().isATIElementArraySupported() )
            ret = ret | BIT(PRRE_VT_RNG_BIT) | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_ATI_2,PRRE_VT_VENDOR_BITS,3);
        else
            ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_ATI_1,PRRE_VT_VENDOR_BITS,3);
    }
    else
    {
        if ( PRREhwInfo::get().getVideo().isNVVertexArrayRangeSupported() )
        {
            if ( PRREhwInfo::get().getVideo().isNVElementArraySupported() )
                ret = ret | BIT(PRRE_VT_RNG_BIT) | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_NVIDIA_2,PRRE_VT_VENDOR_BITS,3);
            else
                ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_NVIDIA_1,PRRE_VT_VENDOR_BITS,3);
        }
    }

    // at this point, if VA_BIT is not set, the selected mode is either display list or immediate mode, depending on vmod

    return ret;
} // selectVertexTransferMode()


PRREVertexTransfer::~PRREVertexTransfer()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRRETexture()


/**
    Gets vertex modifying habit.
    It applies to the entire geometry of the mesh, including all of its submeshes.
    Level-2 objects use the property of their parent mesh.
*/
TPRRE_VERTEX_MODIFYING_HABIT PRREVertexTransfer::getVertexModifyingHabit() const
{
    return pImpl->getVertexModifyingHabit();
} // getVertexModifyingHabit()


/**
    Sets vertex modifying habit.
    It applies to the entire geometry of the mesh, including all of its submeshes.
    The setting is ignored for level-2 objects as it is applied by their level-1 parent mesh.
*/
void PRREVertexTransfer::SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    pImpl->SetVertexModifyingHabit(vmod);
} // SetVertexModifyingHabit()


/**
    Gets vertex referencing mode.
    It applies to the entire geometry of the mesh, including all of its submeshes.
    Level-2 objects use the property of their level-1 parent mesh.
*/
TPRRE_VERTEX_REFERENCING_MODE PRREVertexTransfer::getVertexReferencingMode() const
{
    return pImpl->getVertexReferencingMode();
} // getVertexReferencingMode()

 
/**
    Sets vertex referencing mode.
    It applies to the entire geometry of the mesh, including all of its submeshes.
    The setting is ignored for level-2 objects as it is applied by their level-1 parent mesh.
*/
void PRREVertexTransfer::SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    pImpl->SetVertexReferencingMode(vref);
} // SetVertexReferencingMode()


/**
    Gets vertex transfer mode.
    It applies to the entire geometry of the mesh, including all of its submeshes.
    Level-2 objects use the property of their level-1 parent mesh.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREVertexTransfer::getVertexTransferMode() const
{
    return pImpl->getVertexTransferMode();
} // getVertexTransferMode()


/**
    Sets vertex transfer mode.
    Vertex transfer mode gets selected automatically by PPP.
    However, it can be also set manually for custom reasons.
    The setting may not happen if the selected transfer mode is not available for some reason, for
    example, if the selected mode is not supported on the current hardware.
    It applies to the entire geometry of the mesh, including all of its submeshes.
    The setting is ignored for level-2 meshes as it is applied by their level-1 parent mesh.
*/
void PRREVertexTransfer::SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    pImpl->SetVertexTransferMode(vtrans);
} // SetVertexTransferMode()


/**
    Gets the amount of allocated system memory.

    @return Amount if allocated system memory in Bytes.
*/
TPRREuint PRREVertexTransfer::getUsedSystemMemory() const
{
    return sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only PRREObject3D or PRREObject3DManager creates it.

    @param matMgr                A MaterialManager instance to be used for constructing of ancestor class.
    @param vmod                  What vertex modifying habit to be set for the new Mesh3D instance.
    @param vref                  What vertex referencing mode to be set for the new Mesh3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREVertexTransfer::PRREVertexTransfer(
    PRREMaterialManager& matMgr,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory )
{
    PRREFiledManaged::getConsole().OLnOI("PRREVertexTransfer() ...");
    pImpl = new PRREVertexTransferImpl(this, matMgr, vmod, vref, bForceUseClientMemory);
    PRREFiledManaged::getConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREVertexTransfer::PRREVertexTransfer()
{
    PRREFiledManaged::getConsole().OLnOI("PRREVertexTransfer() default ctor ...");
    pImpl = PGENULL;
    PRREFiledManaged::getConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREVertexTransfer::PRREVertexTransfer(const PRREVertexTransfer& other)
{
}


PRREVertexTransfer& PRREVertexTransfer::operator=(const PRREVertexTransfer&)
{
    return *this;
}


/**
    Transfers vertices over the vertex pipeline.
    This invokes the draw commands based on the current vertex transfer mode.
*/
void PRREVertexTransfer::TransferVertices()
{
    pImpl->TransferVertices();
}


// ############################### PRIVATE ###############################




