/*
    ###################################################################################
    PRREImage.cpp
    This file is part of PRRE.
    PRREImage class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Material/PRREImageManager.h"
#include "../../include/internal/Material/PRREImageImpl.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../include/external/Hardware/PRREhwInfo.h"

using namespace std;


/*
   PRREImage::PRREImageImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREImage::PRREImageImpl::~PRREImageImpl()
{
    _pOwner->getConsole().OLnOI("~PRREImage() ...");
    FlushResources();
    nImagesTotal--;
    _pOwner->getConsole().SOLnOO("Done!");
    _pOwner = NULL;
} // ~PRREImage()


TPRREuint PRREImage::PRREImageImpl::getWidth() const
{
    return nWidth;
} // getWidth()


TPRREuint PRREImage::PRREImageImpl::getHeight() const
{
    return nHeight;
} // getHeight()


TPRREuint PRREImage::PRREImageImpl::getBitsPerPixels() const
{
    return nBits;
} // getBitsPerPixels()


TPRRE_PIXEL_COMPONENT_ORDER PRREImage::PRREImageImpl::getPixelComponentOrder() const
{
    return clrCompOrder;
} // getPixelComponentOrder()


TPRRE_PIXEL_COMPONENT_ORDER PRREImage::PRREImageImpl::getOriginalPixelComponentOrder() const
{
    return clrCompOrderOrig;
} // getOriginalPixelComponentOrder()


void PRREImage::PRREImageImpl::SetPixelComponentOrder(TPRRE_PIXEL_COMPONENT_ORDER cord)
{
    _pOwner->getConsole().OLn("PRREImage::SetPixelComponentOrder(%PXCO)", cord);

    if ( pPixels == PGENULL )
        return;

    if ( cord == clrCompOrder )
    {
        _pOwner->getConsole().OLn("  WARNING: nothing to do, equal or incompatible component orders!");
        _pOwner->getConsole().OLn("  clrCompOrder == %PXCO", clrCompOrder);
        return;
    }

    if ( (abs(cord-clrCompOrder) > 1) ||
         ( ((cord-clrCompOrder == 1) && (cord % 2 == 0)) ||
           ((cord-clrCompOrder == -1) && (cord % 2 == 1)) ) )
    {
        bChanged = true;
        TPRREuint swapcount = PRREImageManager::getColorConversionSwapCount(cord, clrCompOrder);
        _pOwner->getConsole().OLn("  swapcount = %d", swapcount);
        while ( swapcount > 0 )
        {
            _pOwner->getConsole().OI();
            SwapColors(clrCompOrder, cord, (TPRREbyte) swapcount);
            swapcount--;
            _pOwner->getConsole().OO();
        }
    } // if

    _pOwner->getConsole().OLn("");
} // SetPixelComponentOrder()


PRREColor PRREImage::PRREImageImpl::getPixel(TPRREuint x, TPRREuint y)
{
    PRREColor ret;

    if ( pPixels == PGENULL )
        return ret;

    if ( (x >= nWidth) || (y >= nHeight) )
    {
        return ret;
    }

    TPRREuint index = getIndexToPixel(x,y);
    TPRREbyte rIndex, gIndex, bIndex, aIndex;
    SetColorComponentsIndices(rIndex, gIndex, bIndex, aIndex, clrCompOrder);
    
    ret.Set(pPixels[index+rIndex], pPixels[index+gIndex], pPixels[index+bIndex], aIndex == -1 ? 255 : pPixels[index+aIndex]);

    return ret;
} // getPixel()


PRREColor PRREImage::PRREImageImpl::getPixel(TPRREuint x, TPRREuint y) const
{
    PRREColor ret;

    if ( pPixels == PGENULL )
        return ret;

    if ( (x >= nWidth) || (y >= nHeight) )
        return ret;

    TPRREuint index = getIndexToPixel(x,y);
    TPRREbyte rIndex, gIndex, bIndex, aIndex;
    SetColorComponentsIndices(rIndex, gIndex, bIndex, aIndex, clrCompOrder);
    
    ret.Set(pPixels[index+rIndex], pPixels[index+gIndex], pPixels[index+bIndex], aIndex == -1 ? 255 : pPixels[index+aIndex]);

    return ret;
} // getPixel()


void PRREImage::PRREImageImpl::SetPixel(TPRREuint x, TPRREuint y, TPRREubyte r, TPRREubyte g, TPRREubyte b, TPRREubyte a)                                   
{
    if ( pPixels == PGENULL )
        return;

    if ( (x >= nWidth) || (y >= nHeight) )
    {
        return;
    }

    bChanged = true;
    TPRREuint index = getIndexToPixel(x,y);
    TPRREbyte rIndex, gIndex, bIndex, aIndex;
    SetColorComponentsIndices(rIndex, gIndex, bIndex, aIndex, clrCompOrder);

    pPixels[index+rIndex] = r;
    pPixels[index+gIndex] = g;
    pPixels[index+bIndex] = b;
    if ( aIndex > -1 )
        pPixels[index+aIndex] = a;
} // SetPixel()


void PRREImage::PRREImageImpl::SetPixel(TPRREuint x, TPRREuint y, PRREColor clr)
{
    SetPixel(x,y, clr.getRed(), clr.getGreen(), clr.getBlue(), clr.getAlpha());
} // SetPixel()


TPRREbool PRREImage::PRREImageImpl::isChanged() const
{
    return bChanged;
} // isChanged()


const TPRREubyte* PRREImage::PRREImageImpl::getPixels() const
{
    return pPixels;
} // getPixels()


TPRREubyte* PRREImage::PRREImageImpl::getPixels()
{
    return pPixels;
} // getPixels()


TPRREuint PRREImage::PRREImageImpl::getPixelsSize() const
{
    return nSizePixels;
} // getPixelsSize()


void PRREImage::PRREImageImpl::FlushResources()
{
    if ( pPixels == PGENULL )
        return;

    delete[] pPixels;
    pPixels = PGENULL;
    nSizePixels = 0;
} // FlushResources()


TPRREuint PRREImage::PRREImageImpl::getUsedSystemMemory() const
{
    return ( this->getPixelsSize() + sizeof(*this) );
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


TPRREuint PRREImage::PRREImageImpl::nImagesTotal = 0;


/**
    Saves indices of the color components to the given params [-1 - 3].
    Only the a(lpha) can become -1 if the image has no alpha.
*/
void PRREImage::PRREImageImpl::SetColorComponentsIndices(TPRREbyte& r, TPRREbyte& g, TPRREbyte& b, TPRREbyte& a, const TPIXCOMPORD pxcord)
{
    a = (pxcord % 2) == 1 ? 3 : -1;

    switch (pxcord)
    {
    case PRRE_RGBA: case PRRE_RGB: r = 0; g = 1; b = 2; break;
    case PRRE_RBGA: case PRRE_RBG: r = 0; g = 2; b = 1; break;
    case PRRE_BGRA: case PRRE_BGR: r = 2; g = 1; b = 0; break;
    case PRRE_BRGA: case PRRE_BRG: r = 1; g = 2; b = 0; break;
    case PRRE_GBRA: case PRRE_GBR: r = 2; g = 0; b = 1; break;
    case PRRE_GRBA: case PRRE_GRB: r = 1; g = 0; b = 2; break;
    default: {}
    }
} // SetColorComponentsIndices()


/**
    Returns a 1-step intermediate pixel component order between the given 2.
    Useful if there is 2 steps between the given values.
    If there is less than 2 steps, the 2 given values will be returned.
*/
TPIXCOMPORD PRREImage::PRREImageImpl::getIntermediatePixelCompOrder(TPIXCOMPORD from, TPIXCOMPORD to)
{
    if ( from == PRRE_RGB && ( to == PRRE_GBR || to == PRRE_BRG ) )
        return PRRE_RBG;
    if ( from == PRRE_RBG && ( to == PRRE_GRB || to == PRRE_BGR ) )
        return PRRE_RGB;
    if ( from == PRRE_GRB && ( to == PRRE_RBG || to == PRRE_BGR ) )
        return PRRE_RGB;
    if ( from == PRRE_BGR && ( to == PRRE_RBG || to == PRRE_GRB ) )
        return PRRE_RGB;
    if ( from == PRRE_GBR && ( to == PRRE_RGB || to == PRRE_BRG ) )
        return PRRE_GRB;
    if ( from == PRRE_BRG && ( to == PRRE_RGB || to == PRRE_GBR ) )
        return PRRE_RBG;

    if ( from == PRRE_RGBA && ( to == PRRE_GBRA || to == PRRE_BRGA ) )
        return PRRE_RBGA;
    if ( from == PRRE_RBGA && ( to == PRRE_GRBA || to == PRRE_BGRA ) )
        return PRRE_RGBA;
    if ( from == PRRE_GRBA && ( to == PRRE_RBGA || to == PRRE_BGRA ) )
        return PRRE_RGBA;
    if ( from == PRRE_BGRA && ( to == PRRE_RBGA || to == PRRE_GRBA ) )
        return PRRE_RGBA;
    if ( from == PRRE_GBRA && ( to == PRRE_RGBA || to == PRRE_BRGA ) )
        return PRRE_GRBA;
    if ( from == PRRE_BRGA && ( to == PRRE_RGBA || to == PRRE_GBRA ) )
        return PRRE_RBGA;

    return to;
} // getIntermediatePixelCompOrder()


/**
    Only ImageManager can create it.
    Sets default values to members.
*/
PRREImage::PRREImageImpl::PRREImageImpl(PRREImage* parent)
{
    _pOwner = parent;
    initMembers(0, 0, 0, PRRE_RGBA, PRRE_RGBA, false, false, PGENULL, 0);
    nImagesTotal++;
} // PRREImage()


PRREImage::PRREImageImpl::PRREImageImpl(const PRREImage::PRREImageImpl& img)
{
    // do not copy _pOwner!
    _pOwner = NULL;
    initMembers(img.nBits, img.nWidth, img.nHeight, img.clrCompOrderOrig, img.clrCompOrder,
                img.bUpsideDown, img.bChanged, img.pPixels, img.nSizePixels);
    nImagesTotal++;
} // PRREImage(...)


PRREImage::PRREImageImpl& PRREImage::PRREImageImpl::operator=(const PRREImage::PRREImageImpl& img)
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
TPRREuint PRREImage::PRREImageImpl::getIndexToPixel(TPRREuint x, TPRREuint y) const
{
    const int mplier = (clrCompOrder % 2) == 1 ? 4 : 3;
    TPRREuint y2 = y;
    if ( bUpsideDown )
        y2 = nHeight-y2-1;
    return (y2*nWidth + x) * mplier;
} // getIndexToPixel()


/**
    Transforms the color component order of the image to another.
*/
void PRREImage::PRREImageImpl::SwapColors(TPRRE_PIXEL_COMPONENT_ORDER from, TPRRE_PIXEL_COMPONENT_ORDER to, TPRREbyte swapcount)
{
    _pOwner->getConsole().OLn("PRREImage::SwapColors(%PXCO, %PXCO, %d)", from, to, swapcount);
    if ( swapcount < 1 )
    {
        _pOwner->getConsole().OLn("");
        return;
    }
    else if ( swapcount == 2 )
    {
        to = getIntermediatePixelCompOrder(from, to);
        _pOwner->getConsole().OLn("  New target order is %PXCO.", to);
    }
    
    clrCompOrder = to;
    TPRREbyte dstR, dstG, dstB, dstA;
    SetColorComponentsIndices(dstR, dstG, dstB, dstA, to);

    TPRREbyte srcR, srcG, srcB, srcA;
    SetColorComponentsIndices(srcR, srcG, srcB, srcA, from);

    _pOwner->getConsole().OLn("  dstR, srcR == %d, %d", dstR, srcR);
    _pOwner->getConsole().OLn("  dstG, srcG == %d, %d", dstG, srcG);
    _pOwner->getConsole().OLn("  dstB, srcB == %d, %d", dstB, srcB);

    TPRREbool done = false;
    TPRREubyte tmp = 0;

    TPRREbyte srcIdx, dstIdx;
    for (TPRREbyte i = 0; i <= 2 && !done; i++)
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
            for (TPRREuint x = 0; x < nWidth; x++)
            {
                for (TPRREuint y = 0; y < nHeight; y++)
                {
                    tmp = pPixels[getIndexToPixel(x,y)+srcIdx];
                    pPixels[getIndexToPixel(x,y)+srcIdx] = pPixels[getIndexToPixel(x,y)+dstIdx];
                    pPixels[getIndexToPixel(x,y)+dstIdx] = tmp;
                } // for y
            } // for x
        }
    } // for i

    _pOwner->getConsole().OLn("");
} // SwapColors()


bool PRREImage::PRREImageImpl::initMembers(
    TPRREuint bits, TPRREuint w, TPRREuint h,
    TPIXCOMPORD clrCOrderOrig, TPIXCOMPORD clrCOrder,
    TPRREbool upsDown, TPRREbool chngd,
    TPRREubyte* pxls, TPRREuint npxls )
{
    if ( npxls > 0 )
    {
        try
        {
            pPixels = new TPRREubyte[npxls];
            if ( pxls )
            {
                pPixels = (TPRREubyte*) memcpy(pPixels, pxls, npxls);
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
TPRREbool PRREImage::PRREImageImpl::readBMP32pixels(HANDLE f)
{
    DWORD bytesread;
    // at this point, nSizePixels should already contain the correct value set by loadBMP()
    ReadFile(f, pPixels, nSizePixels, &bytesread, NULL);
    clrCompOrder = PRRE_BGRA;
    clrCompOrderOrig = clrCompOrder;
    nBits = 32;
    return true;
} // readBMP32pixels()


/**
    Used by readBMPpixels() for 24-bpp BMPs.
    @return True on success, false on error.
*/
TPRREbool PRREImage::PRREImageImpl::readBMP24pixels(HANDLE f)
{
    DWORD bytesread;
    ReadFile(f, pPixels, nSizePixels, &bytesread, NULL);
    clrCompOrder = PRRE_BGR;
    clrCompOrderOrig = PRRE_BGR;
    nBits = 24;
    return true;
} // readBMP24pixels()


/**
    Reads palette indices from given file. The buffer returned by this function should be freed by caller.
    Used by readBMPnpixels where n < 24, so where palette is available.
    @return NULL on failure, otherwise pointer to palette indices.
*/
unsigned char* PRREImage::PRREImageImpl::readPaletteIndices(const HANDLE f)
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
            _pOwner->getConsole().EOLn("ERROR: bytesread != nPaletteIndexArraySize: %d != %d", bytesread, nPaletteIndexArraySize);
            delete[] pPaletteIndexArray;
            return NULL;
        }
    }
    catch (const std::bad_alloc&)
    {
        _pOwner->getConsole().EOLn("  ERROR: failed to allocate pPaletteIndexArray!");
    }

    return pPaletteIndexArray;
}


/**
    Used by readBMPpixels() for 8-bpp BMPs.
    @return True on success, false on error.
*/
TPRREbool PRREImage::PRREImageImpl::readBMP8pixels(const HANDLE f, const RGBQUAD* palette)
{
    if ( (nWidth % 4) != 0 )
    {
        _pOwner->getConsole().EOLn("ERROR: 8 bpp image nWidth doesnt divisible by 4 (%d mod 4 == %d)!",
                           nWidth, nWidth % 4);
        return false;
    }

    // we already have the palette, but still need to grab the indices into this palette for each pixel
    unsigned char* const pPaletteIndexArray = readPaletteIndices(f);
    if ( !pPaletteIndexArray )
        return false;
    
    TPRREuint k = 0;
    for (TPRREuint y = 0; y < nHeight; ++y)
        for (TPRREuint x = 0; x < nWidth; ++x)
        {
            for (TPRREuint c = 0; c < 3; ++c)
                pPixels[(y*nWidth+x)*3+c] = ((const TPRREubyte*) palette)[pPaletteIndexArray[k]*4+2-c];
            k++;
        }
    
    delete[] pPaletteIndexArray;
    clrCompOrder = PRRE_RGB;
    clrCompOrderOrig = clrCompOrder;
    nBits = 24;
    return true;
} // readBMP8pixels()


/**
    Used by readBMPpixels() for 4-bpp BMPs.
    @return True on success, false on error.
*/
TPRREbool PRREImage::PRREImageImpl::readBMP4pixels(HANDLE f, const RGBQUAD* palette)
{
    if ( (nWidth % 8) != 0 )
    {
        _pOwner->getConsole().EOLn("ERROR: 4 bpp image nWidth doesnt divisible by 8 (%d mod 8 == %d)!",
                           nWidth, nWidth % 8);
        return false;
    }

    // we already have the palette, but still need to grab the indices into this palette for each pixel
    unsigned char* const pPaletteIndexArray = readPaletteIndices(f);
    if ( !pPaletteIndexArray )
        return false;

    TPRREuint k = 0;
    for (TPRREuint y = 0; y < nHeight; ++y)
    {
        for (TPRREuint x = 0; x < nWidth; x += 2)
        {
            for (int c = 0; c < 3; ++c)
                pPixels[(y*nWidth+x)*3+c] = ((const TPRREubyte*) palette)[pPaletteIndexArray[k]/16*4+2-c];

            for (int c = 0; c < 3; ++c)
                pPixels[(y*nWidth+(x+1))*3+c] = ((const TPRREubyte*) palette)[pPaletteIndexArray[k]%16*4+2-c];

            k++;
        }
    }
    
    delete[] pPaletteIndexArray;
    clrCompOrder = PRRE_RGB;
    clrCompOrderOrig = clrCompOrder;
    nBits = 24;
    return true;
} // readBMP4pixels()


/**
    Used by readBMPpixels() for 1-bpp BMPs.
    @return True on success, false on error.
*/
TPRREbool PRREImage::PRREImageImpl::readBMP1pixels(HANDLE f, const RGBQUAD* palette)
{
    if ( (nWidth % 32) != 0 )
    {
        _pOwner->getConsole().EOLn("ERROR: 1 bpp image nWidth doesnt divisible by 32 (%d mod 32 == %d)!",
                           nWidth, nWidth % 32);
        return false;
    }

    // we already have the palette, but still need to grab the indices into this palette for each pixel
    unsigned char* const pPaletteIndexArray = readPaletteIndices(f);
    if ( !pPaletteIndexArray )
        return false;

    TPRREuint k = 0;
    for (TPRREuint y = 0; y < nHeight; ++y)
        for (TPRREuint x = 0; x < nWidth; x += 8)
        {
            for (TPRREuint x2 = 0; x2 < 8; ++x2)
                for (TPRREuint c = 0; c < 3; ++c)
                    pPixels[(y*nWidth+(x+x2))*3+c] = ((const TPRREubyte*) palette)[((pPaletteIndexArray[k]>>(7-x2))&1)*4+2-c];
            k++;
        }
    
    delete[] pPaletteIndexArray;   
    clrCompOrder = PRRE_RGB;
    clrCompOrderOrig = clrCompOrder;
    nBits = 24;
    return true;
} // readBMP1pixels()


/**
    Reads pixels into memory in correct format.
    @return True on success, false on error.
*/
bool PRREImage::PRREImageImpl::readBMPpixels(HANDLE f, const RGBQUAD* palette, WORD nBitCount)
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
   PRREImage
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREImage::~PRREImage()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREImage()


/**
    Gets the width of the image.
    @return Width of the image in pixels.
*/
TPRREuint PRREImage::getWidth() const
{
    return pImpl->getWidth();
} // getWidth()


/**
    Gets the height of the image.
    @return Height of the image in pixels.
*/
TPRREuint PRREImage::getHeight() const
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
TPRREuint PRREImage::getBitsPerPixels() const
{
    return pImpl->getBitsPerPixels();
} // getBitsPerPixels()


/**
    Gets the actual color component order.
    Same result as getOriginalPixelComponentOrder(), if the order hasn't been changed by SwapColors() or SetPixelComponentOrder() yet.

    @return Pixel component order.
*/
TPRRE_PIXEL_COMPONENT_ORDER PRREImage::getPixelComponentOrder() const
{
    return pImpl->getPixelComponentOrder();
} // getPixelComponentOrder()


/**
    Gets the original color component order.
    For loaded BMPs:
     - 32 bit BMPs: PRRE_BGRA
     - 24 bit BMPs: PRRE_BGR
     - less then 24 bit BMPs: PRRE_RGB.

    @return Original pixel component order.
*/
TPRRE_PIXEL_COMPONENT_ORDER PRREImage::getOriginalPixelComponentOrder() const
{
    return pImpl->getOriginalPixelComponentOrder();
} // getOriginalPixelComponentOrder()


/**
    Transforms the color component order of the image to the given color component order.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).
*/
void PRREImage::SetPixelComponentOrder(TPRRE_PIXEL_COMPONENT_ORDER cord)
{
    pImpl->SetPixelComponentOrder(cord);
} // SetPixelComponentOrder()


/**
    Gets the color of the pixel at the given (x,y) coordinate.

    @return The color of the pixel at the given (x,y) coordinate.
            Black if the pixels are no more in memory.
*/
PRREColor PRREImage::getPixel(TPRREuint x, TPRREuint y)
{
    return pImpl->getPixel(x, y);
} // getPixel()


/**
    Gets the color of the pixel at the given (x,y) coordinate.

    @return The color of the pixel at the given (x,y) coordinate.
            Black if the pixels are no more in memory.
*/
PRREColor PRREImage::getPixel(TPRREuint x, TPRREuint y) const
{
    return pImpl->getPixel(x, y);
} // getPixel()


/**
    Sets the color of the pixel at the given (x,y) coordinate.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).
*/
void PRREImage::SetPixel(TPRREuint x, TPRREuint y, TPRREubyte r, TPRREubyte g, TPRREubyte b, TPRREubyte a)                                   
{
    pImpl->SetPixel(x, y, r, g, b, a);
} // SetPixel()


/**
    Sets the color of the pixel at the given (x,y) coordinate.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).
*/
void PRREImage::SetPixel(TPRREuint x, TPRREuint y, PRREColor clr)
{
    pImpl->SetPixel(x, y, clr);
} // SetPixel()


/**
    True if the pixel data has been changed since loading it from file.
*/
TPRREbool PRREImage::isChanged() const
{
    return pImpl->isChanged();
} // isChanged()


/**
    Gets the pointer to the array of pixels, providing direct access to pixels.
    @return Pointer to array of pixels.
*/
const TPRREubyte* PRREImage::getPixels() const
{
    return pImpl->getPixels();
} // getPixels()


/**
    Gets the pointer to the array of pixels, providing direct access to pixels.
    @return Pointer to array of pixels.
*/
TPRREubyte* PRREImage::getPixels()
{
    return pImpl->getPixels();
} // getPixels()


/**
    Gets the size of the array of pixels.
    @return Size of the array of pixels in Bytes.
            0 if the array has been deleted by FlushResources().
*/
TPRREuint PRREImage::getPixelsSize() const
{
    return pImpl->getPixelsSize();
} // getPixelsSize()


/**
    This can be used if you don't need the pixels of the image to be in the system memory anymore.
*/
void PRREImage::FlushResources()
{
    pImpl->FlushResources();
} // FlushResources()


/**
    Gets the amount of allocated system memory for this image.

    @return Amount of used system memory in Bytes.
*/
TPRREuint PRREImage::getUsedSystemMemory() const
{
    return PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) + sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only ImageManager can create it.
    Sets default values to members.
*/
PRREImage::PRREImage()
{
    getConsole().OLnOI("PRREImage() ...");
    pImpl = new PRREImageImpl(this);
    getConsole().SOLnOO("> Done!");
} // PRREImage()


PRREImage::PRREImage(const PRREImage& img)
    : PRREFiledManaged( img )
{
    getConsole().OLnOI("PRREImage(img) ...");
    pImpl = new PRREImageImpl(*img.pImpl);
    pImpl->_pOwner = this;
    getConsole().SOLnOO("> Done!");
} // PRREImage(...)


PRREImage& PRREImage::operator=(const PRREImage&)
{
    return *this;
}


/**
    Acquires the resources and properties of the given PRREImage object, meaning that
    the given image will lose the pixels and other resources.
    This is a way of moving assets of an image to another image, MOVING, so no array copy is involved.
*/
void PRREImage::Cannibalize(PRREImage& victim)
{
    // Note that we cannot easily copy victim's Impl since copy ctor and assignment operators are empty!
    // Thus the following assignment cannot be used: *(this->pImpl) = *(victim.pImpl)
    // We wouldn't even want to use copy ctor or assignment here since we do not want any array copy.
    // That is why this function is written, to "steal" the arrays of the victim so victim will be "empty".
    // In C++11 this could be done with a move ctor.

    SetName(victim.getName());          // copy the Managed part
    SetFilename(victim.getFilename());  // copy the FiledManaged part

    pImpl->nBits = victim.pImpl->nBits;
    pImpl->nWidth = victim.pImpl->nWidth;
    pImpl->nHeight = victim.pImpl->nHeight;
    pImpl->clrCompOrderOrig = victim.pImpl->clrCompOrderOrig;
    pImpl->clrCompOrder = victim.pImpl->clrCompOrder;
    pImpl->bUpsideDown = victim.pImpl->bUpsideDown;
    pImpl->bChanged = victim.pImpl->bChanged;
    pImpl->nSizePixels = victim.pImpl->nSizePixels;
    pImpl->pPixels = victim.pImpl->pPixels;

    victim.pImpl->nBits = 0;
    victim.pImpl->nWidth = 0;
    victim.pImpl->nHeight = 0;
    victim.pImpl->bChanged = true;
    victim.pImpl->nSizePixels = 0;
    victim.pImpl->pPixels = PGENULL;

}


// ############################### PRIVATE ###############################


