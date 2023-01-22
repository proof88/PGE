#pragma once

/*
    ###################################################################################
    PureImageManager.h
    This file is part of PURE.
    External header.
    PureImageManager and PureImage classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureFiledManager.h"
#include "PureColor.h"


/**
    Pixel component orders.
*/
enum TPure_PIXEL_COMPONENT_ORDER
{
    Pure_RGB,
    Pure_RGBA,
    Pure_RBG,
    Pure_RBGA,
    Pure_BRG,
    Pure_BRGA,
    Pure_BGR,
    Pure_BGRA,
    Pure_GRB,
    Pure_GRBA,
    Pure_GBR,
    Pure_GBRA
}; // TPure_PIXEL_COMPONENT_ORDER

typedef TPure_PIXEL_COMPONENT_ORDER TPIXCOMPORD;



class PureImageManager;

/**
    Image class.
*/
class PureImage :
    public PureFiledManaged
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureImage is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureImage();   

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    
    TPureuint getWidth() const;            /**< Gets the width of the image. */
    TPureuint getHeight() const;           /**< Gets the height of the image. */
    TPureuint getBitsPerPixels() const;    /**< Gets the bit depth of the image. */
    
    TPure_PIXEL_COMPONENT_ORDER
        getPixelComponentOrder() const;          /**< Gets the actual color component order. */
    TPure_PIXEL_COMPONENT_ORDER
        getOriginalPixelComponentOrder() const;  /**< Gets the original color component order. */
	TPurebool
        setPixelComponentOrder(
            TPure_PIXEL_COMPONENT_ORDER corder); /**< Transforms the color component order of the image to the given color component order. */

    PureColor getPixel(TPureuint x, TPureuint y);        /**< Gets the color of the pixel at the given (x,y) coordinate. */
    PureColor getPixel(TPureuint x, TPureuint y) const;  /**< Gets the color of the pixel at the given (x,y) coordinate. */

    TPurebool setPixel(TPureuint x, TPureuint y,
                  TPureubyte r, TPureubyte g, TPureubyte b,
                  TPureubyte a = 0);                             /**< Sets the color of the pixel at the given (x,y) coordinate. */
                             
    TPurebool setPixel(TPureuint x, TPureuint y, PureColor clr); /**< Sets the color of the pixel at the given (x,y) coordinate. */

    TPurebool isChanged() const;              /**< True if the pixel data has been changed since loading it from file. */

    const TPureubyte* getPixels() const;      /**< Gets the pointer to the array of pixels, giving direct access to pixels. */
    TPureubyte*       getPixels();            /**< Gets the pointer to the array of pixels, giving direct access to pixels. */
    TPureuint         getPixelsSize() const;  /**< Gets the size of the array of pixels. */

    virtual void FlushResources();           /**< This can be used if you don't need the pixels of the image to be in the system memory anymore. */
    
    virtual TPureuint getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for this image. */

protected:

    // ---------------------------------------------------------------------------

    PureImage();                                /**< Only ImageManager can create it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    PureImage(const PureImage&);
    PureImage& operator=(const PureImage&);

private:

    class PureImageImpl;
    PureImageImpl* pImpl;

    // ---------------------------------------------------------------------------

    friend class PureImageManager;
    friend class PureTextureManager;
}; // class PureImage


/**
    Image manager class.
*/
class PureImageManager :
    public PureFiledManager
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureImageManager is included")
#endif

public:
    static TPureuint   getColorConversionSwapCount(
        TPIXCOMPORD from, TPIXCOMPORD to);                                  /**< Calculates the number of swaps needed from the given color component order to the other. */
    static TPIXCOMPORD getMirroredPixelComponentOrder(TPIXCOMPORD corder);  /**< Transforms the given color component order to its mirrored form. */
    
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------
    
    PureImageManager(); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureImageManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual PureImage* createFromFile(const char* filename);                    /**< Creates an Image object from the given file. */
    PureImage* createBlank(TPureuint width, TPureuint height, TPureuint bpp);   /**< Creates a blank Image object as specified. */

    virtual void WriteList() const;                   /**< From PureFiledManager, adding logging image size data. */

protected:

    // ---------------------------------------------------------------------------

    PureImageManager(const PureImageManager&);
    PureImageManager& operator=(const PureImageManager&);
    
    virtual void WriteListCallback(const PureManaged& mngd) const;  /**< From PureFiledManager, adding W x H x BPP. */

private:

    class PureImageManagerImpl;
    PureImageManagerImpl* pImpl;

}; // class PureImageManager
