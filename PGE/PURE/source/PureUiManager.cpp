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

#include "imgui.h"                        // this is from the external imgui directory because this we share with apps too
#include "backends/imgui_impl_opengl2.h"  // this is from the internal imgui directory obviously
#include "backends/imgui_impl_win32.h"    // this is from the internal imgui directory obviously

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
   PureUiManagerImpl
   ###########################################################################
*/

class PureUiManagerImpl :
    public PureUiManager
{

public:


    // ---------------------------------------------------------------------------

    virtual ~PureUiManagerImpl();

    void Initialize(HWND wnd_handle, HDC wnd_dc) override;
    void Deinitialize() override;
    TPureBool isInitialized() const override;

    const std::function<void()>& getGuiDrawCallback() const override;
    void setGuiDrawCallback(const std::function<void()>& cb) override;

    PureUiText* textPermanentLegacy(
        const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) override;
    PureUiText* textPermanentLegacy(const std::string& txt, int x, int y) override;

    void removeTextPermanentLegacy(const std::string& text, int x, int y, int height) override;
    void removeAllTextPermanentLegacy() override;

    PureUiText* textTemporalLegacy(
        const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) override;
    PureUiText* textTemporalLegacy(const std::string& txt, int x, int y) override;

    const std::string& getDefaultFontFaceLegacy() const override;

    void setDefaultFontFaceLegacy(const std::string& face) override;

    int  getDefaultFontSizeLegacy() const override;
    void setDefaultFontSizeLegacy(int size) override;

    bool getDefaultFontBoldLegacy() const override;
    void setDefaultFontBoldLegacy(bool bold) override;

    bool getDefaultFontItalicLegacy() const override;
    void setDefaultFontItalicLegacy(bool italic) override;

    bool getDefaultFontUnderlineLegacy() const override;
    void setDefaultFontUnderlineLegacy(bool underline) override;

    bool getDefaultFontStrikeoutLegacy() const override;
    void setDefaultFontStrikeoutLegacy(bool strikeout) override;

    const PureColor& getDefaultColorLegacy() const override;
          PureColor& getDefaultColorLegacy() override;

    void render() override;

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    bool bInitialized;
    HDC hDC;
    ImGuiContext* pImGuiCtx;       /**< Dear ImGui context that we share with user application so it can use OUR ImGui instance. */
    GLfloat mat4x4Identity[4][4];
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
    PureUiManagerImpl();

    PureUiManagerImpl(const PureUiManagerImpl&);
    PureUiManagerImpl& operator=(const PureUiManagerImpl&);

    friend class PureUiManager;

};


// ############################### PUBLIC ################################


PureUiManagerImpl::~PureUiManagerImpl()
{

}


void PureUiManagerImpl::Initialize(HWND wnd_handle, HDC wnd_dc)
{
    if ((wnd_handle == NULL) || (wnd_dc == NULL))
    {
        return;
    }

    getConsole().OLnOI("PureUiManager::Initialize() ...");
    if ( bInitialized )
    {
        getConsole().OLnOO("Already initialized!");     
        return;
    }

    if (PureHwInfo::get().getVideo().isAcceleratorDetected())
    {
        getConsole().OLn("with Dear ImGui %s", IMGUI_VERSION);

        IMGUI_CHECKVERSION();
        pImGuiCtx = ImGui::CreateContext();
        if (!pImGuiCtx)
        {
            getConsole().EOLnOO("ERROR: ImGui::CreateContext() failed!");
            Deinitialize();
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;  // by default it is imgui.ini in current work dir, but I dont want this auto-config behavior now ...
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        if (!ImGui_ImplWin32_Init(wnd_handle))
        {
            getConsole().EOLnOO("ERROR: ImGui::ImGui_ImplWin32_Init() failed!");
            Deinitialize();
            return;
        }

        if (!ImGui_ImplOpenGL2_Init())
        {
            getConsole().EOLnOO("ERROR: ImGui::ImGui_ImplOpenGL2_Init() failed!");
            Deinitialize();
            return;
        }

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);
    } // end isAcceleratorDetected()

    sDefaultFont = PURE_UI_MGR_FONT_DEFAULT_FACE;
    nDefaultFontSize = PURE_UI_MGR_FONT_DEFAULT_SIZE;
    bDefaultFontBold = PURE_UI_MGR_FONT_DEFAULT_BOLD;
    bDefaultFontItalic = PURE_UI_MGR_FONT_DEFAULT_ITALIC;
    bDefaultFontUnderline = PURE_UI_MGR_FONT_DEFAULT_UNDERLINE;
    bDefaultFontStrikeout = PURE_UI_MGR_FONT_DEFAULT_STRIKEOUT;

    hDC = wnd_dc;
    bInitialized = true;

    getConsole().OOSOLn("done!");
}


void PureUiManagerImpl::Deinitialize()
{
    getConsole().OLn("PureuiManagerImpl::Deinitialize()");

    // always deinitialize everything what we can because this is also invoked during initialization error for cleaning up
    bInitialized = false;

    pfGuiCallback = nullptr;

    if (ImGui_ImplOpenGL2_Initialized())
    {
        ImGui_ImplOpenGL2_Shutdown();
    }
    if (ImGui_ImplWin32_Initialized())
    {
        ImGui_ImplWin32_Shutdown();
    }
    if (ImGui::GetCurrentContext())
    {
        ImGui::DestroyContext();
    }
    pImGuiCtx = nullptr;

    // TODO: setting defaults has been copied from Initialize(), let's moved them to a separate funcion!
    sDefaultFont = PURE_UI_MGR_FONT_DEFAULT_FACE;
    nDefaultFontSize = PURE_UI_MGR_FONT_DEFAULT_SIZE;
    bDefaultFontBold = PURE_UI_MGR_FONT_DEFAULT_BOLD;
    bDefaultFontItalic = PURE_UI_MGR_FONT_DEFAULT_ITALIC;
    bDefaultFontUnderline = PURE_UI_MGR_FONT_DEFAULT_UNDERLINE;
    bDefaultFontStrikeout = PURE_UI_MGR_FONT_DEFAULT_STRIKEOUT;

    mTexts.clear();
    for (auto pFont : vFonts)
    {
        delete pFont;
    }
    vFonts.clear();

    hDC = NULL;
} // Deinitialize()


TPureBool PureUiManagerImpl::isInitialized() const
{
    return bInitialized;
}


const std::function<void()>& PureUiManagerImpl::getGuiDrawCallback() const
{
    return pfGuiCallback;
}


void PureUiManagerImpl::setGuiDrawCallback(const std::function<void()>& cb)
{
    if (!bInitialized)
    {
        getConsole().EOLn("%s ERROR: PureUiManager is NOT initialized!", __func__);
        return;
    }

    // It can happen that PURE was initialized by a different thread, or from a DLL, or just for any other unknown
    // reason the caller application might not have the created context of Dear ImGui, so just make sure
    // that we have the context (we dont want to set it in every frame in render() for performance reasons).
    // Since this function is supposed to be called by the application it will also implicitly set the context to
    // the saved context that might had been created by another thread, etc.

    if (!pImGuiCtx)
    {
        // should not happen, but do not save the callback if we have problem with the context
        getConsole().EOLn("%s ERROR: CANNOT HAPPEN: PureUiManager does NOT have Dear ImGui context!", __func__);
        return;
    }

    ImGui::SetCurrentContext(pImGuiCtx);
    pfGuiCallback = cb;
} // setGuiDrawCallback()


PureUiText* PureUiManagerImpl::textPermanentLegacy(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout)
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
            getConsole().EOLn("PureuiManagerImpl::textPermanentLegacy(...) nullptr within vFonts!");
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
} // textPermanentLegacy()


PureUiText* PureUiManagerImpl::textPermanentLegacy(const std::string& txt, int x, int y)
{
    return textPermanentLegacy(txt, x, y, sDefaultFont, nDefaultFontSize, bDefaultFontBold, bDefaultFontItalic, bDefaultFontUnderline, bDefaultFontStrikeout);
}


void PureUiManagerImpl::removeTextPermanentLegacy(const std::string& text, int x, int y, int height)
{
    const unsigned long hashToFind = PureUiText::getHash(text, x, y, height);
    mTexts.erase(hashToFind);  // should return 1 on successful deletion, we should check that
    // we should also unittest what happens when trying to delete unexisting key ... erase() should return 0.
}

void PureUiManagerImpl::removeAllTextPermanentLegacy()
{
    for (auto it = mTexts.begin(); it != mTexts.end(); )
    {
        if (it->second.getPermanent())
            it = mTexts.erase(it);
        else
            ++it;
    }
}

PureUiText* PureUiManagerImpl::textTemporalLegacy(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout)
{
    PureUiText* newText = textPermanentLegacy(txt, x, y, fontface, height, bold, italic, underline, strikeout);
    if ( newText )
        newText->SetPermanent( false );

    return newText;
}

PureUiText* PureUiManagerImpl::textTemporalLegacy(const std::string& txt, int x, int y)
{
    return this->textTemporalLegacy(txt, x, y, sDefaultFont, nDefaultFontSize, bDefaultFontBold, bDefaultFontItalic, bDefaultFontUnderline, bDefaultFontStrikeout);
}

const std::string& PureUiManagerImpl::getDefaultFontFaceLegacy() const
{
    return sDefaultFont;
}

void PureUiManagerImpl::setDefaultFontFaceLegacy(const std::string& face)
{
    sDefaultFont = face;    
}

int PureUiManagerImpl::getDefaultFontSizeLegacy() const
{
    return nDefaultFontSize;
}

void PureUiManagerImpl::setDefaultFontSizeLegacy(int size)
{
    nDefaultFontSize = size;    
}

bool PureUiManagerImpl::getDefaultFontBoldLegacy() const
{
    return bDefaultFontBold;
}

void PureUiManagerImpl::setDefaultFontBoldLegacy(bool bold)
{
    bDefaultFontBold = bold;    
}

bool PureUiManagerImpl::getDefaultFontItalicLegacy() const
{
    return bDefaultFontItalic;
}

void PureUiManagerImpl::setDefaultFontItalicLegacy(bool italic)
{
    bDefaultFontItalic = italic;    
}

bool PureUiManagerImpl::getDefaultFontUnderlineLegacy() const
{
    return bDefaultFontUnderline;
}

void PureUiManagerImpl::setDefaultFontUnderlineLegacy(bool underline)
{
    bDefaultFontUnderline = underline;    
}

bool PureUiManagerImpl::getDefaultFontStrikeoutLegacy() const
{
    return bDefaultFontStrikeout;
}

void PureUiManagerImpl::setDefaultFontStrikeoutLegacy(bool strikeout)
{
    bDefaultFontStrikeout = strikeout;    
}

const PureColor& PureUiManagerImpl::getDefaultColorLegacy() const
{
    return clrDefault;
}

PureColor& PureUiManagerImpl::getDefaultColorLegacy()
{
    return clrDefault;
}

void PureUiManagerImpl::render()
{
    /* legacy PR00FPS 2D rendering begins */

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

    /* legacy PR00FPS 2D rendering ends */

    /* Dear ImGui 2D code begins */

        if (getGuiDrawCallback())
        {

            glLoadMatrixf(*mat4x4Identity);

            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // application code
            getGuiDrawCallback()();

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        }

    /* Dear ImGui 2D code begins */
} // render()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    This is the only usable ctor, this is used by the static createAndGet().
*/
PureUiManagerImpl::PureUiManagerImpl()
{
    bInitialized = false;
    hDC = NULL;

    for (GLint row = 0; row < 4; row++)
        for (GLint col = 0; col < 4; col++)
            mat4x4Identity[row][col] = (row == col);

    clrDefault.Set(255, 255, 255, 255);
}


PureUiManagerImpl::PureUiManagerImpl(const PureUiManagerImpl&)
{

}


PureUiManagerImpl& PureUiManagerImpl::operator=(const PureUiManagerImpl&)
{
    return *this;
}


/*
   PureUiManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureUiManager& PureUiManager::createAndGet()
{
    static PureUiManagerImpl uiMgrInstance;
    return uiMgrInstance;
}

CConsole& PureUiManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


const char* PureUiManager::getLoggerModuleName()
{
    return "PureUiManager";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
