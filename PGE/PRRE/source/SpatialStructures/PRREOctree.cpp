/*
    ###################################################################################
    PRREOctree.cpp
    This file is part of PRRE.
    PRREOctree class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/internal/SpatialStructures/PRREOctree.h"

using namespace std;

/*
   PRREOctree
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREOctree::PRREOctree()
{

} // PRREOctree()


PRREOctree::~PRREOctree()
{

} // ~PRREOctree()


PRREOctree* PRREOctree::insertObject(const PRREObject3D&)
{
    return PGENULL;
}

PRREOctree* PRREOctree::removeObject(const PRREObject3D&)
{
    return PGENULL;
}

const PRREOctree* PRREOctree::findObject(const PRREObject3D&) const
{
    return PGENULL;
}

TPRREuint PRREOctree::getDepthLevel() const
{
    return 0;
}

TPRREuint PRREOctree::getMaxDepthLevel() const
{
    return 0;
}

PRREOctree::NodeType PRREOctree::getNodeType() const
{
    return NodeType::Parent;
}



// ############################## PROTECTED ##############################


PRREOctree::PRREOctree(const PRREOctree&)
{

}


PRREOctree& PRREOctree::operator=(const PRREOctree&)
{
    return *this;
}


// ############################### PRIVATE ###############################

