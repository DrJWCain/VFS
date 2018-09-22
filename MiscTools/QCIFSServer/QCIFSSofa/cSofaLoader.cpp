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

//cSofaLoader.cpp

#include "StdAfx.h"

#include "cSofaLoader.h"
#include "cBasicFile.h"

#include "..\QCIFSFwk\iChildLoaderVisitor.h"
#include "..\QCIFSFwk\iQCIFSFwkHelper.h"
#include "..\QCIFSFwk\iWriteInfo.h"

#include <boost/regex.hpp>
using namespace vfs;

#include "cCurlWrapper.h"

#include "..\..\Shared\json_spirit\json_spirit_value.h"
#include "..\..\Shared\json_spirit\json_spirit_reader_template.h"
#include "..\..\Shared\json_spirit\json_spirit_writer_template.h"

void dump(cMemoryView::Ptr text)
{
  if(text.isNull())
    return;
  std::string str(text->getConstBytes(), text->getSize());
  std::stringstream strStream(str);
  while(!strStream.eof())
  {
    std::string line;
    std::getline(strStream, line);
    QTRACE((L"%S", line.c_str()));
  }
}

void dump(const std::string& str)
{
  std::stringstream strStream(str);
  while(!strStream.eof())
  {
    std::string line;
    std::getline(strStream, line);
    QTRACE((L"%S", line.c_str()));
  }
}

void dump16(cMemoryView::Ptr text)
{
  if(text.isNull())
    return;
  String str((wchar_t*)text->getConstBytes(), text->getSize() / 2);
  StringStream strStream(str);
  while(!strStream.eof())
  {
    String line;
    std::getline(strStream, line);
    QTRACE((L"%s", line.c_str()));
  }
}

#define Uint8 unsigned char
#define Uint32 unsigned long
void dumpBuffer(Uint8* ptr, Uint32 lengthToPrint)
{
  const Uint8* lastLine = 0;
  Uint32 byte = 0;
  Uint32 i = 0;
  wchar_t line[80], *linePtr;

  linePtr = line;

  QTRACE((L"memory %p, lengthToPrint 0x%08x", ptr, lengthToPrint));

  while(i<lengthToPrint)
  {
    if(!(i & 15)) // line break every mod 16
    {
      if(lastLine)
      {
        QTRACE((L"%04x %s", i - 16, line));
      }

      //printf("\n ");
      lastLine = ptr;
      linePtr = line;
      *line = 0;
    }

    wsprintf(linePtr, L"%02x ", *ptr++);
    i++;
    byte++;
    linePtr += 3;
  }

  if(lastLine)
  {
    QTRACE((L"%04x %s", i - (i % 16), line));
  }
}



json_spirit::Object getSpiritObjectFromMemorySafe(vfs::cMemoryView::Ptr mem)
{
  if(mem.isValid())
  {
    char* bytes = mem->getBytes();
    if(bytes[mem->getSize() - 1] == 0x0a)
      bytes[mem->getSize() - 1] = 0;

    //dump(mem);
    std::string str;
    try
    {
      str = std::string(mem->getConstBytes(), mem->getSize());
      //various evil tests ...
      //str = std::string(mem->getConstBytes(), mem->getSize()/2);
      //str = std::string(mem->getConstBytes()+(mem->getSize()/2), mem->getSize()/2);
      //str[0] = '!';
      //... end various evil tests
    }
    catch(...)
    {
      QSOS((L"... !!! Exception converting mem to string !!! ... %d", mem->getSize()));
      dump(mem);
      throw cNotFound(__FILE__, __LINE__, L"... !!! Exception converting mem to string !!! ... %d", mem->getSize());
    }
    //QTRACE((L"%d %S", mem->getSize(), str.c_str()));

    json_spirit::Value value;
    try
    {
      json_spirit::read_string(str, value);
      //readJSONValue(value, str, mem);
      //if(value.is_null())
      //  throw cNotFound(__FILE__, __LINE__, L"");
    }
    catch(...)
    {
      QSOS((L"... !!! Exception calling json_spirit::read_string !!! ... %d-%d", mem->getSize(), str.size()));
      QTRACE((L"%S", str.c_str()));
      dump(mem);
      QTRACE((L"start ... "));
      dumpBuffer((Uint8*)mem->getConstBytes(), (unsigned long)mem->getSize());
      QTRACE((L" ... stop"));
      throw cNotFound(__FILE__, __LINE__, L"... !!! Exception calling json_spirit::read_string !!! ... %d-%d", mem->getSize(), str.size());
    }

    if(value.is_null())
      throw cNotFound(__FILE__, __LINE__, L"getSpiritObjectFromMemorySafe %S", str.c_str());

    json_spirit::Object obj;
    try
    {
      obj = value.get_obj();
    }
    catch(...)
    {
      QSOS((L"... !!! Exception calling json_spirit::Value::get_obj !!! ... %d-%d", mem->getSize(), str.size()));
      QTRACE((L"%S", str.c_str()));
      dump(mem);
      throw cNotFound(__FILE__, __LINE__, L"... !!! Exception calling json_spirit::Value::get_obj !!! ... %d-%d", mem->getSize(), str.size());
    }

    return obj;
  }

  throw cNotFound(__FILE__, __LINE__, L"getSpiritObjectFromMemorySafe NO MEM");
}

std::string getDBPath()
{
  return "http://127.0.0.1:5984/music/";
}


struct cObjVersion
{
  vfs::String ID;
  vfs::String Version;
};

cObjVersion writeVersionedObject(const std::string& dbPath, const json_spirit::Object& obj, bool log)
{
  cObjVersion ret;

  json_spirit::Value value(obj);
  std::string result = json_spirit::write_string(value, json_spirit::pretty_print);

  vfs::cMemoryView::Ptr mem = new vfs::cMemoryView(new cMemory(result.size(), cMemory::eHeap));
  memcpy(mem->getBytes(), result.c_str(), mem->getSize());
  if(log)
    dump(mem);

  vfs::cMemoryView::Ptr reply = HTTPPostJSONWithReply(dbPath, mem, log);
  if(reply.isValid())
  {
    if(log)
      dump(reply);

    const json_spirit::Object& obj = getSpiritObjectFromMemorySafe(reply);
    for(json_spirit::Object::const_iterator it = obj.begin(); it != obj.end(); ++it)
    {
      const std::string& name = it->name_;
      const json_spirit::Value& value = it->value_;
      if(name == "id")
      {
        ret.ID = widen(value.get_str());
      }
      if(name == "rev")
      {
        ret.Version = widen(value.get_str());
      }
    }
  }

  return ret;
}

cObjVersion writeVersionedObject(const json_spirit::Object& obj, bool log)
{
  return writeVersionedObject(getDBPath(), obj, log);
}



unsigned int getDBSeqNo(const std::string& path, bool log)
{
  unsigned int seqNo = 0;
  try
  {
    vfs::cMemoryView::Ptr reply = HTTPGet_basic(path, log, 0);
    if(reply.isValid())
    {
      if(log)
        dump(reply);

      const json_spirit::Object& obj = getSpiritObjectFromMemorySafe(reply);
      for(json_spirit::Object::const_iterator it = obj.begin(); it != obj.end(); ++it)
      {
        const std::string& name = it->name_;
        const json_spirit::Value& value = it->value_;
        if(name == "update_seq")
        {
          seqNo = value.get_int();
          if(log)
            QTRACE((L"%S %d", name.c_str(), seqNo));
        }
      }
    }
  }
  catch(cHTTPError& err)
  {
    if(log)
      QTRACE((L"getDBSeqNo() HTTP ERR: %d, %s. PAYLOAD:%S", err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
  }
  catch(cRecoverable& err)
  {
    QSOS((L"getDBSeqNo() UNKNOWN ERROR: %s", err.getMsg().c_str()));
  }

  return seqNo;
}

unsigned int getDBSeqNo(bool log)
{
  return getDBSeqNo(getDBPath(), log);
}



static const char view_by_artist[] =
"function(doc) \
{\
  if(doc.Type == 'Recording') \
  {\
    emit([doc.Artist], doc);\
  }\
}";

static const char blank_reduce[] = "function(keys, values) { return true; }";


//static const char list_clips[] =
//"function(head, req) \
//{\
//  var row;start({\"headers\": {\"Content-Type\": \"text/html\"}}); \
//  send(\"<html><body><ul>\"); \
//  while(row = getRow()) \
//  {\
//    if(row.value.Type == 'Clip') \
//    { \
//      if(row.value.TimeCompleted != null && row.value.TimeCreated != null)  \
//      {\
//        var hour = row.value.TimeCompleted.Hour - row.value.TimeCreated.Hour;\
//        if(row.value.TimeCompleted.Hour < row.value.TimeCreated.Hour)\
//        {\
//          hour += 24;\
//        }\
//        var minute = row.value.TimeCompleted.Minute - row.value.TimeCreated.Minute;\
//        if(row.value.TimeCompleted.Minute < row.value.TimeCreated.Minute)\
//        {\
//          minute += 60;\
//          hour -= 1;\
//        }\
//        var second = row.value.TimeCompleted.Second - row.value.TimeCreated.Second;\
//        if(row.value.TimeCompleted.Second < row.value.TimeCreated.Second)\
//        {\
//          second += 60;\
//          minute -= 1;\
//        }\
//        send(\"<li>Clip: \" + row.value.NewISAClipID + ', Took:' + hour + ':' + minute + ':' + second + \"</li>\"); \
//      }\
//      else\
//        send(\"<li>Clip: \" + row.value.NewISAClipID + \"</li>\"); \
//    } \
//    else if(row.value.Type == 'RushPart') \
//    {\
//      var length = row.value.SrcRushPart.OutFrame-row.value.SrcRushPart.InFrame; \
//      if(row.value.Error != null)\
//        send(\"<li>RP: Clip Frame:\" + row.value.ClipInfo.ClipFrame + '  Len:' + length + ', ERROR:' + row.value.Error + \"</li>\");\
//      else \
//      {\
//        if(row.value.TimeCompleted != null && row.value.TimeAcquired != null)  \
//        {\
//          var hour = row.value.TimeCompleted.Hour - row.value.TimeAcquired.Hour;\
//          if(row.value.TimeCompleted.Hour < row.value.TimeAcquired.Hour)\
//          {\
//            hour += 24;\
//          }\
//          var minute = row.value.TimeCompleted.Minute - row.value.TimeAcquired.Minute;\
//          if(row.value.TimeCompleted.Minute < row.value.TimeAcquired.Minute)\
//          {\
//            minute += 60;\
//            hour -= 1;\
//          }\
//          var second = row.value.TimeCompleted.Second - row.value.TimeAcquired.Second;\
//          if(row.value.TimeCompleted.Second < row.value.TimeAcquired.Second)\
//          {\
//            second += 60;\
//            minute -= 1;\
//          }\
//          send(\"<li>RP: Clip Frame:\" + row.value.ClipInfo.ClipFrame + '  Len:' + length + ', Took:' + hour + ':' + minute + ':' + second + \"</li>\"); \
//        }\
//        else\
//          send(\"<li>RP: Clip Frame:\" + row.value.ClipInfo.ClipFrame + '  Len:' + length + ', ' + row.value.Progress + '%' + \"</li>\"); \
//      }\
//    }\
//  }\
//  send(\"</ul></body></html>\"); \
//}";
//
//
//
//static const char filter_everything[] =
//"function(doc, req)\
//{\
//  if (doc.Type == 'Clip' && doc._id == req.query.id)\
//  {\
//    return true;\
//  }\
//  else if(doc.Type == 'RushPart' && doc.ClipInfo.ClipID == req.query.id)\
//  {\
//    return true;\
//  }\
//  else if(doc.Type == 'HiddenClip' && doc.ClipID == req.query.id)\
//  {\
//    return true;\
//  }\
//  return false;\
//}";


void addEntry1(bool log)
{
  try
  {
    json_spirit::Object obj;
    obj.push_back(json_spirit::Pair("Type", "Recording"));
    obj.push_back(json_spirit::Pair("Artist", "Motorhead"));
    obj.push_back(json_spirit::Pair("Path", "D:\\Music\\M\\Motorhead - The Best Of Greatest Hits [Bubanee]\\"));
    obj.push_back(json_spirit::Pair("Name", "01 - Ace of Spades.mp3"));

    auto ret = writeVersionedObject(obj, log );
  }
  catch(cHTTPError& err)
  {
    QSOS((L"registerAlienSiteClip() HTTP ERR: %d, %s. PAYLOAD:%S", err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
    throw;
  }

}

void addEntry2(bool log)
{
  try
  {
    json_spirit::Object obj;
    obj.push_back(json_spirit::Pair("Type", "Recording"));
    obj.push_back(json_spirit::Pair("Artist", "Motorhead"));
    obj.push_back(json_spirit::Pair("Path", "D:\\Music\\M\\Motorhead - The Best Of Greatest Hits [Bubanee]\\"));
    obj.push_back(json_spirit::Pair("Name", "06 - Overkill.mp3"));

    auto ret = writeVersionedObject(obj, log);
  }
  catch(cHTTPError& err)
  {
    QSOS((L"registerAlienSiteClip() HTTP ERR: %d, %s. PAYLOAD:%S", err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
    throw;
  }

}

void addEntry3(bool log)
{
  try
  {
    json_spirit::Object obj;
    obj.push_back(json_spirit::Pair("Type", "Recording"));
    obj.push_back(json_spirit::Pair("Artist", "Bowie"));
    obj.push_back(json_spirit::Pair("Path", "D:\\Music\\B\\Bowie 1966 - 1976\\David Bowie - Hunky Dory\\"));
    obj.push_back(json_spirit::Pair("Name", "04 - Life On Mars.mp3"));

    auto ret = writeVersionedObject(obj, log);
  }
  catch(cHTTPError& err)
  {
    QSOS((L"registerAlienSiteClip() HTTP ERR: %d, %s. PAYLOAD:%S", err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
    throw;
  }

}

void addEntry4(bool log)
{
  try
  {
    json_spirit::Object obj;
    obj.push_back(json_spirit::Pair("Type", "Recording"));
    obj.push_back(json_spirit::Pair("Artist", "Led Zepplin"));
    obj.push_back(json_spirit::Pair("Path", "D:\\Music\\L\\Led Zeppelin - Remasters\\"));
    obj.push_back(json_spirit::Pair("Name", "Led Zeppelin - Remasters - 08 - Immigrant Song!!s.mp3"));

    auto ret = writeVersionedObject(obj, log);
  }
  catch(cHTTPError& err)
  {
    QSOS((L"registerAlienSiteClip() HTTP ERR: %d, %s. PAYLOAD:%S", err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
    throw;
  }

}



json_spirit::Array getRows(vfs::cMemoryView::Ptr reply)
{
  json_spirit::Array ret;
  const json_spirit::Object& obj = getSpiritObjectFromMemorySafe(reply);
  for(json_spirit::Object::const_iterator it = obj.begin(); it != obj.end(); ++it)
  {
    const std::string& name = it->name_;
    const json_spirit::Value& value = it->value_;
    if(name == "rows")
    {
      ret = value.get_array();
    }
  }
  return ret;
}

std::list<json_spirit::Object> listArrayObjects(const json_spirit::Array& arr, const std::string& searchTerm)
{
  std::list<json_spirit::Object> objList;
  for(json_spirit::Array::size_type k = 0; k != arr.size(); ++k)
  {
    const json_spirit::Object& obj = arr[k].get_obj();
    for(json_spirit::Object::size_type l = 0; l != obj.size(); ++l)
    {
      const json_spirit::Pair& pair = obj[l];

      const std::string& name = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if(name == searchTerm)
      {
        objList.push_back(value.get_obj());
      }
    }
  }
  return objList;
}

std::list<std::string> listArrayStrings(const json_spirit::Array& arr, const std::string& searchTerm)
{
  std::list<std::string> strList;
  for(json_spirit::Array::size_type k = 0; k != arr.size(); ++k)
  {
    const json_spirit::Object& obj = arr[k].get_obj();
    for(json_spirit::Object::size_type l = 0; l != obj.size(); ++l)
    {
      const json_spirit::Pair& pair = obj[l];

      const std::string& name = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if(name == searchTerm)
      {
        strList.push_back(value.get_array()[0].get_str());
      }
    }
  }
  return strList;
}


std::list<std::pair<std::string,std::string>> recordsByArtist(std::string artist, bool log)
{
  std::list<std::pair<std::string, std::string>> ret;
  try
  {
    std::string key = "[\"" + artist + "\"]";

    std::string url = getDBPath() + "_design/indexes/_view/map_by_artist?key=" + key;
    vfs::cMemoryView::Ptr reply = HTTPGet_basic(url, log, 0);
    if(reply.isValid())
    {
      if(log)
        dump(reply);


      json_spirit::Array arr = getRows(reply);
      std::list<json_spirit::Object> objList = listArrayObjects(arr, "value");
      for(std::list<json_spirit::Object>::const_iterator it = objList.begin(); it != objList.end(); ++it)
      {
        std::string _path;
        std::string _name;
        json_spirit::Object obj = *it;
        for(json_spirit::Object::size_type j = 0; j != obj.size(); ++j)
        {
          json_spirit::Pair& pair = obj[j];
          const std::string& name = pair.name_;
          const json_spirit::Value& value = pair.value_;

          //QTRACE((L"name %S, value %S", name.c_str(), value.get_str().c_str()));
          if(name == "Name")
          {
            //QTRACE((L"here 1"));
            _name = value.get_str();
            //QTRACE((L"_name %S, value %S", _name.c_str(), value.get_str().c_str()));
          }
          if(name == "Path")
          {
            //QTRACE((L"here 2"));
            _path = value.get_str();
            //QTRACE((L"_path %S, value %S", _name.c_str(), value.get_str().c_str()));
          }
          //QTRACE((L"name %S, _path %S", _name.c_str(), _path.c_str()));
        }
        //QTRACE((L"name %S, _path %S", _name.c_str(), _path.c_str()));
        if(!_name.empty() && !_path.empty())
        {
          ret.push_back(std::pair<std::string, std::string>(_name, _path));
        }
      }
    }
  }
  catch(cHTTPError& err)
  {
    QSOS((L"%S HTTP ERR: %d, %s. PAYLOAD:%S", __FUNCTION__, err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
  }

  return ret;
}


std::list<std::string> allArtists(bool log)
{
  std::list<std::string> ret;
  try
  {
    //http://127.0.0.1:5984/music/_design/indexes/_view/view_by_artist
    std::string url = getDBPath() + "_design/indexes/_view/reduce_by_artist?group=true";
    vfs::cMemoryView::Ptr reply = HTTPGet_basic(url, log, 0);
    if(reply.isValid())
    {
      if(log)
        dump(reply);

      json_spirit::Array arr = getRows(reply);
      ret = listArrayStrings(arr, "key");
    }
  }
  catch(cHTTPError& err)
  {
    QSOS((L"%S HTTP ERR: %d, %s. PAYLOAD:%S", __FUNCTION__, err.ResponseCode, err.getMsg().c_str(), err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
  }

  return ret;
}



void createAlienSiteTransferDB(bool log)
{
  bool createNewDB = getDBSeqNo(log) ? false : true;//zero seqNo impies no database
  if(createNewDB)
  {
    try
    {
      vfs::cMemoryView::Ptr reply;
      reply = HTTPPutJSONWithReply(getDBPath(), vfs::cMemoryView::Ptr(), log);
      if(log)
        dump(reply);
    }
    catch(cHTTPError& err)
    {
      if(err.ResponseCode == 412)
      {
        QTRACE((L"createAlienSiteTransferDB() HTTP ERR: %d, %S", err.ResponseCode, err.Payload.isValid() ? err.Payload->getConstBytes() : ""));
      }
      else
        throw;
    }


    json_spirit::Object designDoc;
    json_spirit::Object views;
    {
      json_spirit::Object map;
      map.push_back(json_spirit::Pair("map", view_by_artist));
      views.push_back(json_spirit::Pair("map_by_artist", json_spirit::Value(map)));
    }
    {
      json_spirit::Object map;
      map.push_back(json_spirit::Pair("map", view_by_artist));
      map.push_back(json_spirit::Pair("reduce", blank_reduce));
      views.push_back(json_spirit::Pair("reduce_by_artist", json_spirit::Value(map)));
    }
    designDoc.push_back(json_spirit::Pair("views", json_spirit::Value(views)));

    //json_spirit::Object lists;
    //{
    //  lists.push_back(json_spirit::Pair("clips", list_clips));
    //}
    //designDoc.push_back(json_spirit::Pair("lists", json_spirit::Value(lists)));

    //json_spirit::Object filters;
    //{
    //  filters.push_back(json_spirit::Pair("everything", filter_everything));
    //}
    //designDoc.push_back(json_spirit::Pair("filters", json_spirit::Value(filters)));


    json_spirit::Value value(designDoc);
    std::string result = json_spirit::write_string(value, json_spirit::pretty_print);

    {
      vfs::cMemoryView::Ptr mem = new vfs::cMemoryView(new cMemory(result.size(), cMemory::eHeap));
      memcpy(mem->getBytes(), result.c_str(), mem->getSize());
      if(log)
        dump(mem);

      std::string url = getDBPath() + "_design/indexes";
      //if(!revisionID.empty())
      //  url += "?rev=" + revisionID;

      vfs::cMemoryView::Ptr reply = HTTPPutJSONWithReply(url, mem, log);
      if(log)
        dump(reply);
    }

    addEntry1(log);
    addEntry2(log);
    addEntry3(log);
    addEntry4(log);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void cSofaLoader::addVirtualFile(const String& name, const String& path)
{
  const iWriteInfo& writeInfo = iWriteInfo::singleton();

  cPtr<cBasicFile> p = new cBasicFile(path);

  WIN32_FILE_ATTRIBUTE_DATA attrs = p->getFileAttributes();

  fileInfo fi;
  LARGE_INTEGER sz;
  sz.HighPart = attrs.nFileSizeHigh;
  sz.LowPart = attrs.nFileSizeLow;
  writeInfo.writeFileInfo(&fi
    , sz.QuadPart
    , attrs.ftCreationTime
    , attrs.ftLastAccessTime
    , attrs.ftLastWriteTime
    , attrs.dwFileAttributes
    , name);

  ReadableFileMap.insert(tReadableFileMap::value_type(name, tReadInfo(p, fi)));
}

cSofaLoader::cSofaLoader(const vfs::String& name, cSofaLoader* parent) : Name(name), Parent(parent)
{
  QTRACE((L"cSofaLoader::cSofaLoader %s", Name.c_str()));

  //addVirtualFile(L"David Bowie - Station To Station.mp3", L"D:\\Music\\B\\Bowie 1966 - 1976\\David Bowie - Station To Station [1976]\\David Bowie - Station To Station.mp3");
  //addVirtualFile(L"Motorhead - Ace of Spades.mp3", L"D:\\Music\\M\\Motorhead - The Best Of Greatest Hits [Bubanee]\\01 - Ace of Spades.mp3");

  if(Name.empty())//root
  {
    createAlienSiteTransferDB(false);

    auto artists = allArtists(false);
    for(auto artist : artists)
    {

      FolderMap.insert(tFolderMap::value_type(widen(artist), new cSofaLoader(widen(artist), this)));
      //QTRACE((L"%S", artist.c_str()));
      //auto records = recordsByArtist(artist, false);
      //for(auto record : records)
      //{
      //  QTRACE((L"%S", record.first.c_str()));
      //}
    }

  }
  else
  {
    auto records = recordsByArtist(narrow(Name), false);
    for(auto record : records)
    {
      QTRACE((L"%S", record.first.c_str()));
      addVirtualFile(widen(record.first), widen(record.second + record.first));
    }

  }

}

void cSofaLoader::registerListener(const vfs::cPtr<iChildLoaderVisitor> pChildListener)
{
  QTRACE((L"cSofaLoader::registerListener"));
  if (pChildListener.isValid())
  {
    cChildLoader::registerListener(pChildListener);

    for(tReadableFileMap::const_iterator it = ReadableFileMap.begin(); it != ReadableFileMap.end(); ++it)
    {
      pChildListener->updateFile(
        it->second.second,
        it->second.first,
        cPtr<iFileEvent>());
    }

    for(tFolderMap::const_iterator it = FolderMap.begin(); it != FolderMap.end(); ++it)
    {
      //QTRACE((L"cSofaLoader::registerListener %s %s %s", Name.c_str(), it->first.c_str(), it->second->name().c_str()));
      pChildListener->updateFolder(
        it->first,
        it->second, 
        cPtr<iRename>(),
        cPtr<iCreate>(),
        cConstPtr<cMemoryView>(),
        cPtr<iFileEvent>(),
        true);
    }
  }
}

