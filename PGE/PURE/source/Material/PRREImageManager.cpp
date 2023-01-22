/*
    ###################################################################################
    PureImageManager.cpp
    This file is part of Pure.
    PureImageManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Material/PureImageManager.h"
#include "../../include/internal/Material/PureImageImpl.h"
#include "../../include/internal/Purepragmas.h"
#include "../../include/external/Hardware/PureHwInfo.h"

using namespace std;

/*
   PureImageManager::PureImageManagerImpl
   ###########################################################################
*/


class PureImageManager::PureImageManagerImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PureImageManagerImpl();

    PureImage* loadBMP(const char* filename);  /**< This loads BMP files, handles the actual file operations, and creates the Image object. */


protected:
    static PureHwInfo& pHWInfo;             /**< We need some info on HW. */

    // ---------------------------------------------------------------------------
    
private:
    PureImageManager* _pOwner;          /**< The owner public object who creates this pimpl object. */
    TPureuint         nRunningCounter;  /**< Always increased when creating a new Image instance. */

    PureImageManagerImpl(PureImageManager* parent);

    PureImageManagerImpl(const PureImageManagerImpl&);
    PureImageManagerImpl& operator=(const PureImageManagerImpl&);

    PureImage* loadBMPfail(HANDLE f, PureImage* img, RGBQUAD* palette, const char* msg);    /**< Used by loadBMP() when an error occurs. */
    PureImage* createFromFileFail(const char* msg);     /**< Used by createFromFile() when an error occurs. */

    friend class PureImageManager;

};


// ############################### PUBLIC ################################


PureImageManager::PureImageManagerImpl::~PureImageManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PureImageManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();
    _pOwner = NULL;
} // ~PureImageManager()


/**
    This loads BMP files, handles the actual file operations, and creates the Image object.
    Can load any bit depth BMP in theory, but below 16 bits, the rules are the following:
     - at 8 bits (256 colors) the width of the image must be divisible by 4;
     - at 4 bits (16 colors) the width of the image must be divisible by 8;
     - at 1 bits (2 colors) the width of the image must be divisible by 32.

    @return The created Image object on success, PGENULL otherwise.
*/
PureImage* PureImageManager::PureImageManagerImpl::loadBMP(const char* filename)
{
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    DWORD bitmaplength;
    RGBQUAD* palette = NULL;
    DWORD palettesize = 0;
    DWORD bytesread;
    PureImage* pNewImage = NULL;

    HANDLE bitmapfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

    _pOwner->getConsole().OLnOI("PureImageManager::loadBMP(\"%s\")", filename);
    if ( bitmapfile == INVALID_HANDLE_VALUE )
        return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: bitmapfile == INVALID_HANDLE_VALUE, returning PGENULL!");
                                                   
    ReadFile(bitmapfile, &file_header, sizeof(BITMAPFILEHEADER), &bytesread, NULL);
    _pOwner->getConsole().OLn("Read %d bytes of total %d bytes BITMAPFILEHEADER", bytesread, sizeof(BITMAPFILEHEADER));
    ReadFile(bitmapfile, &info_header, sizeof(BITMAPINFOHEADER), &bytesread, NULL);
    _pOwner->getConsole().OLn("Read %d bytes of total %d bytes BITMAPINFOHEADER", bytesread, sizeof(BITMAPINFOHEADER));
    
    if ( info_header.biCompression != BI_RGB )
        return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: info_header.biCompression != BI_RGB, returning PGENULL!");
    
    _pOwner->getConsole().OLn("W x H x Bpp: %d x %d x %d", info_header.biWidth, info_header.biHeight, info_header.biBitCount);

    // Once I managed to save a bmp in Photoshop where biHeight was negative, so we need to make sure everything we have is positive
    info_header.biWidth    = abs(info_header.biWidth);
    info_header.biHeight   = abs(info_header.biHeight);
    info_header.biBitCount = (WORD) abs((int) info_header.biBitCount);

    if ( (bitmaplength = info_header.biWidth * info_header.biHeight * (info_header.biBitCount < 32 ? 3 : 4)) == 0 )
        return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: bitmaplength == 0, returning returning PGENULL!");

    try
    {
        if ( info_header.biClrUsed > 0 )
        {
            palettesize = info_header.biClrUsed * sizeof(RGBQUAD);
            palette = new RGBQUAD[palettesize];
            ReadFile(bitmapfile, palette, palettesize, &bytesread, NULL);
            _pOwner->getConsole().OLn("Read %d bytes of total %d bytes of RGBQUAD array for palette", bytesread, palettesize);
            if ( bytesread != palettesize )
            {
                return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: bytesread != palettesize, returning PGENULL!");
            }
        } 

        pNewImage = new PureImage();
        // Pure_RGB is just a fake value at this point
        if ( !pNewImage->pImpl->initMembers(info_header.biBitCount, info_header.biWidth, info_header.biHeight,
                                    Pure_RGB, Pure_RGB, true, false, PGENULL, bitmaplength) )
            return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: initMembers() failed, returning PGENULL!");

        _pOwner->getConsole().OLn("offset: %d", file_header.bfOffBits);

        if ( !pNewImage->pImpl->readBMPpixels(bitmapfile, palette, info_header.biBitCount) )
            return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: readBMPpixels() failed!");

        CloseHandle(bitmapfile);
        delete[] palette;

        _pOwner->getConsole().SOLnOO("> bitmap successfully loaded into memory, using %d bytes (%d kbytes)",
            sizeof(pNewImage)+pNewImage->getPixelsSize(), (sizeof(*pNewImage)+pNewImage->getPixelsSize())/1024);
    } // try
    catch (const std::bad_alloc&)
    {
        return loadBMPfail(bitmapfile, pNewImage, palette, "ERROR: failed to allocated palette!");
    }
    
    return pNewImage;

} // loadBMP()


// ############################## PROTECTED ##############################


PureHwInfo& PureImageManager::PureImageManagerImpl::pHWInfo = PureHwInfo::get();


// ############################### PRIVATE ###############################


/**
    @param parent Public class instance owning this pimpl object.
*/
PureImageManager::PureImageManagerImpl::PureImageManagerImpl(PureImageManager* parent)
{
    _pOwner = parent;
    _pOwner->getConsole().OLn("PureImageManager() ...");
    nRunningCounter = 0;
    _pOwner->getConsole().OISOLnOO("Done!");
} // PureImageManager(...)


PureImageManager::PureImageManagerImpl::PureImageManagerImpl(const PureImageManager::PureImageManagerImpl&)
{

}


PureImageManager::PureImageManagerImpl& PureImageManager::PureImageManagerImpl::operator=(const PureImageManager::PureImageManagerImpl&)
{
    return *this;
}


/**
    Used by loadBMP() when an error occurs.
    @return PGENULL always.
*/
PureImage* PureImageManager::PureImageManagerImpl::loadBMPfail( HANDLE f, PureImage* img, RGBQUAD* palette, const char* msg)
{
    _pOwner->getConsole().EOLn(msg);
    CloseHandle(f);
    delete img;
    delete[] palette;
    _pOwner->getConsole().OO();
    return PGENULL;
} // loadBMPfail()


/**
    Used by createFromFile() when an error occurs.
    @return PGENULL always.
*/
PureImage* PureImageManager::PureImageManagerImpl::createFromFileFail(const char* msg)
{
    _pOwner->getConsole().EOLn(msg);
    _pOwner->getConsole().OLn("");
    _pOwner->getConsole().OO();
    return PGENULL;
} // createFromFileFail()


/*
   PureImageManager
   ###########################################################################
*/

// ############################### PUBLIC ################################


/**
    Calculates the number of needed swaps needed from the given color component order to the other.
    @return Number of swaps needed from the given color component order to the other.
*/
TPureuint PureImageManager::getColorConversionSwapCount(TPIXCOMPORD from, TPIXCOMPORD to)
{
    if ( (from % 2) == 1 ) 
        from = (TPIXCOMPORD)(from-1);
    if ( (to % 2) == 1 )
        to = (TPIXCOMPORD)(to-1);
    if ( from == to )
        return 0;

    switch (from)
    {
    case Pure_RGB: return (to == Pure_GBR) || (to == Pure_BRG) ? 2 : 1;
    case Pure_RBG: return (to == Pure_GRB) || (to == Pure_BGR) ? 2 : 1;
    case Pure_GRB: return (to == Pure_RBG) || (to == Pure_BGR) ? 2 : 1;
    case Pure_BGR: return (to == Pure_RBG) || (to == Pure_GRB) ? 2 : 1;
    case Pure_GBR: return (to == Pure_RGB) || (to == Pure_BRG) ? 2 : 1;
    default /* BRG*/: return (to == Pure_RGB) || (to == Pure_GBR) ? 2 : 1;
    }
} // getColorConversionSwapCount()


/**
    Transforms the given color component order to its mirrored form.
    @return The mirrored form of the given color component order.
*/
TPIXCOMPORD PureImageManager::getMirroredPixelComponentOrder(TPIXCOMPORD corder)
{
    switch (corder)
    {
    case Pure_RGB : return Pure_BGR;
    case Pure_RBG : return Pure_GBR;
    case Pure_GBR : return Pure_RBG;
    case Pure_GRB : return Pure_BRG;
    case Pure_BGR : return Pure_RGB;
    case Pure_BRG : return Pure_GRB;
    case Pure_RGBA: return Pure_BGRA;
    case Pure_RBGA: return Pure_GBRA;
    case Pure_GBRA: return Pure_RBGA;
    case Pure_GRBA: return Pure_BRGA;
    case Pure_BGRA: return Pure_RGBA;
    case Pure_BRGA: return Pure_GRBA;
    default: return Pure_RGBA;
    }
} // getMirroredPixelComponentOrder()


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureImageManager::PureImageManager() :
    PureFiledManager()
{
    pImpl = new PureImageManagerImpl(this);
} // PureImageManager(...)


PureImageManager::~PureImageManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureImageManager()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureImageManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureImageManager::getLoggerModuleName()
{
    return "PureImageManager";
} // getLoggerModuleName()


/**
    Creates an Image object from the given file.
    Only BMP format is supported at this time.
    Can load any bit depth BMP in theory, but below 16 bits, the rules are the following:
     - at 8 bits (256 colors) the width of the image must be divisible by 4;
     - at 4 bits (16 colors) the width of the image must be divisible by 8;
     - at 1 bits (2 colors) the width of the image must be divisible by 32.

    Note that with 32 bit BMP files you can utilize real alpha component of pixel colors, but
    not all image editing application saves real alpha values into 32 bit color depth bitmap files.
    For example, IrfanView and MS Paint saves 0 or 255 for every pixel's alpha component even if the
    source image was a true transparent PNG.
    But Adobe Flash properly saves transparency data.
    If you want to utilize transparency of 32 bit bitmaps properly, follow the instructions given
    in at PureMaterial::setBlendFuncs().

    @return The created Image object on success, PGENULL otherwise.
*/
PureImage* PureImageManager::createFromFile(const char* filename)
{
    getConsole().OLnOI("PureImageManager::createFromFile(\"%s\")", filename);

    if ( filename == NULL )
        return pImpl->createFromFileFail("ERROR: NULLPOINTER!");

    if ( !PFL::fileExists(filename) )
        return pImpl->createFromFileFail("ERROR: file doesn't exist!");

    string sFileExt = PFL::getExtension(filename);
    if ( sFileExt == "" )
        return pImpl->createFromFileFail("ERROR: no file extension!");

#pragma warning(disable:4244)  /* int-char conversion in std::transform */
    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
#pragma warning(default:4244)
    getConsole().OLn("ext: .%s, ", sFileExt.c_str());
    PureImage* pNewImage;

    if ( sFileExt == "BMP" )
    {
        getConsole().OLn("calling loadBMP()... ");
        pNewImage = pImpl->loadBMP(filename);
    }
    else return pImpl->createFromFileFail("ERROR: unsupported extension!");

    if ( pNewImage == PGENULL )
        return pImpl->createFromFileFail("ERROR: loadXXX() returned PGENULL!");
    
    pNewImage->SetName("Image " + std::to_string(pImpl->nRunningCounter++));
    pNewImage->SetFilename( filename );
    Attach( *pNewImage );

    getConsole().SOLnOO("> Image loaded, name: %s!", pNewImage->getName().c_str());
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
PureImage* PureImageManager::createBlank(TPureuint width, TPureuint height, TPureuint bpp)
{
    if ( (width == 0) || (height == 0) || (bpp != 24) )
        return PGENULL;

    PureImage* pNewImage = PGENULL;
    try
    {
        pNewImage = new PureImage();

        // ppxls = PGENULL means it won't try to copy anything to the allocated buffer so the content of the buffer will be undefined.
        if ( !pNewImage->pImpl->initMembers(bpp, width, height, Pure_RGB, Pure_RGB, true, false, PGENULL, width * height * bpp/8) )
        {
            delete pNewImage;
            getConsole().EOLn("ERROR: Failed initMembers(%d, %d, %d, ...)", bpp, width, height);
            getConsole().OLn("");
            return PGENULL;
        }

        Attach( *pNewImage );

        pNewImage->SetName("Image " + std::to_string(pImpl->nRunningCounter++));

        getConsole().SOLnOO("> Blank Image created, name: %s!", pNewImage->getName().c_str());
        getConsole().OLn("");
    }
    catch (const std::bad_alloc&)
    {
        getConsole().EOLn("ERROR: failed to instantiate PureImage!");
        getConsole().OLn("");
    }

    return pNewImage;
}


// ############################## PROTECTED ##############################


PureImageManager::PureImageManager(const PureImageManager&)
{
    // UNUSED
}


PureImageManager& PureImageManager::operator=(const PureImageManager&)
{
    // UNUSED
    return *this;
}


/**
    From PureFiledManager, adding W x H x BPP.
*/
void PureImageManager::WriteListCallback(const PureManaged& mngd) const
{
    const PureImage& img = (PureImage&) mngd;
    PureFiledManager::WriteListCallback(img);
    getConsole().OIOLnOO("%dx%dx%d bpp", img.getWidth(), img.getHeight(), img.getBitsPerPixels());
} // WriteListCallback()


/**
    From PureFiledManager, adding logging image size data.
*/
void PureImageManager::WriteList() const
{
    getConsole().OLn("PureImageManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PureFiledManager::WriteList();
    getConsole().OO();
}


// ############################### PRIVATE ###############################

