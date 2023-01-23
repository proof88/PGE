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
    
    TPureUInt getWidth() const;          
    TPureUInt getHeight() const;          
    TPureUInt getBitsPerPixels() const;    
    
    TPURE_PIXEL_COMPONENT_ORDER
        getPixelComponentOrder() const;          
    TPURE_PIXEL_COMPONENT_ORDER
        getOriginalPixelComponentOrder() const;  
	TPureBool
        setPixelComponentOrder(
            TPURE_PIXEL_COMPONENT_ORDER corder); 

    PureColor getPixel(TPureUInt x, TPureUInt y);       
    PureColor getPixel(TPureUInt x, TPureUInt y) const;

    TPureBool setPixel(TPureUInt x, TPureUInt y,
                  TPureUByte r, TPureUByte g, TPureUByte b,
                  TPureUByte a = 0);                            
                             
    TPureBool setPixel(TPureUInt x, TPureUInt y, PureColor clr);

    TPureBool isChanged() const;         

    const TPureUByte* getPixels() const;     
    TPureUByte*       getPixels();           
    TPureUInt         getPixelsSize() const; 

    virtual void FlushResources();          
    
    virtual TPureUInt getUsedSystemMemory() const;  

    TPureBool cannibalize(PureImage& victim);

protected:

    // ---------------------------------------------------------------------------

private:

    static TPureUInt nImagesTotal;

    // ---------------------------------------------------------------------------

    static TPureBool setColorComponentsIndices(
        TPureByte& r,
        TPureByte& g,
        TPureByte& b,
        TPureByte& a,
        TPIXCOMPORD pxcord);  /**< Saves indices of the color components to the given params [-1 - 3]. */

    static TPIXCOMPORD getIntermediatePixelCompOrder(
        TPIXCOMPORD from,
        TPIXCOMPORD to);      /**< Returns a 1-step intermediate pixel component order between the given 2. */

    // ---------------------------------------------------------------------------

    PureImage*  _pOwner;             /**< The owner public object who creates this pimpl object. */
    TPureUInt   nBits;               /**< Bit depth (number of color bits per pixel). */
    TPureUInt   nWidth;              /**< Width (pixel). */
    TPureUInt   nHeight;             /**< Height (pixel). */
    TPIXCOMPORD clrCompOrderOrig;    /**< Original color component order: right after loading. */
    TPIXCOMPORD clrCompOrder;        /**< Actual color component order (swapColors(), setPixelComponentOrder() are changing it). */
    TPureBool   bUpsideDown;         /**< True if the 1st row is the last row and the last row is the 1st row. */
    TPureBool   bChanged;            /**< True if the pixel data has been changed since loading it from file. */
    TPureUByte* pPixels;             /**< Array of pixels. */
    TPureUInt   nSizePixels;         /**< Size of array of pixels. */

    // ---------------------------------------------------------------------------

    explicit PureImageImpl(PureImage* parent);             /**< Only Image or ImageManager can create it. */
    PureImageImpl(const PureImageImpl&);
    PureImageImpl& operator=(const PureImageImpl&);

    TPureUInt getIndexToPixel(
        TPureUInt x, TPureUInt y ) const;      /**< Gets the index in pPixels to the pixel at the given (x,y) coordinate. */
    
    TPureBool swapColors(
        TPURE_PIXEL_COMPONENT_ORDER from,
        TPURE_PIXEL_COMPONENT_ORDER to,
        TPureByte swapcount );                 /**< Transforms the color component order of the image to another. */

    bool initMembers(
        TPureUInt bits, TPureUInt w, TPureUInt h,
        TPIXCOMPORD clrCOrderOrig, TPIXCOMPORD clrCOrder,
        TPureBool upsDown, TPureBool chngd,
        TPureUByte* pxls, TPureUInt npxls ); 

    TPureBool  readBMP32pixels(HANDLE f);   /**< Used by readBMPpixels() for 32-bpp BMPs. */

    TPureBool  readBMP24pixels(HANDLE f);   /**< Used by readBMPpixels() for 24-bpp BMPs. */

    unsigned char* readPaletteIndices(const HANDLE f);  /**< Reads palette indices from given file. */

    TPureBool  readBMP8pixels(
        HANDLE f, const RGBQUAD* palette);     /**< Used by readBMPpixels() for 8-bpp BMPs. */

    TPureBool  readBMP4pixels(
        HANDLE f, const RGBQUAD* palette);     /**< Used by readBMPpixels() for 4-bpp BMPs. */

    TPureBool  readBMP1pixels(
        HANDLE f,
        const RGBQUAD* palette);               /**< Used by readBMPpixels() for 1-bpp BMPs. */

    TPureBool  readBMPpixels(
        HANDLE f,
        const RGBQUAD* palette,
        WORD nBitCount);                       /**< Reads pixels into memory in correct format. */

    // ---------------------------------------------------------------------------

    friend class PureImage;
    friend class PureImageManager;
};
