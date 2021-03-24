#pragma once

/*
    ###################################################################################
    PRREImageManager.h
    This file is part of PRRE.
    External header.
    PRREImageManager and PRREImage classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREFiledManager.h"
#include "PRREColor.h"


/**
    Pixel component orders.
*/
enum TPRRE_PIXEL_COMPONENT_ORDER
{
    PRRE_RGB,
    PRRE_RGBA,
    PRRE_RBG,
    PRRE_RBGA,
    PRRE_BRG,
    PRRE_BRGA,
    PRRE_BGR,
    PRRE_BGRA,
    PRRE_GRB,
    PRRE_GRBA,
    PRRE_GBR,
    PRRE_GBRA
}; // TPRRE_PIXEL_COMPONENT_ORDER

typedef TPRRE_PIXEL_COMPONENT_ORDER TPIXCOMPORD;



class PRREImageManager;

/**
    Image class.
*/
class PRREImage :
    public PRREFiledManaged
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREImage is included")
#endif

public:
    virtual ~PRREImage();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */
    
    TPRREuint getWidth() const;            /**< Gets the width of the image. */
    TPRREuint getHeight() const;           /**< Gets the height of the image. */
    TPRREuint getBitsPerPixels() const;    /**< Gets the bit depth of the image. */
    
    TPRRE_PIXEL_COMPONENT_ORDER
        getPixelComponentOrder() const;          /**< Gets the actual color component order. */
    TPRRE_PIXEL_COMPONENT_ORDER
        getOriginalPixelComponentOrder() const;  /**< Gets the original color component order. */
	TPRREbool
        setPixelComponentOrder(
            TPRRE_PIXEL_COMPONENT_ORDER corder); /**< Transforms the color component order of the image to the given color component order. */

    PRREColor getPixel(TPRREuint x, TPRREuint y);        /**< Gets the color of the pixel at the given (x,y) coordinate. */
    PRREColor getPixel(TPRREuint x, TPRREuint y) const;  /**< Gets the color of the pixel at the given (x,y) coordinate. */

    TPRREbool setPixel(TPRREuint x, TPRREuint y,
                  TPRREubyte r, TPRREubyte g, TPRREubyte b,
                  TPRREubyte a = 0);                             /**< Sets the color of the pixel at the given (x,y) coordinate. */
                             
    TPRREbool setPixel(TPRREuint x, TPRREuint y, PRREColor clr); /**< Sets the color of the pixel at the given (x,y) coordinate. */

    TPRREbool isChanged() const;              /**< True if the pixel data has been changed since loading it from file. */

    const TPRREubyte* getPixels() const;      /**< Gets the pointer to the array of pixels, giving direct access to pixels. */
    TPRREubyte*       getPixels();            /**< Gets the pointer to the array of pixels, giving direct access to pixels. */
    TPRREuint         getPixelsSize() const;  /**< Gets the size of the array of pixels. */

    virtual void FlushResources();           /**< This can be used if you don't need the pixels of the image to be in the system memory anymore. */
    
    virtual TPRREuint getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for this image. */

protected:

    // ---------------------------------------------------------------------------

    PRREImage();                                /**< Only ImageManager can create it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    PRREImage(const PRREImage&);
    PRREImage& operator=(const PRREImage&);

private:

    class PRREImageImpl;
    PRREImageImpl* pImpl;

    // ---------------------------------------------------------------------------

    friend class PRREImageManager;
    friend class PRRETextureManager;
}; // class PRREImage


/**
    Image manager class.
*/
class PRREImageManager :
    public PRREFiledManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREImageManager is included")
#endif

public:
    static TPRREuint   getColorConversionSwapCount(
        TPIXCOMPORD from, TPIXCOMPORD to);                                  /**< Calculates the number of swaps needed from the given color component order to the other. */
    static TPIXCOMPORD getMirroredPixelComponentOrder(TPIXCOMPORD corder);  /**< Transforms the given color component order to its mirrored form. */

    // ---------------------------------------------------------------------------
    
    PRREImageManager(); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PRREImageManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    virtual PRREImage* createFromFile(const char* filename);                    /**< Creates an Image object from the given file. */
    PRREImage* createBlank(TPRREuint width, TPRREuint height, TPRREuint bpp);   /**< Creates a blank Image object as specified. */

    virtual void WriteList() const;

protected:

    // ---------------------------------------------------------------------------

    PRREImageManager(const PRREImageManager&);
    PRREImageManager& operator=(const PRREImageManager&);
    
    virtual void WriteListCallback(const PRREManaged& mngd) const;  /**< From PRREFiledManager, adding W x H x BPP. */

private:

    class PRREImageManagerImpl;
    PRREImageManagerImpl* pImpl;

}; // class PRREImageManager
