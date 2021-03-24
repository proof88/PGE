#pragma once

/*
    ###################################################################################
    PGESysCFG.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine cfg file handler
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"
#include <map>
#include <shlobj.h>
#include "PGEcfgVariable.h"


/**
    PR00F's Game Engine cfg file handler class.
*/
class PGESysCFG
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGESysCFG is included")   
#endif

public:
    explicit PGESysCFG(const char* gameTitle);
    virtual ~PGESysCFG();

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    // Profile-independent
    // ---------------------------------------------------------------------------
    std::string         getMyDocsFolder() const;               /**< Returns the path to user documents. */
    std::string         getLangFileName() const;               /**< Returns the file name of the selected language file. */
    int                 readLanguageData(std::string** &langTable) const; /**< Reads the language file into the given table. */
    bool                areProfilesInMyDocs() const;           /**< Returns whether player profiles are stored in user documents folder. */
    std::string         getPathToProfiles() const;             /**< Returns the path to the profiles. */
    int                 getProfilesCount() const;              /**< Returns the number of found profiles. */
    const std::string** getProfilesList() const;               /**< Returns the found profile names. */
    const std::string** getProfilePlayersList() const;         /**< Returns the found player names in profiles. */
    int                 addProfile(const char* sUser,
                                   const char* sNick);         /**< Creates a new profile. */
    int                 deleteProfile(int nIndex);             /**< Deletes the specified profile. */

    // Active profile-dependent
    // ---------------------------------------------------------------------------
    int  getProfile() const;        /**< Gets index of active profile. */
    void SetProfile(int nIndex);    /**< Sets index of active profile. */
    PGEcfgVariable& getVar(const char* varName);              /**< Gets a cvar by the given name. */
    const PGEcfgVariable& getVar(const char* varName) const;  /**< Gets a cvar by the given name. */
    void DeleteVar(const char* varName);  /** Deletes a cvar by the given name. */
    int  getVarsCount() const;      /**< Gets the number of cvars. */
    bool readConfiguration();       /**< Loads configuration for current profile from file. */
    bool writeConfiguration();      /**< Saves configuration for current profile to file. */
    
private:
    #define PGE_SYS_CFG_FILE_MAGIC_START "!PNGGECFG"
    #define PGE_SYS_CFG_PLAYER_NAME_CVAR "cl_name"

    std::string   sPathToMyDocs;     /**< Path to user documents. */
    std::string   sLangFileName;     /**< Filename of language file. */
    bool          bMainCFGinMyDocs;  /**< Do we store cfg file in user documents folder? */
    std::string   sPathToProfiles;   /**< Search path for profiles based on bMainCFGinMyDocs and pPathToMyDocs. */
    int           nProfilesCount;    /**< Number of found profiles. */
    std::string** sFoundProfiles;    /**< Directory names of found profiles (just add ".cfg" for profile file). */
    std::string** sFoundProfilePlayerNames;  /**< Found profile names (player names). */
    int           nActiveProfile;    /**< Filename of active profile. */
    std::map<std::string, PGEcfgVariable> mCVarList;  /**< Loaded cvars for active profile. */
    PGEcfgVariable cvarWrong;        /**< This is returned when trying to access an invalid cvar. */

    // ---------------------------------------------------------------------------

    PGESysCFG();

    PGESysCFG(const PGESysCFG&);  
    PGESysCFG& operator=(const PGESysCFG&);

    bool findMyDocsFolder();         /**< Stores user docs folder path in sPathToMyDocs. */
    bool getPlayerNameFromFile(
        const char* cFilename,
        std::string& sPlayerName );  /**< Gets player name from given cfg file. */

    void LoadProfilesList();   /**< Fills up sFoundProfiles and sFoundProfilePlayerNames. */
    void ClearVars();          /**< Removes all cvars. */
    void WriteConfiguration(
        std::ofstream& f_cfg,
        const std::string& sUser,
        const std::string& sNick);  /**< Writes lines to an opened file. */

}; // class PGESysCFG
