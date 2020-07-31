#pragma once

/*
    ###################################################################################
    PRREImageImpl.h
    This file is part of PRRE.
    Internal header.
    PRREImageImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREImageManager.h"
#include "gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

// for HANDLE and etc, to be removed ...
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>

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
