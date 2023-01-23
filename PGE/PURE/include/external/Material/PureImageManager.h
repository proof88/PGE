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
enum TPURE_PIXEL_COMPONENT_ORDER
{
    PURE_RGB,
    PURE_RGBA,
    PURE_RBG,
    PURE_RBGA,
    PURE_BRG,
    PURE_BRGA,
    PURE_BGR,
    PURE_BGRA,
    PURE_GRB,
    PURE_GRBA,
    PURE_GBR,
    PURE_GBRA
}; // TPURE_PIXEL_COMPONENT_ORDER

typedef TPURE_PIXEL_COMPONENT_ORDER TPIXCOMPORD;



class PureImageManager;

/**
    Image class.
*/
class PureImage :
    public PureFiledManaged
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureImage is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureImage();   

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    
    TPureUInt getWidth() const;            /**< Gets the width of the image. */
    TPureUInt getHeight() const;           /**< Gets the height of the image. */
    TPureUInt getBitsPerPixels() const;    /**< Gets the bit depth of the image. */
    
    TPURE_PIXEL_COMPONENT_ORDER
        getPixelComponentOrder() const;          /**< Gets the actual color component order. */
    TPURE_PIXEL_COMPONENT_ORDER
        getOriginalPixelComponentOrder() const;  /**< Gets the original color component order. */
	TPureBool
        setPixelComponentOrder(
            TPURE_PIXEL_COMPONENT_ORDER corder); /**< Transforms the color component order of the image to the given color component order. */

    PureColor getPixel(TPureUInt x, TPureUInt y);        /**< Gets the color of the pixel at the given (x,y) coordinate. */
    PureColor getPixel(TPureUInt x, TPureUInt y) const;  /**< Gets the color of the pixel at the given (x,y) coordinate. */

    TPureBool setPixel(TPureUInt x, TPureUInt y,
                  TPureUByte r, TPureUByte g, TPureUByte b,
                  TPureUByte a = 0);                             /**< Sets the color of the pixel at the given (x,y) coordinate. */
                             
    TPureBool setPixel(TPureUInt x, TPureUInt y, PureColor clr); /**< Sets the color of the pixel at the given (x,y) coordinate. */

    TPureBool isChanged() const;              /**< True if the pixel data has been changed since loading it from file. */

    const TPureUByte* getPixels() const;      /**< Gets the pointer to the array of pixels, giving direct access to pixels. */
    TPureUByte*       getPixels();            /**< Gets the pointer to the array of pixels, giving direct access to pixels. */
    TPureUInt         getPixelsSize() const;  /**< Gets the size of the array of pixels. */

    virtual void FlushResources();           /**< This can be used if you don't need the pixels of the image to be in the system memory anymore. */
    
    virtual TPureUInt getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for this image. */

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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureImageManager is included")
#endif

public:
    static TPureUInt   getColorConversionSwapCount(
        TPIXCOMPORD from, TPIXCOMPORD to);                                  /**< Calculates the number of swaps needed from the given color component order to the other. */
    static TPIXCOMPORD getMirroredPixelComponentOrder(TPIXCOMPORD corder);  /**< Transforms the given color component order to its mirrored form. */
    
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------
    
    PureImageManager(); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureImageManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual PureImage* createFromFile(const char* filename);                    /**< Creates an Image object from the given file. */
    PureImage* createBlank(TPureUInt width, TPureUInt height, TPureUInt bpp);   /**< Creates a blank Image object as specified. */

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
