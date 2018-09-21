// Copyright 2018 Grass Valley, A Belden Brand

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//cGenTreeResource.cpp

#include "StdAfx.h"

#include ".\cgentreeresource.h"

#include "cShare.h"
#include "iDirLocation.h"
#include "cWriteInfo.h"
#include "iNameMangler.h"
#include "cQCIFSFwkHelper.h"
#include "iQCIFSFwk.h"
#include "cRenameHandler.h"
#include "QTubeOptions.h"
#include "LogCategories.h"

#include "..\iComposite.h"
//#include "../../MiscPlugins/QOptions\QOptions.h"

using namespace vfs;

namespace
{
  static unsigned int getQTubeOptionsBitmask()
  {
    unsigned short optionsBitmask(USHRT_MAX);

    //// QTube Option check (SQCF-1129)
    //// Enables certain URLs in QTube
    //// checked on each GenqTreeResource constructor. Assumes Transformers have local licences
    //try
    //{
    //  iOptionManager::singleton().checkOut (L"QTube Transformer Product"); // only perform options checks on transformers
    //  QTRACE((L"Checking QTube Transformer options"));

    //  optionsBitmask = 0;
    //  try
    //  {
    //    iOptionManager::singleton().checkOut (L"QTube API Developer");
    //    optionsBitmask += kQTubeAPIDeveloper;
    //    QTRACE((L"Got QTube Developer option"));
    //  }
    //  catch (cRecoverable&) {}
    //  try
    //  {
    //    iOptionManager::singleton().checkOut (L"QTube API Publish");
    //    optionsBitmask += kQTubeAPIPublish;
    //    QTRACE((L"Got QTube Publish option"));
    //  }
    //  catch (cRecoverable&) {}
    //  try
    //  {
    //    iOptionManager::singleton().checkOut (L"QTube API Browser");
    //    optionsBitmask += kQTubeAPIBrowser;
    //    QTRACE((L"Got QTube Browser option"));
    //  }
    //  catch (cRecoverable&) {}
    //  try
    //  {
    //    iOptionManager::singleton().checkOut (L"QTube API Edit");
    //    optionsBitmask += kQTubeAPIEdit;
    //    QTRACE((L"Got QTube Edit option"));
    //  }
    //  catch (cRecoverable&) {}
    //  try
    //  {
    //    iOptionManager::singleton().checkOut (L"QTube API Seat");
    //    optionsBitmask += kQTubeAPISeat;
    //    QTRACE((L"Got QTube Seat option"));
    //  }
    //  catch (cRecoverable&) {}
    //  try
    //  {
    //    iOptionManager::singleton().checkOut (L"QTube API Intersite");
    //    optionsBitmask += kQTubeAPIIntersite;
    //    QTRACE((L"Got QTube Intersite option"));
    //  }
    //  catch (cRecoverable&) {}
    //  QTRACE((L"Done checking QTube Transformer options"));
    //}
    //catch (cRecoverable&) {}

    return optionsBitmask;
  }

  static const unsigned int m_QTube_API_OptionsBitmask(getQTubeOptionsBitmask());
}

SYSTEMTIME getCurrentTime()
{
  SYSTEMTIME st;
  GetSystemTime(&st);
  return st;
}

cGenTreeResource::cGenTreeResource(const cPtr<cShare> pShare
  , const cPtr<cRenameHandler> pRenameHandler
  , const cPtr<iNameMangler> pNameMangler
  , const cPtr<iNamespaceCreator> pNamespaceCreator)
: m_pShare(pShare)
, m_pRenameHandler(pRenameHandler)
, m_pNameMangler(pNameMangler)
, m_pNamespaceCreator(pNamespaceCreator)
, m_creationTime(getCurrentTime())
{ 
}

cGenTreeResource::~cGenTreeResource(void)
{
}

// operations
DWORD cGenTreeResource::Search(const String& searchPath, tFileInfoList &fileInfoList)
{
  if (m_pShare.isNull())
    return ERROR_BAD_ENVIRONMENT;

  DWORD ret = m_pShare->getSearchResults(searchPath, fileInfoList);

  if ((ERROR_SUCCESS != ret) && fileInfoList.empty())
  {
    bool tryAgain = false;
    if(m_pNamespaceCreator.isValid())
      tryAgain = m_pNamespaceCreator->lazyDirectoryCheck(searchPath, m_pRenameHandler->getLogin());
    if(tryAgain)
    {
      cPtr<iDirLocation> pParentLocation = getParent(searchPath);
      if(pParentLocation.isValid())
        pParentLocation->refreshCache();

      ret = m_pShare->getSearchResults(searchPath, fileInfoList);
      
    }
  }
  return ret;
}

cPtr<iComposite> cGenTreeResource::NTCreateFolder(const String& searchPath
  , LPSECURITY_ATTRIBUTES lpAtts
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err)
{
  const String sMangledName = makeMangledName(searchPath);
  const cPtr<iComposite> pComposite = openExisting(sMangledName
    , 0
    , 0
    , lpAtts
    , OPEN_EXISTING
    , 0
    , NULL
    , pFileInfo
    , err
    , cPtr<iFileProcessingCallback>()
    , true);
  if (pComposite.isNull())
    return createNew(sMangledName
    , 0
    , 0
    , lpAtts
    , CREATE_NEW
    , FILE_ATTRIBUTE_DIRECTORY
    , NULL
    , pFileInfo
    , err
    , cPtr<iFileProcessingCallback>());

  err = ERROR_FILE_EXISTS;
  return cPtr<iComposite>();
}

cPtr<iComposite> cGenTreeResource::NTCreateFile(const String& searchPath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err
  , cPtr<iFileProcessingCallback> pCallback
  , ULONGLONG fid)
{
  cPtr<iComposite> ret; 

  if (m_pShare.isNull())
  {
    err = ERROR_BAD_ENVIRONMENT;
  }
  else if (!optionCheck(searchPath) && (dwDesiredAccess != 128) && (searchPath.find(L".") != String::npos) && (searchPath.find(L"web.config") == String::npos)) // Access 128 is IIS configuration thread monitoring changes to web.config
  {
    QSOS((L"Denied access to %s, rights asked for %d", searchPath.c_str(), int(dwDesiredAccess)));
    err = ERROR_ACCESS_DENIED;
  }
  else if (cQCIFSFwkHelper::singleton().lowMemory(95))
  {
    QSOS((L"Refusing create request - low memory."));
    err = ERROR_TOO_MANY_OPEN_FILES;
    if (m_QTube_API_OptionsBitmask != USHRT_MAX) // True if product is a Transformer
    {
      iBootManager::singleton().setState(iBootManager::eBootState_RestartNecessary);
      iExceptionManager::singleton().fatal (
        0, /* flags */
        iExceptionManager::cFatalDiagnostics (
        EXIT_SUCCESS, "cGenTreeResource.cpp" /* src file */, 198 /* src line */, L"" /* heading */, L"Low Memory - Closing"));
    }
  }
  else
  {
    const String sMangledName = makeMangledName(searchPath);
    switch(dwCreationDisposition)
    {
    case CREATE_NEW:
      ret = createNew(sMangledName
        , dwDesiredAccess
        , dwShareMode
        , lpAtts
        , dwCreationDisposition
        , dwFlagsAndAtts
        , hTemplateFile
        , pFileInfo
        , err
        , pCallback);
      break;
    case CREATE_ALWAYS:
    case OPEN_ALWAYS:
      {
        const cPtr<iComposite> pComposite = openExisting(sMangledName
          , dwDesiredAccess
          , dwShareMode
          , lpAtts
          , dwCreationDisposition
          , dwFlagsAndAtts
          , hTemplateFile
          , pFileInfo
          , err
          , pCallback
          , false
          , fid);
        if (pComposite.isValid())
          ret = pComposite;
        else
        {
          ret = createNew(sMangledName
            , dwDesiredAccess
            , dwShareMode
            , lpAtts
            , dwCreationDisposition
            , dwFlagsAndAtts
            , hTemplateFile
            , pFileInfo
            , err
            , pCallback);
        }
        break;
      }

      break;
    default:
      ret = openExisting(sMangledName
      , dwDesiredAccess
      , dwShareMode
      , lpAtts
      , dwCreationDisposition
      , dwFlagsAndAtts
      , hTemplateFile
      , pFileInfo
      , err
      , pCallback
      , false
      , fid);
      break;
    }
  }

  return ret;
}

DWORD cGenTreeResource::DeleteDirectory(const String &sPath)
{
  return Delete(0, makeMangledName(sPath));
}

DWORD cGenTreeResource::Delete(DWORD SearchAttributes, const String &filename)
{
  const String sMangledName(makeMangledName(filename));
  const cPtr<iDirLocation> pDirLocation = get(sMangledName);
  if (pDirLocation.isNull())
    return ERROR_PATH_NOT_FOUND;

  DWORD dw;
  const cPtr<iComposite> pComposite = pDirLocation->Create(sMangledName
    , GENERIC_READ
    , FILE_SHARE_READ
    , NULL
    , OPEN_EXISTING
    , FILE_ATTRIBUTE_NORMAL
    , NULL
    , dw
    , cPtr<iFileProcessingCallback>());
  return pComposite.isValid() ? pComposite->Delete() : dw;
}

DWORD cGenTreeResource::Rename(DWORD SearchAttributes
  , const std::wstring &originalPath
  , const std::wstring &newPath)
{
  // get the source
  cPtr<iDirLocation> pOriginalLocation = get(makeMangledName(originalPath));
  if (!pOriginalLocation.isValid())
    return ERROR_PATH_NOT_FOUND;

  cPtr<iRename> pSourceRename = pOriginalLocation->Rename();
  if (pSourceRename.isNull())
    return ERROR_ACCESS_DENIED;

  return m_pRenameHandler->Rename(pSourceRename, makeMangledName(newPath));
}

DWORD cGenTreeResource::QueryBasicInfo(const std::wstring &rsPath, LPWIN32_FILE_ATTRIBUTE_DATA pFileInfo)
{
  BY_HANDLE_FILE_INFORMATION bhi;

  const DWORD dwRet = getData(makeMangledName(rsPath), &bhi);
  if (ERROR_SUCCESS == dwRet)
  {
    pFileInfo->dwFileAttributes = bhi.dwFileAttributes;
    pFileInfo->ftCreationTime = bhi.ftCreationTime;
    pFileInfo->ftLastAccessTime = bhi.ftLastAccessTime;
    pFileInfo->ftLastWriteTime = bhi.ftLastWriteTime;
    pFileInfo->nFileSizeHigh = bhi.nFileSizeHigh;
    pFileInfo->nFileSizeLow = bhi.nFileSizeLow;
  }

  return dwRet;
}

DWORD cGenTreeResource::QueryStandardInfo(const String& rsPath, LPBY_HANDLE_FILE_INFORMATION pFileInfo)
{
  return getData(rsPath, pFileInfo);
}

DWORD cGenTreeResource::getData(const String& rsPath, LPBY_HANDLE_FILE_INFORMATION pFileInfo) const
{
  String searchPath = makeMangledName(rsPath);
  cPtr<iDirLocation> pDirLocation = get(searchPath);
  if (pDirLocation.isNull())
  {
    bool tryAgain = false;
    if(m_pNamespaceCreator.isValid())
      tryAgain = m_pNamespaceCreator->lazyDirectoryCheck(searchPath, m_pRenameHandler->getLogin());
    if(tryAgain)
    {
      cPtr<iDirLocation> pParentLocation = getParent(searchPath);
      if(pParentLocation.isValid())
        pParentLocation->refreshCache();
      pDirLocation = get(searchPath);
    }
    if (pDirLocation.isNull())
    {
      return ERROR_FILE_NOT_FOUND;
    }
  }

  fileInfo fi;
  const DWORD dwRet = pDirLocation->writeInfo(&fi, 0);
  if (ERROR_SUCCESS == dwRet)
    cWriteInfo::singleton().writeFileInfo(pFileInfo, &fi);

  return dwRet;
}

DWORD cGenTreeResource::QueryFSAttributeInfo(query_FS_Attribute_Info *pFSAttribInfo)
{
  //QTRACE((L"cGenTreeResource::QueryFSAttributeInfo()"));
  //[#1] p.61

  if (m_pShare.isNull())
    return ERROR_BAD_ENVIRONMENT;

  pFSAttribInfo->attribs =  FILE_CASE_SENSITIVE_SEARCH
    | FILE_CASE_PRESERVED_NAMES
    | FILE_UNICODE_ON_DISK
    | FILE_PERSISTENT_ACLS
    | FILE_FILE_COMPRESSION
    | FILE_VOLUME_QUOTAS
    | FILE_SUPPORTS_SPARSE_FILES
    | FILE_SUPPORTS_REPARSE_POINTS
    | FILE_SUPPORTS_OBJECT_IDS
    | FILE_SUPPORTS_ENCRYPTION;
    //| FILE_NAMED_STREAMS;  Not currently supported here (SQCF-191)
  pFSAttribInfo->maxFileNameLength = _MAX_FNAME-1;
  pFSAttribInfo->sName = m_pShare->Name();

  return ERROR_SUCCESS;
}

DWORD cGenTreeResource::QueryFSVolumeInfo(PQuery_FS_Volume_Info pFSVolInfo)
{
  //QTRACE((L"cGenTreeResource::QueryFSVolumeInfo()"));
  //[#1] p.59

  if (m_pShare.isNull())
    return ERROR_BAD_ENVIRONMENT;

  WCHAR pNameBuff[255];
  WCHAR pFileNameBuff[255];
  DWORD maxComponentLength;
  DWORD fileFlags;
  GetVolumeInformation(L"C:\\"
    , pNameBuff
    , 255
    , &(pFSVolInfo->serialNumber)
    , &maxComponentLength
    , &fileFlags
    , pFileNameBuff
    , 255);

  pFSVolInfo->volCreationTime = m_creationTime;
  pFSVolInfo->sLabel = m_pShare->label();

  return ERROR_SUCCESS;
}

DWORD cGenTreeResource::QueryInfoAllocation(DISK_GEOMETRY_EX &disk_geometry, PULARGE_INTEGER pulFreeSpace
                                  , LPDWORD pSectorsPerCluster)
{
  if (m_pShare.isNull())
    return ERROR_BAD_ENVIRONMENT;

  disk_geometry.Geometry.BytesPerSector = m_pShare->bytesPerSector();
  disk_geometry.DiskSize.QuadPart = m_pShare->totalSpace();
  pulFreeSpace->QuadPart = m_pShare->freeSpace();
  *pSectorsPerCluster = m_pShare->sectorsPerCluster();

  return ERROR_SUCCESS;
}

// constants
DWORD cGenTreeResource::getBytesPerCluster()
{
  if (m_pShare.isNull())
    return ERROR_BAD_ENVIRONMENT;

  return m_pShare->bytesPerSector() * m_pShare->sectorsPerCluster();
}

inline cPtr<iDirLocation> cGenTreeResource::get(const String& sPath) const
{
  if (m_pShare.isValid())
    return m_pShare->get(sPath);

  QSOS((L"cGenTreeResource::get() - bad directory factory"));
  return cPtr<iDirLocation>();
}

cPtr<iDirLocation> cGenTreeResource::getParent(const String& sPath) const
{
  if (m_pShare.isValid())
    return m_pShare->getParent(sPath);

  return cPtr<iDirLocation>();
}

cPtr<iComposite> cGenTreeResource::openExisting(const String& searchPath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err
  , cPtr<iFileProcessingCallback> pCallback
  , bool isFolder
  , ULONGLONG fid) const
{
  // SQCF-1685 - Ignore web.config files so we process the file/folder ourselves, rather than IIS looking up the web.config rule for
  // IIS to process the file/folder. This improves thumbnail loading speed, as we aren't automatically loading each thumbnail.
  if (searchPath.find(L"web.config") != String::npos 
    && searchPath != L"\\web.config" 
    && searchPath.find(L"search") == String::npos     // We need web.config files for search strings
    && searchPath.find(L"\\um\\") == String::npos)    // We need web.config for user management folders to manage user authorization
  {
    QSOS((L"Found web.config in %s", searchPath.c_str()));
    err = ERROR_FILE_NOT_FOUND;
    return cPtr<iComposite>();
  }

  cPtr<iDirLocation> pDirLocation = get(searchPath);
  if (pDirLocation.isNull())
  {
    bool tryAgain = false;
    if(m_pNamespaceCreator.isValid())
      tryAgain = m_pNamespaceCreator->lazyDirectoryCheck(searchPath, m_pRenameHandler->getLogin());
    if(tryAgain)
    {
      cPtr<iDirLocation> pParentLocation = getParent(searchPath);
      if(pParentLocation.isValid())
        pParentLocation->refreshCache();
      pDirLocation = get(searchPath);
    }
    if (pDirLocation.isNull())
    {
      if(isFolder)
        err = ERROR_PATH_NOT_FOUND;
      else
        err = ERROR_FILE_NOT_FOUND;
      return cPtr<iComposite>();
    }
  }

  cPtr<iComposite> pComposite = pDirLocation->Create(searchPath
    , dwDesiredAccess
    , dwShareMode
    , lpAtts
    , OPEN_EXISTING
    , dwFlagsAndAtts
    , hTemplateFile
    , err
    , pCallback);
  if (pComposite.isValid())
  {
    pComposite->writeInfo(pFileInfo, fid);
    err = ERROR_SUCCESS;
  }

  return pComposite;
}

cPtr<iComposite> cGenTreeResource::createNew(const String& searchPath
  , DWORD dwDesiredAccess
  , DWORD dwShareMode
  , LPSECURITY_ATTRIBUTES lpAtts
  , DWORD dwCreationDisposition
  , DWORD dwFlagsAndAtts
  , HANDLE hTemplateFile
  , LPBY_HANDLE_FILE_INFORMATION pFileInfo
  , DWORD &err
  , cPtr<iFileProcessingCallback> pCallback)
{
  cPtr<iDirLocation> pDirLocation = m_pShare->getParent(searchPath);
  if (pDirLocation.isNull())
  {
    bool tryAgain = false;
    if(m_pNamespaceCreator.isValid())
      tryAgain = m_pNamespaceCreator->lazyDirectoryCheck(searchPath, m_pRenameHandler->getLogin());
    if(tryAgain)
    {
      cPtr<iDirLocation> pParentLocation = getParent(searchPath);
      if(pParentLocation.isValid())
        pParentLocation->refreshCache();
      pDirLocation = get(searchPath);
    }
    if (pDirLocation.isNull())
    {
      err = ERROR_PATH_NOT_FOUND;
      return cPtr<iComposite>();
    }
  }

  cPtr<iComposite> pComposite = pDirLocation->Create(searchPath
    , dwDesiredAccess
    , dwShareMode
    , lpAtts
    , dwCreationDisposition
    , dwFlagsAndAtts
    , hTemplateFile
    , err
    , pCallback);
  if (pComposite.isValid())
    pComposite->writeInfo(pFileInfo, 0);

  return pComposite;
}

inline String cGenTreeResource::makeMangledName(const String& sPath) const
{
  if (m_pNameMangler.isValid())
    return m_pNameMangler->makeMangledName(sPath);
  return sPath;
}

bool cGenTreeResource::optionCheck(const String& path)
{ 
  bool ret = true;
  QMSG((kCtgLicenseOptions, L"Checking  %s - %d", path.c_str(), m_QTube_API_OptionsBitmask));

  // First lower the path so we're not case sensitive
  String testPath(toLower(path));

  // Now switch all zones to be homezone
  String::size_type zonePos = testPath.find(L"zone-");
  if (zonePos != String::npos)
  {
    String::size_type endPos = testPath.find(L"\\", zonePos);
    if (endPos != String::npos)
      testPath = L"\\homezone" + testPath.substr(endPos);
    else 
      testPath = L"\\homezone";
  }

  String buildPath = L"";
  String::size_type pos = 0;
  int ctr = 0;
  const bool isClipsOrRushes = (testPath.find(L"\\clips\\") != String::npos) || (testPath.find(L"\\rushes\\") != String::npos);
  do 
  {   
    String::size_type nextPos = testPath.find(L"\\", pos+1);
    if ((ctr++ == 3) && isClipsOrRushes) // This is because where ctr == 3 in path is a clip or rush id
    {
      if (nextPos == String::npos)
        break;
      testPath = testPath.substr(0, pos) + testPath.substr(nextPos);
    }
    else
    {
      pos = nextPos;
      String blah = testPath.substr(0, pos);
      std::map<String, unsigned int>::const_iterator option = kQTube_API_Options.find(blah);
      if (option != kQTube_API_Options.end())
      {
        QMSG((kCtgLicenseOptions, L"Found option check %d", option->second));
        ret = (option->second & m_QTube_API_OptionsBitmask) != 0;
        break;
      }
    }
  }
  while (pos != String::npos);

  if (ret)
    QMSG((kCtgLicenseOptions, L"Allowing %s", path.c_str()));
  else 
    QMSG((kCtgLicenseOptions, L"Refusing %s", path.c_str()));

  return ret;
}