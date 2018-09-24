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

//cCalcDiskGeometry.h

#pragma once

class cCalcDiskGeometry
{
public:
  cCalcDiskGeometry(PDISK_GEOMETRY_EX pDiskGeometry, PULARGE_INTEGER pFreeSpace, DWORD sectorPerCluster)
  : m_pDiskGeometry(pDiskGeometry)
  , m_pFreeSpace(pFreeSpace)
  , m_dwBytesPerCluster(pDiskGeometry->Geometry.BytesPerSector * sectorPerCluster)
  {
    if (m_dwBytesPerCluster <= 0)
      m_dwBytesPerCluster = 1;
    if (m_pDiskGeometry->DiskSize.QuadPart <= 0)
      m_pDiskGeometry->DiskSize.QuadPart = 1;
  };

  ~cCalcDiskGeometry() { }; // stubbed

  ULONG getTotalAllocUnits()
  {
    return static_cast<ULONG>(m_pDiskGeometry->DiskSize.QuadPart);
  };
  
  ULONG getFreeAllocUnits()
  {
    return static_cast<ULONG>(m_pFreeSpace->QuadPart);
  };

private:
  DWORD m_dwBytesPerCluster;

  const PDISK_GEOMETRY_EX m_pDiskGeometry;
  const PULARGE_INTEGER m_pFreeSpace;
};
