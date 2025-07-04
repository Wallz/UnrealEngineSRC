#ifndef _RCONVEX_H
#define _RCONVEX_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 1997-2002 MathEngine PLC

   $Name: t-stevet-RWSpre-020531 $

   Date: $Date: 2002/04/04 15:29:02 $ - Revision: $Revision: 1.9.2.2 $

   This software and its accompanying manuals have been developed
   by MathEngine PLC ("MathEngine") and the copyright and all other
   intellectual property rights in them belong to MathEngine. All
   rights conferred by law (including rights under international
   copyright conventions) are reserved to MathEngine. This software
   may also incorporate information which is confidential to
   MathEngine.

   Save to the extent permitted by law, or as otherwise expressly
   permitted by MathEngine, this software and the manuals must not
   be copied (in whole or in part), re-arranged, altered or adapted
   in any way without the prior written consent of the Company. In
   addition, the information contained in the software may not be
   disseminated without the prior written consent of MathEngine.

 */

/** @file
 * Convex graphic creation.
 */

#include <McdConvexMesh.h>
#include <MeViewer.h>

#ifdef __cplusplus
extern "C"
{
#endif

RGraphic* MEAPI RGraphicConvexCreate(RRender *const rc, const McdConvexMeshID conv,
                                     const float color[4], const MeMatrix4Ptr matrix);

#ifdef __cplusplus
}
#endif

#endif
