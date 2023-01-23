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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureIIncrementalRenderPipeline is included")
#endif

public:

    virtual void GeometricStage(
        const TXYZ* pVertices,
        TPureUInt nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPureUInt nVertexIndices_h,
        unsigned int nIndicesType,
        TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPureInt vp_x,
        TPureInt vp_y,
        TPureUInt vp_w,
        TPureUInt vp_h
    ) = 0;

    virtual void RenderStage(
        TPureUInt nVertexIndices_h,
        TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
        TPureInt vp_x,
        TPureInt vp_y,
        TPureUInt vp_w,
        TPureUInt vp_h
    ) = 0;

};