#pragma once

/*
    ###################################################################################
    PGEcfgProfiles.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine profile config handler
    Made by PR00F88
    ###################################################################################
*/

#include <map>
#include <shlobj.h>

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"
#include "PGEcfgFile.h"


/**
    PR00F's Game Engine profile config handler class.
*/
class PGEcfgProfiles : public PGEcfgFile
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEcfgProfiles is included")   
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    explicit PGEcfgProfiles();
    virtual ~PGEcfgProfiles();

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */

    // Profile-independent
    // ---------------------------------------------------------------------------
    static std::string  getMyDocsFolder();                     /**< Returns the path to user documents. */

    bool                reinitialize(const char* gameTitle);   /**< Resets initial state. */
    void                shutdown();                            /**< Resets everything. */

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

    void ProcessCommandLine(const char* szCmdLine);               /**< Application can pass command line to be parsed for extra one-time configuration. */
    
    const std::map<std::string, PGEcfgVariable>& getCommandLineVars() const;

    // Active profile-dependent
    // ---------------------------------------------------------------------------
    int  getProfileIndex() const;                       /**< Gets index of active profile. */
    const std::string getProfileName() const;           /**< Gets name of active profile. */
    bool setProfile(int nIndex);                        /**< Sets active profile by index. */
    bool setProfile(const std::string& sProfileName);   /**< Sets active profileby name. */
    void DeleteVar(const char* varName);                /**< Deletes a cvar by the given name. */
    bool readConfiguration();                           /**< Loads configuration for current profile from file. */
    bool writeConfiguration();                          /**< Saves configuration for current profile to file. */
    
protected:
    virtual bool validateOnLoad(std::ifstream& f) const override;
    virtual bool validateOnSave(std::ofstream& f) const override;

    std::map<std::string, PGEcfgVariable>& getCommandLineVars();

private:
    #define PGE_SYS_CFG_FILE_MAGIC_START "!PNGGECFG"
    #define PGE_SYS_CFG_PLAYER_NAME_CVAR "cl_name"

    static std::string   sPathToMyDocs;     /**< Path to user documents. */

    std::string   sLangFileName;     /**< Filename of language file. */
    bool          bMainCFGinMyDocs;  /**< Do we store cfg file in user documents folder? */
    std::string   sPathToProfiles;   /**< Search path for profiles based on bMainCFGinMyDocs and pPathToMyDocs. */
    int           nProfilesCount;    /**< Number of found profiles. */
    std::string** sFoundProfiles;    /**< Directory names of found profiles (just add ".cfg" for profile file). */
    std::string** sFoundProfilePlayerNames;  /**< Found profile names (player names). */
    int           nActiveProfile;    /**< Index of active profile. */
    std::string   m_sActiveProfile;  /**< Name of active profile. */
    PGEcfgVariable cvarWrong;        /**< This is returned when trying to access an invalid cvar. */

    std::map<std::string, PGEcfgVariable> m_commandLineVars;  /**< Any added variables parsed by ProcessCommandLine(). */

    // ---------------------------------------------------------------------------

    static bool findMyDocsFolder();         /**< Stores user docs folder path in sPathToMyDocs. */

    PGEcfgProfiles(const PGEcfgProfiles&);  
    PGEcfgProfiles& operator=(const PGEcfgProfiles&);

    bool getPlayerNameFromFile(
        const char* cFilename,
        std::string& sPlayerName );  /**< Gets player name from given cfg file. */

    void LoadProfilesList();   /**< Fills up sFoundProfiles and sFoundProfilePlayerNames. */
    void ClearVars();          /**< Removes all cvars. */

}; // class PGEcfgProfiles
