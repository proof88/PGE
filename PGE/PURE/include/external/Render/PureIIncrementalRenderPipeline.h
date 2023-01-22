#pragma once

/*
    ###################################################################################
    PureIIncrementalRenderPipeline.h
    This file is part of PURE.
    External header.
    Incremental renderer pipeline interface.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "../PureCamera.h"
#include "../Object3D/PureObject3DManager.h"
#include "../PureUiManager.h"


/**
    Incremental renderer pipeline interface.

*/
class PureIIncrementalRenderPipeline
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureIIncrementalRenderPipeline is included")
#endif

public:

    virtual void GeometricStage(
        const TXYZ* pVertices,
        TPureuint nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPureuint nVertexIndices_h,
        unsigned int nIndicesType,
        TPure_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPureint vp_x,
        TPureint vp_y,
        TPureuint vp_w,
        TPureuint vp_h
    ) = 0;

    virtual void RenderStage(
        TPureuint nVertexIndices_h,
        TPure_TRANSFORMED_VERTEX* pVerticesTransf,
        TPureint vp_x,
        TPureint vp_y,
        TPureuint vp_w,
        TPureuint vp_h
    ) = 0;

};