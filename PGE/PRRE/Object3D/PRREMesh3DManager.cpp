/*
    ###################################################################################
    PRREMesh3DManager.cpp
    This file is part of PRRE.
    PRREMesh3DManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "PRREMesh3DManager.h"
#include "PRREMesh3DImpl.h"
#include "../PRREGLextensionFuncs.h"
#include "../PRREGLsnippets.h"
#include "../Hardware/PRREhwInfo.h"

using namespace std;

/*
   PRREMesh3DManager::PRREMesh3DManagerImpl
   ###########################################################################
*/


class PRREMesh3DManager::PRREMesh3DManagerImpl
{

public:
    virtual ~PRREMesh3DManagerImpl();

    TPRREbool isInitialized() const;

    TPRREbool isMinimalIndexStorageEnabled() const;
    void      SetMinimalIndexStorageEnabled(TPRREbool state);

protected:
    PRREMesh3D* loadOBJ(const char* filename);   /**< Loads OBJ files, handles the actual file operations, and creates the Object3D object. */

private:

    PRREMesh3DManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

    PRREMaterialManager& materialMgr;           /**< Used to auto-load/create materials for objects. */

    TPRREbool            bInited;               /**< True if successfully inited, false if not functional. */
    TPRREbool            bMinimalIndexStorage;  /**< True if storage of indices is minimalized. */

    // ---------------------------------------------------------------------------

    PRREMesh3DManagerImpl();
    PRREMesh3DManagerImpl(PRREMesh3DManager* owner, PRREMaterialManager& matMgr);

    PRREMesh3DManagerImpl(const PRREMesh3DManagerImpl&);
    PRREMesh3DManagerImpl& operator=(const PRREMesh3DManagerImpl&);

    friend class PRREMesh3DManager;

};


// ############################### PUBLIC ################################


PRREMesh3DManager::PRREMesh3DManagerImpl::~PRREMesh3DManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRREMesh3DManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PRREMesh3DManager()


TPRREbool PRREMesh3DManager::PRREMesh3DManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPRREbool PRREMesh3DManager::PRREMesh3DManagerImpl::isMinimalIndexStorageEnabled() const
{
    /* currently not used, but it is in the todo list history txt */
    return bMinimalIndexStorage;
}


void PRREMesh3DManager::PRREMesh3DManagerImpl::SetMinimalIndexStorageEnabled(TPRREbool state)
{
    /* currently not used, but it is in the todo list history txt */
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
PRREMesh3D* PRREMesh3DManager::PRREMesh3DManagerImpl::loadOBJ(const char* filename)
{
    const HANDLE objfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

    PRREMesh3D* obj = NULL;
    DWORD     bytesread = 0;
    TPRREuint lines_h = 0;
    DWORD     filebuffer_size = 0;
    DWORD     nLastError;

    _pOwner->getConsole().OLnOI("PRREMesh3DManager::Load_OBJ(""%s"")", filename);
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
    obj = new PRREMesh3D(PRRE_PM_TRIANGLES);
    _pOwner->CreateMaterialForMesh(*obj);
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
        PRREMesh3D* subobj = new PRREMesh3D(PRRE_PM_TRIANGLES);
        _pOwner->CreateMaterialForMesh(*subobj);
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
        PRREMesh3D* const currSubObj = (PRREMesh3D*) (obj->getAttachedAt(i));
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
        PRREMesh3D* const currSubObj = (PRREMesh3D*) (obj->getAttachedAt(i));
        currSubObj->p->nFaces_h = lines_end[i] - lines_start[i] + 1;
        // currSubObj->p->pFaces = (TFACE4*) malloc( sizeof(TFACE4) * currSubObj->p->nFaces_h );
        currSubObj->p->nVertexIndices_h   = currSubObj->p->nFaces_h * 3;
        currSubObj->p->pVertexIndices   = malloc( PRREGLsnippets::getSizeofIndexType(currSubObj->p->nIndicesType) * currSubObj->p->nVertexIndices_h );
        currSubObj->p->nVertices_h  = currSubObj->p->nVertexIndices_h;
        currSubObj->p->pVertices        = (TXYZ*)  malloc( sizeof(TXYZ) * currSubObj->p->nVertices_h );
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
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,                  k*3+1, tmpParseVertexIndices[1] - prevvertcount);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,                  k*3,   tmpParseVertexIndices[2] - prevvertcount);

            // Putting geometry data into arrays. We store geometry data redundantly.
            currSubObj->p->pVertices[k*3]    = tmpSubMeshesVertices[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pVertexIndices, k*3) ];
            currSubObj->p->pVertices[k*3+1]  = tmpSubMeshesVertices[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pVertexIndices, k*3+1) ];
            currSubObj->p->pVertices[k*3+2]  = tmpSubMeshesVertices[i][ currSubObj->p->getIndexFromArray(currSubObj->p->pVertexIndices, k*3+2) ];
            // don't need to fill pVerticesTransf array here, that task is for during rendering!
            currSubObj->p->pMaterial->getTexcoords()[k*3]   = tmpSubMeshesTexcoords[i][ tmpParseTexcoordIndices[2] - prevtexcoordcount ];
            currSubObj->p->pMaterial->getTexcoords()[k*3+1] = tmpSubMeshesTexcoords[i][ tmpParseTexcoordIndices[1] - prevtexcoordcount ];
            currSubObj->p->pMaterial->getTexcoords()[k*3+2] = tmpSubMeshesTexcoords[i][ tmpParseTexcoordIndices[0] - prevtexcoordcount ];

            // Update the index values because we now store geometry data redundantly.
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,   k*3,   k*3);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,   k*3+1, k*3+1);
            currSubObj->p->SetIndexInArray(currSubObj->p->pVertexIndices,   k*3+2, k*3+2);
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
                    PRREMesh3D* const pSubObj = (PRREMesh3D*)(obj->getAttachedAt(isubmsh));
                    pSubObj->SetName(pSubModelName);
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
        PRREMesh3D* const currSubObj = (PRREMesh3D*) (obj->getAttachedAt(i));
        for (TPRREuint j = 0; j < currSubObj->p->pMaterial->getColorsCount(); j++)
        {
            currSubObj->p->pMaterial->getColors()[j].red = 1.0f;                                                         
            currSubObj->p->pMaterial->getColors()[j].green = 1.0f;
            currSubObj->p->pMaterial->getColors()[j].blue = 1.0f;
            currSubObj->p->pMaterial->getColors()[j].alpha = 1.0f;
        }
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


PRREMesh3DManager::PRREMesh3DManagerImpl::PRREMesh3DManagerImpl() :
    materialMgr(materialMgr)
{

}


/**
    @param owner  Public class instance owning this pimpl object.
    @param matMgr A MaterialManager instance to be used for creating materials used by Mesh3D instances.
*/
PRREMesh3DManager::PRREMesh3DManagerImpl::PRREMesh3DManagerImpl(PRREMesh3DManager* owner, PRREMaterialManager& matMgr) :
    materialMgr(matMgr)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PRREMesh3DManager() ...");
    bMinimalIndexStorage = true;
    bInited = true;
    _pOwner->getConsole().SOLnOO("Done!");
} // PRREMesh3DManager(...)


PRREMesh3DManager::PRREMesh3DManagerImpl::PRREMesh3DManagerImpl(const PRREMesh3DManager::PRREMesh3DManagerImpl&) :
    materialMgr(materialMgr)
{

}


PRREMesh3DManager::PRREMesh3DManagerImpl& PRREMesh3DManager::PRREMesh3DManagerImpl::operator=(const PRREMesh3DManager::PRREMesh3DManagerImpl&)
{
    return *this;    
}



/*
   PRREMesh3DManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
*/
PRREMesh3DManager::PRREMesh3DManager(PRREMaterialManager& matMgr)
{
    p = new PRREMesh3DManagerImpl(this, matMgr);
} // PRREMesh3DManager(...)


PRREMesh3DManager::~PRREMesh3DManager()
{
    delete p;
    p = NULL;
} // ~PRREMesh3DManager()


/**
    Tells whether the object is correctly initialized or not.
    @return True if the given TextureManager instance was initialized correctly, false otherwise.
*/
TPRREbool PRREMesh3DManager::isInitialized() const
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
TPRREbool PRREMesh3DManager::isMinimalIndexStorageEnabled() const
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
void PRREMesh3DManager::SetMinimalIndexStorageEnabled(TPRREbool state)
{
    p->SetMinimalIndexStorageEnabled(state);
}


/**
    Creates a new plane mesh with the given sizes.

    @param a                     Length of plane (size on X-axis).
    @param b                     Height of plane (size on Y-axis).
    
    @return New 3D mesh with 1 face and 4 vertices.
            PGENULL if Mesh3DManager is not yet initialized.
*/
PRREMesh3D* PRREMesh3DManager::createPlane(TPRREfloat a, TPRREfloat b)
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREMesh3DManager::createPlane(%f, %f)", a, b);

    PRREMesh3D* const mesh = new PRREMesh3D(PRRE_PM_QUADS);
    PRREMesh3D* const submesh = new PRREMesh3D(PRRE_PM_QUADS);
    mesh->Attach(*submesh);
    ConvertToPlane(*mesh, a, b);
    Attach( *mesh );

    getConsole().SOLnOO("> Plane mesh created successfully!");
    getConsole().OLn("");

    return mesh;

} // createPlane()


/**
    Creates a new box mesh with the given sizes.
    This box consists of quads instead of triangles and has 24 vertices.
    Every vertex has a normal, a color, and texture uvw-coordinates in separate arrays.
    All faces are defined counter-clockwise starting from the lower left vertex, thus
    the order of texture uvw-coordinates are the same for each face.
    Order of faces is: front face towards the viewer, back, left, right, top, bottom.

    @param a                     Length of box (size on X-axis).
    @param b                     Height of box (size on Y-axis).
    @param c                     Depth of box (size on Z-axis).

    @return New 3D box mesh with 6 faces and 24 vertices.
            PGENULL if Mesh3DManager is not yet initialized.
*/
PRREMesh3D* PRREMesh3DManager::createBox(TPRREfloat a, TPRREfloat b, TPRREfloat c)
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREMesh3DManager::createBox(%f, %f, %f)", a, b, c);

    PRREMesh3D* const mesh = new PRREMesh3D(PRRE_PM_QUADS);
    PRREMesh3D* const submesh = new PRREMesh3D(PRRE_PM_QUADS);
    mesh->Attach(*submesh);
    ConvertToBox(*mesh, a, b, c);
    Attach( *mesh );

    getConsole().SOLnOO("> Box mesh created successfully!");
    getConsole().OLn("");

    return mesh;

} // createBox()


/**
    Creates a new cube mesh with the given sizes.
    Equivalent to createBox(a, a, a).

    @param a                     Length of edges.

    @return New 3D cube mesh with 6 faces and 24 vertices.
            PGENULL if PRREMesh3DManager is not yet initialized.
*/
PRREMesh3D* PRREMesh3DManager::createCube(TPRREfloat a)
{
    getConsole().OLnOI("PRREMesh3DManager::createCube(%f), passing to createBox() ...", a);
    PRREMesh3D* const cube = createBox(a, a, a);
    getConsole().SOLnOO("> Cube mesh created successfully!");
    getConsole().OLn("");
    return cube;
} // createCube()


/**
    Creates 3D mesh from the given file.

    @param filename              The model file to be loaded to be an Object3D instance.

    @return The created mesh.
            PGENULL if PRREMesh3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PRREMesh3D* PRREMesh3DManager::createFromFile(const char* filename)
{
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREMesh3DManager::createFromFile(\"%s\")", filename);

    if ( filename == NULL )
    {
        getConsole().EOLnOO("ERROR: input was NULL, returning PGENULL");
        getConsole().OLn("");
        return PGENULL;
    }

    PRREMesh3D* obj = PGENULL;

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
        obj = p->loadOBJ(filename);
        obj->SetFilename(filename);
        getConsole().OO();
        Attach( *obj );
    }    
    else
    {
        getConsole().EOLnOO("ERROR: unsupported extension!");
        getConsole().OLn("");
        return PGENULL;
    }

    getConsole().SOLnOO("> Mesh loaded successfully!");
    getConsole().OLn("");
    return obj; 
} // createFromFile()


// ############################## PROTECTED ##############################


PRREMesh3DManager::PRREMesh3DManager()
{

}


PRREMesh3DManager::PRREMesh3DManager(const PRREMesh3DManager&)
{

}


PRREMesh3DManager& PRREMesh3DManager::operator=(const PRREMesh3DManager&)
{
    return *this;    
}


/**
    Converts the given Mesh to a Plane Mesh with the given sizes.
    Expects level-1 Mesh (so it must have subobject) and it must have exactly 1 subobject only.
    This function is very useful for descendant classes that want to construct custom objects
    based on Mesh3D, since during the constructing of custom object, this function can be invoked
    to set up the underlying geometry.
    Example use case: Object3D, when being constructed, can be given as parameter to this function,
    so it will be a renderable Plane object, without any array copy operation during construction.
*/
void PRREMesh3DManager::ConvertToPlane(
        PRREMesh3D& mesh, TPRREfloat a, TPRREfloat b)
{
    getConsole().OLn("PRREMesh3DManager::ConvertToPlane(%f, %f)", a, b);
    if ( (mesh.p->nVertices_h > 0) || (mesh.p->pVertices != PGENULL) )
    {
        getConsole().EOLn("ERROR: given Mesh has vertices (%d), expecting real level-1 Mesh object!", mesh.p->nVertices_h);
        return;
    }

    if ( mesh.getCount() != 1 )
    {
        getConsole().EOLn("ERROR: given Mesh has %d manageds, expectedg is 1!", mesh.getCount());
        return;
    }

    mesh.p->primitiveFormat = PRRE_PM_QUADS;
    mesh.p->vPos.Set(0.0f, 0.0f, 0.0f);
    mesh.p->vSize.Set(a, b, 0.0f);
    if ( mesh.p->pMaterial != PGENULL )
    {
        p->materialMgr.DeleteAttachedInstance( *(mesh.p->pMaterial) );
    }
    CreateMaterialForMesh(mesh);

    mesh.p->nMinIndex = 0;
    mesh.p->nMaxIndex = 0;

    PRREMesh3D* const submesh = (PRREMesh3D*) mesh.getAttachedAt(0);

    submesh->p->primitiveFormat = mesh.p->primitiveFormat;
    submesh->p->vPos.Set(0.0f, 0.0f, 0.0f);
    submesh->p->vSize.Set(a, b, 0.0f);
    mesh.p->nIndicesType = GL_UNSIGNED_BYTE;
    submesh->p->nVertices_h  = 4;
    submesh->p->nNormals_h   = submesh->p->nVertices_h;
    submesh->p->nFaces_h     = 1;
    submesh->p->nVertexIndices_h   = submesh->p->nVertices_h;

    if ( submesh->p->pVertices != PGENULL )
    {
        free(submesh->p->pVertices);
    }
    if ( submesh->p->pNormals != PGENULL )
    {
        free(submesh->p->pNormals);
    }
    if ( submesh->p->pVertexIndices != PGENULL )
    {
        free(submesh->p->pVertexIndices);
    }
    submesh->p->pVertices       = (TXYZ*)       malloc( sizeof(TXYZ)   * submesh->p->nVertices_h );
    submesh->p->pNormals   = (TXYZ*)       malloc( sizeof(TXYZ)   * submesh->p->nNormals_h );
    //submesh->p->pFaces     = (TFACE4*)     malloc( sizeof(TFACE4) * submesh->p->nFaces_h );
    submesh->p->nIndicesType = mesh.p->nIndicesType;
    submesh->p->pVertexIndices   = malloc( PRREGLsnippets::getSizeofIndexType(submesh->p->nIndicesType) * submesh->p->nVertexIndices_h );

    if ( submesh->p->pMaterial != PGENULL )
    {
        p->materialMgr.DeleteAttachedInstance( *(submesh->p->pMaterial) );
    }
    CreateMaterialForMesh(*submesh);
    submesh->p->pMaterial->AllocateArrays(
        submesh->p->nVertices_h,
        submesh->p->nVertices_h,
        submesh->p->nVertexIndices_h,
        PRREGLsnippets::getSizeofIndexType(submesh->p->nIndicesType) );

    submesh->p->pVertices[0].x  = -a / 2.0f;
    submesh->p->pVertices[0].y  = -b / 2.0f;
    submesh->p->pVertices[0].z  = 0.0f;
    // no need to fill pVerticesTransf array, that task is for during rendering
    submesh->p->pNormals[0].x   = 0.0f;
    submesh->p->pNormals[0].y   = 0.0f;
    submesh->p->pNormals[0].z   = -1.0f;
    submesh->p->pMaterial->getColors()[0].red    = 1.0f;
    submesh->p->pMaterial->getColors()[0].green  = 1.0f;
    submesh->p->pMaterial->getColors()[0].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[0].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[0].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[0].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[0].w = 0.0f;

    submesh->p->pVertices[1].x  = a / 2.0f;
    submesh->p->pVertices[1].y  = -b / 2.0f;
    submesh->p->pVertices[1].z  = 0.0f;
    submesh->p->pNormals[1].x   = 0.0f;
    submesh->p->pNormals[1].y   = 0.0f;
    submesh->p->pNormals[1].z   = -1.0f;
    submesh->p->pMaterial->getColors()[1].red    = 1.0f;
    submesh->p->pMaterial->getColors()[1].green  = 1.0f;
    submesh->p->pMaterial->getColors()[1].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[1].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[1].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[1].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[1].w = 0.0f;

    submesh->p->pVertices[2].x  = a / 2.0f;
    submesh->p->pVertices[2].y  = b / 2.0f;
    submesh->p->pVertices[2].z  = 0.0f;
    submesh->p->pNormals[2].x   = 0.0f;
    submesh->p->pNormals[2].y   = 0.0f;
    submesh->p->pNormals[2].z   = -1.0f;
    submesh->p->pMaterial->getColors()[2].red    = 1.0f;
    submesh->p->pMaterial->getColors()[2].green  = 1.0f;
    submesh->p->pMaterial->getColors()[2].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[2].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[2].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[2].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[2].w = 0.0f;

    submesh->p->pVertices[3].x  = -a / 2.0f;
    submesh->p->pVertices[3].y  = b / 2.0f;
    submesh->p->pVertices[3].z  = 0.0f;
    submesh->p->pNormals[3].x   = 0.0f;
    submesh->p->pNormals[3].y   = 0.0f;
    submesh->p->pNormals[3].z   = -1.0f;
    submesh->p->pMaterial->getColors()[3].red    = 1.0f;
    submesh->p->pMaterial->getColors()[3].green  = 1.0f;
    submesh->p->pMaterial->getColors()[3].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[3].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[3].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[3].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[3].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[0].vertex_ids[i] = i;
        submesh->p->pFaces[0].texcoord_ids[i] = i;
        submesh->p->pFaces[0].normal_ids[i] = i;
    }*/

    for (TPRREuint i = 0; i < submesh->p->nVertexIndices_h; i++)
    {
        submesh->p->SetIndexInArray(submesh->p->pVertexIndices, i, i);
        if ( submesh->p->nMinIndex > i )
            submesh->p->nMinIndex = i;
        if ( submesh->p->nMaxIndex < i )
            submesh->p->nMaxIndex = i;
    }
}


/**
    Converts the given Mesh to a Box Mesh with the given sizes.
    Expects level-1 Mesh (so it must have subobject) and it must have exactly 1 subobject only.
    This function is very useful for descendant classes that want to construct custom objects
    based on Mesh3D, since during the constructing of custom object, this function can be invoked
    to set up the underlying geometry.
    Example use case: Object3D, when being constructed, can be given as parameter to this function,
    so it will be a renderable Box object, without any array copy operation during construction.
*/
void PRREMesh3DManager::ConvertToBox(
        PRREMesh3D& mesh, TPRREfloat a, TPRREfloat b, TPRREfloat c)
{
    getConsole().OLn("PRREMesh3DManager::ConvertToBox(%f, %f, %f)", a, b, c);
    if ( (mesh.p->nVertices_h > 0) || (mesh.p->pVertices != PGENULL) )
    {
        getConsole().EOLn("ERROR: given Mesh has vertices (%d), expecting real level-1 Mesh object!", mesh.p->nVertices_h);
        return;
    }

    if ( mesh.getCount() != 1 )
    {
        getConsole().EOLn("ERROR: given Mesh has %d manageds, expectedg is 1!", mesh.getCount());
        return;
    }

    mesh.p->primitiveFormat = PRRE_PM_QUADS;
    mesh.p->vPos.Set(0.0f, 0.0f, 0.0f);
    mesh.p->vSize.Set(a, b, c);
    mesh.p->nIndicesType = GL_UNSIGNED_BYTE;
    if ( mesh.p->pMaterial != PGENULL )
    {
        p->materialMgr.DeleteAttachedInstance( *(mesh.p->pMaterial) );
    }
    CreateMaterialForMesh(mesh);

    mesh.p->nMinIndex = 0;
    mesh.p->nMaxIndex = 0;

    PRREMesh3D* const submesh = (PRREMesh3D*) mesh.getAttachedAt(0);
    submesh->p->primitiveFormat = mesh.p->primitiveFormat;
    submesh->p->vPos.Set(0.0f, 0.0f, 0.0f);
    submesh->p->vSize.Set(a, b, c);
    submesh->p->nVertices_h  = 24;
    submesh->p->nNormals_h   = submesh->p->nVertices_h;
    submesh->p->nFaces_h     = 6;

    submesh->p->nVertexIndices_h   = submesh->p->nVertices_h;

    if ( submesh->p->pVertices != PGENULL )
    {
        free(submesh->p->pVertices);
    }
    if ( submesh->p->pNormals != PGENULL )
    {
        free(submesh->p->pNormals);
    }
    if ( submesh->p->pVertexIndices != PGENULL )
    {
        free(submesh->p->pVertexIndices);
    }
    submesh->p->pVertices       = (TXYZ*)  malloc( sizeof(TXYZ)       * submesh->p->nVertices_h );
    submesh->p->pNormals        = (TXYZ*)  malloc( sizeof(TXYZ)       * submesh->p->nNormals_h );
    //submesh->p->pFaces     = (TFACE4*)     malloc( sizeof(TFACE4)     * submesh->p->nFaces_h );
    submesh->p->nIndicesType = mesh.p->nIndicesType;
    submesh->p->pVertexIndices   = malloc( PRREGLsnippets::getSizeofIndexType(submesh->p->nIndicesType) * submesh->p->nVertexIndices_h );

    if ( submesh->p->pMaterial != PGENULL )
    {
        p->materialMgr.DeleteAttachedInstance( *(submesh->p->pMaterial) );
    }
    submesh->p->pMaterial = p->materialMgr.createMaterial();
    submesh->p->pMaterial->AllocateArrays(
        submesh->p->nVertices_h,
        submesh->p->nVertices_h,
        submesh->p->nVertexIndices_h,
        PRREGLsnippets::getSizeofIndexType(submesh->p->nIndicesType) );

    // front face
    submesh->p->pVertices[0].x  = -a / 2.0f;
    submesh->p->pVertices[0].y  = -b / 2.0f;
    submesh->p->pVertices[0].z  = -c / 2.0f;
    submesh->p->pNormals[0].x   = 0.0f;
    submesh->p->pNormals[0].y   = 0.0f;
    submesh->p->pNormals[0].z   = -1.0f;
    submesh->p->pMaterial->getColors()[0].red    = 1.0f;
    submesh->p->pMaterial->getColors()[0].green  = 1.0f;
    submesh->p->pMaterial->getColors()[0].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[0].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[0].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[0].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[0].w = 0.0f;

    submesh->p->pVertices[1].x  = a / 2.0f;
    submesh->p->pVertices[1].y  = -b / 2.0f;
    submesh->p->pVertices[1].z  = -c / 2.0f;
    submesh->p->pNormals[1].x   = 0.0f;
    submesh->p->pNormals[1].y   = 0.0f;
    submesh->p->pNormals[1].z   = -1.0f;
    submesh->p->pMaterial->getColors()[1].red    = 1.0f;
    submesh->p->pMaterial->getColors()[1].green  = 1.0f;
    submesh->p->pMaterial->getColors()[1].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[1].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[1].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[1].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[1].w = 0.0f;

    submesh->p->pVertices[2].x  = a / 2.0f;
    submesh->p->pVertices[2].y  = b / 2.0f;
    submesh->p->pVertices[2].z  = -c / 2.0f;
    submesh->p->pNormals[2].x   = 0.0f;
    submesh->p->pNormals[2].y   = 0.0f;
    submesh->p->pNormals[2].z   = -1.0f;
    submesh->p->pMaterial->getColors()[2].red    = 1.0f;
    submesh->p->pMaterial->getColors()[2].green  = 1.0f;
    submesh->p->pMaterial->getColors()[2].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[2].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[2].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[2].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[2].w = 0.0f;

    submesh->p->pVertices[3].x  = -a / 2.0f;
    submesh->p->pVertices[3].y  = b / 2.0f;
    submesh->p->pVertices[3].z  = -c / 2.0f;
    submesh->p->pNormals[3].x   = 0.0f;
    submesh->p->pNormals[3].y   = 0.0f;
    submesh->p->pNormals[3].z   = -1.0f;
    submesh->p->pMaterial->getColors()[3].red    = 1.0f;
    submesh->p->pMaterial->getColors()[3].green  = 1.0f;
    submesh->p->pMaterial->getColors()[3].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[3].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[3].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[3].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[3].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[0].vertex_ids[i] = i;
        submesh->p->pFaces[0].texcoord_ids[i] = i;
        submesh->p->pFaces[0].normal_ids[i] = i;
    }*/

    // back face
    submesh->p->pVertices[4].x  = a / 2.0f;
    submesh->p->pVertices[4].y  = -b / 2.0f;
    submesh->p->pVertices[4].z  = c / 2.0f;
    submesh->p->pNormals[4].x   = 0.0f;
    submesh->p->pNormals[4].y   = 0.0f;
    submesh->p->pNormals[4].z   = 1.0f;
    submesh->p->pMaterial->getColors()[4].red    = 1.0f;
    submesh->p->pMaterial->getColors()[4].green  = 1.0f;
    submesh->p->pMaterial->getColors()[4].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[4].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[4].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[4].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[4].w = 0.0f;

    submesh->p->pVertices[5].x  = -a / 2.0f;
    submesh->p->pVertices[5].y  = -b / 2.0f;
    submesh->p->pVertices[5].z  = c / 2.0f;
    submesh->p->pNormals[5].x   = 0.0f;
    submesh->p->pNormals[5].y   = 0.0f;
    submesh->p->pNormals[5].z   = 1.0f;
    submesh->p->pMaterial->getColors()[5].red    = 1.0f;
    submesh->p->pMaterial->getColors()[5].green  = 1.0f;
    submesh->p->pMaterial->getColors()[5].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[5].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[5].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[5].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[5].w = 0.0f;

    submesh->p->pVertices[6].x  = -a / 2.0f;
    submesh->p->pVertices[6].y  = b / 2.0f;
    submesh->p->pVertices[6].z  = c / 2.0f;
    submesh->p->pNormals[6].x   = 0.0f;
    submesh->p->pNormals[6].y   = 0.0f;
    submesh->p->pNormals[6].z   = 1.0f;
    submesh->p->pMaterial->getColors()[6].red    = 1.0f;
    submesh->p->pMaterial->getColors()[6].green  = 1.0f;
    submesh->p->pMaterial->getColors()[6].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[6].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[6].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[6].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[6].w = 0.0f;

    submesh->p->pVertices[7].x  = a / 2.0f;
    submesh->p->pVertices[7].y  = b / 2.0f;
    submesh->p->pVertices[7].z  = c / 2.0f;
    submesh->p->pNormals[7].x   = 0.0f;
    submesh->p->pNormals[7].y   = 0.0f;
    submesh->p->pNormals[7].z   = 1.0f;
    submesh->p->pMaterial->getColors()[7].red    = 1.0f;
    submesh->p->pMaterial->getColors()[7].green  = 1.0f;
    submesh->p->pMaterial->getColors()[7].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[7].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[7].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[7].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[7].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[1].vertex_ids[i] = 4+i;
        submesh->p->pFaces[1].texcoord_ids[i] = 4+i;
        submesh->p->pFaces[1].normal_ids[i] = 4+i;
    }*/

    // left face
    submesh->p->pVertices[8].x  = -a / 2.0f;
    submesh->p->pVertices[8].y  = -b / 2.0f;
    submesh->p->pVertices[8].z  = c / 2.0f;
    submesh->p->pNormals[8].x   = -1.0f;
    submesh->p->pNormals[8].y   = 0.0f;
    submesh->p->pNormals[8].z   = 0.0f;
    submesh->p->pMaterial->getColors()[8].red    = 1.0f;
    submesh->p->pMaterial->getColors()[8].green  = 1.0f;
    submesh->p->pMaterial->getColors()[8].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[8].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[8].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[8].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[8].w = 0.0f;

    submesh->p->pVertices[9].x  = -a / 2.0f;
    submesh->p->pVertices[9].y  = -b / 2.0f;
    submesh->p->pVertices[9].z  = -c / 2.0f;
    submesh->p->pNormals[9].x   = -1.0f;
    submesh->p->pNormals[9].y   = 0.0f;
    submesh->p->pNormals[9].z   = 0.0f;
    submesh->p->pMaterial->getColors()[9].red    = 1.0f;
    submesh->p->pMaterial->getColors()[9].green  = 1.0f;
    submesh->p->pMaterial->getColors()[9].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[9].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[9].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[9].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[9].w = 0.0f;

    submesh->p->pVertices[10].x  = -a / 2.0f;
    submesh->p->pVertices[10].y  = b / 2.0f;
    submesh->p->pVertices[10].z  = -c / 2.0f;
    submesh->p->pNormals[10].x   = -1.0f;
    submesh->p->pNormals[10].y   = 0.0f;
    submesh->p->pNormals[10].z   = 0.0f;
    submesh->p->pMaterial->getColors()[10].red    = 1.0f;
    submesh->p->pMaterial->getColors()[10].green  = 1.0f;
    submesh->p->pMaterial->getColors()[10].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[10].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[10].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[10].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[10].w = 0.0f;

    submesh->p->pVertices[11].x  = -a / 2.0f;
    submesh->p->pVertices[11].y  = b / 2.0f;
    submesh->p->pVertices[11].z  = c / 2.0f;
    submesh->p->pNormals[11].x   = -1.0f;
    submesh->p->pNormals[11].y   = 0.0f;
    submesh->p->pNormals[11].z   = 0.0f;
    submesh->p->pMaterial->getColors()[11].red    = 1.0f;
    submesh->p->pMaterial->getColors()[11].green  = 1.0f;
    submesh->p->pMaterial->getColors()[11].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[11].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[11].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[11].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[11].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[2].vertex_ids[i] = 8+i;
        submesh->p->pFaces[2].texcoord_ids[i] = 8+i;
        submesh->p->pFaces[2].normal_ids[i] = 8+i;
    }*/

    // right face
    submesh->p->pVertices[12].x  = a / 2.0f;
    submesh->p->pVertices[12].y  = -b / 2.0f;
    submesh->p->pVertices[12].z  = -c / 2.0f;
    submesh->p->pNormals[12].x   = 1.0f;
    submesh->p->pNormals[12].y   = 0.0f;
    submesh->p->pNormals[12].z   = 0.0f;
    submesh->p->pMaterial->getColors()[12].red    = 1.0f;
    submesh->p->pMaterial->getColors()[12].green  = 1.0f;
    submesh->p->pMaterial->getColors()[12].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[12].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[12].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[12].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[12].w = 0.0f;

    submesh->p->pVertices[13].x  = a / 2.0f;
    submesh->p->pVertices[13].y  = -b / 2.0f;
    submesh->p->pVertices[13].z  = c / 2.0f;
    submesh->p->pNormals[13].x   = 1.0f;
    submesh->p->pNormals[13].y   = 0.0f;
    submesh->p->pNormals[13].z   = 0.0f;
    submesh->p->pMaterial->getColors()[13].red    = 1.0f;
    submesh->p->pMaterial->getColors()[13].green  = 1.0f;
    submesh->p->pMaterial->getColors()[13].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[13].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[13].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[13].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[13].w = 0.0f;

    submesh->p->pVertices[14].x  = a / 2.0f;
    submesh->p->pVertices[14].y  = b / 2.0f;
    submesh->p->pVertices[14].z  = c / 2.0f;
    submesh->p->pNormals[14].x   = 1.0f;
    submesh->p->pNormals[14].y   = 0.0f;
    submesh->p->pNormals[14].z   = 0.0f;
    submesh->p->pMaterial->getColors()[14].red    = 1.0f;
    submesh->p->pMaterial->getColors()[14].green  = 1.0f;
    submesh->p->pMaterial->getColors()[14].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[14].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[14].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[14].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[14].w = 0.0f;

    submesh->p->pVertices[15].x  = a / 2.0f;
    submesh->p->pVertices[15].y  = b / 2.0f;
    submesh->p->pVertices[15].z  = -c / 2.0f;
    submesh->p->pNormals[15].x   = 1.0f;
    submesh->p->pNormals[15].y   = 0.0f;
    submesh->p->pNormals[15].z   = 0.0f;
    submesh->p->pMaterial->getColors()[15].red    = 1.0f;
    submesh->p->pMaterial->getColors()[15].green  = 1.0f;
    submesh->p->pMaterial->getColors()[15].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[15].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[15].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[15].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[15].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[3].vertex_ids[i] = 12+i;
        submesh->p->pFaces[3].texcoord_ids[i] = 12+i;
        submesh->p->pFaces[3].normal_ids[i] = 12+i;
    }*/

    // top face
    submesh->p->pVertices[16].x  = -a / 2.0f;
    submesh->p->pVertices[16].y  = b / 2.0f;
    submesh->p->pVertices[16].z  = -c / 2.0f;
    submesh->p->pNormals[16].x   = 0.0f;
    submesh->p->pNormals[16].y   = 1.0f;
    submesh->p->pNormals[16].z   = 0.0f;
    submesh->p->pMaterial->getColors()[16].red    = 1.0f;
    submesh->p->pMaterial->getColors()[16].green  = 1.0f;
    submesh->p->pMaterial->getColors()[16].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[16].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[16].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[16].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[16].w = 0.0f;

    submesh->p->pVertices[17].x  = a / 2.0f;
    submesh->p->pVertices[17].y  = b / 2.0f;
    submesh->p->pVertices[17].z  = -c / 2.0f;
    submesh->p->pNormals[17].x   = 0.0f;
    submesh->p->pNormals[17].y   = 1.0f;
    submesh->p->pNormals[17].z   = 0.0f;
    submesh->p->pMaterial->getColors()[17].red    = 1.0f;
    submesh->p->pMaterial->getColors()[17].green  = 1.0f;
    submesh->p->pMaterial->getColors()[17].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[17].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[17].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[17].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[17].w = 0.0f;

    submesh->p->pVertices[18].x  = a / 2.0f;
    submesh->p->pVertices[18].y  = b / 2.0f;
    submesh->p->pVertices[18].z  = c / 2.0f;
    submesh->p->pNormals[18].x   = 0.0f;
    submesh->p->pNormals[18].y   = 1.0f;
    submesh->p->pNormals[18].z   = 0.0f;
    submesh->p->pMaterial->getColors()[18].red    = 1.0f;
    submesh->p->pMaterial->getColors()[18].green  = 1.0f;
    submesh->p->pMaterial->getColors()[18].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[18].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[18].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[18].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[18].w = 0.0f;

    submesh->p->pVertices[19].x  = -a / 2.0f;
    submesh->p->pVertices[19].y  = b / 2.0f;
    submesh->p->pVertices[19].z  = c / 2.0f;
    submesh->p->pNormals[19].x   = 0.0f;
    submesh->p->pNormals[19].y   = 1.0f;
    submesh->p->pNormals[19].z   = 0.0f;
    submesh->p->pMaterial->getColors()[19].red    = 1.0f;
    submesh->p->pMaterial->getColors()[19].green  = 1.0f;
    submesh->p->pMaterial->getColors()[19].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[19].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[19].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[19].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[19].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[4].vertex_ids[i] = 16+i;
        submesh->p->pFaces[4].texcoord_ids[i] = 16+i;
        submesh->p->pFaces[4].normal_ids[i] = 16+i;
    }*/

    // bottom face
    submesh->p->pVertices[20].x  = a / 2.0f;
    submesh->p->pVertices[20].y  = -b / 2.0f;
    submesh->p->pVertices[20].z  = -c / 2.0f;
    submesh->p->pNormals[20].x   = 0.0f;
    submesh->p->pNormals[20].y   = -1.0f;
    submesh->p->pNormals[20].z   = 0.0f;
    submesh->p->pMaterial->getColors()[20].red    = 1.0f;
    submesh->p->pMaterial->getColors()[20].green  = 1.0f;
    submesh->p->pMaterial->getColors()[20].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[20].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[20].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[20].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[20].w = 0.0f;

    submesh->p->pVertices[21].x  = -a / 2.0f;
    submesh->p->pVertices[21].y  = -b / 2.0f;
    submesh->p->pVertices[21].z  = -c / 2.0f;
    submesh->p->pNormals[21].x   = 0.0f;
    submesh->p->pNormals[21].y   = -1.0f;
    submesh->p->pNormals[21].z   = 0.0f;
    submesh->p->pMaterial->getColors()[21].red    = 1.0f;
    submesh->p->pMaterial->getColors()[21].green  = 1.0f;
    submesh->p->pMaterial->getColors()[21].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[21].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[21].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[21].v = 0.0f;
    submesh->p->pMaterial->getTexcoords()[21].w = 0.0f;

    submesh->p->pVertices[22].x  = -a / 2.0f;
    submesh->p->pVertices[22].y  = -b / 2.0f;
    submesh->p->pVertices[22].z  = c / 2.0f;
    submesh->p->pNormals[22].x   = 0.0f;
    submesh->p->pNormals[22].y   = -1.0f;
    submesh->p->pNormals[22].z   = 0.0f;
    submesh->p->pMaterial->getColors()[22].red    = 1.0f;
    submesh->p->pMaterial->getColors()[22].green  = 1.0f;
    submesh->p->pMaterial->getColors()[22].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[22].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[22].u = 1.0f;
    submesh->p->pMaterial->getTexcoords()[22].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[22].w = 0.0f;

    submesh->p->pVertices[23].x  = a / 2.0f;
    submesh->p->pVertices[23].y  = -b / 2.0f;
    submesh->p->pVertices[23].z  = c / 2.0f;
    submesh->p->pNormals[23].x   = 0.0f;
    submesh->p->pNormals[23].y   = -1.0f;
    submesh->p->pNormals[23].z   = 0.0f;
    submesh->p->pMaterial->getColors()[23].red    = 1.0f;
    submesh->p->pMaterial->getColors()[23].green  = 1.0f;
    submesh->p->pMaterial->getColors()[23].blue   = 1.0f;
    submesh->p->pMaterial->getColors()[23].alpha  = 1.0f;
    submesh->p->pMaterial->getTexcoords()[23].u = 0.0f;
    submesh->p->pMaterial->getTexcoords()[23].v = 1.0f;
    submesh->p->pMaterial->getTexcoords()[23].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->p->pFaces[5].vertex_ids[i] = 20+i;
        submesh->p->pFaces[5].texcoord_ids[i] = 20+i;
        submesh->p->pFaces[5].normal_ids[i] = 20+i;
    }*/

    for (TPRREuint i = 0; i < submesh->p->nVertexIndices_h; i++)
    {
        submesh->p->SetIndexInArray(submesh->p->pVertexIndices,   i, i);
        if ( submesh->p->nMinIndex > i )
            submesh->p->nMinIndex = i;
        if ( submesh->p->nMaxIndex < i )
            submesh->p->nMaxIndex = i;
    }
}


void PRREMesh3DManager::CreateMaterialForMesh(PRREMesh3D& mesh) const 
{
    mesh.p->pMaterial = p->materialMgr.createMaterial();
}


// ############################### PRIVATE ###############################


