#pragma once

/*
    ###################################################################################
    PRREuiManager.cpp
    This file is part of PRRE.
    PRRE User Interface Manager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREuiManager.h"
#include "Hardware/PRREhwInfo.h"
#include "PRREuiFontWin.h"
#include "PRREGLextensionFuncs.h"


/*
 fontok
 - Gl_Text és Beginning OpenGL Game Programming Source Code.rar\Beginning OpenGL Game Programming Source Code\Chapter11\ sample-k:
   - 1 dlist / 1 char
   - nem kell létezõ bmp hozzá
   - createfont(), wglusefontbitmaps() vagy wglusefontoutlines(), glListBase(), glCallLists()
 - NatureScene és portalengine sample-k:
   - 1 dlist / 1 char
   - létezõ bmp kell
   - mi generáljuk a dlist-eket, triangle strip, 1 nagy textúra van, texcoordok eltérnek
 - FreeType library-vel, élsimított betûk


 todo: gui
 - http://openglgui.sourceforge.net/gui_tut1.html
 - MVC model: http://www.songho.ca/opengl/gl_mvc.html
 - GLUI
 */



/*
   PRREuiManagerImpl
   ###########################################################################
*/

class PRREuiManagerImpl :
    public PRREuiManager
{

public:


    // ---------------------------------------------------------------------------

    virtual ~PRREuiManagerImpl();

    void PRREuiManager::Initialize(HDC wnd_dc);
    void Deinitialize();
    TPRREbool isInitialized() const;

    PRREuiText* addText(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout);
    PRREuiText* addText(const std::string& txt, int x, int y);

    void RemoveText(const std::string& text, int x, int y, int height);

    PRREuiText* text(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout);
    PRREuiText* text(const std::string& txt, int x, int y);

    const std::string& getDefaultFontFace() const;

    void SetDefaultFontFace(const std::string& face);

    int  getDefaultFontSize() const;
    void SetDefaultFontSize(int size);

    bool getDefaultFontBold() const;
    void SetDefaultFontBold(bool bold);

    bool getDefaultFontItalic() const;
    void SetDefaultFontItalic(bool italic);

    bool getDefaultFontUnderline() const;
    void SetDefaultFontUnderline(bool underline);

    bool getDefaultFontStrikeout() const;
    void SetDefaultFontStrikeout(bool strikeout);

    const PRREColor& getDefaultColor() const;
          PRREColor& getDefaultColor();

    void Render();

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    bool bInitialized;
    HDC hDC;
    std::map<unsigned long, PRREuiText> mTexts;
    std::vector<PRREuiFontWin> vFonts;

    // dont define these as constants ... should be set at initialization based on system.
    std::string sDefaultFont;
    int nDefaultFontSize;
    bool bDefaultFontBold;
    bool bDefaultFontItalic;
    bool bDefaultFontUnderline;
    bool bDefaultFontStrikeout;

    PRREColor clrDefault;

    /**
        This is the only usable ctor, this is used by the static createAndGet().
    */
    PRREuiManagerImpl();

    PRREuiManagerImpl(const PRREuiManagerImpl&);
    PRREuiManagerImpl& operator=(const PRREuiManagerImpl&);

    friend class PRREuiManager;

};


// ############################### PUBLIC ################################


PRREuiManagerImpl::~PRREuiManagerImpl()
{

}


/**
    Sets members to real values within the singleton instance.
    Does nothing if wnd_dc is NULL.
*/
void PRREuiManagerImpl::Initialize(HDC wnd_dc)
{
    if ( wnd_dc == NULL )
        return;

    getConsole().OIOLn("PRREuiManager::Initialize() ...");
    getConsole().OI();
    if ( bInitialized )
    {
        getConsole().OLnOO("Already initialized!");     
        getConsole().OO();
        return;
    }

    sDefaultFont = PRRE_UI_MGR_FONT_DEFAULT_FACE;
    nDefaultFontSize = PRRE_UI_MGR_FONT_DEFAULT_SIZE;
    bDefaultFontBold = PRRE_UI_MGR_FONT_DEFAULT_BOLD;
    bDefaultFontItalic = PRRE_UI_MGR_FONT_DEFAULT_ITALIC;
    bDefaultFontUnderline = PRRE_UI_MGR_FONT_DEFAULT_UNDERLINE;
    bDefaultFontStrikeout = PRRE_UI_MGR_FONT_DEFAULT_STRIKEOUT;

    hDC = wnd_dc;
    bInitialized = true;

    if ( bInitialized )
    {
        getConsole().OOSOLn("done!");
        getConsole().OO();
    }
    else
    {
        getConsole().OOEOLn("Failed to initialize 1 or more UI class(es)!");
        getConsole().OO();
        return;
    }
}


/**
    This deletes all UI elements and allocated resources, Initialize() can be called again later.
*/
void PRREuiManagerImpl::Deinitialize()
{
    getConsole().OLn("PRREuiManagerImpl::Deinitialize()");

    if ( !bInitialized )
    {
        getConsole().OLnOO("Already deinitialized!");     
        getConsole().OO();
        return;
    }

    // TODO: setting defaults has been copied from Initialize(), let's moved them to a separate funcion!
    sDefaultFont = PRRE_UI_MGR_FONT_DEFAULT_FACE;
    nDefaultFontSize = PRRE_UI_MGR_FONT_DEFAULT_SIZE;
    bDefaultFontBold = PRRE_UI_MGR_FONT_DEFAULT_BOLD;
    bDefaultFontItalic = PRRE_UI_MGR_FONT_DEFAULT_ITALIC;
    bDefaultFontUnderline = PRRE_UI_MGR_FONT_DEFAULT_UNDERLINE;
    bDefaultFontStrikeout = PRRE_UI_MGR_FONT_DEFAULT_STRIKEOUT;

    mTexts.clear();

    hDC = NULL;
    bInitialized = false;
} // Deinitialize()


/**
    Gets whether we are initialized.
*/
TPRREbool PRREuiManagerImpl::isInitialized() const
{
    return bInitialized;
}


/**
    Adds permanent text to the UI.
    Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
    displayed in all next rendered frames.
*/
PRREuiText* PRREuiManagerImpl::addText(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout)
{
    // here we should search for the font, maybe we have already created it before
    PRREuiFontWin* pUIfontWin = NULL;
    std::vector<PRREuiFontWin>::iterator itFont = vFonts.begin();
    while ( itFont != vFonts.end() )
    {
        if ( (itFont->getFontFaceName() == fontface) && (itFont->getHeight() == height) && (itFont->getBold() == bold) &&
             (itFont->getItalic() == italic) && (itFont->getUnderline() == underline) && (itFont->getStrikeOut() == strikeout) )
            break;
        
        itFont++;
    }
    if ( itFont == vFonts.end() )
    {
        vFonts.push_back( PRREuiFontWin(fontface, height, bold, italic, underline, strikeout, hDC) );
        pUIfontWin = &(vFonts.back());
    }
    else
    {
        pUIfontWin = &(*itFont);
    }

    const unsigned long nHash = PRREuiText::getHash(txt, x, y, height);
    mTexts[nHash] = PRREuiText(txt, x, y, *pUIfontWin);
    mTexts[nHash].SetPermanent(true);
    mTexts[nHash].getColor() = clrDefault;

    return &(mTexts[nHash]);
}


/**
    Adds permanent text to the UI with default properties.
    Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
    displayed in all next rendered frames.
*/
PRREuiText* PRREuiManagerImpl::addText(const std::string& txt, int x, int y)
{
    return addText(txt, x, y, sDefaultFont, nDefaultFontSize, bDefaultFontBold, bDefaultFontItalic, bDefaultFontUnderline, bDefaultFontStrikeout);
}


/**
    Deletes permanent text from the UI.
    Please note that this command is not needed to be used on temporary texts.
*/
void PRREuiManagerImpl::RemoveText(const std::string& text, int x, int y, int height)
{
    const unsigned long hashToFind = PRREuiText::getHash(text, x, y, height);
    mTexts.erase(hashToFind);  // should return 1 on successful deletion, we should check that
    // we should also unittest what happens when trying to delete unexisting key ... erase() should return 0.
}

/**
    Adds temporary text to the UI.
    Temporary texts live until the next rendered frame. This means that in order to display the same temporary text for
    more than 1 frame, you have to issue this command before rendering every single frame. Although there isn't much
    performance issue with using temporary texts during multiple frames, adding permanent text could be a better choice.
*/
PRREuiText* PRREuiManagerImpl::text(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout)
{
    PRREuiText* newText = addText(txt, x, y, fontface, height, bold, italic, underline, strikeout);
    if ( newText )
        newText->SetPermanent( false );

    return newText;
}

/**
    Adds temporary text to the UI.
    Temporary texts live until the next rendered frame. This means that in order to display the same temporary text for
    more than 1 frame, you have to issue this command before rendering every single frame. Although there isn't much
    performance issue with using temporary texts during multiple frames, adding permanent text could be a better choice.
*/
PRREuiText* PRREuiManagerImpl::text(const std::string& txt, int x, int y)
{
    return this->text(txt, x, y, sDefaultFont, nDefaultFontSize, bDefaultFontBold, bDefaultFontItalic, bDefaultFontUnderline, bDefaultFontStrikeout);
}

const std::string& PRREuiManagerImpl::getDefaultFontFace() const
{
    return sDefaultFont;
}

void PRREuiManagerImpl::SetDefaultFontFace(const std::string& face)
{
    sDefaultFont = face;    
}

int PRREuiManagerImpl::getDefaultFontSize() const
{
    return nDefaultFontSize;
}

void PRREuiManagerImpl::SetDefaultFontSize(int size)
{
    nDefaultFontSize = size;    
}

bool PRREuiManagerImpl::getDefaultFontBold() const
{
    return bDefaultFontBold;
}

void PRREuiManagerImpl::SetDefaultFontBold(bool bold)
{
    bDefaultFontBold = bold;    
}

bool PRREuiManagerImpl::getDefaultFontItalic() const
{
    return bDefaultFontItalic;
}

void PRREuiManagerImpl::SetDefaultFontItalic(bool italic)
{
    bDefaultFontItalic = italic;    
}

bool PRREuiManagerImpl::getDefaultFontUnderline() const
{
    return bDefaultFontUnderline;
}

void PRREuiManagerImpl::SetDefaultFontUnderline(bool underline)
{
    bDefaultFontUnderline = underline;    
}

bool PRREuiManagerImpl::getDefaultFontStrikeout() const
{
    return bDefaultFontStrikeout;
}

void PRREuiManagerImpl::SetDefaultFontStrikeout(bool strikeout)
{
    bDefaultFontStrikeout = strikeout;    
}

/**
    Returns reference to default color.
    The default color is black by default.
*/
const PRREColor& PRREuiManagerImpl::getDefaultColor() const
{
    return clrDefault;
}

/**
    Returns reference to default color.
    The default color is black by default.
*/
PRREColor& PRREuiManagerImpl::getDefaultColor()
{
    return clrDefault;
}

void PRREuiManagerImpl::Render()
{
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    // we hide texts having alpha too low, this way legacy proofps won't have problem
    // this is because some texts are still displayed by proofps even if they have alpha == 0.0f
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.1f);

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE0_ARB);
    }
    glDisable(GL_TEXTURE_2D);

    for (std::map<unsigned long, PRREuiText>::const_iterator it = mTexts.begin(); it != mTexts.end(); )
    {
        it->second.PrintText();

        if ( !(it->second.getPermanent()) )
            mTexts.erase( it++ );
        else
            ++it ;
    }
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    This is the only usable ctor, this is used by the static createAndGet().
*/
PRREuiManagerImpl::PRREuiManagerImpl()
{
    bInitialized = false;
    hDC = NULL;
    clrDefault.Set(255, 255, 255, 255);
}


PRREuiManagerImpl::PRREuiManagerImpl(const PRREuiManagerImpl&)
{

}


PRREuiManagerImpl& PRREuiManagerImpl::operator=(const PRREuiManagerImpl&)
{
    return *this;
}


/*
   PRREuiManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PRREuiManager& PRREuiManager::createAndGet()
{
    static PRREuiManagerImpl uiMgrInstance;
    return uiMgrInstance;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################






