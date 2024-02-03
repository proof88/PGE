#pragma once

/*
    ###################################################################################
    PureUiManager.cpp
    This file is part of PURE.
    Pure User Interface Manager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../include/external/PureUiManager.h"

#include <cassert>

#include "imgui.h"  // this is from the external imgui directory because this we share with apps too

#include "../include/external/Hardware/PureHwInfo.h"
#include "../include/internal/PurePragmas.h"
#include "../include/internal/PureUiFontWin.h"
#include "../include/internal/PureGLextensionFuncs.h"


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
   PureuiManagerImpl
   ###########################################################################
*/

class PureuiManagerImpl :
    public PureUiManager
{

public:


    // ---------------------------------------------------------------------------

    virtual ~PureuiManagerImpl();

    void Initialize(HDC wnd_dc) override;
    void Deinitialize() override;
    TPureBool isInitialized() const override;

    const std::function<void()>& getGuiDrawCallback() const override;
    void setGuiDrawCallback(const std::function<void()>& cb) override;

    PureUiText* addText(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) override;
    PureUiText* addText(const std::string& txt, int x, int y) override;

    void RemoveText(const std::string& text, int x, int y, int height) override;
    void RemoveAllPermanentTexts() override;

    PureUiText* text(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) override;
    PureUiText* text(const std::string& txt, int x, int y) override;

    const std::string& getDefaultFontFace() const override;

    void SetDefaultFontFace(const std::string& face) override;

    int  getDefaultFontSize() const override;
    void SetDefaultFontSize(int size) override;

    bool getDefaultFontBold() const override;
    void SetDefaultFontBold(bool bold) override;

    bool getDefaultFontItalic() const override;
    void SetDefaultFontItalic(bool italic) override;

    bool getDefaultFontUnderline() const override;
    void SetDefaultFontUnderline(bool underline) override;

    bool getDefaultFontStrikeout() const override;
    void SetDefaultFontStrikeout(bool strikeout) override;

    const PureColor& getDefaultColor() const override;
          PureColor& getDefaultColor() override;

    void Render() override;

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    bool bInitialized;
    HDC hDC;
    std::map<unsigned long, PureUiText> mTexts;
    std::vector<PureUiFontWin*> vFonts;

    std::function<void()> pfGuiCallback;

    // dont define these as constants ... should be set at initialization based on system.
    std::string sDefaultFont;
    int nDefaultFontSize;
    bool bDefaultFontBold;
    bool bDefaultFontItalic;
    bool bDefaultFontUnderline;
    bool bDefaultFontStrikeout;

    PureColor clrDefault;

    /**
        This is the only usable ctor, this is used by the static createAndGet().
    */
    PureuiManagerImpl();

    PureuiManagerImpl(const PureuiManagerImpl&);
    PureuiManagerImpl& operator=(const PureuiManagerImpl&);

    friend class PureUiManager;

};


// ############################### PUBLIC ################################


PureuiManagerImpl::~PureuiManagerImpl()
{

}


/**
    Sets members to real values within the singleton instance.
    Does nothing if wnd_dc is NULL.
*/
void PureuiManagerImpl::Initialize(HDC wnd_dc)
{
    if ( wnd_dc == NULL )
        return;

    getConsole().OIOLn("PureUiManager::Initialize() ...");
    getConsole().OI();
    if ( bInitialized )
    {
        getConsole().OLnOO("Already initialized!");     
        getConsole().OO();
        return;
    }

    sDefaultFont = PURE_UI_MGR_FONT_DEFAULT_FACE;
    nDefaultFontSize = PURE_UI_MGR_FONT_DEFAULT_SIZE;
    bDefaultFontBold = PURE_UI_MGR_FONT_DEFAULT_BOLD;
    bDefaultFontItalic = PURE_UI_MGR_FONT_DEFAULT_ITALIC;
    bDefaultFontUnderline = PURE_UI_MGR_FONT_DEFAULT_UNDERLINE;
    bDefaultFontStrikeout = PURE_UI_MGR_FONT_DEFAULT_STRIKEOUT;

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
void PureuiManagerImpl::Deinitialize()
{
    getConsole().OLn("PureuiManagerImpl::Deinitialize()");

    if ( !bInitialized )
    {
        getConsole().OLnOO("Already deinitialized!");     
        getConsole().OO();
        return;
    }

    pfGuiCallback = nullptr;

    // TODO: setting defaults has been copied from Initialize(), let's moved them to a separate funcion!
    sDefaultFont = PURE_UI_MGR_FONT_DEFAULT_FACE;
    nDefaultFontSize = PURE_UI_MGR_FONT_DEFAULT_SIZE;
    bDefaultFontBold = PURE_UI_MGR_FONT_DEFAULT_BOLD;
    bDefaultFontItalic = PURE_UI_MGR_FONT_DEFAULT_ITALIC;
    bDefaultFontUnderline = PURE_UI_MGR_FONT_DEFAULT_UNDERLINE;
    bDefaultFontStrikeout = PURE_UI_MGR_FONT_DEFAULT_STRIKEOUT;

    mTexts.clear();
    for (auto pFont : vFonts)
        delete pFont;
    vFonts.clear();

    hDC = NULL;
    bInitialized = false;
} // Deinitialize()


/**
    Gets whether we are initialized.
*/
TPureBool PureuiManagerImpl::isInitialized() const
{
    return bInitialized;
}


const std::function<void()>& PureuiManagerImpl::getGuiDrawCallback() const
{
    return pfGuiCallback;
}


void PureuiManagerImpl::setGuiDrawCallback(const std::function<void()>& cb)
{
    pfGuiCallback = cb;
}


/**
    Adds permanent text to the UI.
    Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
    displayed in all next rendered frames.
*/
PureUiText* PureuiManagerImpl::addText(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout)
{
    // here we should search for the font, maybe we have already created it before
    PureUiFontWin* pUIfontWin = NULL;
    std::vector<PureUiFontWin*>::iterator itFont = vFonts.begin();
    while ( itFont != vFonts.end() )
    {
        PureUiFontWin* piFont = *itFont;
        
        assert(piFont);
        if (!piFont)
        {
            getConsole().EOLn("PureuiManagerImpl::addText(...) nullptr within vFonts!");
            continue;
        }

        if ( (piFont->getFontFaceName() == fontface) && (piFont->getHeight() == height) && (piFont->getBold() == bold) &&
             (piFont->getItalic() == italic) && (piFont->getUnderline() == underline) && (piFont->getStrikeOut() == strikeout) )
            break;
        
        itFont++;
    }
    if ( itFont == vFonts.end() )
    {
        pUIfontWin = new PureUiFontWin(fontface, height, bold, italic, underline, strikeout, hDC);
        vFonts.push_back( pUIfontWin );
    }
    else
    {
        pUIfontWin = *itFont;
    }

    const unsigned long nHash = PureUiText::getHash(txt, x, y, height);
    mTexts[nHash] = PureUiText(txt, x, y, *pUIfontWin);
    mTexts[nHash].SetPermanent(true);
    mTexts[nHash].getColor() = clrDefault;

    return &(mTexts[nHash]);
}


/**
    Adds permanent text to the UI with default properties.
    Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
    displayed in all next rendered frames.
*/
PureUiText* PureuiManagerImpl::addText(const std::string& txt, int x, int y)
{
    return addText(txt, x, y, sDefaultFont, nDefaultFontSize, bDefaultFontBold, bDefaultFontItalic, bDefaultFontUnderline, bDefaultFontStrikeout);
}


/**
    Deletes permanent text from the UI.
    Please note that this command is not needed to be used on temporary texts.
*/
void PureuiManagerImpl::RemoveText(const std::string& text, int x, int y, int height)
{
    const unsigned long hashToFind = PureUiText::getHash(text, x, y, height);
    mTexts.erase(hashToFind);  // should return 1 on successful deletion, we should check that
    // we should also unittest what happens when trying to delete unexisting key ... erase() should return 0.
}

/**
    Deletes all permanent texts from the UI.
*/
void PureuiManagerImpl::RemoveAllPermanentTexts()
{
    for (auto it = mTexts.begin(); it != mTexts.end(); )
    {
        if (it->second.getPermanent())
            it = mTexts.erase(it);
        else
            ++it;
    }
}

/**
    Adds temporary text to the UI.
    Temporary texts live until the next rendered frame. This means that in order to display the same temporary text for
    more than 1 frame, you have to issue this command before rendering every single frame. Although there isn't much
    performance issue with using temporary texts during multiple frames, adding permanent text could be a better choice.
*/
PureUiText* PureuiManagerImpl::text(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout)
{
    PureUiText* newText = addText(txt, x, y, fontface, height, bold, italic, underline, strikeout);
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
PureUiText* PureuiManagerImpl::text(const std::string& txt, int x, int y)
{
    return this->text(txt, x, y, sDefaultFont, nDefaultFontSize, bDefaultFontBold, bDefaultFontItalic, bDefaultFontUnderline, bDefaultFontStrikeout);
}

const std::string& PureuiManagerImpl::getDefaultFontFace() const
{
    return sDefaultFont;
}

void PureuiManagerImpl::SetDefaultFontFace(const std::string& face)
{
    sDefaultFont = face;    
}

int PureuiManagerImpl::getDefaultFontSize() const
{
    return nDefaultFontSize;
}

void PureuiManagerImpl::SetDefaultFontSize(int size)
{
    nDefaultFontSize = size;    
}

bool PureuiManagerImpl::getDefaultFontBold() const
{
    return bDefaultFontBold;
}

void PureuiManagerImpl::SetDefaultFontBold(bool bold)
{
    bDefaultFontBold = bold;    
}

bool PureuiManagerImpl::getDefaultFontItalic() const
{
    return bDefaultFontItalic;
}

void PureuiManagerImpl::SetDefaultFontItalic(bool italic)
{
    bDefaultFontItalic = italic;    
}

bool PureuiManagerImpl::getDefaultFontUnderline() const
{
    return bDefaultFontUnderline;
}

void PureuiManagerImpl::SetDefaultFontUnderline(bool underline)
{
    bDefaultFontUnderline = underline;    
}

bool PureuiManagerImpl::getDefaultFontStrikeout() const
{
    return bDefaultFontStrikeout;
}

void PureuiManagerImpl::SetDefaultFontStrikeout(bool strikeout)
{
    bDefaultFontStrikeout = strikeout;    
}

/**
    Returns reference to default color.
    The default color is black by default.
*/
const PureColor& PureuiManagerImpl::getDefaultColor() const
{
    return clrDefault;
}

/**
    Returns reference to default color.
    The default color is black by default.
*/
PureColor& PureuiManagerImpl::getDefaultColor()
{
    return clrDefault;
}

void PureuiManagerImpl::Render()
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

    if ( PureHwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE0_ARB);
    }
    glDisable(GL_TEXTURE_2D);

    for (std::map<unsigned long, PureUiText>::const_iterator it = mTexts.begin(); it != mTexts.end(); )
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
PureuiManagerImpl::PureuiManagerImpl()
{
    bInitialized = false;
    hDC = NULL;
    clrDefault.Set(255, 255, 255, 255);
}


PureuiManagerImpl::PureuiManagerImpl(const PureuiManagerImpl&)
{

}


PureuiManagerImpl& PureuiManagerImpl::operator=(const PureuiManagerImpl&)
{
    return *this;
}


/*
   PureUiManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PureUiManager& PureUiManager::createAndGet()
{
    static PureuiManagerImpl uiMgrInstance;
    return uiMgrInstance;
}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureUiManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureUiManager::getLoggerModuleName()
{
    return "PureUiManager";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################






