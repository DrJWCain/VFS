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

// cTrans2QueryFSInfo.cpp
//
// References:
// [#1] SNIA CIFS Technical Reference Revision 1.0

#include "stdafx.h"

#if CIFS 

#include ".\ctrans2queryfsinfo.h"
#include "cSMBResponse.h"
#include "trans2Structs.h"
#include "cCalcDiskGeometry.h"

#include "..\iTreeResource.h"

cTrans2QueryFsInfo::cTrans2QueryFsInfo(const cPtr<iTreeResource> pTreeResource
  , const PREQ_QUERY_FS_INFORMATION pReq
  , cPtr<cSMBResponse> pResponse)
: cTrans2Response(pResponse)
, m_pTreeResource(pTreeResource)
{
  switch (pReq->InformationLevel)
  {
  case SMB_INFO_ALLOCATION:
    pResponse = Allocation(pResponse);
    break;
  case SMB_QUERY_FS_VOLUME_INFO:
    pResponse = VolumeInfo(pResponse);
    break;
  case SMB_QUERY_FS_SIZE_INFO:
    pResponse = SizeInfo(pResponse);
    break;
  case SMB_QUERY_FS_ATTRIBUTE_INFO:
    pResponse = AttributeInfo(pResponse);
    break;
  case SMB_QUERY_FS_DEVICE_INFO:
    pResponse = DeviceInfo(pResponse);
    break;
  case SMB_QUERY_CIFS_UNIX_INFO:
    pResponse = UnixInfo(pResponse);
    break;
  default:
    pResponse = processUnhandled(pResponse);
    break;
  }
}

cTrans2QueryFsInfo::~cTrans2QueryFsInfo(void)
{
}

// generic unhandled pResponse
cPtr<cSMBResponse> cTrans2QueryFsInfo::processUnhandled(cPtr<cSMBResponse> pResponse) const
{
  QSOS((L"Unhandled trans2 QUERY_FS_INFO request"));
  return cTrans2Response::processUnhandled();
}

// query fs commands
cPtr<cSMBResponse> cTrans2QueryFsInfo::VolumeInfo(cPtr<cSMBResponse> pResponse) const
{
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer); 
  query_FS_Volume_Info fsVol;
  const DWORD dwRet = m_pTreeResource->QueryFSVolumeInfo(&fsVol);
  if (dwRet != ERROR_SUCCESS)
  {
    pResponse->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }
  else
  {
    // point past byteCount fields
    pBuffer += sizeof(USHORT);

    // set parameter info
    setParams(pBuffer, 0);
    
    // set data info
    PsmbQueryFsVolumeInfo pQueryfsVolData = reinterpret_cast<PsmbQueryFsVolumeInfo>(pBuffer);
    ZeroMemory(pQueryfsVolData, sizeof(smbQueryFsVolumeInfo));
    pQueryfsVolData->Hours = fsVol.volCreationTime.wHour;
    pQueryfsVolData->Minutes = fsVol.volCreationTime.wMinute;
    pQueryfsVolData->TwoSeconds = fsVol.volCreationTime.wSecond/2;
    pQueryfsVolData->SerialNumber = fsVol.serialNumber;
    pQueryfsVolData->LabelLength = static_cast<ULONG>(fsVol.sLabel.size()*sizeof(WCHAR));
    pQueryfsVolData->Reserved1 = 0;
    pQueryfsVolData->Reserved2 = 0;
    wcscpy(pQueryfsVolData->Label, fsVol.sLabel.c_str());
    int dataSize = sizeof(smbQueryFsVolumeInfo) + pQueryfsVolData->LabelLength;
    setData(pBuffer, static_cast<USHORT>(dataSize));

    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer + dataSize
      - (sizeof(USHORT) + reinterpret_cast<PUCHAR>(pByteCount)));
  }

  return pResponse;
}

cPtr<cSMBResponse> cTrans2QueryFsInfo::SizeInfo(cPtr<cSMBResponse> pResponse) const
{
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer);

  DISK_GEOMETRY_EX disk_geometry;
  ULARGE_INTEGER free_space;
  DWORD sectorPerCluster;

  const DWORD dwRet = m_pTreeResource->QueryInfoAllocation(disk_geometry, &free_space, &sectorPerCluster);
  if (dwRet != ERROR_SUCCESS)
  {
    pResponse->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }
  else
  {
    // point past byteCount field
    pBuffer += sizeof(USHORT);

    // set parameter info (i.e. none)
    setParams(pBuffer, 0);
    
    // set data info
    cCalcDiskGeometry calcDiskGeometry(&disk_geometry, &free_space, sectorPerCluster);
    PsmbQueryFsSizeInfo pQueryFSSizeData = reinterpret_cast<PsmbQueryFsSizeInfo>(pBuffer);
    pQueryFSSizeData->TotalAllocationUnits.QuadPart = calcDiskGeometry.getTotalAllocUnits();
    pQueryFSSizeData->FreeAllocationUnits.QuadPart = calcDiskGeometry.getFreeAllocUnits();
    pQueryFSSizeData->sectorsPerAllocationUnit = 1;//max(disk_geometry.Geometry.SectorsPerTrack, 1);
    pQueryFSSizeData->bytesPerSector = disk_geometry.Geometry.BytesPerSector;
    int dataSize = sizeof(smbQueryFsSizeInfo);
    setData(pBuffer, static_cast<USHORT>(dataSize));

    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer + dataSize
      - (sizeof(USHORT) + reinterpret_cast<PUCHAR>(pByteCount)));
  }

  return pResponse;
}

cPtr<cSMBResponse> cTrans2QueryFsInfo::AttributeInfo(cPtr<cSMBResponse> pResponse) const
{
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer); 
  query_FS_Attribute_Info fsAtts;
  const DWORD dwRet = m_pTreeResource->QueryFSAttributeInfo(&fsAtts);
  if (dwRet != ERROR_SUCCESS)
  {
    pResponse->setDosError(SMB_ERR_CLASS_DOS, dwRet);
  }
  else
  {
    // point past byteCount field
    pBuffer += sizeof(USHORT);

    // set parameter info (i.e. none)
    setParams(pBuffer, 0);

    // set data info
    PsmbQueryFsAttributeInfo pQueryFSAttData = reinterpret_cast<PsmbQueryFsAttributeInfo>(pBuffer);
    pQueryFSAttData->FileSystemAttributes = fsAtts.attribs;
    pQueryFSAttData->MaxFileLength = fsAtts.maxFileNameLength;
    pQueryFSAttData->nameLength = static_cast<USHORT>(fsAtts.sName.size()*sizeof(WCHAR));
    wcscpy(pQueryFSAttData->sName, fsAtts.sName.c_str());
    int dataSize = sizeof(smbQueryFsAttributeInfo) + pQueryFSAttData->nameLength;
    setData(pBuffer, static_cast<USHORT>(dataSize));

    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer + dataSize
      - (sizeof(USHORT) + reinterpret_cast<PUCHAR>(pByteCount)));
  }

  return pResponse;
}

cPtr<cSMBResponse> cTrans2QueryFsInfo::DeviceInfo(cPtr<cSMBResponse> pResponse) const
{
  // RHRHRH - I could implement this properly through the iTreeResource interface
  // but at the moment we are only sharing remote file systems and I've got plenty else to do
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer); 

  // point past byteCount field
  pBuffer += sizeof(USHORT);

  // set parameter info (i.e. none)
  setParams(pBuffer, 0);
  
  // set data info
  PsmbQueryFsDeviceInfo pQueryDevice = reinterpret_cast<PsmbQueryFsDeviceInfo>(pBuffer);
  pQueryDevice->DeviceType = FILE_DEVICE_DISK_FILE_SYSTEM;
  pQueryDevice->Characteristics = FILE_REMOTE_DEVICE;
  int dataSize = sizeof(smbQueryFsDeviceInfo);
  setData(pBuffer, static_cast<USHORT>(dataSize));

  // set the byte count of the data section
  *pByteCount = static_cast<USHORT>(pBuffer + dataSize
    - (sizeof(USHORT) + reinterpret_cast<PUCHAR>(pByteCount)));

  return pResponse;
}

cPtr<cSMBResponse> cTrans2QueryFsInfo::Allocation(cPtr<cSMBResponse> pResponse) const
{
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer); 

  DISK_GEOMETRY_EX disk_geometry;
  ULARGE_INTEGER used_space;
  DWORD sectorsPerCluster;
  const DWORD dwRet = m_pTreeResource->QueryInfoAllocation(disk_geometry, &used_space, &sectorsPerCluster);

  if (dwRet != ERROR_SUCCESS)
  {
    pResponse->setDosError(SMB_ERR_CLASS_SERVER, dwRet);
  }
  else
  {
    // point past byteCount field
    pBuffer += sizeof(USHORT);

    // set parameter info (i.e. none)
    setParams(pBuffer, 0);
    
    // set data info
    cCalcDiskGeometry calcDiskGeometry(&disk_geometry, &used_space, sectorsPerCluster);
    PFSALLOCATE pQueryAllocation = reinterpret_cast<PFSALLOCATE>(pBuffer);
    pQueryAllocation->idFileSystem = 0;
    pQueryAllocation->cSectorUnit = sectorsPerCluster;
    pQueryAllocation->cUnit = calcDiskGeometry.getTotalAllocUnits();
    pQueryAllocation->cUnitAvail = calcDiskGeometry.getFreeAllocUnits();
    pQueryAllocation->cbSector = static_cast<USHORT>(disk_geometry.Geometry.BytesPerSector);
    int dataSize = sizeof(FSALLOCATE);
    setData(pBuffer, static_cast<USHORT>(dataSize));

    // set the byte count of the data section
    *pByteCount = static_cast<USHORT>(pBuffer + dataSize
      - (sizeof(USHORT) + reinterpret_cast<PUCHAR>(pByteCount)));
  }

  return pResponse;
}

cPtr<cSMBResponse> cTrans2QueryFsInfo::UnixInfo(cPtr<cSMBResponse> pResponse) const
{
  PUCHAR pBuffer = pResponse->getData(0);
  const PUSHORT pByteCount = reinterpret_cast<PUSHORT>(pBuffer); 

  // point past byteCount field
  pBuffer += sizeof(USHORT);

  // set parameter info (i.e. none)
  setParams(pBuffer, 0);
  
  // set data info
  PRESP_SMB_QUERY_CIFS_UNIX_INFO pQueryUnix = reinterpret_cast<PRESP_SMB_QUERY_CIFS_UNIX_INFO>(pBuffer);
  pQueryUnix->MajorVersion = 1;
  pQueryUnix->MinorVersion = 0;
  pQueryUnix->Flags = CIFS_UNIX_LARGE_READ_CAP;
  int dataSize = sizeof(RESP_SMB_QUERY_CIFS_UNIX_INFO);
  setData(pBuffer, static_cast<USHORT>(dataSize));

  // set the byte count of the data section
  *pByteCount = static_cast<USHORT>(pBuffer + dataSize
    - (sizeof(USHORT) + reinterpret_cast<PUCHAR>(pByteCount)));

  return pResponse;
}

#endif // CIFS 