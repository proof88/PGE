/*
    ###################################################################################
    PureImageManager.cpp
    This file is part of PURE.
    PureImageManager class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Material/PureImageManager.h"
#include "../../include/internal/Material/PureImageImpl.h"
#include "../../include/internal/PurePragmas.h"
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
    TPureUInt         nRunningCounter;  /**< Always increased when creating a new Image instance. */

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
        // PURE_RGB is just a fake value at this point
        if ( !pNewImage->pImpl->initMembers(info_header.biBitCount, info_header.biWidth, info_header.biHeight,
                                    PURE_RGB, PURE_RGB, true, false, PGENULL, bitmaplength) )
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
TPureUInt PureImageManager::getColorConversionSwapCount(TPIXCOMPORD from, TPIXCOMPORD to)
{
    if ( (from % 2) == 1 ) 
        from = (TPIXCOMPORD)(from-1);
    if ( (to % 2) == 1 )
        to = (TPIXCOMPORD)(to-1);
    if ( from == to )
        return 0;

    switch (from)
    {
    case PURE_RGB: return (to == PURE_GBR) || (to == PURE_BRG) ? 2 : 1;
    case PURE_RBG: return (to == PURE_GRB) || (to == PURE_BGR) ? 2 : 1;
    case PURE_GRB: return (to == PURE_RBG) || (to == PURE_BGR) ? 2 : 1;
    case PURE_BGR: return (to == PURE_RBG) || (to == PURE_GRB) ? 2 : 1;
    case PURE_GBR: return (to == PURE_RGB) || (to == PURE_BRG) ? 2 : 1;
    default /* BRG*/: return (to == PURE_RGB) || (to == PURE_GBR) ? 2 : 1;
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
    case PURE_RGB : return PURE_BGR;
    case PURE_RBG : return PURE_GBR;
    case PURE_GBR : return PURE_RBG;
    case PURE_GRB : return PURE_BRG;
    case PURE_BGR : return PURE_RGB;
    case PURE_BRG : return PURE_GRB;
    case PURE_RGBA: return PURE_BGRA;
    case PURE_RBGA: return PURE_GBRA;
    case PURE_GBRA: return PURE_RBGA;
    case PURE_GRBA: return PURE_BRGA;
    case PURE_BGRA: return PURE_RGBA;
    case PURE_BRGA: return PURE_GRBA;
    default: return PURE_RGBA;
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
    not all image editing applications save real alpha values into 32 bit color depth bitmap files.

    For example, IrfanView and MS Paint save 0 or 255 for every pixel's alpha component even if the
    source image was a true transparent PNG: 0 for background and 255 for foreground pixels. This can be ok in some cases,
    but usually not good enough from quality perspective in most cases, since the edges of a shape will look sharp.

    Adobe Flash properly saves transparency data to 32 bit color depth bitmap file without any hassle.

    Adobe Photoshop CS2 is a bit tricky but can work properly. If you open a transparent PNG without background, or
    remove background of an image, it will look transparent, but when you save that into a 32 bit depth BMP file, 0 alpha
    will be set for all pixels which basically makes this BMP useless for us from transparency perspective.
    To work around this problem, you also need to manually edit the transparency channel on the Channels tab.
    By default it is black (0) even when background is removed.
    I read the trick here: https://stackoverflow.com/questions/7369649/how-to-convert-32-bit-bmp-to-contain-alpha-channel

    In Photoshop, removing the background and saving to a different format like PNG would automatically save alpha values
    properly without the need of manually editing the transparency channel, but for a proper BMP we need to deal with this hassle.
    Note that loading the true transparent BMP again in Photoshop will also work fine, although there will be white background
    displayed but transparency channel will be properly loaded from the 32bit BMP.
    
    If you want to utilize transparency of 32 bit bitmaps properly, follow the instructions given
    in at PureMaterial::setBlendFuncs().

    The BMP loader implemented in PURE is not compatible with all BMP formats, and this might lead to texture misalignment
    once you create texture from such BMP file and apply it to an Object3D instance.
    One case of such misalignment issue is if you export the BMP with color space information.
    Some applications like GIMP give option to skip writing color space information, so a workaround for this issue can be if
    you are selecting the option "Do not write color space information": https://github.com/proof88/PRooFPS-dd/issues/271 .
    
    If you do not have such control over the BMP exporting procedure, then as another workaround you need to manually
    adjust the texture UV-coordinates. Texture UV-coordinates can be accessed using PureMaterial::getTexcoords().
    Note that in case the geometry of your PureObject3D instance is already stored in video memory, you need to re-upload geometry
    to video memory after correcting the texture coordinates. This can be done by setting the already set vertex transfer mode
    again for the PureObject3D instace, e.g.:

        PureObject3D* obj = (... create your object ...);
        (... adjust texture coordinates by writing to obj.getMaterial().getTexcoords() ...);
        obj.setVertexTransferMode( obj.getVertexTransferMode() );

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
PureImage* PureImageManager::createBlank(TPureUInt width, TPureUInt height, TPureUInt bpp)
{
    if ( (width == 0) || (height == 0) || (bpp != 24) )
        return PGENULL;

    PureImage* pNewImage = PGENULL;
    try
    {
        pNewImage = new PureImage();

        // ppxls = PGENULL means it won't try to copy anything to the allocated buffer so the content of the buffer will be undefined.
        if ( !pNewImage->pImpl->initMembers(bpp, width, height, PURE_RGB, PURE_RGB, true, false, PGENULL, width * height * bpp/8) )
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

