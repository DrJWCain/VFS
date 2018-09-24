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

//couchAccess.cpp

#include "StdAfx.h"

#include "couchAccess.h"
#include "dumpers.h"
#include "cCurlWrapper.h"

#include "..\..\Shared\json_spirit\json_spirit_value.h"
#include "..\..\Shared\json_spirit\json_spirit_reader_template.h"
#include "..\..\Shared\json_spirit\json_spirit_writer_template.h"

#include <boost/lexical_cast.hpp>

using namespace vfs;



std::string getDBPath()
{
  return "http://127.0.0.1:5984/music/";
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

static const char view_by_duration[] =
"function(doc) { \
  emit(doc.Duration, doc); \
}";

static const char view_by_tracknum[] =
"function(doc) { \
  emit(doc.TrackNum, doc); \
}";


static const char blank_reduce[] = "function(keys, values) { return true; }";




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


std::list<std::pair<std::string, std::string>> recordsByArtist(std::string artist, bool log)
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

          if(name == "Name")
          {
            _name = value.get_str();
          }
          if(name == "Path")
          {
            _path = value.get_str();
          }
        }
        if(!_name.empty() && !_path.empty())
        {
          ret.push_back(std::pair<std::string, std::string>(_name, _path + _name));
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


std::list<std::pair<std::string, std::string>> recordsByDuration(std::string query, bool log)
{
  std::list<std::pair<std::string, std::string>> ret;
  try
  {
    std::string url = getDBPath() + "_design/indexes/_view/map_by_duration";
    if(!query.empty())
      url += "?" + query;
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
        int _dur = 0;
        json_spirit::Object obj = *it;
        for(json_spirit::Object::size_type j = 0; j != obj.size(); ++j)
        {
          json_spirit::Pair& pair = obj[j];
          const std::string& name = pair.name_;
          const json_spirit::Value& value = pair.value_;

          if(name == "Name")
          {
            _name = value.get_str();
          }
          if(name == "Path")
          {
            _path = value.get_str();
          }
          if(name == "Duration")
          {
            _dur = value.get_int();
          }
        }
        if(!_name.empty() && !_path.empty())
        {
          ret.push_back(std::pair<std::string, std::string>(boost::lexical_cast<std::string>(_dur) + " - " + _name, _path + _name));
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


std::list<std::pair<std::string, std::string>> recordsByTracknum(std::string query, bool log)
{
  std::list<std::pair<std::string, std::string>> ret;
  try
  {
    std::string url = getDBPath() + "_design/indexes/_view/map_by_tracknum";
    if(!query.empty())
      url += "?" + query;
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
        int _tracknum = 0;
        json_spirit::Object obj = *it;
        for(json_spirit::Object::size_type j = 0; j != obj.size(); ++j)
        {
          json_spirit::Pair& pair = obj[j];
          const std::string& name = pair.name_;
          const json_spirit::Value& value = pair.value_;

          if(name == "Name")
          {
            _name = value.get_str();
          }
          if(name == "Path")
          {
            _path = value.get_str();
          }
          if(name == "TrackNum")
          {
            _tracknum = value.get_int();
          }
        }
        if(!_name.empty() && !_path.empty())
        {
          ret.push_back(std::pair<std::string, std::string>(boost::lexical_cast<std::string>(_tracknum) + " - " + _name, _path + _name));
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
    {
      json_spirit::Object map;
      map.push_back(json_spirit::Pair("map", view_by_duration));
      views.push_back(json_spirit::Pair("map_by_duration", json_spirit::Value(map)));
    }
    {
      json_spirit::Object map;
      map.push_back(json_spirit::Pair("map", view_by_tracknum));
      views.push_back(json_spirit::Pair("map_by_tracknum", json_spirit::Value(map)));
    }


    designDoc.push_back(json_spirit::Pair("views", json_spirit::Value(views)));

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
  }
}

