#pragma once

/*
    ###################################################################################
    PRREIIncrementalRenderPipeline.h
    This file is part of PRRE.
    External header.
    Incremental renderer pipeline interface.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include "../PRRECamera.h"
#include "../Object3D/PRREObject3DManager.h"
#include "../PRREuiManager.h"


/**
    Incremental renderer pipeline interface.

*/
class PRREIIncrementalRenderPipeline
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREIIncrementalRenderPipeline is included")
#endif

public:

    virtual void GeometricStage(
        const TXYZ* pVertices,
        TPRREuint nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPRREuint nVertexIndices_h,
        unsigned int nIndicesType,
        TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPRREint vp_x,
        TPRREint vp_y,
        TPRREuint vp_w,
        TPRREuint vp_h
    ) = 0;

    virtual void RenderStage(
        TPRREuint nVertexIndices_h,
        TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
        TPRREint vp_x,
        TPRREint vp_y,
        TPRREuint vp_w,
        TPRREuint vp_h
    ) = 0;

};