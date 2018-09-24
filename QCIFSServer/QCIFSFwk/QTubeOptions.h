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


#pragma once

#include "boost/assign.hpp"

// Bitmask
const unsigned int kQTubeAPIDeveloper = 1;
const unsigned int kQTubeAPIPublish = 2;
const unsigned int kQTubeAPIBrowser = 4;
const unsigned int kQTubeAPIEdit = 8;
const unsigned int kQTubeAPISeat = 16;
const unsigned int kQTubeAPIIntersite = 32;

const unsigned int kQTubeAPIAll = kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIBrowser + kQTubeAPIEdit + kQTubeAPISeat + kQTubeAPIIntersite;


const std::map<vfs::String, unsigned int> kQTube_API_Options = boost::assign::map_list_of 
(L"\\index.html", kQTubeAPIBrowser) 
(L"\\qtubenew.xap", kQTubeAPIBrowser)
(L"\\qtube.xap", kQTubeAPIBrowser)
(L"\\qtube.html", kQTubeAPIAll)
(L"\\dev.html", kQTubeAPIAll)
(L"\\bin", kQTubeAPIAll)
(L"\\command", kQTubeAPIAll)
(L"\\clone", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIIntersite)
(L"\\process", kQTubeAPIAll)
(L"\\publish", kQTubeAPIAll)
(L"\\publish2", kQTubeAPIAll)
(L"\\homezone\\areaproperties.xml", kQTubeAPIAll)
(L"\\homezone\\customframesizes.xml", kQTubeAPIBrowser)
(L"\\homezone\\groupproperties.xml", kQTubeAPIAll)
(L"\\homezone\\loggingproperties.xml", kQTubeAPIAll)
(L"\\homezone\\searchconstraints.xml", kQTubeAPIAll) // This is used for all searching
(L"\\homezone\\searchservice", kQTubeAPIAll) 
(L"\\homezone\\seatproperties.xml", kQTubeAPIAll)
(L"\\homezone\\siteproperties.xml", kQTubeAPIAll)
(L"\\homezone\\transformerproperties.xml", kQTubeAPIAll)
(L"\\homezone\\customframesizes.xml", kQTubeAPIBrowser)
(L"\\homezone\\zoneinfo.xml", kQTubeAPIAll)
(L"\\homezone\\restore", kQTubeAPIAll)
(L"\\homezone\\clips\\search", kQTubeAPIAll) 
(L"\\homezone\\clips\\stills", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIBrowser + kQTubeAPIIntersite)
(L"\\homezone\\clips\\qstreams", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIBrowser + kQTubeAPIIntersite)
(L"\\homezone\\clips\\streams", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIBrowser + kQTubeAPIIntersite)
(L"\\homezone\\clips\\ports", kQTubeAPIAll)
(L"\\homezone\\clips\\metadata\\manifest.xml", kQTubeAPIAll)
(L"\\homezone\\clips\\metadata\\vfs.aaf", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIEdit + kQTubeAPISeat + kQTubeAPIIntersite)
(L"\\homezone\\clips\\metadata\\relations.xml", kQTubeAPIAll)
(L"\\homezone\\rushes\\direct", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIIntersite)
(L"\\homezone\\rushes\\stills", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIBrowser + kQTubeAPIIntersite)
(L"\\homezone\\rushes\\qstreams", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIIntersite)
(L"\\homezone\\rushes\\streams", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIIntersite + kQTubeAPIEdit)
(L"\\homezone\\rushes\\nstreams", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIIntersite + kQTubeAPISeat)
(L"\\homezone\\rushes\\nqstreams", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIIntersite + kQTubeAPISeat)
(L"\\homezone\\rushes\\ports", kQTubeAPIAll)
(L"*", kQTubeAPIDeveloper + kQTubeAPIPublish + kQTubeAPIBrowser + kQTubeAPIEdit + kQTubeAPISeat + kQTubeAPIIntersite);