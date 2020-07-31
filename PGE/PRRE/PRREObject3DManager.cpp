/*
    ###################################################################################
    PRREObject3DManager.cpp
    This file is part of PRRE.
    PRREObject3DManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREObject3DManager.h"
#include "PRREObject3DImpl.h"
#include "PRREGLextensionFuncs.h"
#include "PRREGLsnippets.h"
#include "PRREhwInfo.h"

using namespace std;

/*
   PRREObject3DManager::PRREObject3DManagerImpl
   ###########################################################################
*/


class PRREObject3DManager::PRREObject3DManagerImpl
{

public:
    virtual ~PRREObject3DManagerImpl();

    TPRREbool isInitialized() const;

    TPRREbool isMinimalIndexStorageEnabled() const;
    void      SetMinimalIndexStorageEnabled(TPRREbool state);

protected:
    PRREObject3D* loadOBJ(
        const char* filename,
        TPRRE_VERTEX_MODIFYING_HABIT vmod,
        TPRRE_VERTEX_REFERENCING_MODE vref,
        TPRREbool bForceUseClientMemory = false);   /**< Loads OBJ files, handles the actual file operations, and creates the Object3D object. */

private:

    PRREObject3DManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

    TPRREbool            bInited;               /**< True if successfully inited, false if not functional. */
    TPRREbool            bMinimalIndexStorage;  /**< True if storage of indices is minimalized. */
    PRRETextureManager&  textureMgr;            /**< Used to auto-load textures for OBJ files. */
    PRREMaterialManager& materialMgr;           /**< Used to auto-load/create materials for objects. */

    // ---------------------------------------------------------------------------

    PRREObject3DManagerImpl();
    PRREObject3DManagerImpl(
        PRREObject3DManager* owner,
        PRRETextureManager& texMgr,
        PRREMaterialManager& matMgr );

    PRREObject3DManagerImpl(const PRREObject3DManagerImpl&);
    PRREObject3DManagerImpl& operator=(const PRREObject3DManagerImpl&);

    friend class PRREObject3DManager;

};


// ############################### PUBLIC ################################


PRREObject3DManager::PRREObject3DManagerImpl::~PRREObject3DManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRREObject3DManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PRREObject3DManager()


TPRREbool PRREObject3DManager::PRREObject3DManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPRREbool PRREObject3DManager::PRREObject3DManagerImpl::isMinimalIndexStorageEnabled() const
{
    return bMinimalIndexStorage;
}


void PRREObject3DManager::PRREObject3DManagerImpl::SetMinimalIndexStorageEnabled(TPRREbool state)
{
    bMinimalIndexStorage = state;
}


// ############################## PROTECTED ##############################


/**
    Loads OBJ files, handles the actual file operations, and creates the Object3D object.

    @param filename              The model file to be loaded to be an Object3D instance.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return The created Object3D object on success, PGENULL otherwise.
*/
PRREObject3D* PRREObject3DManager::PRREObject3DManagerImpl::loadOBJ(
    const char* filename,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    const HANDLE objfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

    PRREObject3D* obj = NULL;
    DWORD     bytesread = 0;
    TPRREuint lines_h = 0;
    DWORD     filebuffer_size = 0;
    DWORD     nLastError;

    _pOwner->getConsole().OLnOI("PRREObject3DManager::Load_OBJ(""%s"")", filename);
    if ( objfile == INVALID_HANDLE_VALUE )
    {
        _pOwner->getConsole().EOLnOO("ERROR: objfile == INVALID_HANDLE_VALUE, returning NULL");
        return NULL;
    }

    filebuffer_size = GetFileSize(objfile, NULL);
    if ( (filebuffer_size == 0) || (filebuffer_size == INVALID_FILE_SIZE) )
    {
        nLastError = GetLastError();
        _pOwner->getConsole().EOLnOO("ERROR: filebuffer_size == %d, nLastError == %d, returning NULL", filebuffer_size, nLastError);
        // Note that nLastError can still be NO_ERROR. In that case it means that file size is actually INVALID_FILE_SIZE (0xFFFFFFFF) or
        // larger (for that case the lpFileSizeHigh param could be used or GetFileSizeEx()). In any case, we don't want to proceed
        // further, as we don't want to try malloc 0xFFFFFFFF or larger memory area. No model files should reach this size, or
        // a different approach is needed to read the whole file.
        CloseHandle(objfile);
        return NULL;
    }

    char* const filebuffer = (char* const) malloc(filebuffer_size * sizeof(char));
    ReadFile(objfile, filebuffer, filebuffer_size, &bytesread, NULL);
    CloseHandle(objfile);
    if ( bytesread != filebuffer_size )
    {
        _pOwner->getConsole().EOLnOO("ERROR: %d != %d (filebuffer_size != bytesread), returning NULL", filebuffer_size, bytesread);
        return NULL;
    }

    _pOwner->getConsole().OLn("File read into buffer (%d bytes) and closed, preprocessing file ...", filebuffer_size);
    lines_h = PFL::numCharAppears(10, filebuffer, filebuffer_size) + 1; // total lines in file
    _pOwner->getConsole().OLn("lines_h = %d", lines_h);

    // TODO: performance: instead of memcpy-ing from filebuffer to lines[], lines[i] should be set to point properly into filebuffer
    char** const lines = (char** const) calloc(lines_h, sizeof(char*)); 
    const char* tmpptr = filebuffer;
    const char* prevtmpptr;
    TPRREuint actline = 0;
    do
    {                                                                            
        if ( tmpptr == filebuffer )
        {
            prevtmpptr = tmpptr;
            tmpptr = tmpptr-1;
        }
        else
        {
            prevtmpptr = tmpptr;
        }
        // finding line end
        tmpptr = (char*) memchr(tmpptr+1, 10, filebuffer_size-((DWORD)(tmpptr-filebuffer)));
                    
        if ( (tmpptr != NULL) && (prevtmpptr != NULL) )
        {    // found a new line, allocating mem for it and put it in
            lines[actline] = (char*) calloc(tmpptr-prevtmpptr, sizeof(char));
            if ( tmpptr-prevtmpptr > 0 )
            {
                if ( prevtmpptr == filebuffer )    // if this is the 1st line
                    memcpy(lines[actline], prevtmpptr, tmpptr-prevtmpptr-1);
                else
                    memcpy(lines[actline], prevtmpptr+1, tmpptr-prevtmpptr-1);
            }
            actline++;
        }
        else
        {    // no more endline chars but chars after the last endline char are still needed
            if ( (DWORD) (prevtmpptr - filebuffer) < filebuffer_size )
            {
                lines[actline] = (char*) calloc(filebuffer_size-(prevtmpptr-filebuffer)+1, sizeof(char));
                memcpy(lines[actline], prevtmpptr, filebuffer_size-(prevtmpptr-filebuffer));
            }
        }
    } while (tmpptr != NULL);
    free(filebuffer);

    _pOwner->getConsole().OLn("preprocessing done, gathering data ...");
    
    _pOwner->getConsole().OLnOI("creating parent object ...");
    obj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    obj->p->primitiveFormat = PRRE_PM_TRIANGLES;
    obj->p->nTexcoordsVBO.resize( materialMgr.getMaximumLayerCount() );
    obj->p->pMaterial = materialMgr.createMaterial();
    _pOwner->getConsole().OLnOO("parent object created!");

    _pOwner->getConsole().OLnOI("creating subobjects ...");
    TPRREuint submeshes_h = 0;
    // counting submeshes
    for (TPRREuint i = 0; i < lines_h; i++)
    {
        if ( memchr(lines[i], '#', strlen((char*)lines[i])) == 0 )
        {
            if ( strchr((char*) lines[i],'g') > 0  )
                submeshes_h++;
        }
    }
    submeshes_h = submeshes_h / 2; // number of 'g'-s are twice as the number of submeshes
    obj->PreAlloc(submeshes_h);
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {
        _pOwner->getConsole().OI();
        PRREObject3D* subobj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
        subobj->p->primitiveFormat = PRRE_PM_TRIANGLES;
        subobj->p->nTexcoordsVBO.resize( materialMgr.getMaximumLayerCount() );
        subobj->p->pMaterial = materialMgr.createMaterial();
        _pOwner->getConsole().OO();
        obj->Attach(*subobj);
    }
    _pOwner->getConsole().OLn("submesh count = %d", submeshes_h);
    _pOwner->getConsole().OLnOO("subobjects created!");

    // We read distinct vertices, texcoords and normals data into tmp arrays.
    // At the end, we fill the subObjects' vertices, texcoords and normals arrays with redundant
    // data based on the temporary arrays, so both direct referencing and indexing modes will be available.
    // Redundant storage is essential for indexed vertex arrays (element arrays). Indexing in non-redundant
    // format is only ok when multiple index arrays can be used for the same mesh: 1 for vertices, 1 for texcoords, etc.
    // But when only 1 element array can be used for all of these, like glDrawElements(), these must be stored
    // redundantly.
    TXYZ** tmpSubMeshesVertices  = (TXYZ**) malloc(submeshes_h * sizeof(TXYZ*));
    TUVW** tmpSubMeshesTexcoords = (TUVW**) malloc(submeshes_h * sizeof(TUVW*));
    TXYZ** tmpSubMeshesNormals   = (TXYZ**) malloc(submeshes_h * sizeof(TXYZ*));
    TPRREuint* tmpSubMeshesVertices_h  = (TPRREuint*) malloc(submeshes_h * sizeof(TPRREuint));
    TPRREuint* tmpSubMeshesTexcoords_h = (TPRREuint*) malloc(submeshes_h * sizeof(TPRREuint));
    TPRREuint* tmpSubMeshesNormals_h   = (TPRREuint*) malloc(submeshes_h * sizeof(TPRREuint));

    TPRREuint* const lines_start = (TPRREuint*) malloc(submeshes_h * sizeof(TPRREuint));
    TPRREuint* const lines_end = (TPRREuint*) malloc(submeshes_h * sizeof(TPRREuint));
    
    bool sector = false;
    TPRREuint actsubmesh = 0;
    TPRREuint k;
    
    // finding starting and ending lines of vertices for every submesh
    _pOwner->getConsole().OLnOI("parsing vertices ...");
    for (TPRREuint i = 0; i < lines_h; i++)
    {
        if ( (strstr((char*) lines[i],"v") > 0) && (strstr((char*) lines[i],"#") == 0) && (strstr((char*) lines[i],"vt") == 0) && (strstr((char*) lines[i],"vn") == 0) && (strstr((char*) lines[i],"g") == 0))
        {
            if ( !sector )
            {
                sector = true;
                lines_start[actsubmesh] = i;
            }
        }
        else
        {
            if ( sector )
            {
                sector = false;
                lines_end[actsubmesh] = i-1;
                actsubmesh++;
            }
        }
    }
                
    int numparsed = 0;
    // parsing vertices
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {                    
        /*obj->subObjects[i]->nVertices_h = lines_end[i] - lines_start[i] + 1;
        obj->subObjects[i]->pVertices = (TXYZ*) malloc( sizeof(TXYZ) * obj->subObjects[i]->nVertices_h );*/
        tmpSubMeshesVertices_h[i] = lines_end[i] - lines_start[i] + 1;
        tmpSubMeshesVertices[i] = (TXYZ*) malloc( sizeof(TXYZ) * tmpSubMeshesVertices_h[i] );
        _pOwner->getConsole().OLn("tmpSubMeshesVertices_h[%d] == %d", i, tmpSubMeshesVertices_h[i]);
        k = 0;
        for (TPRREuint j = lines_start[i]; j <= lines_end[i]; j++)
        {
            numparsed = sscanf((char*)lines[j], "%*s %f %f %f", &(tmpSubMeshesVertices[i][k].x), &(tmpSubMeshesVertices[i][k].y), &(tmpSubMeshesVertices[i][k].z));
            if ( numparsed != 3 )
            {
                _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ vertices section: numparsed == %d", numparsed);
                _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
            }    
            k++;
        }
    }
    _pOwner->getConsole().OLnOO("vertices parsed!");

    // finding starting and ending lines of texcoords for every submesh
    _pOwner->getConsole().OLnOI("parsing texcoords ...");
    sector = false;
    actsubmesh = 0;
    for (TPRREuint i = 0; i < lines_h; i++)
    {
        if ( (strstr((char*) lines[i], "vt") > 0) && (strstr((char*) lines[i], "#") == 0) )
        {
            if ( !sector )
            {
                sector = true;
                lines_start[actsubmesh] = i;
            }
        }
        else
        {
            if ( sector )
            {
                sector = false;
                lines_end[actsubmesh] = i-1;
                actsubmesh++;
            }
        }
    }
                
    k = 0;
    // parsing texcoords
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {              
        /*obj->subObjects[i]->nTexcoords_h = lines_end[i] - lines_start[i] + 1;
        obj->subObjects[i]->pTexcoords = (TUVW*) malloc( sizeof(TUVW) * obj->subObjects[i]->nTexcoords_h );*/
        tmpSubMeshesTexcoords_h[i] = lines_end[i] - lines_start[i] + 1;
        tmpSubMeshesTexcoords[i] = (TUVW*) malloc( sizeof(TUVW) * tmpSubMeshesTexcoords_h[i] );
        _pOwner->getConsole().OLn("tmpSubMeshesTexcoords_h[%d] == %d", i, tmpSubMeshesTexcoords_h[i]);
        k = 0;
        for (TPRREuint j = lines_start[i]; j <= lines_end[i]; j++)
        {
            numparsed = sscanf((char*)lines[j], "%*s %f %f %f", &(tmpSubMeshesTexcoords[i][k].u), &(tmpSubMeshesTexcoords[i][k].v), &(tmpSubMeshesTexcoords[i][k].w));
            if ( numparsed != 3 )
            {
                _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ texcoords section: numparsed == %d", numparsed);
                _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
            }    
            k++;
        }
    }
    _pOwner->getConsole().OLnOO("texcoords parsed!");

    // finding starting and ending lines of normals for every submesh
    _pOwner->getConsole().OLnOI("parsing normals ...");
    sector = false;
    actsubmesh = 0;
    for (TPRREuint i = 0; i < lines_h; i++)
    {
        if ( (strstr((char*) lines[i], "vn") > 0) && (strstr((char*) lines[i], "#") == 0) )
        {
            if ( !sector )
            {
                sector = true;
                lines_start[actsubmesh] = i;
            }
        }
        else
        {
            if ( sector )
            {
                sector = false;
                lines_end[actsubmesh] = i-1;
                actsubmesh++;
            }
        }
    }

    // parsing normals
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {           
        /*obj->subObjects[i]->nNormals_h = lines_end[i] - lines_start[i] + 1;
        obj->subObjects[i]->pNormals = (TXYZ*) malloc( sizeof(TXYZ) * obj->subObjects[i]->nNormals_h );*/
        tmpSubMeshesNormals_h[i] = lines_end[i] - lines_start[i] + 1;
        tmpSubMeshesNormals[i] = (TXYZ*) malloc( sizeof(TXYZ) * tmpSubMeshesNormals_h[i] );
        _pOwner->getConsole().OLn("tmpSubMeshesNormals_h[%d] == %d", i, tmpSubMeshesNormals_h[i]);
        k = 0;
        for (TPRREuint j = lines_start[i]; j <= lines_end[i]; j++)
        {
            numparsed = sscanf((char*)lines[j], "%*s %f %f %f", &(tmpSubMeshesNormals[i][k].x), &(tmpSubMeshesNormals[i][k].y), &(tmpSubMeshesNormals[i][k].z));
            if ( numparsed != 3 )
            {
                _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ normals section: numparsed == %d", numparsed);
                _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
            }    
            k++;
        }
    }
    _pOwner->getConsole().OLnOO("normals parsed!");

    // finding starting and ending lines of faces for every submesh
    _pOwner->getConsole().OLnOI("parsing faces ...");
    sector = false;
    actsubmesh = 0;
    for (TPRREuint i = 0; i < lines_h; i++)
    {
        if ( (strstr((char*) lines[i], "f") > 0) && (strstr((char*) lines[i], "#") == 0) && ((strstr((char*) lines[i], "g ") == 0)) )
        {
            if ( !sector )
            {
                sector = true;
                lines_start[actsubmesh] = i;
            }
        }
        else
        {
            if ( sector )
            {
                sector = false;
                lines_end[actsubmesh] = i-1;
                actsubmesh++;
            }
        }
    }

    // we have to determine min and max vertex indices here, prior to allocating arrays
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {                    
        PRREObject3D* const currSubObj = (PRREObject3D*) (obj->getAttachedAt(i));
        k = 0;
        // todo: later this loop can be replaced by a simple assignment like: k = lines_end[i] - lines_start[i] +/-1 or sg like that
        for (TPRREuint j = lines_start[i]; j <= lines_end[i]; j++)
        {
            // there will be submesh-local indices in the end, all starting from nMinIndex = 0 to nMaxIndex = k*3+2
            if ( currSubObj->p->nMinIndex > k*3 )
                currSubObj->p->nMinIndex = k*3;
            if ( currSubObj->p->nMaxIndex < k*3+2 )
                currSubObj->p->nMaxIndex = k*3+2;
            k++;
        }
        // determining what type of values to be stored in the index arrays ...
        if ( currSubObj->p->nMaxIndex <= UCHAR_MAX )
        {
            currSubObj->p->nIndicesType = GL_UNSIGNED_BYTE;
            _pOwner->getConsole().OLn("submeshes_h[%d] nIndicesType = GL_UNSIGNED_BYTE", i);
        }
        else if ( currSubObj->p->nMaxIndex <= USHRT_MAX )
        {
            currSubObj->p->nIndicesType = GL_UNSIGNED_SHORT;
            _pOwner->getConsole().OLn("submeshes_h[%d] nIndicesType = GL_UNSIGNED_SHORT", i);
        }
        else
        {
            currSubObj->p->nIndicesType = GL_UNSIGNED_INT;
            _pOwner->getConsole().OLn("submeshes_h[%d] nIndicesType = GL_UNSIGNED_INT", i);
        }
        if ( currSubObj->p->nMinIndex != 0 )
        {
            _pOwner->getConsole().EOLn("ERROR: submeshes_h[%d] nMinIndex == %d", i, currSubObj->p->nMinIndex);
            _ASSERT( false );
        }
    }

    // parsing faces
    int prevvertcount = 1;
    int prevnormcount = 1;
    int prevtexcoordcount = 1;
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {                    
        PRREObject3D* const currSubObj = (PRREObject3D*) (obj->getAttachedAt(i));
        currSubObj->p->nFaces_h = lines_end[i] - lines_start[i] + 1;
        // currSubObj->p->pFaces = (TFACE4*) malloc( sizeof(TFACE4) * currSubObj->p->nFaces_h );
        currSubObj->p->nVertexIndices_h   = currSubObj->p->nFaces_h * 3;
        currSubObj->p->nNormalIndices_h   = currSubObj->p->nVertexIndices_h;
        currSubObj->p->pVertexIndices   = malloc( PRREGLsnippets::getSizeofIndexType(currSubObj->p->nIndicesType) * currSubObj->p->nVertexIndices_h );
        currSubObj->p->pNormalIndices   = malloc( PRREGLsnippets::getSizeofIndexType(currSubObj->p->nIndicesType) * currSubObj->p->nNormalIndices_h );
        currSubObj->p->nVertices_h  = currSubObj->p->nVertexIndices_h;
        currSubObj->p->nNormals_h   = currSubObj->p->nNormalIndices_h;
        currSubObj->p->pVertices        = (TXYZ*)  malloc( sizeof(TXYZ) * currSubObj->p->nVertices_h );
        currSubObj->p->pVerticesTransf  = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * currSubObj->p->nVertices_h );
        currSubObj->p->pNormals         = (TXYZ*)  malloc( sizeof(TXYZ) * currSubObj->p->nNormals_h );

        currSubObj->p->pMaterial->AllocateArrays(
            currSubObj->p->nVertices_h,
            currSubObj->p->nVertexIndices_h,
            currSubObj->p->nVertexIndices_h,
            PRREGLsnippets::getSizeofIndexType(currSubObj->p->nIndicesType) );

        _pOwner->getConsole().OLn("obj->subObjects[%d]->nFaces_h == %d", i, currSubObj->p->nFaces_h);
        k = 0;
        // parsing faces of a submesh
        for (TPRREuint j = lines_start[i]; j <= lines_end[i]; j++)
        {
            TPRREuint tmpParseVertexIndices[3];
            TPRREuint tmpParseTexcoordIndices[3];
            TPRREuint tmpParseNormalIndices[3];
            numparsed = sscanf((char*)lines[j], "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
                                &(tmpParseVertexIndices[0]),
                                &(tmpParseTexcoordIndices[0]),
                                &(tmpParseNormalIndices[0]),
                                &(tmpParseVertexIndices[1]),
                                &(tmpParseTexcoordIndices[1]),
                                &(tmpParseNormalIndices[1]),
                                &(tmpParseVertexIndices[2]),
                                &(tmpParseTexcoordIndices[2]),
                                &(tmpParseNormalIndices[2]));
            if ( numparsed != 9 )
            {
                _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ faces section: numparsed == %d", numparsed);
                _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
            }    

            // Putting parsed indices into index arrays.
            // We decrease the parsed index values by the previously accumulated index values because OBJ stores global indices but we need submesh-local indices.
            // SetIndexInArray() stores indices with minimal storage size, eg. 1 as byte, 500 as short, so passing submesh-local indices here is CRITICAL to avoid chopping values.
            /* Note: exchange k*3 and k*3+2 for CW-CCW change. This maybe a feature in the future. */
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,                  k*3+2, tmpParseVertexIndices[0] - prevvertcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3+2, tmpParseTexcoordIndices[0] - prevtexcoordcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pNormalIndices,                  k*3+2, tmpParseNormalIndices[0] - prevnormcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,                  k*3+1, tmpParseVertexIndices[1] - prevvertcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3+1, tmpParseTexcoordIndices[1] - prevtexcoordcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pNormalIndices,                  k*3+1, tmpParseNormalIndices[1] - prevnormcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,                  k*3,   tmpParseVertexIndices[2] - prevvertcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3,   tmpParseTexcoordIndices[2] - prevtexcoordcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pNormalIndices,                  k*3,   tmpParseNormalIndices[2] - prevnormcount);

            // Putting geometry data into arrays. We store geometry data redundantly.
            currSubObj->p->pVertices[k*3]    = tmpSubMeshesVertices[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pVertexIndices, k*3) ];
            currSubObj->p->pVertices[k*3+1]  = tmpSubMeshesVertices[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pVertexIndices, k*3+1) ];
            currSubObj->p->pVertices[k*3+2]  = tmpSubMeshesVertices[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pVertexIndices, k*3+2) ];
            // don't need to fill pVerticesTransf array here, that task is for during rendering!
            currSubObj->p->pMaterial->getTexcoords()[k*3]   = tmpSubMeshesTexcoords[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3) ];
            currSubObj->p->pMaterial->getTexcoords()[k*3+1] = tmpSubMeshesTexcoords[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3+1) ];
            currSubObj->p->pMaterial->getTexcoords()[k*3+2] = tmpSubMeshesTexcoords[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3+2) ];
            currSubObj->p->pNormals[k*3]     = tmpSubMeshesNormals[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pNormalIndices, k*3) ];
            currSubObj->p->pNormals[k*3+1]   = tmpSubMeshesNormals[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pNormalIndices, k*3+1) ];
            currSubObj->p->pNormals[k*3+2]   = tmpSubMeshesNormals[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pNormalIndices, k*3+2) ];
            
            // Update the index values because we now store geometry data redundantly.
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,   k*3,   k*3);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,   k*3+1, k*3+1);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,   k*3+2, k*3+2);
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3,   k*3);
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3+1, k*3+1);
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getTexcoordIndices(), k*3+2, k*3+2);
            currSubObj->p->SetIndexInArray(currSubObj->p->pNormalIndices,   k*3,   k*3);
            currSubObj->p->SetIndexInArray(currSubObj->p->pNormalIndices,   k*3+1, k*3+1);
            currSubObj->p->SetIndexInArray(currSubObj->p->pNormalIndices,   k*3+2, k*3+2);
            k++;
        } 
        prevvertcount += tmpSubMeshesVertices_h[i];
        prevnormcount += tmpSubMeshesNormals_h[i];
        prevtexcoordcount += tmpSubMeshesTexcoords_h[i];
    }
    _pOwner->getConsole().OLnOO("faces parsed!");

    _pOwner->getConsole().OLnOI("checking submodel names and legacy-loading textures implicitly by submodel names ...");
    TPRREuint isubmsh = 0;
    bool isname = true;
    for (TPRREuint i = 0; i < lines_h; i++)
    {
        if ( memchr(lines[i],'#', strlen((char*)lines[i])) == 0 )
        {
            if ( strchr((char*) lines[i],'g') > 0  )
            {
                if ( isname )
                {
                    char* const pSubModelName = lines[i] + 2;
                    char* const pPipePos = strstr(pSubModelName, "|");
                    PRREObject3D* const pSubObj = (PRREObject3D*)(obj->getAttachedAt(isubmsh));
                    if ( pPipePos == NULL )
                    {
                        pSubObj->SetName(pSubModelName);
                    }
                    else
                    {
                        const char* const pTexName = pPipePos + sizeof(char);
                        PRRETexture* const tex = textureMgr.createFromFile( PFL::getDirectory(filename).append(pTexName).c_str() );
                        pSubObj->getMaterial().SetTexture(tex);
                        // since we have loaded texture from submodelname, we can get rid of the texture filename part of it
                        *pPipePos = '\0';
                        pSubObj->SetName(pSubModelName); 
                    }
                    _pOwner->getConsole().OLn("subObjects[%d]->getName() = %s", isubmsh, pSubObj->getName().c_str());
                    isubmsh++;
                }
                isname = !isname;         
            }
        }        
    }
    _pOwner->getConsole().OLnOO("submodel names checked!");

    _pOwner->getConsole().OLnOI("submodel material settings ...");
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {
        PRREObject3D* const currSubObj = (PRREObject3D*) (obj->getAttachedAt(i));
        for (TPRREuint j = 0; j < currSubObj->p->pMaterial->getColorsCount(); j++)
        {
            currSubObj->p->pMaterial->getColors()[j].red = 1.0f;                                                         
            currSubObj->p->pMaterial->getColors()[j].green = 1.0f;
            currSubObj->p->pMaterial->getColors()[j].blue = 1.0f;
            currSubObj->p->pMaterial->getColors()[j].alpha = 1.0f;
        }
        for (TPRREuint j = 0; j < currSubObj->p->pMaterial->getColorIndicesCount(); j++)
            currSubObj->p->SetIndexInArray(currSubObj->p->pMaterial->getColorIndices(), j, 0);
    }
    _pOwner->getConsole().OLnOO("submodel material settings done!");

    _pOwner->getConsole().OLnOI("freeing up temporary buffers ...");
    for (TPRREuint i = 0; i < submeshes_h; i++)
    {
        free( tmpSubMeshesVertices[i] );
        free( tmpSubMeshesTexcoords[i] );
        free( tmpSubMeshesNormals[i] );
        tmpSubMeshesVertices[i] = NULL;
        tmpSubMeshesTexcoords[i] = NULL;
        tmpSubMeshesNormals[i] = NULL;
    }
    free(tmpSubMeshesVertices);
    free(tmpSubMeshesTexcoords);
    free(tmpSubMeshesNormals);
    tmpSubMeshesVertices = NULL;
    tmpSubMeshesTexcoords = NULL;
    tmpSubMeshesNormals = NULL;

    for (TPRREuint i = 0; i < lines_h; i++)
        free(lines[i]);
    free(lines_start);
    free(lines_end);
    free(lines);
    _pOwner->getConsole().OLnOO("done freeing up temporary buffers!");

    obj->RecalculateSize();

    _pOwner->getConsole().SOLnOO("> OBJ loaded!");

    return obj;
} // loadOBJ()


// ############################### PRIVATE ###############################


PRREObject3DManager::PRREObject3DManagerImpl::PRREObject3DManagerImpl() :
    textureMgr(textureMgr),
    materialMgr(materialMgr)
{

}


/**
    @param owner  Public class instance owning this pimpl object.
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
*/
PRREObject3DManager::PRREObject3DManagerImpl::PRREObject3DManagerImpl(PRREObject3DManager* owner, PRRETextureManager& texMgr, PRREMaterialManager& matMgr) :
    textureMgr(texMgr),
    materialMgr(matMgr)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PRREObject3DManager(console) ...");
    bInited = false;
    bMinimalIndexStorage = true;
    // we rely on texture manager initialized state since that also relies on initialized state of hwinfo
    bInited = texMgr.isInitialized();
    _pOwner->getConsole().SOLnOO("Done!");
} // PRREObject3DManager(...)


PRREObject3DManager::PRREObject3DManagerImpl::PRREObject3DManagerImpl(const PRREObject3DManager::PRREObject3DManagerImpl&) :
    textureMgr(textureMgr),
    materialMgr(materialMgr)
{

}


PRREObject3DManager::PRREObject3DManagerImpl& PRREObject3DManager::PRREObject3DManagerImpl::operator=(const PRREObject3DManager::PRREObject3DManagerImpl&)
{
    return *this;    
}



/*
   PRREObject3DManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Gets whether the given source and destination blend factors really mean blending or not.
    If source factor is PRRE_ONE and destination factor is PRRE_ZERO, there's no blending even if blending is enabled.

    @return True if the given blend factors really mean blending, false otherwise.
*/
TPRREbool PRREObject3DManager::isBlendFuncBlends(TPRRE_BLENDFACTORS sfactor, TPRRE_BLENDFACTORS dfactor)
{
    return !( (sfactor == PRRE_ONE) && (dfactor == PRRE_ZERO) );
} // isBlendFuncBlends()


/**
    Same as isBlendFuncBlends().
    @return True if the given blend factors really mean blending, false otherwise.
*/
TPRREbool PRREObject3DManager::isBFB(TPRRE_BLENDFACTORS sfactor, TPRRE_BLENDFACTORS dfactor)
{
    return isBlendFuncBlends(sfactor, dfactor);
} // isBFB()


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
TPRREbool PRREObject3DManager::isVertexTransferModeSelectable(TPRRE_VERTEX_TRANSFER_MODE vtrans)
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
TPRREbool PRREObject3DManager::isVertexReferencingIndexed(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return BIT_READ(vtrans, PRRE_VT_VREF_BIT) == 1u;
} // isVertexReferencingIndexed()


/**
    Tells whether the given Vertex Transfer Mode means dynamic modifying habit.
    @return True if the given mode means dynamic modifying habit.
*/
TPRREbool PRREObject3DManager::isVertexModifyingDynamic(TPRRE_VERTEX_TRANSFER_MODE vtrans)
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
TPRRE_VERTEX_TRANSFER_MODE PRREObject3DManager::selectVertexTransferMode(
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


/**
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
*/
PRREObject3DManager::PRREObject3DManager(PRRETextureManager& texMgr, PRREMaterialManager& matMgr)
{
    p = new PRREObject3DManagerImpl(this, texMgr, matMgr);
} // PRREObject3DManager(...)


PRREObject3DManager::~PRREObject3DManager()
{
    delete p;
    p = NULL;
} // ~PRREObject3DManager()


/**
    Tells whether the object is correctly initialized or not.
    @return True if the given TextureManager instance was initialized correctly, false otherwise.
*/
TPRREbool PRREObject3DManager::isInitialized() const
{
    return p->isInitialized();
} // isInitialized()


/**
    Tells whether storage of indices is minimalized or not.
    Minimalized storage of indices means that the software always tries to pack geometry indices into as small storage types as possible.
    This can happen in both system and video memory.
    If enabled, geometry indices generally take less memory and rendering may be faster.
    Enabled by default.
*/
TPRREbool PRREObject3DManager::isMinimalIndexStorageEnabled() const
{
    return p->isMinimalIndexStorageEnabled();
}


/**
    Sets whether storage of indices is minimalized or not.
    Minimalized storage of indices means that the software always tries to pack geometry indices into as small storage types as possible.
    This can happen in both system or video memory.
    If enabled, geometry indices generally take less memory and rendering may be faster.
    Enabled by default.

    Note: disabling is unimplemented currently!
*/
void PRREObject3DManager::SetMinimalIndexStorageEnabled(TPRREbool state)
{
    p->SetMinimalIndexStorageEnabled(state);
}


/**
    Creates a new plane with the given sizes.

    @param a                     Length of plane (size on X-axis).
    @param b                     Height of plane (size on Y-axis).
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
    
    @return New 3D object with 1 face and 4 vertices.
            PGENULL if Object3DManager is not yet initialized.
*/
PRREObject3D* PRREObject3DManager::createPlane(
    TPRREfloat a, TPRREfloat b,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory)
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createPlane(%f, %f)", a, b);

    PRREObject3D* const obj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    Attach( *obj );
    obj->p->vSize.Set(a, b, 0.0f);
    obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    obj->p->pMaterial = p->materialMgr.createMaterial();

    PRREObject3D* const subobject = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    obj->Attach(*subobject);
    subobject->p->vSize.Set(a, b, 0.0f);
    subobject->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    subobject->p->pMaterial = p->materialMgr.createMaterial();

    subobject->p->nVertices_h  = 4;
    subobject->p->nNormals_h   = subobject->p->nVertices_h;
    subobject->p->nFaces_h     = 1;

    subobject->p->nVertexIndices_h   = subobject->p->nVertices_h;
    subobject->p->nNormalIndices_h   = subobject->p->nVertexIndices_h;

    subobject->p->pVertices       = (TXYZ*)       malloc( sizeof(TXYZ)   * subobject->p->nVertices_h );
    subobject->p->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*)      malloc( sizeof(TPRRE_TRANSFORMED_VERTEX)   * subobject->p->nVertices_h );
    subobject->p->pNormals   = (TXYZ*)       malloc( sizeof(TXYZ)   * subobject->p->nNormals_h );
    //subobject->p->pFaces     = (TFACE4*)     malloc( sizeof(TFACE4) * subobject->p->nFaces_h );
    subobject->p->nIndicesType = GL_UNSIGNED_BYTE;
    subobject->p->pVertexIndices   = malloc( PRREGLsnippets::getSizeofIndexType(subobject->p->nIndicesType) * subobject->p->nVertexIndices_h );
    subobject->p->pNormalIndices   = malloc( PRREGLsnippets::getSizeofIndexType(subobject->p->nIndicesType) * subobject->p->nNormalIndices_h );

    subobject->p->pMaterial->AllocateArrays(
        subobject->p->nVertices_h,
        subobject->p->nVertices_h,
        subobject->p->nVertexIndices_h,
        PRREGLsnippets::getSizeofIndexType(subobject->p->nIndicesType) );

    subobject->p->pVertices[0].x  = -a / 2.0f;
    subobject->p->pVertices[0].y  = -b / 2.0f;
    subobject->p->pVertices[0].z  = 0.0f;
    // no need to fill pVerticesTransf array, that task is for during rendering
    subobject->p->pNormals[0].x   = 0.0f;
    subobject->p->pNormals[0].y   = 0.0f;
    subobject->p->pNormals[0].z   = -1.0f;
    subobject->p->pMaterial->getColors()[0].red    = 1.0f;
    subobject->p->pMaterial->getColors()[0].green  = 1.0f;
    subobject->p->pMaterial->getColors()[0].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[0].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[0].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[0].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[0].w = 0.0f;

    subobject->p->pVertices[1].x  = a / 2.0f;
    subobject->p->pVertices[1].y  = -b / 2.0f;
    subobject->p->pVertices[1].z  = 0.0f;
    subobject->p->pNormals[1].x   = 0.0f;
    subobject->p->pNormals[1].y   = 0.0f;
    subobject->p->pNormals[1].z   = -1.0f;
    subobject->p->pMaterial->getColors()[1].red    = 1.0f;
    subobject->p->pMaterial->getColors()[1].green  = 1.0f;
    subobject->p->pMaterial->getColors()[1].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[1].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[1].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[1].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[1].w = 0.0f;

    subobject->p->pVertices[2].x  = a / 2.0f;
    subobject->p->pVertices[2].y  = b / 2.0f;
    subobject->p->pVertices[2].z  = 0.0f;
    subobject->p->pNormals[2].x   = 0.0f;
    subobject->p->pNormals[2].y   = 0.0f;
    subobject->p->pNormals[2].z   = -1.0f;
    subobject->p->pMaterial->getColors()[2].red    = 1.0f;
    subobject->p->pMaterial->getColors()[2].green  = 1.0f;
    subobject->p->pMaterial->getColors()[2].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[2].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[2].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[2].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[2].w = 0.0f;

    subobject->p->pVertices[3].x  = -a / 2.0f;
    subobject->p->pVertices[3].y  = b / 2.0f;
    subobject->p->pVertices[3].z  = 0.0f;
    subobject->p->pNormals[3].x   = 0.0f;
    subobject->p->pNormals[3].y   = 0.0f;
    subobject->p->pNormals[3].z   = -1.0f;
    subobject->p->pMaterial->getColors()[3].red    = 1.0f;
    subobject->p->pMaterial->getColors()[3].green  = 1.0f;
    subobject->p->pMaterial->getColors()[3].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[3].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[3].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[3].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[3].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[0].vertex_ids[i] = i;
        subobject->p->pFaces[0].texcoord_ids[i] = i;
        subobject->p->pFaces[0].normal_ids[i] = i;
    }*/

    for (TPRREuint i = 0; i < subobject->p->nVertexIndices_h; i++)
    {
        subobject->p->SetIndexInArray(subobject->p->pVertexIndices,   i, i);
        subobject->p->SetIndexInArray(subobject->p->pMaterial->getColorIndices(),    i, i);
        subobject->p->SetIndexInArray(subobject->p->pNormalIndices,   i, i);
        subobject->p->SetIndexInArray(subobject->p->pMaterial->getTexcoordIndices(), i, i);
        if ( subobject->p->nMinIndex > i )
            subobject->p->nMinIndex = i;
        if ( subobject->p->nMaxIndex < i )
            subobject->p->nMaxIndex = i;
    }

    // although PPP has already selected the vtransmode, we set it again
    // to actually allocate the needed resources for the geometry
    getConsole().OI();
    obj->SetVertexTransferMode( obj->getVertexTransferMode() );
    getConsole().OO();

    getConsole().SOLnOO("> Plane created successfully!");
    getConsole().OLn("");

    return obj;

} // createPlane()


/**
    Creates a new box with the given sizes.
    This box consists of quads instead of triangles and has 24 vertices.
    Every vertex has a normal, a color, and texture uvw-coordinates in separate arrays.
    All faces are defined counter-clockwise starting from the lower left vertex, thus
    the order of texture uvw-coordinates are the same for each face.
    Order of faces is: front face towards the viewer, back, left, right, top, bottom.

    @param a                     Length of box (size on X-axis).
    @param b                     Height of box (size on Y-axis).
    @param c                     Depth of box (size on Z-axis).
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return New 3D box with 6 faces and 24 vertices.
            PGENULL if Object3DManager is not yet initialized.
*/
PRREObject3D* PRREObject3DManager::createBox(
    TPRREfloat a, TPRREfloat b, TPRREfloat c,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createBox(%f, %f, %f)", a, b, c);

    PRREObject3D* const obj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    Attach( *obj );
    obj->p->vSize.Set(a, b, c);
    obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    obj->p->pMaterial = p->materialMgr.createMaterial();

    PRREObject3D* const subobject = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    obj->Attach(*subobject);
    subobject->p->vSize.Set(a, b, c);
    subobject->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    subobject->p->pMaterial = p->materialMgr.createMaterial();

    subobject->p->nVertices_h  = 24;
    subobject->p->nNormals_h   = subobject->p->nVertices_h;
    subobject->p->nFaces_h     = 6;

    subobject->p->nVertexIndices_h   = subobject->p->nVertices_h;
    subobject->p->nNormalIndices_h   = subobject->p->nVertexIndices_h;

    subobject->p->pVertices       = (TXYZ*)  malloc( sizeof(TXYZ)       * subobject->p->nVertices_h );
    subobject->p->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX)       * subobject->p->nVertices_h );
    subobject->p->pNormals        = (TXYZ*)  malloc( sizeof(TXYZ)       * subobject->p->nNormals_h );
    //subobject->p->pFaces     = (TFACE4*)     malloc( sizeof(TFACE4)     * subobject->p->nFaces_h );
    subobject->p->nIndicesType = GL_UNSIGNED_BYTE;
    subobject->p->pVertexIndices   = malloc( PRREGLsnippets::getSizeofIndexType(subobject->p->nIndicesType) * subobject->p->nVertexIndices_h );
    subobject->p->pNormalIndices   = malloc( PRREGLsnippets::getSizeofIndexType(subobject->p->nIndicesType) * subobject->p->nNormalIndices_h );

    subobject->p->pMaterial->AllocateArrays(
        subobject->p->nVertices_h,
        subobject->p->nVertices_h,
        subobject->p->nVertexIndices_h,
        PRREGLsnippets::getSizeofIndexType(subobject->p->nIndicesType) );

    // front face
    subobject->p->pVertices[0].x  = -a / 2.0f;
    subobject->p->pVertices[0].y  = -b / 2.0f;
    subobject->p->pVertices[0].z  = -c / 2.0f;
    subobject->p->pNormals[0].x   = 0.0f;
    subobject->p->pNormals[0].y   = 0.0f;
    subobject->p->pNormals[0].z   = -1.0f;
    subobject->p->pMaterial->getColors()[0].red    = 1.0f;
    subobject->p->pMaterial->getColors()[0].green  = 1.0f;
    subobject->p->pMaterial->getColors()[0].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[0].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[0].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[0].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[0].w = 0.0f;

    subobject->p->pVertices[1].x  = a / 2.0f;
    subobject->p->pVertices[1].y  = -b / 2.0f;
    subobject->p->pVertices[1].z  = -c / 2.0f;
    subobject->p->pNormals[1].x   = 0.0f;
    subobject->p->pNormals[1].y   = 0.0f;
    subobject->p->pNormals[1].z   = -1.0f;
    subobject->p->pMaterial->getColors()[1].red    = 1.0f;
    subobject->p->pMaterial->getColors()[1].green  = 1.0f;
    subobject->p->pMaterial->getColors()[1].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[1].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[1].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[1].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[1].w = 0.0f;

    subobject->p->pVertices[2].x  = a / 2.0f;
    subobject->p->pVertices[2].y  = b / 2.0f;
    subobject->p->pVertices[2].z  = -c / 2.0f;
    subobject->p->pNormals[2].x   = 0.0f;
    subobject->p->pNormals[2].y   = 0.0f;
    subobject->p->pNormals[2].z   = -1.0f;
    subobject->p->pMaterial->getColors()[2].red    = 1.0f;
    subobject->p->pMaterial->getColors()[2].green  = 1.0f;
    subobject->p->pMaterial->getColors()[2].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[2].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[2].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[2].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[2].w = 0.0f;

    subobject->p->pVertices[3].x  = -a / 2.0f;
    subobject->p->pVertices[3].y  = b / 2.0f;
    subobject->p->pVertices[3].z  = -c / 2.0f;
    subobject->p->pNormals[3].x   = 0.0f;
    subobject->p->pNormals[3].y   = 0.0f;
    subobject->p->pNormals[3].z   = -1.0f;
    subobject->p->pMaterial->getColors()[3].red    = 1.0f;
    subobject->p->pMaterial->getColors()[3].green  = 1.0f;
    subobject->p->pMaterial->getColors()[3].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[3].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[3].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[3].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[3].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[0].vertex_ids[i] = i;
        subobject->p->pFaces[0].texcoord_ids[i] = i;
        subobject->p->pFaces[0].normal_ids[i] = i;
    }*/

    // back face
    subobject->p->pVertices[4].x  = a / 2.0f;
    subobject->p->pVertices[4].y  = -b / 2.0f;
    subobject->p->pVertices[4].z  = c / 2.0f;
    subobject->p->pNormals[4].x   = 0.0f;
    subobject->p->pNormals[4].y   = 0.0f;
    subobject->p->pNormals[4].z   = 1.0f;
    subobject->p->pMaterial->getColors()[4].red    = 1.0f;
    subobject->p->pMaterial->getColors()[4].green  = 1.0f;
    subobject->p->pMaterial->getColors()[4].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[4].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[4].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[4].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[4].w = 0.0f;

    subobject->p->pVertices[5].x  = -a / 2.0f;
    subobject->p->pVertices[5].y  = -b / 2.0f;
    subobject->p->pVertices[5].z  = c / 2.0f;
    subobject->p->pNormals[5].x   = 0.0f;
    subobject->p->pNormals[5].y   = 0.0f;
    subobject->p->pNormals[5].z   = 1.0f;
    subobject->p->pMaterial->getColors()[5].red    = 1.0f;
    subobject->p->pMaterial->getColors()[5].green  = 1.0f;
    subobject->p->pMaterial->getColors()[5].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[5].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[5].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[5].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[5].w = 0.0f;

    subobject->p->pVertices[6].x  = -a / 2.0f;
    subobject->p->pVertices[6].y  = b / 2.0f;
    subobject->p->pVertices[6].z  = c / 2.0f;
    subobject->p->pNormals[6].x   = 0.0f;
    subobject->p->pNormals[6].y   = 0.0f;
    subobject->p->pNormals[6].z   = 1.0f;
    subobject->p->pMaterial->getColors()[6].red    = 1.0f;
    subobject->p->pMaterial->getColors()[6].green  = 1.0f;
    subobject->p->pMaterial->getColors()[6].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[6].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[6].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[6].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[6].w = 0.0f;

    subobject->p->pVertices[7].x  = a / 2.0f;
    subobject->p->pVertices[7].y  = b / 2.0f;
    subobject->p->pVertices[7].z  = c / 2.0f;
    subobject->p->pNormals[7].x   = 0.0f;
    subobject->p->pNormals[7].y   = 0.0f;
    subobject->p->pNormals[7].z   = 1.0f;
    subobject->p->pMaterial->getColors()[7].red    = 1.0f;
    subobject->p->pMaterial->getColors()[7].green  = 1.0f;
    subobject->p->pMaterial->getColors()[7].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[7].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[7].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[7].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[7].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[1].vertex_ids[i] = 4+i;
        subobject->p->pFaces[1].texcoord_ids[i] = 4+i;
        subobject->p->pFaces[1].normal_ids[i] = 4+i;
    }*/

    // left face
    subobject->p->pVertices[8].x  = -a / 2.0f;
    subobject->p->pVertices[8].y  = -b / 2.0f;
    subobject->p->pVertices[8].z  = c / 2.0f;
    subobject->p->pNormals[8].x   = -1.0f;
    subobject->p->pNormals[8].y   = 0.0f;
    subobject->p->pNormals[8].z   = 0.0f;
    subobject->p->pMaterial->getColors()[8].red    = 1.0f;
    subobject->p->pMaterial->getColors()[8].green  = 1.0f;
    subobject->p->pMaterial->getColors()[8].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[8].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[8].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[8].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[8].w = 0.0f;

    subobject->p->pVertices[9].x  = -a / 2.0f;
    subobject->p->pVertices[9].y  = -b / 2.0f;
    subobject->p->pVertices[9].z  = -c / 2.0f;
    subobject->p->pNormals[9].x   = -1.0f;
    subobject->p->pNormals[9].y   = 0.0f;
    subobject->p->pNormals[9].z   = 0.0f;
    subobject->p->pMaterial->getColors()[9].red    = 1.0f;
    subobject->p->pMaterial->getColors()[9].green  = 1.0f;
    subobject->p->pMaterial->getColors()[9].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[9].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[9].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[9].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[9].w = 0.0f;

    subobject->p->pVertices[10].x  = -a / 2.0f;
    subobject->p->pVertices[10].y  = b / 2.0f;
    subobject->p->pVertices[10].z  = -c / 2.0f;
    subobject->p->pNormals[10].x   = -1.0f;
    subobject->p->pNormals[10].y   = 0.0f;
    subobject->p->pNormals[10].z   = 0.0f;
    subobject->p->pMaterial->getColors()[10].red    = 1.0f;
    subobject->p->pMaterial->getColors()[10].green  = 1.0f;
    subobject->p->pMaterial->getColors()[10].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[10].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[10].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[10].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[10].w = 0.0f;

    subobject->p->pVertices[11].x  = -a / 2.0f;
    subobject->p->pVertices[11].y  = b / 2.0f;
    subobject->p->pVertices[11].z  = c / 2.0f;
    subobject->p->pNormals[11].x   = -1.0f;
    subobject->p->pNormals[11].y   = 0.0f;
    subobject->p->pNormals[11].z   = 0.0f;
    subobject->p->pMaterial->getColors()[11].red    = 1.0f;
    subobject->p->pMaterial->getColors()[11].green  = 1.0f;
    subobject->p->pMaterial->getColors()[11].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[11].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[11].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[11].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[11].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[2].vertex_ids[i] = 8+i;
        subobject->p->pFaces[2].texcoord_ids[i] = 8+i;
        subobject->p->pFaces[2].normal_ids[i] = 8+i;
    }*/

    // right face
    subobject->p->pVertices[12].x  = a / 2.0f;
    subobject->p->pVertices[12].y  = -b / 2.0f;
    subobject->p->pVertices[12].z  = -c / 2.0f;
    subobject->p->pNormals[12].x   = 1.0f;
    subobject->p->pNormals[12].y   = 0.0f;
    subobject->p->pNormals[12].z   = 0.0f;
    subobject->p->pMaterial->getColors()[12].red    = 1.0f;
    subobject->p->pMaterial->getColors()[12].green  = 1.0f;
    subobject->p->pMaterial->getColors()[12].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[12].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[12].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[12].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[12].w = 0.0f;

    subobject->p->pVertices[13].x  = a / 2.0f;
    subobject->p->pVertices[13].y  = -b / 2.0f;
    subobject->p->pVertices[13].z  = c / 2.0f;
    subobject->p->pNormals[13].x   = 1.0f;
    subobject->p->pNormals[13].y   = 0.0f;
    subobject->p->pNormals[13].z   = 0.0f;
    subobject->p->pMaterial->getColors()[13].red    = 1.0f;
    subobject->p->pMaterial->getColors()[13].green  = 1.0f;
    subobject->p->pMaterial->getColors()[13].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[13].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[13].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[13].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[13].w = 0.0f;

    subobject->p->pVertices[14].x  = a / 2.0f;
    subobject->p->pVertices[14].y  = b / 2.0f;
    subobject->p->pVertices[14].z  = c / 2.0f;
    subobject->p->pNormals[14].x   = 1.0f;
    subobject->p->pNormals[14].y   = 0.0f;
    subobject->p->pNormals[14].z   = 0.0f;
    subobject->p->pMaterial->getColors()[14].red    = 1.0f;
    subobject->p->pMaterial->getColors()[14].green  = 1.0f;
    subobject->p->pMaterial->getColors()[14].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[14].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[14].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[14].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[14].w = 0.0f;

    subobject->p->pVertices[15].x  = a / 2.0f;
    subobject->p->pVertices[15].y  = b / 2.0f;
    subobject->p->pVertices[15].z  = -c / 2.0f;
    subobject->p->pNormals[15].x   = 1.0f;
    subobject->p->pNormals[15].y   = 0.0f;
    subobject->p->pNormals[15].z   = 0.0f;
    subobject->p->pMaterial->getColors()[15].red    = 1.0f;
    subobject->p->pMaterial->getColors()[15].green  = 1.0f;
    subobject->p->pMaterial->getColors()[15].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[15].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[15].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[15].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[15].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[3].vertex_ids[i] = 12+i;
        subobject->p->pFaces[3].texcoord_ids[i] = 12+i;
        subobject->p->pFaces[3].normal_ids[i] = 12+i;
    }*/

    // top face
    subobject->p->pVertices[16].x  = -a / 2.0f;
    subobject->p->pVertices[16].y  = b / 2.0f;
    subobject->p->pVertices[16].z  = -c / 2.0f;
    subobject->p->pNormals[16].x   = 0.0f;
    subobject->p->pNormals[16].y   = 1.0f;
    subobject->p->pNormals[16].z   = 0.0f;
    subobject->p->pMaterial->getColors()[16].red    = 1.0f;
    subobject->p->pMaterial->getColors()[16].green  = 1.0f;
    subobject->p->pMaterial->getColors()[16].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[16].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[16].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[16].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[16].w = 0.0f;

    subobject->p->pVertices[17].x  = a / 2.0f;
    subobject->p->pVertices[17].y  = b / 2.0f;
    subobject->p->pVertices[17].z  = -c / 2.0f;
    subobject->p->pNormals[17].x   = 0.0f;
    subobject->p->pNormals[17].y   = 1.0f;
    subobject->p->pNormals[17].z   = 0.0f;
    subobject->p->pMaterial->getColors()[17].red    = 1.0f;
    subobject->p->pMaterial->getColors()[17].green  = 1.0f;
    subobject->p->pMaterial->getColors()[17].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[17].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[17].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[17].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[17].w = 0.0f;

    subobject->p->pVertices[18].x  = a / 2.0f;
    subobject->p->pVertices[18].y  = b / 2.0f;
    subobject->p->pVertices[18].z  = c / 2.0f;
    subobject->p->pNormals[18].x   = 0.0f;
    subobject->p->pNormals[18].y   = 1.0f;
    subobject->p->pNormals[18].z   = 0.0f;
    subobject->p->pMaterial->getColors()[18].red    = 1.0f;
    subobject->p->pMaterial->getColors()[18].green  = 1.0f;
    subobject->p->pMaterial->getColors()[18].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[18].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[18].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[18].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[18].w = 0.0f;

    subobject->p->pVertices[19].x  = -a / 2.0f;
    subobject->p->pVertices[19].y  = b / 2.0f;
    subobject->p->pVertices[19].z  = c / 2.0f;
    subobject->p->pNormals[19].x   = 0.0f;
    subobject->p->pNormals[19].y   = 1.0f;
    subobject->p->pNormals[19].z   = 0.0f;
    subobject->p->pMaterial->getColors()[19].red    = 1.0f;
    subobject->p->pMaterial->getColors()[19].green  = 1.0f;
    subobject->p->pMaterial->getColors()[19].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[19].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[19].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[19].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[19].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[4].vertex_ids[i] = 16+i;
        subobject->p->pFaces[4].texcoord_ids[i] = 16+i;
        subobject->p->pFaces[4].normal_ids[i] = 16+i;
    }*/

    // bottom face
    subobject->p->pVertices[20].x  = a / 2.0f;
    subobject->p->pVertices[20].y  = -b / 2.0f;
    subobject->p->pVertices[20].z  = -c / 2.0f;
    subobject->p->pNormals[20].x   = 0.0f;
    subobject->p->pNormals[20].y   = -1.0f;
    subobject->p->pNormals[20].z   = 0.0f;
    subobject->p->pMaterial->getColors()[20].red    = 1.0f;
    subobject->p->pMaterial->getColors()[20].green  = 1.0f;
    subobject->p->pMaterial->getColors()[20].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[20].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[20].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[20].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[20].w = 0.0f;

    subobject->p->pVertices[21].x  = -a / 2.0f;
    subobject->p->pVertices[21].y  = -b / 2.0f;
    subobject->p->pVertices[21].z  = -c / 2.0f;
    subobject->p->pNormals[21].x   = 0.0f;
    subobject->p->pNormals[21].y   = -1.0f;
    subobject->p->pNormals[21].z   = 0.0f;
    subobject->p->pMaterial->getColors()[21].red    = 1.0f;
    subobject->p->pMaterial->getColors()[21].green  = 1.0f;
    subobject->p->pMaterial->getColors()[21].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[21].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[21].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[21].v = 0.0f;
    subobject->p->pMaterial->getTexcoords()[21].w = 0.0f;

    subobject->p->pVertices[22].x  = -a / 2.0f;
    subobject->p->pVertices[22].y  = -b / 2.0f;
    subobject->p->pVertices[22].z  = c / 2.0f;
    subobject->p->pNormals[22].x   = 0.0f;
    subobject->p->pNormals[22].y   = -1.0f;
    subobject->p->pNormals[22].z   = 0.0f;
    subobject->p->pMaterial->getColors()[22].red    = 1.0f;
    subobject->p->pMaterial->getColors()[22].green  = 1.0f;
    subobject->p->pMaterial->getColors()[22].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[22].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[22].u = 1.0f;
    subobject->p->pMaterial->getTexcoords()[22].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[22].w = 0.0f;

    subobject->p->pVertices[23].x  = a / 2.0f;
    subobject->p->pVertices[23].y  = -b / 2.0f;
    subobject->p->pVertices[23].z  = c / 2.0f;
    subobject->p->pNormals[23].x   = 0.0f;
    subobject->p->pNormals[23].y   = -1.0f;
    subobject->p->pNormals[23].z   = 0.0f;
    subobject->p->pMaterial->getColors()[23].red    = 1.0f;
    subobject->p->pMaterial->getColors()[23].green  = 1.0f;
    subobject->p->pMaterial->getColors()[23].blue   = 1.0f;
    subobject->p->pMaterial->getColors()[23].alpha  = 1.0f;
    subobject->p->pMaterial->getTexcoords()[23].u = 0.0f;
    subobject->p->pMaterial->getTexcoords()[23].v = 1.0f;
    subobject->p->pMaterial->getTexcoords()[23].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        subobject->p->pFaces[5].vertex_ids[i] = 20+i;
        subobject->p->pFaces[5].texcoord_ids[i] = 20+i;
        subobject->p->pFaces[5].normal_ids[i] = 20+i;
    }*/

    for (TPRREuint i = 0; i < subobject->p->nVertexIndices_h; i++)
    {
        subobject->p->SetIndexInArray(subobject->p->pVertexIndices,   i, i);
        subobject->p->SetIndexInArray(subobject->p->pMaterial->getColorIndices(),    i, i);
        subobject->p->SetIndexInArray(subobject->p->pNormalIndices,   i, i);
        subobject->p->SetIndexInArray(subobject->p->pMaterial->getTexcoordIndices(), i, i);
        if ( subobject->p->nMinIndex > i )
            subobject->p->nMinIndex = i;
        if ( subobject->p->nMaxIndex < i )
            subobject->p->nMaxIndex = i;
    }

    // although PPP has already selected the vtransmode, we set it again
    // to actually allocate the needed resources for the geometry
    getConsole().OI();
    obj->SetVertexTransferMode( obj->getVertexTransferMode() );
    getConsole().OO();

    getConsole().SOLnOO("> Box created successfully!");
    getConsole().OLn("");

    return obj;

} // createBox()


/**
    Creates a new cube with the given sizes.
    Equivalent to createBox(a, a, a).

    @param a                     Length of edges.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return New 3D cube with 6 faces and 24 vertices.
            PGENULL if Object3DManager is not yet initialized.
*/
PRREObject3D* PRREObject3DManager::createCube(
    TPRREfloat a,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    getConsole().OLnOI("PRREObject3DManager::createCube(%f), passing to createBox() ...", a);
    PRREObject3D* const cube = createBox(a, a, a, vmod, vref, bForceUseClientMemory);
    getConsole().SOLnOO("> Cube created successfully!");
    getConsole().OLn("");
    return cube;
} // createCube()


/**
    Creates object from the given file.

    @param filename              The model file to be loaded to be an Object3D instance.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return The created object.
            PGENULL if Object3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PRREObject3D* PRREObject3DManager::createFromFile(
    const char* filename,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createFromFile(\"%s\")", filename);

    if ( filename == NULL )
    {
        getConsole().EOLnOO("ERROR: input was NULL, returning PGENULL");
        getConsole().OLn("");
        return PGENULL;
    }

    PRREObject3D* obj = PGENULL;

    string sFileExt = PFL::getExtension(filename);
    if ( sFileExt == "" )
    {                                              
        getConsole().EOLnOO("ERROR: no file extension!");
        getConsole().OLn("");
        return PGENULL;    
    }
    if ( !PFL::fileExists(filename) )
    {
        getConsole().EOLnOO("ERROR: file doesn't exist!");
        getConsole().OLn("");
        return PGENULL;
    }

    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    getConsole().OLn("ext: .%s", sFileExt.c_str());
    if ( sFileExt == "OBJ" )
    {
        getConsole().OI();
        obj = p->loadOBJ(filename, vmod, vref, bForceUseClientMemory);
        obj->SetFilename(filename);
        getConsole().OO();
        Attach( *obj );

        // although PPP has already selected the vtransmode, we set it again
        // to actually allocate the needed resources for the geometry
        getConsole().OI();
        obj->SetVertexTransferMode( obj->getVertexTransferMode() );
        getConsole().OO();
    }    
    else
    {
        getConsole().EOLnOO("ERROR: unsupported extension!");
        getConsole().OLn("");
        return PGENULL;
    }

    getConsole().SOLnOO("> Object loaded successfully!");
    getConsole().OLn("");
    return obj; 
} // createFromFile()


/**
    Creates object from the given file.
    Equals to calling createFromFile(filename, PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, false).

    @param filename The model file to be loaded to be an Object3D instance.

    @return The created object.
            PGENULL if Object3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PRREObject3D* PRREObject3DManager::createFromFile(const char* filename)
{
    return createFromFile(filename, PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, false);    
} // createFromFile()


/**
    Creates a new object by cloning an already existing object.
    The clone will have its own properties such as position, visibility, etc. but the geometry will not be cloned.
    Useful for saving memory on geometry data.

    @return A clone instance.
            PGENULL if Object3DManager is not yet initialized.
*/
PRREObject3D* PRREObject3DManager::createCloned(PRREObject3D& referredobj)
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI(
        "PRREObject3DManager::createCloned(\"%s\")",
        (referredobj.getFilename().length() > 0) ? referredobj.getFilename().c_str() : referredobj.getName().c_str() );

    PRREObject3D* obj = new PRREObject3D();
    Attach( *obj );
    obj->p->pRefersto = &referredobj;
    obj->p->vPos = referredobj.getPosVec();
    obj->p->vAngle = referredobj.getAngleVec();
    obj->p->vSize = referredobj.getSizeVec();
    obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    
    // we create just a dummy material for this object, no need to copy texcoords, etc ...
    // however we need to set blendfunc and envcolor, because renderer would not get referredobj blendfunc ...
    // probably this should be treated as bug of renderer, however I fix it from objectmanager side here.
    obj->p->pMaterial = p->materialMgr.createMaterial();
    obj->p->pMaterial->SetBlendFuncs( referredobj.getMaterial().getSourceBlendFunc(), referredobj.getMaterial().getDestinationBlendFunc() );
    obj->p->pMaterial->getTextureEnvColor().Set(
        referredobj.getMaterial().getTextureEnvColor().getRed(),
        referredobj.getMaterial().getTextureEnvColor().getGreen(),
        referredobj.getMaterial().getTextureEnvColor().getBlue(),
        referredobj.getMaterial().getTextureEnvColor().getAlpha() );

    obj->p->bAffectedByLights = referredobj.isLit();
    obj->p->bDoubleSided = referredobj.isDoubleSided();
    obj->p->bWireframe = referredobj.isWireframed();
    obj->p->bWireframedCull = referredobj.isWireframedCulled();
    obj->p->bAffectZBuffer = referredobj.isAffectingZBuffer();
    obj->p->vScaling = referredobj.getScaling();
    obj->p->bVisible = referredobj.isVisible();
    obj->p->rotation = referredobj.getRotationOrder();
    obj->p->primitiveFormat = referredobj.getPrimitiveFormat();
    obj->p->bStickedToScreen = referredobj.isStickedToScreen();
    obj->p->vertexTransferMode = referredobj.getVertexTransferMode();
    obj->SetFilename( referredobj.getFilename() );
    obj->SetName( referredobj.getName() );

    getConsole().SOLnOO("> Object cloned successfully!");
    getConsole().OLn("");
    return obj;
}


// ############################## PROTECTED ##############################


PRREObject3DManager::PRREObject3DManager()
{

}


PRREObject3DManager::PRREObject3DManager(const PRREObject3DManager&)
{

}


PRREObject3DManager& PRREObject3DManager::operator=(const PRREObject3DManager&)
{
    return *this;    
}


// ############################### PRIVATE ###############################


