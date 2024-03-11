#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_1_1.h
    This file is part of PURE.
    Internal header.
    Pure OpenGL 1.1 capabilities class.
    Made by PR00F88
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 1.1 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_1_1 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_1_1 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_1_1(
        const std::string& sVidFeaturesOGL,
        const std::string& sVidFeaturesWGL,
        const std::string& sVidVersionOGL,
        const TPureUInt& nVidVersionGLSLmaj,
        const TPureUInt& nVidVersionGLSLmin) :
    PureHwVideoDiscoverOpenGLbase(
        sVidFeaturesOGL,
        sVidFeaturesWGL,
        sVidVersionOGL,
        nVidVersionGLSLmaj,
        nVidVersionGLSLmin) {};

    virtual ~PureHwVideoDiscoverOpenGL_1_1() {};

    virtual std::string getVersion() const
    {
        return "1.1";
    }

    /**
        This method checks for related OpenGL extensions, however final verdict on extension support is declared by Discover...Availability() functions of this class.
        Some higher level manager first invokes this method, then later based on some logic it invokes the Discover...Availability() functions of this class.
        This class doesn't limit the way you invoke its public functions as this class is not intended to have that kind of logic, however the order is important
        from the perspective of the logic of the higher-level class, including logging.  
    */
    virtual bool discoverOpenGLavailability()
    {
        return true;  /* 1.1 is always available, at least on Windows, implemented in MCD */
    }

protected:
    PureHwVideoDiscoverOpenGL_1_1() {};

    PureHwVideoDiscoverOpenGL_1_1(const PureHwVideoDiscoverOpenGL_1_1&) {};
    PureHwVideoDiscoverOpenGL_1_1& operator=(const PureHwVideoDiscoverOpenGL_1_1&) { return *this; };

private:
    
};