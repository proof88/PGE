#pragma once

/*
    ###################################################################################
    PGEconsts.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine constants
    Made by PR00F88
    ###################################################################################
*/


#include "PGEallHeaders.h"


/*
    This file MUSTN'T be included in EXTERNAL header files!
*/


/**
    If PGE_CCONSOLE_IS_ENABLED macro is defined, there will be console window created and printouts will be visible, otherwise not.
*/
#ifndef PGE_CCONSOLE_IS_ENABLED
#define PGE_CCONSOLE_IS_ENABLED
#endif

#define PGE_NAME    "PR00F's Game Engine (PGE)"
#define PGE_VERSION "0.5 private alpha built on " __DATE__" @ " __TIME__

#define PGE_ERRORMSG_BASETITLE   "Error"
#define PGE_WARNINGMSG_BASETITLE "Warning"
#define PGE_INFOMSG_BASETITLE    "Info"

#define PGE_1ST_CFG_FILENAME "basic_cfg.cfg"
#define PGE_BASE_DATA_FOLDER "gamedata"
#define PGE_LANG_FOLDER PGE_BASE_DATA_FOLDER"\\language\\"
#define PGE_PROFILE_FOLDER PGE_BASE_DATA_FOLDER"\\profiles\\"
#define PGE_LANG_DEF_FILE "magyar.txt"

enum PGE_ENUM_LANG
{
    PGE_LANG_E000,
    PGE_LANG_E001,
    PGE_LANG_E002,
    PGE_LANG_E003,
    PGE_LANG_E004,
    PGE_LANG_E005,
    PGE_LANG_E006,
    PGE_LANG_TITLE_ERR,
    PGE_LANG_TITLE_WARN,
    PGE_LANG_TITLE_INFO
}; // enum PGE_ENUM_LANG

#define PGE_INACTIVE_SLEEP 100             /**< setWaitingWhileInactive() */
#define PGE_INACTIVE_AS_ACTIVE false       /**< setInactiveLikeActive() */

#define PGE_CFG_SETTINGSFILE_MIN_SIZE 2000
#define PGE_CFG_SETTINGSFILE_MAX_SIZE 20000



/*
    ==============================
    DEFAULT SETTINGS
    ==============================
*/
// ---------------------------------------------------------------------------

// [ ====== GAMEPLAY ====== ]
#define PGE_CFG_DEF_CL_NAME                             "N�VTELEN J�T�KOS"  // j�t�kos neve alapb�l
#define PGE_CFG_DEF_CL_SHOWBLOOD                        "1"       // v�r mutat�sa
#define PGE_CFG_DEF_CL_WPN_AUTOPICKUP                   "2"  // fegyver automatikus felv�tele
#define PGE_CFG_DEF_CL_WPN_INCASEOF_OUTOFAMMO_STILLHAVE "0"  // t�r ki�r�l, de m�g van t�r, mi legyen
#define PGE_CFG_DEF_CL_WPN_INCASEOF_OUTOFAMMO_NOTHAVE   "0"  // t�r ki�r�l, de m�r nincs, mi legyen
#define PGE_CFG_DEF_CL_INTERACTIVE_KEYBD                "0" // billenty�zet interaktivit�sa
#define PGE_CFG_DEF_CL_INTERACTIVE_ODD                  "0" // optikai meghajt� interaktivit�sa
#define PGE_CFG_DEF_CL_INTERACTIVE_PRINTER              "0" // nyomtat� interaktivit�sa
#define PGE_CFG_DEF_CL_RUNTOGGLE                        "0"
#define PGE_CFG_DEF_CL_SHOW_PLAYERNAMES                 "1"    // j�t�kosok nev�nek kirajzol�sa
#define PGE_CFG_DEF_CL_SHOW_XHAIR                       "1"          // van-e c�lkereszt
#define PGE_CFG_DEF_CL_INTERACTIVE_XHAIR                "1"   // interakt�v c�lkereszt
#define PGE_CFG_DEF_CL_SHOW_HUD_HEALTH                  "1"   // HP
#define PGE_CFG_DEF_CL_SHOW_HUD_WPNINFO                 "1"   // fegyver �llapota
#define PGE_CFG_DEF_CL_SHOW_HUD_ARMOR                   "1"   // p�nc�lzat �llapota
#define PGE_CFG_DEF_CL_SHOW_HUD_BODYINFO                "1"   // szerver, testr�szek �llapota
#define PGE_CFG_DEF_CL_HUD_TYPE                         "1"           // HUD elrendez�si st�lusa
#define PGE_CFG_DEF_CL_HUD_COLOR                        "20 20 200"   // HUD alapsz�ne
#define PGE_CFG_DEF_CL_STEALTHMODE                      "1"         // lopakod� m�d el�rhet�-e (f�n�k m�d)
#define PGE_CFG_DEF_CL_MONITORGREENMODE                 "1"  // monitor lekapcsolhat-e
#define PGE_CFG_DEF_CL_SCREENSAVERDISABLE               "1"  // k�perny�k�m�l� tiltva

// [ ====== MOUSE ====== ]
#define PGE_CFG_DEF_MOUSE_SENSITIVITY             "1.0"
#define PGE_CFG_DEF_MOUSE_SMOOTHOUT               "1.0"
#define PGE_CFG_DEF_MOUSE_USE_CONVENTIONAL_METHOD "1"
#define PGE_CFG_DEF_MOUSE_REVERSE                 "0"
#define PGE_CFG_DEF_MOUSE_LEFT                    "CMD_FIRE1"
#define PGE_CFG_DEF_MOUSE_RIGHT                   "CMD_FIRE2"
#define PGE_CFG_DEF_MOUSE_CENTER                  "CMD_USE"
#define PGE_CFG_DEF_MOUSE_SCROLLUP                "CMD_SWITCHTO_PREVWPN"
#define PGE_CFG_DEF_MOUSE_SCROLLDOWN              "CMD_SWITCHTO_NEXTWPN"
#define PGE_CFG_DEF_MOUSE_SCROLLLEFT              "CMD_NULL"
#define PGE_CFG_DEF_MOUSE_SCROLLRIGHT             "CMD_NUL"

// [ ====== KEYBOARD ====== ]
#define PGE_CFG_DEF_KEYBD_CMD01 "VK_UP CMD_MOVE_FWD"
#define PGE_CFG_DEF_KEYBD_CMD02 "VK_DOWN CMD_MOVE_BWD"
#define PGE_CFG_DEF_KEYBD_CMD03 "VK_LEFT CMD_TURN_LEFT"
#define PGE_CFG_DEF_KEYBD_CMD04 "VK_RIGHT CMD_TURN_RIGHT"
#define PGE_CFG_DEF_KEYBD_CMD05 "VK_CTRL CMD_CROUCH"
#define PGE_CFG_DEF_KEYBD_CMD06 "VK_SHIFT CMD_RUN"

// [ ====== VIDEO ====== ]
#define PGE_CFG_DEF_GFX_LASTRENDERER ""
#define PGE_CFG_DEF_GFX_RES_X        "1024"    // 0: a desktop�
#define PGE_CFG_DEF_GFX_RES_Y        "768"    // 0: a desktop�
#define PGE_CFG_DEF_GFX_ASPECT_RATIO "4:3" // 4:3  16:9  16:10
#define PGE_CFG_DEF_GFX_REFRESHRATE  "85"  // HZ, 0: A LEHET� LEGNAGYOBB, 1: A desktop�
#define PGE_CFG_DEF_GFX_BITS_COLOR   "32"  // 0: a desktop�, EGY�B ESET�N 16-OT �LL�T BE
#define PGE_CFG_DEF_GFX_BITS_DEPTH   "24"  // EGY�B ESET�N 16-OT �LL�T BE
#define PGE_CFG_DEF_GFX_WINDOWED     "0"   // ABLAKBAN VAGY TELJES K�PERNY�BEN
#define PGE_CFG_DEF_GFX_GAMMA        "1.0"

#define PGE_CFG_DEF_GFX_AA_LEVEL  "" // 0-2-4-...
#define PGE_CFG_DEF_GFX_AA_METHOD "" // QUINCUNX, STB ...

#define PGE_CFG_DEF_GFX_MOTIONBLUR        "3" // 0-NINCS, 1-CSAK S�R�L�SKOR, 2-CSAK GYORS ELMOZDUL�S ESET�N, 3-MINDIG
#define PGE_CFG_DEF_GFX_MOTIONBLUR_METHOD "0" // 0: FIXP, 1: SHADER

#define PGE_CFG_DEF_GFX_TEX_SHADERS "1" // SHADERREL TEXT�R�ZZON, VAGY HAGYOM�NYOSAN
// HA HAGYOM�NYOS, AKKOR A LENTI ENABLE LIST�BAN LEGFELJEBB ANNYI LEHET BEKAPCSOLVA, AMENNYI
// TEXT�R�Z� EGYS�G�NK VAN

#define PGE_CFG_DEF_GFX_TEX_ENABLE_DIFFUSE  "1" // ALAP TEXT�R�Z�S ENGED�LYEZ�SE
#define PGE_CFG_DEF_GFX_TEX_ENABLE_SPECULAR "1" // SPEKUL�RIS TEX ENGED�LYEZ�SE
#define PGE_CFG_DEF_GFX_TEX_ENABLE_BUMP     "1" // �RDESS�GI TEX ENGED�LYEZ�SE
#define PGE_CFG_DEF_GFX_TEX_ENABLE_DETAIL   "1" // R�SZLETESS�GI TEX ENGED�LYEZ�SE
#define PGE_CFG_DEF_GFX_TEX_ENABLE_LIGHT    "1" // F�NYT�RK�PEK ENGED�LYEZ�SE

#define PGE_CFG_DEF_GFX_TEX_LEVELS_DIFFUSE  "0" // KISEBB M�RETRE HOZZA-E AZ ALAP TEXEKET, 0-1-2-...
#define PGE_CFG_DEF_GFX_TEX_LEVELS_SPECULAR "0" // KISEBB M�RETRE HOZZA-E A SPEKUL�RIS TEXEKET, 0-1-2-...
#define PGE_CFG_DEF_GFX_TEX_LEVELS_BUMP     "0" // KISEBB M�RETRE HOZZA-E AZ �RDESS�GI TEXEKET, 0-1-2-...
#define PGE_CFG_DEF_GFX_TEX_LEVELS_DETAIL   "0" // KISEBB M�RETRE HOZZA-E A R�SZLETESS�GI TEXEKET, 0-1-2-...
#define PGE_CFG_DEF_GFX_TEX_LEVELS_LIGHT    "0" // KISEBB M�RETRE HOZZA-E A F�NYTEXEKET, 0-1-2-...

#define PGE_CFG_DEF_GFX_TEX_COMPRESSION "1" // T�M�R�TSE-E A TEXT�R�KAT
#define PGE_CFG_DEF_GFX_TEX_FILTERING   "GL_LINEAR_MIPMAP_LINEAR"
#define PGE_CFG_DEF_GFX_TEX_ANISOFILTER "0" // 0-2-4-8-16X

#define PGE_CFG_DEF_GFX_SHADOWS "0" // 0: NINCS, 1: VET�TETT, 2: SOFT VET�TETT

#define PGE_CFG_DEF_GFX_REFLECTIONS "0" // T�KR�Z�D�SEK, PL. T�K�R
#define PGE_CFG_DEF_GFX_REFRACTIONS "0" // F�NYT�R�S, PL. V�Z ALATTI DOLGOK FEL�LR�L ELMOS�DNAK

#define PGE_CFG_DEF_GFX_DECALS "1" // L�VED�K NYOMOK

#define PGE_CFG_DEF_GFX_SMOKE_COMPLEXITY      ""  // F�ST KOMPLEXIT�S
#define PGE_CFG_DEF_GFX_SKY                   "1" // MUTASSA-E AZ �GBOLTOT
#define PGE_CFG_DEF_GFX_LENSFLARE             "1" // LENSFLARE
#define PGE_CFG_DEF_GFX_SHOWFPS               "2" // 0: NE, 1: CSAK AKTU�LIS FPS, 2: MIN+MAX+AVG+AKTU�LIS FPS
#define PGE_CFG_DEF_GFX_MULTITHREADING        "0" // T�BB SZ�LLAL FUSSON-E A GRAF MOTOR
#define PGE_CFG_DEF_GFX_ERRONEOUS_CARD_HELPER "0" // HIB�S (SZEMETEL�) VGA K�RTYA ESET�N H�NYSZOR ISM�TELJEN MEG
                // BIZONYOS GRAFIKUS PARANCSOKAT, H�THA �GY JOBB LESZ ...

// [ ====== AUDIO ====== ]
#define PGE_CFG_DEF_SFX_VOLUME_SFX   "100"
#define PGE_CFG_DEF_SFX_VOLUME_MUSIC "100"
#define PGE_CFG_DEF_SFX_SWAPCHANNELS "1"

// [ ====== SERVER ====== ]
#define PGE_CFG_DEF_SV_NAME "AZ �N SZERVEREM"
#define PGE_CFG_DEF_SV_MAXCLIENTS "10"
#define PGE_CFG_DEF_SV_BODY_DISAPPEAR_TIME "120" // SECONDS BEFORE CORPSES DISAPPEAR, 0: IMMEDIATELY, -1: NEVER
#define PGE_CFG_DEF_SV_DEMANDMATCHINGCFG "" // SHALL SERVER DEMAND CLIENTS TO COMPLY WITH SERVER RULES
    // 0 - NO
    // 1 - ALL BUT BLOOD
    // 2 - ALL

// RULES
#define PGE_CFG_DEF_SV_SHOWBLOOD        "1"  // 0: NO BLOOD; 1: WILL SHOW BLOOD EVEN IF OFF AT CLIENT
#define PGE_CFG_DEF_SV_SHADOWS          "0"  // SAME OPTIONS AS AT GFX_SHADOWS, NO RULE IF -1
#define PGE_CFG_DEF_SV_DECALS           "1"  // SAME OPTIONS AS AT GFX_DECALS, NO RULE IF -1
#define PGE_CFG_DEF_SV_SHOWXHAIR        "1"  // FORCING NO XHAIR WHEN 0, UNDEFINED OTHERWISE
#define PGE_CFG_DEF_SV_BITS_DEPTH       "24" // 32 OR 24, MINIMUM CLIENT ZDEPTH, OTHERWISE UNDEFINED
#define PGE_CFG_DEF_SV_BITS_COLOR       "16" // 32 FOR MINIMUM 32 BIT CLIENT CDEPTH, OTHERWISE UNDEFINED
#define PGE_CFG_DEF_SV_SMOKE_COMPLEXITY ""   // MINIMUM VALUE FOR CLIENT, -1 MEANS UNDEFINED
#define PGE_CFG_DEF_SV_DIFFUSE
#define PGE_CFG_DEF_SV_SPECULAR
#define PGE_CFG_DEF_SV_BUMP
#define PGE_CFG_DEF_SV_DETAIL
#define PGE_CFG_DEF_SV_LIGHT

#define PGE_CFG_DEF_SV_FRIENDLYFIRE         "1" // IF 1, CAN DAMAGE TEAMMATES
#define PGE_CFG_DEF_SV_AUTOKICK_TKRS        "0" // IF >0, TEAMKILLING #VALUE# TIMES RESULTS IN AUTOKICK, 0 MEANS NO AUTOKICK
#define PGE_CFG_DEF_SV_AUTOKICK_MAXPING     ""  // IF >0, AUTOKICKING PLAYER IF PLAYER PING EXCEEDS #VALUE#, 0 MEANS NO AUTOKICK
#define PGE_CFG_DEF_SV_AUTOKICK_MAXPINGWAIT "5" // SECONDS WHILE EXCEEDING MAXPING IS ALLOWED, REACHING THIS RESULTS IN AUTOKICK

#define PGE_CFG_DEF_SV_BUNNY          "1"   // HOPPING BUNNY ALLOWED ON SERVER
#define PGE_CFG_DEF_SV_SNAILATTACK    "1"   // SNAIL INVASION ALLOWED ON SERVER
#define PGE_CFG_DEF_SV_WEIGHTSLOWS    "1"   // INVENTORY ITEMS' WEIGHT SLOWS DOWN PLAYERS
#define PGE_CFG_DEF_SV_WEIGHTLIMIT    "50"  // MAX WEIGHT OF ITEMS IN INVENTORY, 0: NO LIMIT
#define PGE_CFG_DEF_SV_XPLOSIONPUSH   "1"   // XPLOSIONS PUSH PLAYERS (CS<->QUAKE)
#define PGE_CFG_DEF_SV_LEVELTIME      "10"  // LEVEL TIME, 0: FOREVER
#define PGE_CFG_DEF_SV_ROUNDTIME      "120" // ROUND TIME LIMIT, 0: DEATHMATCH, NO MULTIPLE ROUNDS
#define PGE_CFG_DEF_SV_MAXTEAMS       "2"   // TEAM LIMIT, 0: NO LIMIT
#define PGE_CFG_DEF_SV_MAXTEAMPLAYERS "5"   // MAX PLAYER PER TEAM, 0: NO LIMIT


/*
    ==============================
    SETTINGS ARRAY SELECTORS
    ==============================
*/
// ---------------------------------------------------------------------------

enum PGE_SETTINGS_ENUMS
{
    PGE_CFG_ENUM_CL_NAME,
    PGE_CFG_ENUM_CL_SHOWBLOOD,
    PGE_CFG_ENUM_CL_WPN_AUTOPICKUP,
    PGE_CFG_ENUM_CL_WPN_INCASEOF_OUTOFAMMO_STILLHAVE,
    PGE_CFG_ENUM_CL_WPN_INCASEOF_OUTOFAMMO_NOTHAVE,
    PGE_CFG_ENUM_CL_INTERACTIVE_KEYBD,
    PGE_CFG_ENUM_CL_INTERACTIVE_ODD,
    PGE_CFG_ENUM_CL_INTERACTIVE_PRINTER,
    PGE_CFG_ENUM_CL_RUNTOGGLE,
    PGE_CFG_ENUM_CL_SHOW_PLAYERNAMES,
    PGE_CFG_ENUM_CL_SHOW_XHAIR,
    PGE_CFG_ENUM_CL_INTERACTIVE_XHAIR,
    PGE_CFG_ENUM_CL_SHOW_HUD_HEALTH,
    PGE_CFG_ENUM_CL_SHOW_HUD_WPNINFO,
    PGE_CFG_ENUM_CL_SHOW_HUD_ARMOR,
    PGE_CFG_ENUM_CL_SHOW_HUD_BODYINFO,
    PGE_CFG_ENUM_CL_HUD_TYPE,
    PGE_CFG_ENUM_CL_HUD_COLOR,
    PGE_CFG_ENUM_CL_STEALTHMODE,
    PGE_CFG_ENUM_CL_MONITORGREENMODE,
    PGE_CFG_ENUM_CL_SCREENSAVERDISABLE,
        
    // [ ====== MOUSE ====== ]
    PGE_CFG_ENUM_MOUSE_SENSITIVITY,
    PGE_CFG_ENUM_MOUSE_SMOOTHOUT,
    PGE_CFG_ENUM_MOUSE_USE_CONVENTIONAL_METHOD,
    PGE_CFG_ENUM_MOUSE_REVERSE,
    PGE_CFG_ENUM_MOUSE_LEFT,
    PGE_CFG_ENUM_MOUSE_RIGHT,
    PGE_CFG_ENUM_MOUSE_CENTER,
    PGE_CFG_ENUM_MOUSE_SCROLLUP,
    PGE_CFG_ENUM_MOUSE_SCROLLDOWN,
    PGE_CFG_ENUM_MOUSE_SCROLLLEFT,
    PGE_CFG_ENUM_MOUSE_SCROLLRIGHT,
       
    // [ ====== KEYBOARD ====== ]
    PGE_CFG_ENUM_KEYBD_CMD01,
    PGE_CFG_ENUM_KEYBD_CMD02,
    PGE_CFG_ENUM_KEYBD_CMD03,
    PGE_CFG_ENUM_KEYBD_CMD04,
    PGE_CFG_ENUM_KEYBD_CMD05,
    PGE_CFG_ENUM_KEYBD_CMD06,

    // [ ====== VIDEO ====== ]
    PGE_CFG_ENUM_GFX_LASTRENDERER,
    PGE_CFG_ENUM_GFX_RES_X,
    PGE_CFG_ENUM_GFX_RES_Y,
    PGE_CFG_ENUM_GFX_ASPECT_RATIO,
    PGE_CFG_ENUM_GFX_REFRESHRATE,
    PGE_CFG_ENUM_GFX_BITS_COLOR,
    PGE_CFG_ENUM_GFX_BITS_DEPTH,
    PGE_CFG_ENUM_GFX_WINDOWED,
    PGE_CFG_ENUM_GFX_GAMMA,

    PGE_CFG_ENUM_GFX_AA_LEVEL,
    PGE_CFG_ENUM_GFX_AA_METHOD,

    PGE_CFG_ENUM_GFX_MOTIONBLUR,
    PGE_CFG_ENUM_GFX_MOTIONBLUR_METHOD,

    PGE_CFG_ENUM_GFX_TEX_SHADERS,

    PGE_CFG_ENUM_GFX_TEX_ENABLE_DIFFUSE,
    PGE_CFG_ENUM_GFX_TEX_ENABLE_SPECULAR,
    PGE_CFG_ENUM_GFX_TEX_ENABLE_BUMP,
    PGE_CFG_ENUM_GFX_TEX_ENABLE_DETAIL,
    PGE_CFG_ENUM_GFX_TEX_ENABLE_LIGHT,

    PGE_CFG_ENUM_GFX_TEX_LEVELS_DIFFUSE,
    PGE_CFG_ENUM_GFX_TEX_LEVELS_SPECULAR,
    PGE_CFG_ENUM_GFX_TEX_LEVELS_BUMP,
    PGE_CFG_ENUM_GFX_TEX_LEVELS_DETAIL,
    PGE_CFG_ENUM_GFX_TEX_LEVELS_LIGHT,

    PGE_CFG_ENUM_GFX_TEX_COMPRESSION,
    PGE_CFG_ENUM_GFX_TEX_FILTERING,
    PGE_CFG_ENUM_GFX_TEX_ANISOFILTER,

    PGE_CFG_ENUM_GFX_SHADOWS,

    PGE_CFG_ENUM_GFX_REFLECTIONS,
    PGE_CFG_ENUM_GFX_REFRACTIONS,

    PGE_CFG_ENUM_GFX_DECALS,

    PGE_CFG_ENUM_GFX_SMOKE_COMPLEXITY,
    PGE_CFG_ENUM_GFX_SKY,
    PGE_CFG_ENUM_GFX_LENSFLARE,
    PGE_CFG_ENUM_GFX_SHOWFPS,
    PGE_CFG_ENUM_GFX_MULTITHREADING,
    PGE_CFG_ENUM_GFX_ERRONEOUS_CARD_HELPER,

    // [ ====== AUDIO ====== ]
    PGE_CFG_ENUM_SFX_VOLUME_SFX,
    PGE_CFG_ENUM_SFX_VOLUME_MUSIC,
    PGE_CFG_ENUM_SFX_SWAPCHANNELS,

    // [ ====== SERVER ====== ]
    PGE_CFG_ENUM_SV_NAME,
    PGE_CFG_ENUM_SV_MAXCLIENTS,
    PGE_CFG_ENUM_SV_BODY_DISAPPEAR_TIME,
    PGE_CFG_ENUM_SV_DEMANDMATCHINGCFG,

    // RULES
    PGE_CFG_ENUM_SV_SHOWBLOOD,
    PGE_CFG_ENUM_SV_SHADOWS,
    PGE_CFG_ENUM_SV_DECALS,
    PGE_CFG_ENUM_SV_SHOWXHAIR,
    PGE_CFG_ENUM_SV_BITS_DEPTH,
    PGE_CFG_ENUM_SV_BITS_COLOR,
    PGE_CFG_ENUM_SV_SMOKE_COMPLEXITY,

    PGE_CFG_ENUM_SV_DIFFUSE,
    PGE_CFG_ENUM_SV_SPECULAR,
    PGE_CFG_ENUM_SV_BUMP,
    PGE_CFG_ENUM_SV_DETAIL,
    PGE_CFG_ENUM_SV_LIGHT,

    PGE_CFG_ENUM_SV_FRIENDLYFIRE,
    PGE_CFG_ENUM_SV_AUTOKICK_TKRS,
    PGE_CFG_ENUM_SV_AUTOKICK_MAXPING,
    PGE_CFG_ENUM_SV_AUTOKICK_MAXPINGWAIT,

    PGE_CFG_ENUM_SV_BUNNY,
    PGE_CFG_ENUM_SV_SNAILATTACK,
    PGE_CFG_ENUM_SV_WEIGHTSLOWS,
    PGE_CFG_ENUM_SV_WEIGHTLIMIT,
    PGE_CFG_ENUM_SV_XPLOSIONPUSH,
    PGE_CFG_ENUM_SV_LEVELTIME,
    PGE_CFG_ENUM_SV_ROUNDTIME,
    PGE_CFG_ENUM_SV_MAXTEAMS,
    PGE_CFG_ENUM_SV_MAXTEAMPLAYERS,
}; // enum PGE_SETTINGS_ENUMS