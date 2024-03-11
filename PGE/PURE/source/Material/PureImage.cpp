/*
    ###################################################################################
    PureImage.cpp
    This file is part of PURE.
    PureImage class.
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
   PureImage::PureImageImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureImage::PureImageImpl::~PureImageImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PureImage() ...");
    FlushResources();
    nImagesTotal--;
    _pOwner->getManagedConsole().SOLnOO("Done!");
    _pOwner = NULL;
} // ~PureImage()


TPureUInt PureImage::PureImageImpl::getWidth() const
{
    return nWidth;
} // getWidth()


TPureUInt PureImage::PureImageImpl::getHeight() const
{
    return nHeight;
} // getHeight()


TPureUInt PureImage::PureImageImpl::getBitsPerPixels() const
{
    return nBits;
} // getBitsPerPixels()


TPURE_PIXEL_COMPONENT_ORDER PureImage::PureImageImpl::getPixelComponentOrder() const
{
    return clrCompOrder;
} // getPixelComponentOrder()


TPURE_PIXEL_COMPONENT_ORDER PureImage::PureImageImpl::getOriginalPixelComponentOrder() const
{
    return clrCompOrderOrig;
} // getOriginalPixelComponentOrder()


TPureBool PureImage::PureImageImpl::setPixelComponentOrder(TPURE_PIXEL_COMPONENT_ORDER cord)
{
    _pOwner->getManagedConsole().OLn("PureImage::setPixelComponentOrder(%PXCO)", cord);

    if ( pPixels == PGENULL )
        return false;

    if ( cord == clrCompOrder )
    {
        _pOwner->getManagedConsole().OLn("  WARNING: nothing to do, equal component orders!");
        _pOwner->getManagedConsole().OLn("  clrCompOrder == %PXCO", clrCompOrder);
        return true;
    }

    bool retVal = true;
    if ( (abs(cord-clrCompOrder) > 1) ||
         ( ((cord-clrCompOrder == 1) && (cord % 2 == 0)) ||
           ((cord-clrCompOrder == -1) && (cord % 2 == 1)) ) )
    {
        bChanged = true;
        TPureUInt swapcount = PureImageManager::getColorConversionSwapCount(cord, clrCompOrder);
        _pOwner->getManagedConsole().OLn("  swapcount = %d", swapcount);
        while ( swapcount > 0 )
        {
            _pOwner->getManagedConsole().OI();
            retVal &= swapColors(clrCompOrder, cord, (TPureByte) swapcount);
            swapcount--;
            _pOwner->getManagedConsole().OO();
        }
    }

    _pOwner->getManagedConsole().OLn("");
    return retVal;
} // setPixelComponentOrder()


PureColor PureImage::PureImageImpl::getPixel(TPureUInt x, TPureUInt y)
{
    PureColor ret;

    if ( pPixels == PGENULL )
        return ret;

    if ( (x >= nWidth) || (y >= nHeight) )
        return ret;

    TPureUInt index = getIndexToPixel(x,y);
    TPureByte rIndex, gIndex, bIndex, aIndex;
    if ( !setColorComponentsIndices(rIndex, gIndex, bIndex, aIndex, clrCompOrder) )
        return ret;
    
    ret.Set(pPixels[index+rIndex], pPixels[index+gIndex], pPixels[index+bIndex], aIndex == -1 ? 255 : pPixels[index+aIndex]);

    return ret;
} // getPixel()


PureColor PureImage::PureImageImpl::getPixel(TPureUInt x, TPureUInt y) const
{
    PureColor ret;

    if ( pPixels == PGENULL )
        return ret;

    if ( (x >= nWidth) || (y >= nHeight) )
        return ret;

    TPureUInt index = getIndexToPixel(x,y);
    TPureByte rIndex, gIndex, bIndex, aIndex;
    if ( !setColorComponentsIndices(rIndex, gIndex, bIndex, aIndex, clrCompOrder) )
        return ret;
    
    ret.Set(pPixels[index+rIndex], pPixels[index+gIndex], pPixels[index+bIndex], aIndex == -1 ? 255 : pPixels[index+aIndex]);

    return ret;
} // getPixel()


TPureBool PureImage::PureImageImpl::setPixel(TPureUInt x, TPureUInt y, TPureUByte r, TPureUByte g, TPureUByte b, TPureUByte a)                                   
{
    if ( pPixels == PGENULL )
        return false;

    if ( (x >= nWidth) || (y >= nHeight) )
        return false;

    TPureUInt index = getIndexToPixel(x,y);
    TPureByte rIndex, gIndex, bIndex, aIndex;
    if ( !setColorComponentsIndices(rIndex, gIndex, bIndex, aIndex, clrCompOrder) )
        return false;

    bChanged = true;
    pPixels[index+rIndex] = r;
    pPixels[index+gIndex] = g;
    pPixels[index+bIndex] = b;
    if ( aIndex > -1 )
        pPixels[index+aIndex] = a;
    return true;
} // setPixel()


TPureBool PureImage::PureImageImpl::setPixel(TPureUInt x, TPureUInt y, PureColor clr)
{
    return setPixel(x,y, clr.getRed(), clr.getGreen(), clr.getBlue(), clr.getAlpha());
} // setPixel()


TPureBool PureImage::PureImageImpl::isChanged() const
{
    return bChanged;
} // isChanged()


const TPureUByte* PureImage::PureImageImpl::getPixels() const
{
    return pPixels;
} // getPixels()


TPureUByte* PureImage::PureImageImpl::getPixels()
{
    return pPixels;
} // getPixels()


TPureUInt PureImage::PureImageImpl::getPixelsSize() const
{
    return nSizePixels;
} // getPixelsSize()


void PureImage::PureImageImpl::FlushResources()
{
    if ( pPixels == PGENULL )
        return;

    delete[] pPixels;
    pPixels = PGENULL;
    nSizePixels = 0;
} // FlushResources()


TPureUInt PureImage::PureImageImpl::getUsedSystemMemory() const
{
    return ( this->getPixelsSize() + sizeof(*this) );
} // getUsedSystemMemory()


/**
    Acquires the resources and properties of the given PureImage object, meaning that
    the given image will lose the pixels and other resources.
    This is a way of moving assets of an image to another image, MOVING, so no array copy is involved.

    @return True on success, false on failure.
*/
TPureBool PureImage::PureImageImpl::cannibalize(PureImage& victim)
{
    // Note that we cannot easily copy victim's Impl since copy ctor and assignment operators are empty!
    // Thus the following assignment cannot be used: *pImpl = *(victim.pImpl)
    // We wouldn't even want to use copy ctor or assignment here since we do not want any array copy.
    // That is why this function is written, to "steal" the arrays of the victim so victim will be "empty".
    // In C++11 this could be done with a move ctor.

    _pOwner->SetName(victim.getName());          // copy the Managed part
    _pOwner->SetFilename(victim.getFilename());  // copy the FiledManaged part

    nBits = victim.pImpl->nBits;
    nWidth = victim.pImpl->nWidth;
    nHeight = victim.pImpl->nHeight;
    clrCompOrderOrig = victim.pImpl->clrCompOrderOrig;
    clrCompOrder = victim.pImpl->clrCompOrder;
    bUpsideDown = victim.pImpl->bUpsideDown;
    bChanged = victim.pImpl->bChanged;
    nSizePixels = victim.pImpl->nSizePixels;
    pPixels = victim.pImpl->pPixels;

    victim.pImpl->nBits = 0;
    victim.pImpl->nWidth = 0;
    victim.pImpl->nHeight = 0;
    victim.pImpl->bChanged = true;
    victim.pImpl->nSizePixels = 0;
    victim.pImpl->pPixels = PGENULL;

    return true;

}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


TPureUInt PureImage::PureImageImpl::nImagesTotal = 0;


/**
    Saves indices of the color components to the given params [-1 - 3].
    Only the a(lpha) can become -1 if the image has no alpha. That is not an error scenario.
    @return True on success, false otherwise.
*/
TPureBool PureImage::PureImageImpl::setColorComponentsIndices(TPureByte& r, TPureByte& g, TPureByte& b, TPureByte& a, const TPIXCOMPORD pxcord)
{
    a = (pxcord % 2) == 1 ? 3 : -1;

    switch (pxcord)
    {
    case PURE_RGBA: case PURE_RGB: r = 0; g = 1; b = 2; break;
    case PURE_RBGA: case PURE_RBG: r = 0; g = 2; b = 1; break;
    case PURE_BGRA: case PURE_BGR: r = 2; g = 1; b = 0; break;
    case PURE_BRGA: case PURE_BRG: r = 1; g = 2; b = 0; break;
    case PURE_GBRA: case PURE_GBR: r = 2; g = 0; b = 1; break;
    case PURE_GRBA: case PURE_GRB: r = 1; g = 0; b = 2; break;
    default: return false;
    }
    return true;
} // setColorComponentsIndices()


/**
    Returns a 1-step intermediate pixel component order between the given 2.
    Useful if there is 2 steps between the given values.
    If there is less than 2 steps, the 2 given values will be returned.
*/
TPIXCOMPORD PureImage::PureImageImpl::getIntermediatePixelCompOrder(TPIXCOMPORD from, TPIXCOMPORD to)
{
    if ( from == PURE_RGB && ( to == PURE_GBR || to == PURE_BRG ) )
        return PURE_RBG;
    if ( from == PURE_RBG && ( to == PURE_GRB || to == PURE_BGR ) )
        return PURE_RGB;
    if ( from == PURE_GRB && ( to == PURE_RBG || to == PURE_BGR ) )
        return PURE_RGB;
    if ( from == PURE_BGR && ( to == PURE_RBG || to == PURE_GRB ) )
        return PURE_RGB;
    if ( from == PURE_GBR && ( to == PURE_RGB || to == PURE_BRG ) )
        return PURE_GRB;
    if ( from == PURE_BRG && ( to == PURE_RGB || to == PURE_GBR ) )
        return PURE_RBG;

    if ( from == PURE_RGBA && ( to == PURE_GBRA || to == PURE_BRGA ) )
        return PURE_RBGA;
    if ( from == PURE_RBGA && ( to == PURE_GRBA || to == PURE_BGRA ) )
        return PURE_RGBA;
    if ( from == PURE_GRBA && ( to == PURE_RBGA || to == PURE_BGRA ) )
        return PURE_RGBA;
    if ( from == PURE_BGRA && ( to == PURE_RBGA || to == PURE_GRBA ) )
        return PURE_RGBA;
    if ( from == PURE_GBRA && ( to == PURE_RGBA || to == PURE_BRGA ) )
        return PURE_GRBA;
    if ( from == PURE_BRGA && ( to == PURE_RGBA || to == PURE_GBRA ) )
        return PURE_RBGA;

    return to;
} // getIntermediatePixelCompOrder()


/**
    Only ImageManager can create it.
    Sets default values to members.
*/
PureImage::PureImageImpl::PureImageImpl(PureImage* parent)
{
    _pOwner = parent;
    initMembers(0, 0, 0, PURE_RGBA, PURE_RGBA, false, false, PGENULL, 0);
    nImagesTotal++;
} // PureImage()


PureImage::PureImageImpl::PureImageImpl(const PureImage::PureImageImpl& img)
{
    // do not copy _pOwner!
    _pOwner = NULL;
    initMembers(img.nBits, img.nWidth, img.nHeight, img.clrCompOrderOrig, img.clrCompOrder,
                img.bUpsideDown, img.bChanged, img.pPixels, img.nSizePixels);
    nImagesTotal++;
} // PureImage(...)


PureImage::PureImageImpl& PureImage::PureImageImpl::operator=(const PureImage::PureImageImpl& img)
{
    // do not copy _pOwner!
    _pOwner = NULL;
    initMembers(img.nBits, img.nWidth, img.nHeight, img.clrCompOrderOrig, img.clrCompOrder,
                img.bUpsideDown, img.bChanged, img.pPixels, img.nSizePixels);
    return *this;
}


/**
    Gets the index in pPixels to the pixel at the given (x,y) coordinate.
*/
TPureUInt PureImage::PureImageImpl::getIndexToPixel(TPureUInt x, TPureUInt y) const
{
    const int mplier = (clrCompOrder % 2) == 1 ? 4 : 3;
    TPureUInt y2 = y;
    if ( bUpsideDown )
        y2 = nHeight-y2-1;
    return (y2*nWidth + x) * mplier;
} // getIndexToPixel()


/**
    Transforms the color component order of the image to another.
    @return True on success, false otherwise.
*/
TPureBool PureImage::PureImageImpl::swapColors(TPURE_PIXEL_COMPONENT_ORDER from, TPURE_PIXEL_COMPONENT_ORDER to, TPureByte swapcount)
{
    _pOwner->getManagedConsole().OLn("PureImage::swapColors(%PXCO, %PXCO, %d)", from, to, swapcount);
    if ( swapcount < 1 )
    {
        _pOwner->getManagedConsole().OLn("");
        return true;
    }
    else if ( swapcount == 2 )
    {
        to = getIntermediatePixelCompOrder(from, to);
        _pOwner->getManagedConsole().OLn("  New target order is %PXCO.", to);
    }
    
    TPureByte dstR, dstG, dstB, dstA;
    if ( !setColorComponentsIndices(dstR, dstG, dstB, dstA, to) )
        return false;

    TPureByte srcR, srcG, srcB, srcA;
    if ( !setColorComponentsIndices(srcR, srcG, srcB, srcA, from) )
        return false;

    _pOwner->getManagedConsole().OLn("  dstR, srcR == %d, %d", dstR, srcR);
    _pOwner->getManagedConsole().OLn("  dstG, srcG == %d, %d", dstG, srcG);
    _pOwner->getManagedConsole().OLn("  dstB, srcB == %d, %d", dstB, srcB);

    clrCompOrder = to;

    TPureBool done = false;
    TPureUByte tmp = 0;

    TPureByte srcIdx, dstIdx;
    for (TPureByte i = 0; i <= 2 && !done; i++)
    {
        switch (i)
        {
        case 0: srcIdx = srcR;
                dstIdx = dstR;
                break;
        case 1: srcIdx = srcG;
                dstIdx = dstG;
                break;
        default: srcIdx = srcB;
                 dstIdx = dstB;
        }
        if ( dstIdx-srcIdx != 0 )
        {
            done = true;
            for (TPureUInt x = 0; x < nWidth; x++)
            {
                for (TPureUInt y = 0; y < nHeight; y++)
                {
                    tmp = pPixels[getIndexToPixel(x,y)+srcIdx];
                    pPixels[getIndexToPixel(x,y)+srcIdx] = pPixels[getIndexToPixel(x,y)+dstIdx];
                    pPixels[getIndexToPixel(x,y)+dstIdx] = tmp;
                } // for y
            } // for x
        }
    } // for i

    _pOwner->getManagedConsole().OLn("");
    return true;
} // swapColors()


bool PureImage::PureImageImpl::initMembers(
    TPureUInt bits, TPureUInt w, TPureUInt h,
    TPIXCOMPORD clrCOrderOrig, TPIXCOMPORD clrCOrder,
    TPureBool upsDown, TPureBool chngd,
    TPureUByte* pxls, TPureUInt npxls )
{
    if ( npxls > 0 )
    {
        try
        {
            pPixels = new TPureUByte[npxls];
            if ( pxls )
            {
                pPixels = (TPureUByte*) memcpy(pPixels, pxls, npxls);
            }
        }
        catch (const std::bad_alloc&)
        {
            return false;
        }
    }
    else
    {
        pPixels = pxls;
    }
    nBits = bits;
    nWidth = w;
    nHeight = h;
    clrCompOrderOrig = clrCOrderOrig;
    clrCompOrder = clrCOrder;
    bUpsideDown = upsDown;
    bChanged = chngd;
    nSizePixels = npxls;
    return true;
} // initMembers()


/**
    Used by readBMPpixels() for 32-bpp BMPs.
    @return True on success, false on error.
*/
TPureBool PureImage::PureImageImpl::readBMP32pixels(HANDLE f)
{
    DWORD bytesread;
    // at this point, nSizePixels should already contain the correct value set by loadBMP()
    ReadFile(f, pPixels, nSizePixels, &bytesread, NULL);
    clrCompOrder = PURE_BGRA;
    clrCompOrderOrig = clrCompOrder;
    nBits = 32;
    return true;
} // readBMP32pixels()


/**
    Used by readBMPpixels() for 24-bpp BMPs.
    @return True on success, false on error.
*/
TPureBool PureImage::PureImageImpl::readBMP24pixels(HANDLE f)
{
    DWORD bytesread;
    ReadFile(f, pPixels, nSizePixels, &bytesread, NULL);
    clrCompOrder = PURE_BGR;
    clrCompOrderOrig = PURE_BGR;
    nBits = 24;
    return true;
} // readBMP24pixels()


/**
    Reads palette indices from given file. The buffer returned by this function should be freed by caller.
    Used by readBMPnpixels where n < 24, so where palette is available.
    @return NULL on failure, otherwise pointer to palette indices.
*/
unsigned char* PureImage::PureImageImpl::readPaletteIndices(const HANDLE f)
{
    const DWORD nPaletteIndexArraySize = nWidth * nHeight * sizeof(unsigned char) / (8 / nBits);
    unsigned char* pPaletteIndexArray = NULL;
    try
    {
        pPaletteIndexArray = new unsigned char[nPaletteIndexArraySize];
        DWORD bytesread;
        ReadFile(f, pPaletteIndexArray, nPaletteIndexArraySize, &bytesread, NULL);
        if ( bytesread != nPaletteIndexArraySize )
        {
            _pOwner->getManagedConsole().EOLn("ERROR: bytesread != nPaletteIndexArraySize: %d != %d", bytesread, nPaletteIndexArraySize);
            delete[] pPaletteIndexArray;
            return NULL;
        }
    }
    catch (const std::bad_alloc&)
    {
        _pOwner->getManagedConsole().EOLn("  ERROR: failed to allocate pPaletteIndexArray!");
    }

    return pPaletteIndexArray;
}


/**
    Used by readBMPpixels() for 8-bpp BMPs.
    @return True on success, false on error.
*/
TPureBool PureImage::PureImageImpl::readBMP8pixels(const HANDLE f, const RGBQUAD* palette)
{
    if ( (nWidth % 4) != 0 )
    {
        _pOwner->getManagedConsole().EOLn("ERROR: 8 bpp image nWidth doesnt divisible by 4 (%d mod 4 == %d)!",
                           nWidth, nWidth % 4);
        return false;
    }

    // we already have the palette, but still need to grab the indices into this palette for each pixel
    unsigned char* const pPaletteIndexArray = readPaletteIndices(f);
    if ( !pPaletteIndexArray )
        return false;
    
    TPureUInt k = 0;
    for (TPureUInt y = 0; y < nHeight; ++y)
        for (TPureUInt x = 0; x < nWidth; ++x)
        {
            for (TPureUInt c = 0; c < 3; ++c)
                pPixels[(y*nWidth+x)*3+c] = ((const TPureUByte*) palette)[pPaletteIndexArray[k]*4+2-c];
            k++;
        }
    
    delete[] pPaletteIndexArray;
    clrCompOrder = PURE_RGB;
    clrCompOrderOrig = clrCompOrder;
    nBits = 24;
    return true;
} // readBMP8pixels()


/**
    Used by readBMPpixels() for 4-bpp BMPs.
    @return True on success, false on error.
*/
TPureBool PureImage::PureImageImpl::readBMP4pixels(HANDLE f, const RGBQUAD* palette)
{
    if ( (nWidth % 8) != 0 )
    {
        _pOwner->getManagedConsole().EOLn("ERROR: 4 bpp image nWidth doesnt divisible by 8 (%d mod 8 == %d)!",
                           nWidth, nWidth % 8);
        return false;
    }

    // we already have the palette, but still need to grab the indices into this palette for each pixel
    unsigned char* const pPaletteIndexArray = readPaletteIndices(f);
    if ( !pPaletteIndexArray )
        return false;

    TPureUInt k = 0;
    for (TPureUInt y = 0; y < nHeight; ++y)
    {
        for (TPureUInt x = 0; x < nWidth; x += 2)
        {
            for (int c = 0; c < 3; ++c)
                pPixels[(y*nWidth+x)*3+c] = ((const TPureUByte*) palette)[pPaletteIndexArray[k]/16*4+2-c];

            for (int c = 0; c < 3; ++c)
                pPixels[(y*nWidth+(x+1))*3+c] = ((const TPureUByte*) palette)[pPaletteIndexArray[k]%16*4+2-c];

            k++;
        }
    }
    
    delete[] pPaletteIndexArray;
    clrCompOrder = PURE_RGB;
    clrCompOrderOrig = clrCompOrder;
    nBits = 24;
    return true;
} // readBMP4pixels()


/**
    Used by readBMPpixels() for 1-bpp BMPs.
    @return True on success, false on error.
*/
TPureBool PureImage::PureImageImpl::readBMP1pixels(HANDLE f, const RGBQUAD* palette)
{
    if ( (nWidth % 32) != 0 )
    {
        _pOwner->getManagedConsole().EOLn("ERROR: 1 bpp image nWidth doesnt divisible by 32 (%d mod 32 == %d)!",
                           nWidth, nWidth % 32);
        return false;
    }

    // we already have the palette, but still need to grab the indices into this palette for each pixel
    unsigned char* const pPaletteIndexArray = readPaletteIndices(f);
    if ( !pPaletteIndexArray )
        return false;

    TPureUInt k = 0;
    for (TPureUInt y = 0; y < nHeight; ++y)
        for (TPureUInt x = 0; x < nWidth; x += 8)
        {
            for (TPureUInt x2 = 0; x2 < 8; ++x2)
                for (TPureUInt c = 0; c < 3; ++c)
                    pPixels[(y*nWidth+(x+x2))*3+c] = ((const TPureUByte*) palette)[((pPaletteIndexArray[k]>>(7-x2))&1)*4+2-c];
            k++;
        }
    
    delete[] pPaletteIndexArray;   
    clrCompOrder = PURE_RGB;
    clrCompOrderOrig = clrCompOrder;
    nBits = 24;
    return true;
} // readBMP1pixels()


/**
    Reads pixels into memory in correct format.
    @return True on success, false on error.
*/
bool PureImage::PureImageImpl::readBMPpixels(HANDLE f, const RGBQUAD* palette, WORD nBitCount)
{
    switch ( nBitCount )
    {
    case 32: return readBMP32pixels(f);
    case 24: return readBMP24pixels(f);
    case  8: return readBMP8pixels(f, palette);
    case  4: return readBMP4pixels(f, palette);
    default /*case 1*/: return readBMP1pixels(f, palette);
    }
} // readBMPpixels()


/*
   PureImage
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureImage::~PureImage()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureImage()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureImage::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureImage::getLoggerModuleName()
{
    return "PureImage";
} // getLoggerModuleName()


/**
    Gets the width of the image.
    @return Width of the image in pixels.
*/
TPureUInt PureImage::getWidth() const
{
    return pImpl->getWidth();
} // getWidth()


/**
    Gets the height of the image.
    @return Height of the image in pixels.
*/
TPureUInt PureImage::getHeight() const
{
    return pImpl->getHeight();
} // getHeight()


/**
    Gets the bit depth of the image.
    For loaded BMP images:
     - for 32 bit BMPs: 32
     - less than 32 bit BMPs: 24.

    @return Bit depth of the image (bits per pixel).
*/
TPureUInt PureImage::getBitsPerPixels() const
{
    return pImpl->getBitsPerPixels();
} // getBitsPerPixels()


/**
    Gets the actual color component order.
    Same result as getOriginalPixelComponentOrder(), if the order hasn't been changed by swapColors() or setPixelComponentOrder() yet.

    @return Pixel component order.
*/
TPURE_PIXEL_COMPONENT_ORDER PureImage::getPixelComponentOrder() const
{
    return pImpl->getPixelComponentOrder();
} // getPixelComponentOrder()


/**
    Gets the original color component order.
    For loaded BMPs:
     - 32 bit BMPs: PURE_BGRA
     - 24 bit BMPs: PURE_BGR
     - less then 24 bit BMPs: PURE_RGB.

    @return Original pixel component order.
*/
TPURE_PIXEL_COMPONENT_ORDER PureImage::getOriginalPixelComponentOrder() const
{
    return pImpl->getOriginalPixelComponentOrder();
} // getOriginalPixelComponentOrder()


/**
    Transforms the color component order of the image to the given color component order.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).

    @return True on success, false otherwise.
*/
TPureBool PureImage::setPixelComponentOrder(TPURE_PIXEL_COMPONENT_ORDER cord)
{
    return pImpl->setPixelComponentOrder(cord);
} // setPixelComponentOrder()


/**
    Gets the color of the pixel at the given (x,y) coordinate.

    @return The color of the pixel at the given (x,y) coordinate.
            Black if the pixels are no more in memory.
*/
PureColor PureImage::getPixel(TPureUInt x, TPureUInt y)
{
    return pImpl->getPixel(x, y);
} // getPixel()


/**
    Gets the color of the pixel at the given (x,y) coordinate.

    @return The color of the pixel at the given (x,y) coordinate.
            Black if the pixels are no more in memory.
*/
PureColor PureImage::getPixel(TPureUInt x, TPureUInt y) const
{
    return pImpl->getPixel(x, y);
} // getPixel()


/**
    Sets the color of the pixel at the given (x,y) coordinate.
    Has no effect if the pixels are no longer in system memory (see FlushResources()). This is considered as an error.
    @return True on success, false otherwise.
*/
TPureBool PureImage::setPixel(TPureUInt x, TPureUInt y, TPureUByte r, TPureUByte g, TPureUByte b, TPureUByte a)                                   
{
    return pImpl->setPixel(x, y, r, g, b, a);
} // setPixel()


/**
    Sets the color of the pixel at the given (x,y) coordinate.
    Has no effect if the pixels are no longer in system memory (see FlushResources()). This is considered as an error.
    @return True on success, false otherwise.
*/
TPureBool PureImage::setPixel(TPureUInt x, TPureUInt y, PureColor clr)
{
    return pImpl->setPixel(x, y, clr);
} // setPixel()


/**
    True if the pixel data has been changed since loading it from file.
*/
TPureBool PureImage::isChanged() const
{
    return pImpl->isChanged();
} // isChanged()


/**
    Gets the pointer to the array of pixels, providing direct access to pixels.
    @return Pointer to array of pixels.
*/
const TPureUByte* PureImage::getPixels() const
{
    return pImpl->getPixels();
} // getPixels()


/**
    Gets the pointer to the array of pixels, providing direct access to pixels.
    @return Pointer to array of pixels.
*/
TPureUByte* PureImage::getPixels()
{
    return pImpl->getPixels();
} // getPixels()


/**
    Gets the size of the array of pixels.
    @return Size of the array of pixels in Bytes.
            0 if the array has been deleted by FlushResources().
*/
TPureUInt PureImage::getPixelsSize() const
{
    return pImpl->getPixelsSize();
} // getPixelsSize()


/**
    This can be used if you don't need the pixels of the image to be in the system memory anymore.
*/
void PureImage::FlushResources()
{
    pImpl->FlushResources();
} // FlushResources()


/**
    Gets the amount of allocated system memory for this image.

    @return Amount of used system memory in Bytes.
*/
TPureUInt PureImage::getUsedSystemMemory() const
{
    return PureFiledManaged::getUsedSystemMemory() - sizeof(PureFiledManaged) + sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only ImageManager can create it.
    Sets default values to members.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureImage::PureImage()
{
    getManagedConsole().OLnOI("PureImage() ...");
    pImpl = new PureImageImpl(this);
    getManagedConsole().SOLnOO("> Done!");
} // PureImage()


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureImage::PureImage(const PureImage& img)
    : PureFiledManaged( img )
{
    getManagedConsole().OLnOI("PureImage(img) ...");
    pImpl = new PureImageImpl(*img.pImpl);
    pImpl->_pOwner = this;
    getManagedConsole().SOLnOO("> Done!");
} // PureImage(...)


PureImage& PureImage::operator=(const PureImage&)
{
    // UNUSED
    return *this;
}


// ############################### PRIVATE ###############################


