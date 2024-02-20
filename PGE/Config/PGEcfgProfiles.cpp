#include "PGEcfgProfiles.h"
/*
    ###################################################################################
    PGEcfgProfiles.cpp
    This file is part of PGE.
    PR00F's Game Engine cfg file handler
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "PGEcfgProfiles.h"
#include "../PGEincludes.h"
#include "../PGEpragmas.h"

using namespace std;


// ############################### PUBLIC ################################


PGEcfgProfiles::PGEcfgProfiles() :
    PGEcfgFile(false, false),
    bMainCFGinMyDocs(false),
    nProfilesCount(0),
    nActiveProfile(-1),
    sFoundProfiles(PGENULL),
    sFoundProfilePlayerNames(PGENULL)
{
} // PGEcfgProfiles(...)


PGEcfgProfiles::~PGEcfgProfiles()
{
    getConsole().OLn("PGEcfgProfiles::~PGEcfgProfiles()");
    shutdown();
} // ~PGEcfgProfiles()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEcfgProfiles::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEcfgProfiles::getLoggerModuleName()
{
    return "PGEcfgProfiles";
} // getLoggerModuleName()


/**
    Returns the path to user documents.
    @return The absolute path to user documents directory. There is always a delimiter character at the end.
*/
string PGEcfgProfiles::getMyDocsFolder()
{
    return sPathToMyDocs;
} // getMyDocsFolder()


/**
    Resets initial state.
    Unloads active profile and resets active profile index to -1 (no active profile selected), and re-reads profiles, language data, etc.

    @return True on success, false otherwise.
*/
bool PGEcfgProfiles::reinitialize(const char* gameTitle)
{
    getConsole().OLnOI("PGEcfgProfiles::reinitialize()");

    shutdown();

    if (!findMyDocsFolder())
    {
        getConsole().EOLnOO("ERROR: couldn't find Documents folder!");
        return false;
    }

    ifstream f(PGE_1ST_CFG_FILENAME);
    if (f.fail())
    {
        f.close();
        sLangFileName = std::string(PGE_LANG_FOLDER) + "english.txt";
        getConsole().EOLnOO("ERROR: couldn't find %s!", PGE_1ST_CFG_FILENAME);
        return false;
    } // f.fail()

    f >> sLangFileName;
    sLangFileName = PGE_LANG_FOLDER + sLangFileName;
    string ggg;
    f >> ggg;
    if (ggg == "there")
    {
        bMainCFGinMyDocs = true;
        sPathToProfiles = sPathToMyDocs;
        sPathToProfiles += gameTitle;
        sPathToProfiles += "\\profiles\\";
    }
    else
    {    // ggg == "here"
        bMainCFGinMyDocs = false;
        char pOrigCurrDirr[1024];
        GetCurrentDirectory(1024, pOrigCurrDirr);
        SetCurrentDirectory(PGE_PROFILE_FOLDER);
        char pNewCurrDirr[1024];
        GetCurrentDirectory(1024, pNewCurrDirr);
        sPathToProfiles = pNewCurrDirr;
        sPathToProfiles += "\\";
        SetCurrentDirectory(pOrigCurrDirr);
    }
    getConsole().OLn("user profile dir is: %s", sPathToProfiles.c_str());

    LoadProfilesList();

    getConsole().SOLnOO("> done!");

    return true;
}

/**
    Resets everything.
*/
void PGEcfgProfiles::shutdown()
{
    /* clear current profile */
    ClearVars();
    nActiveProfile = -1;
    m_sActiveProfile.clear();

    /* clear general stuff */
    sPathToProfiles.clear();
    bMainCFGinMyDocs = false;
    m_commandLineVars.clear();
    sLangFileName.clear();
    sPathToMyDocs.clear();
    for (int i = 0; i < nProfilesCount; i++)
    {
        delete sFoundProfiles[i];
        delete sFoundProfilePlayerNames[i];
    }
    nProfilesCount = 0;
    delete[] sFoundProfiles;
    delete[] sFoundProfilePlayerNames;
    sFoundProfiles = PGENULL;
    sFoundProfilePlayerNames = PGENULL;
}

/**
    Returns the file name of the selected language file.
*/
string PGEcfgProfiles::getLangFileName() const
{
    return sLangFileName;
} // getLangFileName()


/**
    Reads the language file into the given table.
    The given pointer will be valid only if the returned number is positive;

    @return The number of read language lines.
*/
int PGEcfgProfiles::readLanguageData(string** &langTable) const
{
    getConsole().OLnOI("PGEcfgProfiles::readLanguageData(...)");
    ifstream f( sLangFileName.c_str() );
    if ( f.fail() )
    {
        getConsole().EOLnOO("ERROR: couldn't open lang file: %s!", sLangFileName.c_str());
        return 0;
    }

    const int nBuffSize = 512;
    int       n         = 0;
    char      pTmp[nBuffSize];
    while ( !f.eof() )
    {
        f.getline(pTmp, nBuffSize);
        if ( (strstr(pTmp, "#") == NULL) && (strlen(pTmp) > 0) )
            n++;
    }
    f.close();

    // seekg() didn't work, the loop below was jumped, neither f.close() now f.open() was working :/
    ifstream g( sLangFileName.c_str() );
    if ( g.fail() )
    {
        getConsole().EOLnOO("ERROR: couldn't open lang file (2nd time): %s!", sLangFileName.c_str());
        return 0;
    }

    langTable = NULL;
    try
    {
        langTable = new string*[n](); // () makes all ptrs NULL
        int i = 0;
        while ( !g.eof() )
        {
            g.getline(pTmp, nBuffSize);
            if ( (strstr(pTmp, "#") == NULL) && (strlen(pTmp) > 0))
            {        
                langTable[i] = new string(pTmp);
                i++;
            }
        }
    }
    catch (const std::bad_alloc&)
    {
        getConsole().EOLnOO("ERROR: memory allocation failure during lang file read!");
        if ( langTable != NULL )
        {
            for (int i = 0; i < n; i++)
            {
                delete langTable[i];
            }
        }
        delete[] langTable;
        return 0;
    }
                                    

    getConsole().SOLnOO("> done!");

    return n;
} // readLanguageData()


/**
    Returns whether player profiles are stored in user documents folder.
    Example: if true, MyDocs/\<GAMENAME\>/profiles, otherwise program folder/gamedata/profiles.
*/
bool PGEcfgProfiles::areProfilesInMyDocs() const
{
    return bMainCFGinMyDocs;
} // areProfilesInMyDocs()


/**
    Returns the path to the profiles.
    @return The absolute path to game profiles directory. There is always a delimiter character at the end.
*/
string PGEcfgProfiles::getPathToProfiles() const
{
    return sPathToProfiles;
} // getPathToProfiles()


/**
    Returns the number of found profiles.
*/
int PGEcfgProfiles::getProfilesCount() const
{
    return nProfilesCount;
} // getProfilesCount()


/**
    Returns the found profile names.
*/
const string** PGEcfgProfiles::getProfilesList() const
{
    return const_cast<const string**>(sFoundProfiles);
} // getProfilesList()


/**
    Returns the found player names in profiles.
*/
const string** PGEcfgProfiles::getProfilePlayersList() const
{
    return const_cast<const string**>(sFoundProfilePlayerNames);
} // getProfilePlayersList()


/**
    Creates a new profile.
    This creates a profile directory and a profile file in it immediately.
    Creating a new profile won't change the current profile. You have to manually switch to it instead.
    The current profile index may change.

    @param sUser The username for the profile, which will be used primarily for the filename.
                 Thus some characters are restricted (?, *, etc ...).
    @param sNick The nickname for the profile, which can hold any character.

    @return Non-negative index of the new profile on success.
            -1 if a profile already exists with the given user name.
            -2 if a profile can't be created for some other reason.
            -3 getPathToProfiles() is empty, maybe forgot to call reinitialize() ?
*/
int PGEcfgProfiles::addProfile(const char* sUser, const char* sNick)
{
    if ((sUser == NULL) || (sNick == NULL))
    {
        return -2;
    }

    getConsole().OLnOI("PGEcfgProfiles::addProfile(%s, %s)", sUser, sNick);

    if (getPathToProfiles().empty())
    {
        getConsole().EOLnOO("ERROR: profiles path unknown!");
        return -3;
    }

    const std::string sFileToCreate = getPathToProfiles() + sUser + "\\" + sUser + ".cfg";
    const std::string sDirToCreate = getPathToProfiles() + sUser;

    const std::string** const sProfileList = getProfilesList();
    bool bSameUserFound = false;
    for (int i = 0; i < getProfilesCount(); i++)
    {
        bSameUserFound = ( *(sProfileList[i]) == sUser );
        if ( bSameUserFound )
            break;
    }

    if ( bSameUserFound )
    {
        getConsole().EOLnOO("ERROR: same user found!");
        return -1;
    }

    if ( _mkdir( sDirToCreate.c_str() ) != 0 )
    {
        getConsole().EOLnOO("ERROR: couldn't create dir: %s", sDirToCreate.c_str());
        return -2;
    }

    ofstream f_cfg( sFileToCreate );
    if ( f_cfg.fail() )
    {
        getConsole().EOLnOO("ERROR: couldn't create file: %s", sFileToCreate.c_str());
        return -2;
    }

    WriteConfiguration(f_cfg, sUser, sNick);
    f_cfg.close();

    LoadProfilesList();

    const std::string** const sNewProfileList = getProfilesList();
    for (int i = 0; i < getProfilesCount(); i++)
    {
        if ( *(sNewProfileList[i]) == sUser )
        {
            getConsole().SOLnOO("> done!");
            return i;
        }
    }

    // if we have reached this point, we couldn't find the added user, so return error
    getConsole().EOLnOO("ERROR: couldn't find the added profile!");
    return -2;
} // addProfile()


/**
    Deletes the specified profile.
    This deletes the profile directory and the profile file in it immediately.
    If the given index is the current profile, the current profile index will be -1 after deletion.
    Otherwise, the current profile and any loaded cvars won't be changed.
    The current profile index may change.

    @param nIndex The index of the profile to be deleted. Only non-negative values are accepted.

    @return  0 on success.
            -1 if invalid index was given.
            -2 if the profile exists but can't be deleted for some reason.
            -3 profile file deleted but its directory could not be deleted.
*/
int PGEcfgProfiles::deleteProfile(int nIndex)
{
    getConsole().OLnOI("PGEcfgProfiles::deleteProfile(%d)", nIndex);

    if ( (nIndex < 0) || (nIndex >= getProfilesCount()) )
    {
        getConsole().EOLnOO("ERROR: invalid index!");
        return -1;
    }

    if ( nIndex == getProfileIndex() )
    {
        getConsole().OLn("deleting current profile ...");
        setProfile( -1 );
    }
    const string sUserToDelete = *getProfilesList()[nIndex];
    const string sFileToDelete = getPathToProfiles() + sUserToDelete + "\\" + sUserToDelete + ".cfg";
    const string sDirToDelete = getPathToProfiles() + sUserToDelete;
    
    if ( remove( sFileToDelete.c_str() ) != 0 )
    {
        getConsole().EOLnOO("ERROR: couldn't delete file: %s", sFileToDelete.c_str());
        return -2;
    }

    int retVal = 0;

    if ( _rmdir( sDirToDelete.c_str() ) != 0 )
    {
        // at this point the cfg is deleted, so not a big problem if the directory can't be deleted.
        // if the user stores other files in that dir, those won't be deleted thus the dir can't be deleted.
        // _rmdir() only deletes empty folders, so the functionality is okay.
        getConsole().EOLnOO("WARNING: couldn't delete dir: %s", sDirToDelete.c_str());
        retVal = -3;
    }

    LoadProfilesList();

    getConsole().SOLnOO("> done!");

    return retVal;
} // deleteProfile()


/**
    Application can pass command line to be parsed for extra one-time configuration.
*/
void PGEcfgProfiles::ProcessCommandLine(const char* szCmdLine)
{
    const std::string sCmdLine(szCmdLine);
    
    // I wanted this function to be less complex, using ancestor class PGEcfgFile's
    // lineIsValueAssignment() and lineHandleAssignment() functions, but they contain
    // some extra logic not fitting here ... anyway, maybe in the future some refactor
    // could make them usable here too.

    size_t nDoubleDashPos = sCmdLine.find("--", 0);
    size_t nAssignmentPos = sCmdLine.find('=', nDoubleDashPos);
    size_t nNextSpacePos = sCmdLine.find(' ', nAssignmentPos);

    while ((nDoubleDashPos != std::string::npos) && (nAssignmentPos != std::string::npos))
    {
        if ((nAssignmentPos > (nDoubleDashPos + 2)) &&
            ((nNextSpacePos == std::string::npos) || (nNextSpacePos > (nAssignmentPos + 1))))
        {   // --a=2
            const std::string sAssignmentExpression =
                sCmdLine.substr(
                    nDoubleDashPos + 2,
                    ((nNextSpacePos == std::string::npos) ? nNextSpacePos : nNextSpacePos - (nDoubleDashPos + 2))
                );

            if (sAssignmentExpression.find(' ') == std::string::npos)
            {
                const std::string sVar = sCmdLine.substr(nDoubleDashPos + 2, nAssignmentPos - (nDoubleDashPos+2));
                const std::string sValue = sCmdLine.substr(
                    nAssignmentPos + 1,
                    (nNextSpacePos == std::string::npos) ? nNextSpacePos : (nNextSpacePos - nAssignmentPos - 1)
                );
                // this looks to be a valid assignment
                getVars()[sVar] = sValue.c_str();
                m_commandLineVars[sVar] = sValue.c_str();
            }
        }
        
        nDoubleDashPos = sCmdLine.find("--", nAssignmentPos);
        nAssignmentPos = sCmdLine.find('=', nDoubleDashPos);
        nNextSpacePos = sCmdLine.find(' ', nAssignmentPos);
    }
}


const std::map<std::string, PGEcfgVariable>& PGEcfgProfiles::getCommandLineVars() const
{
    return m_commandLineVars;
}


/**
    Gets index of active profile.
*/
int PGEcfgProfiles::getProfileIndex() const
{
    return nActiveProfile;
} // getActiveProfile()

/**
    Gets name of active profile.
*/
const std::string PGEcfgProfiles::getProfileName() const
{
    return m_sActiveProfile;
} // getProfileName()


/**
    Sets active profile by index.
    This also implies reading the configuration data for the given profile.
    The active profile index will be -1 and active profile name will be empty if the given index is valid but an error occured while reading config data.
    Note that config variables parsed earlier by ProcessCommandLine() will overwrite the variables read from profile config file.

    @param nIndex A valid profile index in [0..getProfilesCount()-1] range.
                  Specifying -1 will not load anything but erase all previously loaded config variables.
                  Specifying index less than -1 will log error but leave the configuration unchanged.

    @return True if setting active profile was successful, false otherwise.
*/
bool PGEcfgProfiles::setProfile(int nIndex)
{
    getConsole().OLnOI("PGEcfgProfiles::SetProfile(%d)", nIndex);
    if ( nIndex >= getProfilesCount() )
    {
        getConsole().EOLnOO("ERROR: invalid index!");
        return false;
    }
    if ( nIndex == getProfileIndex() )
    {
        getConsole().OLnOO("current profile index was given, return.");
        return true;
    }
    if ( nIndex < -1 )
    {
        getConsole().EOLnOO("ERROR: invalid index!");
        return false;
    }

    nActiveProfile = nIndex;
    if ( nIndex == -1 )
    {
        ClearVars();
        getTemplate().clear();
        m_sActiveProfile = "";
        getConsole().OO();
        return true;
    }

    // at this point, a valid, actual profile index was given which is not the currently set profile
    if ( !readConfiguration() )
    {
        getConsole().EOLnOO("ERROR: readConfiguration() failed!");
        nActiveProfile = -1;
        m_sActiveProfile = "";
        return false;
    }
    m_sActiveProfile = *sFoundProfiles[getProfileIndex()];

    getConsole().SOLnOO("> done!");
    return true;
} // SetProfile()


/**
    Sets active profile by name.
    This also implies reading the configuration data for the given profile.
    The active profile index will be -1 and active profile name will be empty if the given name is valid but an error occured while reading config data.
    Note that config variables parsed earlier by ProcessCommandLine() will overwrite the variables read from profile config file.

    @param sProfileName A valid profile name.
                        Specifying an empty string will not load anything but erase all previously loaded config variables.

    @return True if setting active profile was successful, false otherwise.
*/
bool PGEcfgProfiles::setProfile(const std::string& sProfileName)
{
    getConsole().OLnOI("PGEcfgProfiles::SetProfile(%s)", sProfileName.c_str());
    if (sProfileName.empty())
    {
        // request erasing out profile data
        const bool b = setProfile(-1);
        getConsole().OO();
        return b;
    }

    if (getProfilesCount() == 0)
    {
        getConsole().EOLnOO("ERROR: there is no any profile!");
        return false;
    }

    for (int i = 0; i < nProfilesCount; i++)
    {
        if (*sFoundProfiles[i] == sProfileName)
        {
            const bool b = setProfile(i);
            // sanity check: setProfile() should set requested profile name too 
            if (b)
            {
                if (getProfileName() != sProfileName)
                {
                    getConsole().EOLnOO("ERROR: %s != %s!", getProfileName().c_str(), sProfileName.c_str());
                    // either readConfiguration() in SetProfile(index) failed or there is something else, so erase profile data
                    setProfile(-1);
                    return false;
                }
                else
                {
                    getConsole().SOLnOO("> done!");
                    return true;
                }
            }
            getConsole().EOLnOO("ERROR: found profile with index %d but could not set it!", i);
            return false;
        }
    }

    getConsole().EOLnOO("ERROR: there is no such profile!");
    return false;
} //SetProfile


/**
    Deletes a cvar by the given name.
    You cannot delete the cvar holding the name of the player because it would invalidate the profile.
    If you delete a cvar from the current profile, it will be deleted only on memory-level.
    To save changes made to the current profile, use writeConfiguration().
*/
void PGEcfgProfiles::DeleteVar(const char* varName)
{
    if ( (varName == NULL) || (getProfileIndex() == -1) )
        return;

    if ( strcmp(PGE_SYS_CFG_PLAYER_NAME_CVAR, varName) == 0 )
        return;

    m_vars.erase(varName);
} // DeleteVar()


/**
    Loads configuration for current profile from file.
    Note that config variables parsed earlier by ProcessCommandLine() will overwrite the variables read from profile config file.

    @return True on success, false on failure.
*/
bool PGEcfgProfiles::readConfiguration()
{
    getConsole().OLnOI("PGEcfgProfiles::readConfiguration()");
    if ( getProfileIndex() == -1 )
    {
        getConsole().EOLnOO("ERROR: current profile index is -1!");
        return false;
    }

    ClearVars();
    const string sFile = getPathToProfiles() + *sFoundProfiles[getProfileIndex()] + '\\' + (*sFoundProfiles[getProfileIndex()] + ".cfg");
    if ( !load(sFile.c_str()) )
    {
        getConsole().EOLnOO("ERROR: failed to load file: %s! ", sFile.c_str());
        return false;
    }

    for (const auto& clCvarPair : m_commandLineVars)
    {
        getConsole().OLn("Command Line overriding CVAR %s with value %s", clCvarPair.first.c_str(), clCvarPair.second.getAsString().c_str());
        getVars()[clCvarPair.first] = clCvarPair.second;
    }

    getConsole().SOLnOO("> done!");
    return true;
} // readConfiguration()


/**
    Saves configuration for current profile to file.
    @return True on success, false on failure.
*/
bool PGEcfgProfiles::writeConfiguration()
{
    getConsole().OLnOI("PGEcfgProfiles::writeConfiguration()");
    if ( getProfileIndex() == -1 )
    {
        getConsole().EOLnOO("ERROR: current profile index is -1!");
        return false;
    }

    const string sFile = getPathToProfiles() + *sFoundProfiles[getProfileIndex()] + '\\' + (*sFoundProfiles[getProfileIndex()] + ".cfg");
    ofstream f_cfg( sFile );
    if ( f_cfg.fail() )
    {
        getConsole().EOLnOO("ERROR: couldn't open file: %s", sFile.c_str());
        return false;
    }

    WriteConfiguration(f_cfg, *sFoundProfiles[getProfileIndex()], "");
    f_cfg.close();

    getConsole().SOLnOO("> done!");

    return true;
} // writeConfiguration()


// ############################## PROTECTED ##############################


bool PGEcfgProfiles::validateOnLoad(std::ifstream& f) const
{
    string tmp;
    f >> tmp;
    return tmp == PGE_SYS_CFG_FILE_MAGIC_START;
}

std::map<std::string, PGEcfgVariable>& PGEcfgProfiles::getCommandLineVars()
{
    return m_commandLineVars;
}


// ############################### PRIVATE ###############################


std::string PGEcfgProfiles::sPathToMyDocs;


PGEcfgProfiles::PGEcfgProfiles(const PGEcfgProfiles& other) :
    PGEcfgFile(other.getAllAcceptedVarsDefineRequirement(), other.getCaseSensitiveVars())
{

} 


PGEcfgProfiles& PGEcfgProfiles::operator=(const PGEcfgProfiles&)
{
    return *this;
}


/**
    Stores user docs folder path in sPathToMyDocs.
    @return True if successful, false on failure.
*/
bool PGEcfgProfiles::findMyDocsFolder()
{
    if ( !sPathToMyDocs.empty() )
    {
        CConsole::getConsoleInstance("PGEcfgProfiles").OLn("findMyDocsFolder(): already set folder: %s", sPathToMyDocs.c_str());
        return true;
    }

    char pPath[MAX_PATH];
    LPITEMIDLIST pIDL = PGENULL;

    if ( SHGetSpecialFolderLocation((HWND)0, CSIDL_PERSONAL, &pIDL) != 0 )
    {
        return false;
    }

    if ( !SHGetPathFromIDList(pIDL, pPath) )
    {   // ms layer for unicode is needed for this feature on win95-98-me (http://msdn.microsoft.com/en-us/library/ms812865.aspx) !!!
        return false;    
    }
    
    sPathToMyDocs.append(pPath).append("\\");
    return true;
} // findMyDocsFolder()


/**
    Gets player name from given cfg file.
*/
bool PGEcfgProfiles::getPlayerNameFromFile(const char* cFilename, std::string& sPlayerName)
{
    ifstream f_cfg(cFilename);

    if ( f_cfg.fail() )
    {
        return false;
    }

    if ( !validateOnLoad(f_cfg) )
    {
        return false;
    }

    const std::streamsize nBuffSize = 1024;
    char cLine[nBuffSize];
    bool l = false;
    while ( (!f_cfg.eof()) && (!l) )
    {
        f_cfg.getline(cLine, nBuffSize);
        l = ( strstr(cLine, PGE_SYS_CFG_PLAYER_NAME_CVAR) != NULL );
    }
    f_cfg.close();
    if ( !l )
        return false;

    // found line containing PGE_SYS_CFG_PLAYER_NAME_CVAR
    PFL::strClr(cLine);
    const std::string sTrimmedLine = cLine;
    std::string sVar;
    bool bParseError = false;

    return lineIsValueAssignment(sTrimmedLine, false, sVar, sPlayerName, bParseError);
} // getPlayerNameFromFile()


/**
    Fills up sFoundProfiles and sFoundProfilePlayerNames.
    Counts the found profiles (directories with cfg file inside them with the same name), stores it in nProfilesCount.
    If the current profile index is valid, it may change during this function but the current profile and loaded cvars
    remain unchanged.
*/
void PGEcfgProfiles::LoadProfilesList()
{
    getConsole().OLnOI("PGEcfgProfiles::LoadProfilesList() ...");
    // save current user
    string sOriginalCurrentProfileUser;
    if ( getProfileIndex() != -1 )
        sOriginalCurrentProfileUser = *( getProfilesList()[getProfileIndex()] );

    nProfilesCount = 0;
    char pOrigCurrDirr[1024];
    char pCurrDirr[1024];
    GetCurrentDirectory(1024, pOrigCurrDirr);
    GetCurrentDirectory(1024, pCurrDirr);
    if ( !SetCurrentDirectory(sPathToProfiles.c_str()) && (GetLastError() != ERROR_SUCCESS) )
    {
        getConsole().EOLnOO("ERROR: couldn't set current dir to: %s", sPathToProfiles.c_str());
        return;
    }

    SetLastError(ERROR_SUCCESS);  // need this because sometimes a last error was stuck and we couldn't enter the loop below

    WIN32_FIND_DATA dirdata;
    HANDLE hDirSearch = FindFirstFile("*.*", &dirdata);
    if ( hDirSearch == INVALID_HANDLE_VALUE )
    {
        getConsole().EOLnOO("ERROR: hDirSearch == INVALID_HANDLE_VALUE");
        return;
    }

    sFoundProfiles = PGENULL;
    sFoundProfilePlayerNames = PGENULL;
    while ( (GetLastError() != ERROR_NO_MORE_FILES) && (hDirSearch != INVALID_HANDLE_VALUE) )
    {
        string sCfgFileName;
        if ( (strcmp(dirdata.cFileName,".") != 0) && (strcmp(dirdata.cFileName,"..") != 0) )
        {
            if ( (dirdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
            {
                sCfgFileName = dirdata.cFileName;
                sCfgFileName += ".cfg";
                SetCurrentDirectory(dirdata.cFileName);
                WIN32_FIND_DATA filedata;
                HANDLE hFileSearch = FindFirstFile(sCfgFileName.c_str(), &filedata);
                if ( (GetLastError() != ERROR_NO_MORE_FILES) && (hFileSearch != INVALID_HANDLE_VALUE) )
                {
                    string sPlayerName;
                    // note: temporarily disabled file size checking ... decide later if it's needed
                    /*if ( (filedata.nFileSizeLow >= PGE_CFG_SETTINGSFILE_MIN_SIZE) &&
                         (filedata.nFileSizeLow <= PGE_CFG_SETTINGSFILE_MAX_SIZE) )*/
                    {   // checking file size, just in case ...
                        if ( getPlayerNameFromFile(sCfgFileName.c_str(), sPlayerName) )
                        {
                            nProfilesCount++; 
                            std::string** sNewFoundProfiles = PGENULL;
                            try
                            {
                                if ( sFoundProfiles )
                                {   // mimicing realloc, this to be changed to vector or something!
                                    sNewFoundProfiles = new string*[nProfilesCount]();
                                    memcpy(sNewFoundProfiles, sFoundProfiles, sizeof(string*)*nProfilesCount);
                                    delete[] sFoundProfiles;
                                    sFoundProfiles = sNewFoundProfiles;
                                }
                                else
                                {
                                    sFoundProfiles = new string*[nProfilesCount]();
                                }
                                sFoundProfiles[nProfilesCount-1] = new string(dirdata.cFileName);
                                if ( sFoundProfilePlayerNames )
                                {   // mimicing realloc, this to be changed to vector or something!
                                    std::string** sNewFoundProfilePlayerNames = new string*[nProfilesCount]();
                                    memcpy(sNewFoundProfilePlayerNames, sFoundProfilePlayerNames, sizeof(string*)*nProfilesCount);
                                    delete[] sFoundProfilePlayerNames;
                                    sFoundProfilePlayerNames = sNewFoundProfilePlayerNames;
                                }
                                else
                                {
                                    sFoundProfilePlayerNames = new string*[nProfilesCount]();
                                }
                                sFoundProfilePlayerNames[nProfilesCount-1] = new string(sPlayerName);
                                getConsole().OLn("added user %s ~ %s", dirdata.cFileName, sPlayerName.c_str());
                                if ( getProfileIndex() != -1 )
                                {   // update current profile index that may change during this function
                                    if ( *(sFoundProfiles[nProfilesCount-1]) == sOriginalCurrentProfileUser )
                                        nActiveProfile = nProfilesCount-1;
                                }
                            } // try
                            catch (const std::bad_alloc&)
                            {
                                FindClose(hFileSearch);
                                FindClose(hDirSearch);
                                SetCurrentDirectory( pOrigCurrDirr );
                                if ( sFoundProfiles != NULL )
                                {
                                    for (int i = 0; i < nProfilesCount-1; i++)
                                    {
                                        delete sFoundProfiles[i];
                                    }
                                }
                                if ( sFoundProfilePlayerNames != NULL )
                                {
                                    for (int i = 0; i < nProfilesCount-1; i++)
                                    {
                                        delete sFoundProfilePlayerNames[i];
                                    }
                                }
                                delete[] sFoundProfiles;
                                delete[] sFoundProfilePlayerNames;
                                getConsole().EOLnOO("ERROR: memory allocation failure!");
                                return;
                            }
                        }
                    } // file size check
                }
                FindClose(hFileSearch);
                SetCurrentDirectory( sPathToProfiles.c_str() );
            } // is directory
        } // ., ..
        // find more directories ...
        FindNextFile(hDirSearch, &dirdata);
    }
    FindClose(hDirSearch);
    SetCurrentDirectory( pOrigCurrDirr );

    getConsole().SOLnOO("> done!");

} // LoadProfilesList()


/**
    Removes all cvariables.
*/
void PGEcfgProfiles::ClearVars()
{
    m_vars.clear();
} // ClearVars()


/**
    Writes lines to an opened file.
    The stream must be opened.
    The stream must be closed outside of this function.

    @param f_cfg The opened file stream where the function will write to.
    @param sUser The user name of the config owner.
    @param sNick The nickname of the config owner.
                 If specified, only this cvar will be written to the stream, otherwise all.
                 Specify this only if this is a new profile being added and not yet current profile.
                 You can leave it empty if you want to save the current profile.
*/
void PGEcfgProfiles::WriteConfiguration(ofstream& f_cfg, const string& sUser, const string& sNick)
{
    f_cfg << PGE_SYS_CFG_FILE_MAGIC_START << endl << endl;
    f_cfg << "# " << sUser << "'s settings file" << endl << endl;
    
    if ( sNick.empty() )
    {
        f_cfg << "####### GAMEPLAY #######" << endl << endl;
        f_cfg << "####### MOUSE #######" << endl << endl;
        f_cfg << "####### KEYBOARD #######" << endl << endl;
        f_cfg << "####### VIDEO #######" << endl << endl;
        f_cfg << "####### AUDIO #######" << endl << endl;
        f_cfg << "####### SERVER #######" << endl << endl;

        std::map<std::string, PGEcfgVariable>::const_iterator it;
        for (it = m_vars.begin(); it != m_vars.end(); it++)
            f_cfg << it->first << "=" << it->second.getAsString() << endl;
    }
    else
        f_cfg << PGE_SYS_CFG_PLAYER_NAME_CVAR << "=" << sNick << endl;

    f_cfg << endl;
} // WriteConfiguration()



