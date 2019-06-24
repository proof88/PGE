/*
    ###################################################################################
    PRREImageManager.cpp
    This file is part of PRRE.
    PRREImageManager and PRREImage classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREImageManager.h"
#include "PRREhwInfo.h"

using namespace std;



/*
   PRREImage::PRREImageImpl
   ###########################################################################
*/


class PRREImage::PRREImageImpl
{

public:
    virtual ~PRREImageImpl();
    
    TPRREuint getWidth() const;          
    TPRREuint getHeight() const;          
    TPRREuint getBitsPerPixels() const;    
    
    TPRRE_PIXEL_COMPONENT_ORDER
        getPixelComponentOrder() const;          
    TPRRE_PIXEL_COMPONENT_ORDER
        getOriginalPixelComponentOrder() const;  
	void
        SetPixelComponentOrder(
            TPRRE_PIXEL_COMPONENT_ORDER corder); 

    PRREColor getPixel(TPRREuint x, TPRREuint y);       
    PRREColor getPixel(TPRREuint x, TPRREuint y) const;

    void      SetPixel(TPRREuint x, TPRREuint y,
                  TPRREubyte r, TPRREubyte g, TPRREubyte b,
                  TPRREubyte a = 0);                            
                             
    void      SetPixel(TPRREuint x, TPRREuint y, PRREColor clr);

    TPRREbool isChanged() const;         

    const TPRREubyte* getPixels() const;     
    TPRREubyte*       getPixels();           
    TPRREuint         getPixelsSize() const; 

    virtual void FlushResources();          
    
    virtual TPRREuint getUsedSystemMemory() const;  

protected:

    // ---------------------------------------------------------------------------

private:

    static TPRREuint nImagesTotal;

    // ---------------------------------------------------------------------------

    static void SetColorComponentsIndices(
        TPRREbyte& r,
        TPRREbyte& g,
        TPRREbyte& b,
        TPRREbyte& a,
        TPIXCOMPORD pxcord);  /**< Saves indices of the color components to the given params [-1 - 3]. */

    static TPIXCOMPORD getIntermediatePixelCompOrder(
        TPIXCOMPORD from,
        TPIXCOMPORD to);      /**< Returns a 1-step intermediate pixel component order between the given 2. */

    // ---------------------------------------------------------------------------

    PRREImage*  _pOwner;             /**< The owner public object who creates this pimpl object. */
    TPRREuint   nBits;               /**< Bit depth (number of color bits per pixel). */
    TPRREuint   nWidth;              /**< Width (pixel). */
    TPRREuint   nHeight;             /**< Height (pixel). */
    TPIXCOMPORD clrCompOrderOrig;    /**< Original color component order: right after loading. */
    TPIXCOMPORD clrCompOrder;        /**< Actual color component order (SwapColors(), SetPixelComponentOrder() are changing it). */
    TPRREbool   bUpsideDown;         /**< True if the 1st row is the last row and the last row is the 1st row. */
    TPRREbool   bChanged;            /**< True if the pixel data has been changed since loading it from file. */
    TPRREubyte* pPixels;             /**< Array of pixels. */
    TPRREuint   nSizePixels;         /**< Size of array of pixels. */

    // ---------------------------------------------------------------------------

    explicit PRREImageImpl(PRREImage* parent);             /**< Only Image or ImageManager can create it. */
    PRREImageImpl(const PRREImageImpl&);
    PRREImageImpl& operator=(const PRREImageImpl&);

    TPRREuint getIndexToPixel(
        TPRREuint x, TPRREuint y ) const;      /**< Gets the index in pPixels to the pixel at the given (x,y) coordinate. */
    
    void SwapColors(
        TPRRE_PIXEL_COMPONENT_ORDER from,
        TPRRE_PIXEL_COMPONENT_ORDER to,
        TPRREbyte swapcount );                 /**< Transforms the color component order of the image to another. */

    bool initMembers(
        TPRREuint bits, TPRREuint w, TPRREuint h,
        TPIXCOMPORD clrCOrderOrig, TPIXCOMPORD clrCOrder,
        TPRREbool upsDown, TPRREbool chngd,
        TPRREubyte* pxls, TPRREuint npxls ); 

    TPRREbool  readBMP32pixels(HANDLE f);   /**< Used by readBMPpixels() for 32-bpp BMPs. */

    TPRREbool  readBMP24pixels(HANDLE f);   /**< Used by readBMPpixels() for 24-bpp BMPs. */

    unsigned char* readPaletteIndices(const HANDLE f);  /**< Reads palette indices from given file. */

    TPRREbool  readBMP8pixels(
        HANDLE f, const RGBQUAD* palette);     /**< Used by readBMPpixels() for 8-bpp BMPs. */

    TPRREbool  readBMP4pixels(
        HANDLE f, const RGBQUAD* palette);     /**< Used by readBMPpixels() for 4-bpp BMPs. */

    TPRREbool  readBMP1pixels(
        HANDLE f,
        const RGBQUAD* palette);               /**< Used by readBMPpixels() for 1-bpp BMPs. */

    TPRREbool  readBMPpixels(
        HANDLE f,
        const RGBQUAD* palette,
        WORD nBitCount);                       /**< Reads pixels into memory in correct format. */

    // ---------------------------------------------------------------------------

    friend class PRREImage;
    friend class PRREImageManager;
};


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

    free(pPixels);
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
    _pOwner->getConsole().OLnOI("PRREImage() ...");
    initMembers(0, 0, 0, PRRE_RGBA, PRRE_RGBA, false, false, PGENULL, 0);
    nImagesTotal++;
    _pOwner->getConsole().SOLnOO("> Done!");
} // PRREImage()


PRREImage::PRREImageImpl::PRREImageImpl(const PRREImage::PRREImageImpl& img)
{
    _pOwner->getConsole().OLnOI("PRREImage(img) ...");
    // do not copy _pOwner!
    _pOwner = NULL;
    initMembers(img.nBits, img.nWidth, img.nHeight, img.clrCompOrderOrig, img.clrCompOrder,
                img.bUpsideDown, img.bChanged, img.pPixels, img.nSizePixels);
    nImagesTotal++;
    _pOwner->getConsole().SOLnOO("> Done!");
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
    nBits = bits;
    nWidth = w;
    nHeight = h;
    clrCompOrderOrig = clrCOrderOrig;
    clrCompOrder = clrCOrder;
    bUpsideDown = upsDown;
    bChanged = chngd;
    nSizePixels = npxls;
    if ( nSizePixels > 0 )
    {
        if ( !(pPixels = (TPRREubyte*) malloc(npxls)) )
            return false;
        if ( pxls )
            pPixels = (TPRREubyte*) memcpy(pPixels, pxls, npxls);    
    }
    else
        pPixels = pxls;
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
    const DWORD nPaletteIndexArraySize      = nWidth * nHeight * sizeof(unsigned char) / (8 / nBits);
    unsigned char* const pPaletteIndexArray = (unsigned char*) malloc(nPaletteIndexArraySize);
    DWORD bytesread;
    ReadFile(f, pPaletteIndexArray, nPaletteIndexArraySize, &bytesread, NULL);
    if ( bytesread != nPaletteIndexArraySize )
    {
        _pOwner->getConsole().EOLn("ERROR: bytesread != nPaletteIndexArraySize: %d != %d", bytesread, nPaletteIndexArraySize);
        free(pPaletteIndexArray);
        return NULL;
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
    
    free(pPaletteIndexArray);
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
        for (TPRREuint x = 0; x < nWidth; x += 2)
        {
            for (int c = 0; c < 3; ++c)
                pPixels[(y*nWidth+x)*3+c] = ((const TPRREubyte*) palette)[pPaletteIndexArray[k]/16*4+2-c];

            for (int c = 0; c < 3; ++c)
                pPixels[(y*nWidth+(x+1))*3+c] = ((const TPRREubyte*) palette)[pPaletteIndexArray[k]%16*4+2-c];

            k++;
        }
    
    free(pPaletteIndexArray);
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
    
    free(pPaletteIndexArray);    
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
    delete p;
    p = NULL;
} // ~PRREImage()


/**
    Gets the width of the image.
    @return Width of the image in pixels.
*/
TPRREuint PRREImage::getWidth() const
{
    return p->getWidth();
} // getWidth()


/**
    Gets the height of the image.
    @return Height of the image in pixels.
*/
TPRREuint PRREImage::getHeight() const
{
    return p->getHeight();
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
    return p->getBitsPerPixels();
} // getBitsPerPixels()


/**
    Gets the actual color component order.
    Same result as getOriginalPixelComponentOrder(), if the order hasn't been changed by SwapColors() or SetPixelComponentOrder() yet.

    @return Pixel component order.
*/
TPRRE_PIXEL_COMPONENT_ORDER PRREImage::getPixelComponentOrder() const
{
    return p->getPixelComponentOrder();
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
    return p->getOriginalPixelComponentOrder();
} // getOriginalPixelComponentOrder()


/**
    Transforms the color component order of the image to the given color component order.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).
*/
void PRREImage::SetPixelComponentOrder(TPRRE_PIXEL_COMPONENT_ORDER cord)
{
    p->SetPixelComponentOrder(cord);
} // SetPixelComponentOrder()


/**
    Gets the color of the pixel at the given (x,y) coordinate.

    @return The color of the pixel at the given (x,y) coordinate.
            Black if the pixels are no more in memory.
*/
PRREColor PRREImage::getPixel(TPRREuint x, TPRREuint y)
{
    return p->getPixel(x, y);
} // getPixel()


/**
    Gets the color of the pixel at the given (x,y) coordinate.

    @return The color of the pixel at the given (x,y) coordinate.
            Black if the pixels are no more in memory.
*/
PRREColor PRREImage::getPixel(TPRREuint x, TPRREuint y) const
{
    return p->getPixel(x, y);
} // getPixel()


/**
    Sets the color of the pixel at the given (x,y) coordinate.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).
*/
void PRREImage::SetPixel(TPRREuint x, TPRREuint y, TPRREubyte r, TPRREubyte g, TPRREubyte b, TPRREubyte a)                                   
{
    p->SetPixel(x, y, r, g, b, a);
} // SetPixel()


/**
    Sets the color of the pixel at the given (x,y) coordinate.
    Has no effect if the pixels are no longer in system memory (see FlushResources()).
*/
void PRREImage::SetPixel(TPRREuint x, TPRREuint y, PRREColor clr)
{
    p->SetPixel(x, y, clr);
} // SetPixel()


/**
    True if the pixel data has been changed since loading it from file.
*/
TPRREbool PRREImage::isChanged() const
{
    return p->isChanged();
} // isChanged()


/**
    Gets the pointer to the array of pixels, providing direct access to pixels.
    @return Pointer to array of pixels.
*/
const TPRREubyte* PRREImage::getPixels() const
{
    return p->getPixels();
} // getPixels()


/**
    Gets the pointer to the array of pixels, providing direct access to pixels.
    @return Pointer to array of pixels.
*/
TPRREubyte* PRREImage::getPixels()
{
    return p->getPixels();
} // getPixels()


/**
    Gets the size of the array of pixels.
    @return Size of the array of pixels in Bytes.
            0 if the array has been deleted by FlushResources().
*/
TPRREuint PRREImage::getPixelsSize() const
{
    return p->getPixelsSize();
} // getPixelsSize()


/**
    This can be used if you don't need the pixels of the image to be in the system memory anymore.
*/
void PRREImage::FlushResources()
{
    p->FlushResources();
} // FlushResources()


/**
    Gets the amount of allocated system memory for this image.

    @return Amount of used system memory in Bytes.
*/
TPRREuint PRREImage::getUsedSystemMemory() const
{
    return PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) + sizeof(*this) + p->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only ImageManager can create it.
    Sets default values to members.
*/
PRREImage::PRREImage()
{
    p = new PRREImageImpl(this);
} // PRREImage()


PRREImage::PRREImage(const PRREImage& img)
    : PRREFiledManaged( img )
{
    p = new PRREImageImpl(this);
    *p = *(img.p);
    p->_pOwner = this;
} // PRREImage(...)


PRREImage& PRREImage::operator=(const PRREImage&)
{
    return *this;
}


// ############################### PRIVATE ###############################



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

