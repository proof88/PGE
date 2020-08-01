/*
    ###################################################################################
    PRREImageManager.cpp
    This file is part of PRRE.
    PRREImageManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "PRREImageManager.h"
#include "PRREImageImpl.h"
#include "../Hardware/PRREhwInfo.h"

using namespace std;

/*
   PRREImageManager::PRREImageManagerImpl
   ###########################################################################
*/


class PRREImageManager::PRREImageManagerImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PRREImageManagerImpl();


protected:
    static PRREhwInfo& pHWInfo;             /**< We need some info on HW. */

    // ---------------------------------------------------------------------------
    
private:

    PRREImageManager* _pOwner;       /**< The owner public object who creates this pimpl object. */


    PRREImageManagerImpl(PRREImageManager* parent);

    PRREImageManagerImpl(const PRREImageManagerImpl&);
    PRREImageManagerImpl& operator=(const PRREImageManagerImpl&);

    PRREImage* loadBMPfail(HANDLE f, PRREImage* img, RGBQUAD* palette, const char* msg);    /**< Used by loadBMP() when an error occurs. */
    PRREImage* createFromFileFail(const char* msg);     /**< Used by createFromFile() when an error occurs. */

    friend class PRREImageManager;

};


// ############################### PUBLIC ################################


PRREImageManager::PRREImageManagerImpl::~PRREImageManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRREImageManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();
    _pOwner = NULL;
} // ~PRREImageManager()


// ############################## PROTECTED ##############################


PRREhwInfo& PRREImageManager::PRREImageManagerImpl::pHWInfo = PRREhwInfo::get();


// ############################### PRIVATE ###############################


/**
    @param parent Public class instance owning this pimpl object.
*/
PRREImageManager::PRREImageManagerImpl::PRREImageManagerImpl(PRREImageManager* parent)
{
    _pOwner = parent;
    _pOwner->getConsole().OLn("PRREImageManager(console, hwinfo) ...");
    _pOwner->getConsole().OISOLnOO("Done!");
} // PRREImageManager(...)


PRREImageManager::PRREImageManagerImpl::PRREImageManagerImpl(const PRREImageManager::PRREImageManagerImpl&)
{

}


PRREImageManager::PRREImageManagerImpl& PRREImageManager::PRREImageManagerImpl::operator=(const PRREImageManager::PRREImageManagerImpl&)
{
    return *this;
}


/**
    Used by loadBMP() when an error occurs.
    @return PGENULL always.
*/
PRREImage* PRREImageManager::PRREImageManagerImpl::loadBMPfail( HANDLE f, PRREImage* img, RGBQUAD* palette, const char* msg)
{
    _pOwner->getConsole().EOLn(msg);
    CloseHandle(f);
    delete img;
    free(palette);
    _pOwner->getConsole().OO();
    return PGENULL;
} // loadBMPfail()


/**
    Used by createFromFile() when an error occurs.
    @return PGENULL always.
*/
PRREImage* PRREImageManager::PRREImageManagerImpl::createFromFileFail(const char* msg)
{
    _pOwner->getConsole().EOLn(msg);
    _pOwner->getConsole().OLn("");
    _pOwner->getConsole().OO();
    return PGENULL;
} // createFromFileFail()


/*
   PRREImageManager
   ###########################################################################
*/

// ############################### PUBLIC ################################


/**
    Calculates the number of needed swaps needed from the given color component order to the other.
    @return Number of swaps needed from the given color component order to the other.
*/
TPRREuint PRREImageManager::getColorConversionSwapCount(TPIXCOMPORD from, TPIXCOMPORD to)
{
    if ( (from % 2) == 1 ) 
        from = (TPIXCOMPORD)(from-1);
    if ( (to % 2) == 1 )
        to = (TPIXCOMPORD)(to-1);
    if ( from == to )
        return 0;

    switch (from)
    {
    case PRRE_RGB: return (to == PRRE_GBR) || (to == PRRE_BRG) ? 2 : 1;
    case PRRE_RBG: return (to == PRRE_GRB) || (to == PRRE_BGR) ? 2 : 1;
    case PRRE_GRB: return (to == PRRE_RBG) || (to == PRRE_BGR) ? 2 : 1;
    case PRRE_BGR: return (to == PRRE_RBG) || (to == PRRE_GRB) ? 2 : 1;
    case PRRE_GBR: return (to == PRRE_RGB) || (to == PRRE_BRG) ? 2 : 1;
    default /* BRG*/: return (to == PRRE_RGB) || (to == PRRE_GBR) ? 2 : 1;
    }
} // getColorConversionSwapCount()


/**
    Transforms the given color component order to its mirrored form.
    @return The mirrored form of the given color component order.
*/
TPIXCOMPORD PRREImageManager::getMirroredPixelComponentOrder(TPIXCOMPORD corder)
{
    switch (corder)
    {
    case PRRE_RGB : return PRRE_BGR;
    case PRRE_RBG : return PRRE_GBR;
    case PRRE_GBR : return PRRE_RBG;
    case PRRE_GRB : return PRRE_BRG;
    case PRRE_BGR : return PRRE_RGB;
    case PRRE_BRG : return PRRE_GRB;
    case PRRE_RGBA: return PRRE_BGRA;
    case PRRE_RBGA: return PRRE_GBRA;
    case PRRE_GBRA: return PRRE_RBGA;
    case PRRE_GRBA: return PRRE_BRGA;
    case PRRE_BGRA: return PRRE_RGBA;
    case PRRE_BRGA: return PRRE_GRBA;
    default: return PRRE_RGBA;
    }
} // getMirroredPixelComponentOrder()


PRREImageManager::PRREImageManager() :
    PRREFiledManager()
{
    p = new PRREImageManagerImpl(this);
} // PRREImageManager(...)


PRREImageManager::~PRREImageManager()
{
    delete p;
    p = NULL;
} // ~PRREImageManager()


/**
    Creates an Image object from the given file.
    Only BMP format is supported at this time.
    Can load any bit depth BMP in theory, but below 16 bits, the rules are the following:
     - at 8 bits (256 colors) the width of the image must be divisible by 4;
     - at 4 bits (16 colors) the width of the image must be divisible by 8;
     - at 1 bits (2 colors) the width of the image must be divisible by 32.

    @return The created Image object on success, PGENULL otherwise.
*/
PRREImage* PRREImageManager::createFromFile(const char* filename)
{
    getConsole().OLnOI("PRREImageManager::createFromFile(\"%s\")", filename);

    if ( filename == NULL )
        return p->createFromFileFail("ERROR: NULLPOINTER!");

    if ( !PFL::fileExists(filename) )
        return p->createFromFileFail("ERROR: file doesn't exist!");

    string sFileExt = PFL::getExtension(filename);
    if ( sFileExt == "" )
        return p->createFromFileFail("ERROR: no file extension!");
    
    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    getConsole().O("ext: .%s, ", sFileExt.c_str());
    PRREImage* pNewImage;

    if ( sFileExt == "BMP" )
    {
        getConsole().OLn("calling loadBMP()... ");
        pNewImage = loadBMP(filename);
    }
    else return p->createFromFileFail("ERROR: unsupported extension!");

    if ( pNewImage == PGENULL )
        return p->createFromFileFail("ERROR: loadXXX() returned PGENULL!");
    
    Attach( *pNewImage );

    getConsole().SOLnOO("> Image loaded!");
    getConsole().OLn("");

    return pNewImage;
} // createFromFile()


/**
    Creates a blank Image object as specified.

    @param width  Width of the new image. Cannot be 0.
    @param height Height of the new image. Cannot be 0.
    @param bpp    Bits per pixel. Currently only 24 is supported.

    @return The created Image object on success, PGENULL otherwise.
*/
PRREImage* PRREImageManager::createBlank(TPRREuint width, TPRREuint height, TPRREuint bpp)
{
    if ( (width == 0) || (height == 0) || (bpp != 24) )
        return PGENULL;

    PRREImage* const pNewImage = new PRREImage();

    // ppxls = PGENULL means it won't try to copy anything to the allocated buffer so the content of the buffer will be undefined.
    if ( !pNewImage->p->initMembers(bpp, width, height, PRRE_RGB, PRRE_RGB, true, false, PGENULL, width * height * bpp/8) )
    {
        delete pNewImage;
        getConsole().EOLn("Failed initMembers(%d, %d, %d, ...)", bpp, width, height);
        getConsole().OLn("");
        return PGENULL;
    }

    Attach( *pNewImage );

    getConsole().SOLnOO("> Blank Image created!");
    getConsole().OLn("");

    return pNewImage;
}


// ############################## PROTECTED ##############################


PRREImageManager::PRREImageManager(const PRREImageManager&)
{

}


PRREImageManager& PRREImageManager::operator=(const PRREImageManager&)
{
    return *this;
}


/**
    This loads BMP files, handles the actual file operations, and creates the Image object.
    Can load any bit depth BMP in theory, but below 16 bits, the rules are the following:
     - at 8 bits (256 colors) the width of the image must be divisible by 4;
     - at 4 bits (16 colors) the width of the image must be divisible by 8;
     - at 1 bits (2 colors) the width of the image must be divisible by 32.

    @return The created Image object on success, PGENULL otherwise.
*/
PRREImage* PRREImageManager::loadBMP(const char* filename)
{
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    DWORD bitmaplength;
    RGBQUAD* palette = NULL;
    DWORD palettesize = 0;
    DWORD bytesread;
    PRREImage* pNewImage = NULL;

    HANDLE bitmapfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

    getConsole().OLnOI("PRREImageManager::loadBMP(\"%s\")", filename);
    if ( bitmapfile == INVALID_HANDLE_VALUE )
        return p->loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: bitmapfile == INVALID_HANDLE_VALUE, returning PGENULL!");
                                                   
    ReadFile(bitmapfile, &file_header, sizeof(BITMAPFILEHEADER), &bytesread, NULL);
    getConsole().OLn("Read %d bytes of total %d bytes BITMAPFILEHEADER", bytesread, sizeof(BITMAPFILEHEADER));
    ReadFile(bitmapfile, &info_header, sizeof(BITMAPINFOHEADER), &bytesread, NULL);
    getConsole().OLn("Read %d bytes of total %d bytes BITMAPINFOHEADER", bytesread, sizeof(BITMAPINFOHEADER));
    
    if ( info_header.biCompression != BI_RGB )
        return p->loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: info_header.biCompression != BI_RGB, returning PGENULL!");
    
    getConsole().OLn("W x H x Bpp: %d x %d x %d", info_header.biWidth, info_header.biHeight, info_header.biBitCount);

    // Once I managed to save a bmp in Photoshop where biHeight was negative, so we need to make sure everything we have is positive
    info_header.biWidth    = abs(info_header.biWidth);
    info_header.biHeight   = abs(info_header.biHeight);
    info_header.biBitCount = (WORD) abs((int) info_header.biBitCount);

    if ( (bitmaplength = info_header.biWidth * info_header.biHeight * (info_header.biBitCount < 32 ? 3 : 4)) == 0 )
        return p->loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: bitmaplength == 0, returning returning PGENULL!");

    if ( info_header.biClrUsed > 0 )
    {
        palettesize = info_header.biClrUsed * sizeof(RGBQUAD);
        palette = (RGBQUAD*) malloc(palettesize);
        ReadFile(bitmapfile, palette, palettesize, &bytesread, NULL);
        getConsole().OLn("Read %d bytes of total %d bytes of RGBQUAD array for palette", bytesread, palettesize);
        if ( bytesread != palettesize )
            return p->loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: bytesread != palettesize, returning PGENULL!");
    }
    
    pNewImage = new PRREImage();
    pNewImage->SetFilename( filename );
    // PRRE_RGB is just a fake value at this point
    if ( !pNewImage->p->initMembers(info_header.biBitCount, info_header.biWidth, info_header.biHeight,
                                PRRE_RGB, PRRE_RGB, true, false, PGENULL, bitmaplength) )
        return p->loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: initMembers() failed, returning PGENULL!");

    getConsole().OLn("offset: %d", file_header.bfOffBits);

    if ( !pNewImage->p->readBMPpixels(bitmapfile, palette, info_header.biBitCount) )
        return p->loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: readBMPpixels() failed!");

    CloseHandle(bitmapfile);
    free(palette);

    getConsole().SOLnOO("> bitmap successfully loaded into memory, using %d bytes (%d kbytes)",
        sizeof(pNewImage)+pNewImage->getPixelsSize(), (sizeof(*pNewImage)+pNewImage->getPixelsSize())/1024);
    
    return pNewImage;

} // loadBMP()


/**
    From PRREFiledManager, adding W x H x BPP.
*/
void PRREImageManager::WriteListCallback(const PRREManaged& mngd) const
{
    const PRREImage& img = (PRREImage&) mngd;
    PRREFiledManager::WriteListCallback(img);
    getConsole().OIOLnOO("%dx%dx%d", img.getWidth(), img.getHeight(), img.getBitsPerPixels());
} // WriteListCallback()


// ############################### PRIVATE ###############################

