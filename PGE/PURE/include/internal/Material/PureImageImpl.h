#pragma once

/*
    ###################################################################################
    PureImageImpl.h
    This file is part of PURE.
    Internal header.
    PureImageImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../external/Material/PureImageManager.h"
#include "gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

// for HANDLE and etc, to be removed ...
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>

class PureImage::PureImageImpl
{

public:
    virtual ~PureImageImpl();
    
    TPureuint getWidth() const;          
    TPureuint getHeight() const;          
    TPureuint getBitsPerPixels() const;    
    
    TPure_PIXEL_COMPONENT_ORDER
        getPixelComponentOrder() const;          
    TPure_PIXEL_COMPONENT_ORDER
        getOriginalPixelComponentOrder() const;  
	TPurebool
        setPixelComponentOrder(
            TPure_PIXEL_COMPONENT_ORDER corder); 

    PureColor getPixel(TPureuint x, TPureuint y);       
    PureColor getPixel(TPureuint x, TPureuint y) const;

    TPurebool setPixel(TPureuint x, TPureuint y,
                  TPureubyte r, TPureubyte g, TPureubyte b,
                  TPureubyte a = 0);                            
                             
    TPurebool setPixel(TPureuint x, TPureuint y, PureColor clr);

    TPurebool isChanged() const;         

    const TPureubyte* getPixels() const;     
    TPureubyte*       getPixels();           
    TPureuint         getPixelsSize() const; 

    virtual void FlushResources();          
    
    virtual TPureuint getUsedSystemMemory() const;  

    TPurebool cannibalize(PureImage& victim);

protected:

    // ---------------------------------------------------------------------------

private:

    static TPureuint nImagesTotal;

    // ---------------------------------------------------------------------------

    static TPurebool setColorComponentsIndices(
        TPurebyte& r,
        TPurebyte& g,
        TPurebyte& b,
        TPurebyte& a,
        TPIXCOMPORD pxcord);  /**< Saves indices of the color components to the given params [-1 - 3]. */

    static TPIXCOMPORD getIntermediatePixelCompOrder(
        TPIXCOMPORD from,
        TPIXCOMPORD to);      /**< Returns a 1-step intermediate pixel component order between the given 2. */

    // ---------------------------------------------------------------------------

    PureImage*  _pOwner;             /**< The owner public object who creates this pimpl object. */
    TPureuint   nBits;               /**< Bit depth (number of color bits per pixel). */
    TPureuint   nWidth;              /**< Width (pixel). */
    TPureuint   nHeight;             /**< Height (pixel). */
    TPIXCOMPORD clrCompOrderOrig;    /**< Original color component order: right after loading. */
    TPIXCOMPORD clrCompOrder;        /**< Actual color component order (swapColors(), setPixelComponentOrder() are changing it). */
    TPurebool   bUpsideDown;         /**< True if the 1st row is the last row and the last row is the 1st row. */
    TPurebool   bChanged;            /**< True if the pixel data has been changed since loading it from file. */
    TPureubyte* pPixels;             /**< Array of pixels. */
    TPureuint   nSizePixels;         /**< Size of array of pixels. */

    // ---------------------------------------------------------------------------

    explicit PureImageImpl(PureImage* parent);             /**< Only Image or ImageManager can create it. */
    PureImageImpl(const PureImageImpl&);
    PureImageImpl& operator=(const PureImageImpl&);

    TPureuint getIndexToPixel(
        TPureuint x, TPureuint y ) const;      /**< Gets the index in pPixels to the pixel at the given (x,y) coordinate. */
    
    TPurebool swapColors(
        TPure_PIXEL_COMPONENT_ORDER from,
        TPure_PIXEL_COMPONENT_ORDER to,
        TPurebyte swapcount );                 /**< Transforms the color component order of the image to another. */

    bool initMembers(
        TPureuint bits, TPureuint w, TPureuint h,
        TPIXCOMPORD clrCOrderOrig, TPIXCOMPORD clrCOrder,
        TPurebool upsDown, TPurebool chngd,
        TPureubyte* pxls, TPureuint npxls ); 

    TPurebool  readBMP32pixels(HANDLE f);   /**< Used by readBMPpixels() for 32-bpp BMPs. */

    TPurebool  readBMP24pixels(HANDLE f);   /**< Used by readBMPpixels() for 24-bpp BMPs. */

    unsigned char* readPaletteIndices(const HANDLE f);  /**< Reads palette indices from given file. */

    TPurebool  readBMP8pixels(
        HANDLE f, const RGBQUAD* palette);     /**< Used by readBMPpixels() for 8-bpp BMPs. */

    TPurebool  readBMP4pixels(
        HANDLE f, const RGBQUAD* palette);     /**< Used by readBMPpixels() for 4-bpp BMPs. */

    TPurebool  readBMP1pixels(
        HANDLE f,
        const RGBQUAD* palette);               /**< Used by readBMPpixels() for 1-bpp BMPs. */

    TPurebool  readBMPpixels(
        HANDLE f,
        const RGBQUAD* palette,
        WORD nBitCount);                       /**< Reads pixels into memory in correct format. */

    // ---------------------------------------------------------------------------

    friend class PureImage;
    friend class PureImageManager;
};
