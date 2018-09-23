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

#include "stdafx.h"

#include "cCurlWrapper.h"

#include "..\..\Shared\cURL\include\curl.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>

#ifdef NDEBUG
#pragma comment(lib, "libcurl.lib")
#else 
#pragma comment(lib, "libcurl_debug.lib")
#endif

using namespace vfs;


const int kNumOfRetries = 1;

bool mIsLogging = true;
const vfs::iLogManager::Category kLogCurl = 1u << 0;
const vfs::iLogManager::Category kLogCurlDetail = 1u << 1;


struct write_data_param_basic
{
  write_data_param_basic() : Offset(0) {}
  cMemoryView::Ptr Data;
  size_t Offset;
  std::string FirstHeader;
  std::list<std::string> Headers;
};

//we look for the size tag in the http headers and then allocate
size_t write_header_basic(void *buffer, size_t size, size_t nmemb, void *userp)
{
  size_t length = size * nmemb;
  write_data_param_basic* pointer = (write_data_param_basic*)userp;
  //QTRACE((L"%S", buffer)); //log HTTP headers here

  std::string text((char*)buffer);
  if(pointer->FirstHeader.empty())
    pointer->FirstHeader = text;

  std::string search("Content-Length:");
  std::string::size_type off = text.find(search);
  if(0 == off)
  {
    std::string sizeStr(text, search.size());
    try
    {
      boost::algorithm::trim(sizeStr);
      size_t contentLength = boost::lexical_cast<size_t>(sizeStr);
      if(contentLength % iMemoryManager::singleton().getFastIOAlignment() != 0)
        contentLength = (size_t)ceil((double)contentLength / (double)iMemoryManager::singleton().getFastIOAlignment()) * iMemoryManager::singleton().getFastIOAlignment();
      //QTRACE((L"Length in bytes is %Iu ", contentLength));
      pointer->Data = new cMemoryView(new cMemory(contentLength, cMemory::eHeap, (size_t)iMemoryManager::singleton().getFastIOAlignment()));
    }
    catch(boost::bad_lexical_cast&)
    {
      QSOS((L"Bad Content-Length value \"%S\"", sizeStr.c_str()));
    }
  }
  return length;
}

//we look for the size tag in the http headers and then allocate
size_t write_header_alligned(void *buffer, size_t size, size_t nmemb, void *userp)
{
  size_t length = size * nmemb;
  write_data_param_basic* pointer = (write_data_param_basic*)userp;
  //QTRACE((L"%S", buffer)); //log HTTP headers here

  std::string text((char*)buffer);
  while(text.size() && text[text.size() - 1] < 0x20)//remove trailing whitespace
  {
    text.erase(text.size() - 1);
  }
  if(pointer->FirstHeader.empty())
    pointer->FirstHeader = text;

  pointer->Headers.push_back(text);

  std::string search("Content-Length:");
  std::string::size_type off = text.find(search);
  if(0 == off)
  {
    std::string sizeStr(text, search.size());
    try
    {
      boost::algorithm::trim(sizeStr);
      size_t sizeAnnounced = boost::lexical_cast<size_t>(sizeStr);
      size_t contentLength = sizeAnnounced;
      if(contentLength % iMemoryManager::singleton().getFastIOAlignment() != 0)
        contentLength = (size_t)ceil((double)contentLength / (double)iMemoryManager::singleton().getFastIOAlignment()) * iMemoryManager::singleton().getFastIOAlignment();
      //QTRACE((L"Length in bytes is %Iu (was %Iu)", contentLength, sizeAnnounced));
      pointer->Data = new cMemoryView(new cMemory(contentLength, cMemory::eHeap, (size_t)iMemoryManager::singleton().getFastIOAlignment()));
    }
    catch(boost::bad_lexical_cast&)
    {
      QSOS((L"Bad Content-Length value \"%S\"", sizeStr.c_str()));
    }
  }
  return length;
}

size_t write_data_basic(void *buffer, size_t size, size_t nmemb, void *userp)
{
  size_t length = size * nmemb;
  write_data_param_basic* pointer = (write_data_param_basic*)userp;
  if(!pointer->Offset && pointer->Data.isNull())
  {
    //QSOS((L"Content length not set by HTTP headers - using 8MB"));
    pointer->Data = new cMemoryView(new cMemory(8 * 1024 * 1024, cMemory::eHeap, (size_t)iMemoryManager::singleton().getFastIOAlignment())); //JCJCJC hard coded payload size
  }

  // Case where the HTTP response is trying to write more data than we have allocated for
  if((pointer->Offset + length) > pointer->Data->getSize())
  {
    const size_t eightMB(8 * 1024 * 1024);
    // If the data size is a modulo of 8MB we assume that it's a scenario where the Content length was 
    // not set by HTTP headers, see above. In this case we extend the allocation by 8MB unless it's already 80MB
    if((pointer->Data->getSize() % eightMB == 0) && (pointer->Data->getSize() < (10 * eightMB)))
    {
      QSOS((L"Content length not set by HTTP headers : Extending by 8 MB"));
      cMemoryView::Ptr data = new cMemoryView(new cMemory(pointer->Data->getSize() + eightMB, cMemory::eHeap, (size_t)iMemoryManager::singleton().getFastIOAlignment())); //JCJCJC hard coded payload size
      memcpy(data->getBytes(), pointer->Data->getBytes(), pointer->Data->getSize());
      pointer->Data = data;
    }
    else
    {
      // If the Content length has been set by HTTP headers then the HTTP response doesn't match so we error
      QSOS((L"ERROR %d + %d > %d", length, pointer->Offset, pointer->Data->getSize()));
      return 0;
    }
  }

  memcpy(pointer->Data->getBytes() + pointer->Offset, buffer, length);
  pointer->Offset += length;
  return length;
}



int toAsciiCharacter(char ascii)
{
  //char ret;
  //ascii >> ret;
  return (int)ascii;
}
std::string escapeUrl(const std::string& url)
{
  char* buffer = new char[url.length() + 1];
  strcpy(buffer, url.c_str());
  std::string ret("");
  for(int i = 0; url[i]; ++i)
  {
    int asciiCode = toAsciiCharacter(url[i]);
    if(((asciiCode > 31) && (asciiCode < 48)) || ((asciiCode > 57) && (asciiCode < 65)) || ((asciiCode > 90) && (asciiCode < 95)) || (asciiCode > 122))
    {
      //QTRACE((L"For char %c got code %d", url[i], asciiCode));
      std::stringstream convert;
      convert << std::hex << "%" << asciiCode;
      ret += convert.str();
    }
    else
    {
      ret += url[i];
    }
  }
  delete[] buffer;
  return ret;
}
//}}}
std::string escapeUrl(CURL* curl, const std::string &url)
{
  // There is no easy way to escape the url as curl_easy_escape (hah) will also escape
  // ":" and "/" characters so we will have to iterate through manually and apply 
  // curl_easy_escape to strings between "/" characters (after any initial "http://").
  //
  // We will also have to mindful to allow for the following things which can be a
  // part of a URL (and which curl_easy_escape will cheerfully escape):
  //
  //  - A port number for the host (http://blah.com:8080).
  //  - An anchor (http://blah.com/page3.html#Section2).
  //  - The query delimiter (http://blah.com/t.php?id=5).
  //  - The query continuation ( http://blah.com/t.php?id=5&name=hello).
  //  - Equals character (used in both of the previous 2 examples).
  //
  // Useful for testing escapeUrl.
  //std::string url2 = escapeUrl( curl, "http://blah.com:8080/t.php");
  //std::string url3 = escapeUrl( curl, "http://blah.com/t.php#blah");
  //std::string url4 = escapeUrl( curl, "http://blah.com/t.php?id=1");
  //std::string url5 = escapeUrl( curl, "http://blah.com/t.php?id=1&name=hello");
  //

  // Check for the protocol.
  size_t pos = url.find("://");
  if(pos == std::string::npos)
    pos = 0;
  else
    pos += 3;

  std::string newUrl(url.substr(0, pos));
  size_t len = url.size();
  size_t pos2;
  //std::string str;
  char *data;

  while(pos < len)
  {
    // Identify the next string between delimiters.
    pos2 = url.find_first_of("/:#?&=", pos);
    if(pos2 == std::string::npos)
      pos2 = len;
    std::string str = url.substr(pos, pos2 - pos);

    // Use curl to escape it.
    data = curl_easy_escape(curl, str.c_str(), (int)str.length());

    // Shove it on the end of the new url and only copy
    // the delimiter if we're NOT at the end of the string.
    newUrl += std::string(data);
    if(pos2 != len)
      newUrl += url.substr(pos2, 1);

    curl_free(data);
    pos = pos2 + 1;
  }
  return newUrl;
}


int curl_debug(CURL* curl, curl_infotype type, char *buffer, size_t size, void* userData)
{
  if(CURLINFO_TEXT == type || CURLINFO_HEADER_IN == type || CURLINFO_HEADER_OUT == type)
  {
    std::string information(buffer, buffer + size);
    QMSG((kLogCurl, L"curl:%S", information.c_str()));
  }
  else
  {
    //These are payloads (CURLINFO_DATA_IN & CURLINFO_DATA_OUT etc) and maybe both big & unprintable!!!
    QMSG((kLogCurlDetail, L"curl:%d:%d", type, size));
  }
  return 0;
}



CURL* allocateCurlHandle(bool couchAccess)
{
  return curl_easy_init();
}


CURL* allocateCurlWriteHandle(bool couchAccess)
{
  return curl_easy_init();
}

void freeCurlHandle(CURL* curl, bool couchAccess)
{
  curl_easy_cleanup(curl);
}

void errorCurlHandle(CURL* curl, bool couchAccess)
{
  curl_easy_cleanup(curl);
}

void freeCurlWriteHandle(CURL* curl, bool couchAccess)
{
  curl_easy_cleanup(curl);
}

void errorCurlWriteHandle(CURL* curl, bool couchAccess)
{
  curl_easy_cleanup(curl);
}



bool couchAccess = false;
cMemoryView::Ptr HTTPGet_basic(const std::string& url, bool log, std::list<std::string>* pHeaders)
{
  std::auto_ptr<write_data_param_basic> pointer;

  int numOfRetries = kNumOfRetries;
  bool errorOccured = false;
  do
  {
    errorOccured = false;

    CURL* curl = allocateCurlHandle(couchAccess);
    if(log)
      QTRACE((L"HTTP Get:%S", url.c_str()));
    struct curl_slist *slist = NULL;
    if(pHeaders)
    {
      for(std::list<std::string>::const_iterator it = pHeaders->begin(); it != pHeaders->end(); ++it)
      {
        slist = curl_slist_append(slist, it->c_str());
      }

      pHeaders->clear();
    }
    pointer = std::auto_ptr<write_data_param_basic>(new write_data_param_basic);

    curl_easy_setopt(curl, CURLOPT_URL, escapeUrl(curl, url).c_str());
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header_alligned);
    curl_easy_setopt(curl, CURLOPT_WRITEHEADER, pointer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_basic);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pointer);
    //curl_easy_setopt(curl, CURLOPT_USERAGENT, kStrProductNameAndVersion.c_str());
    if(slist)
    {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    }

    //Very verbose, but useful for CURL error debugging
    if(mIsLogging)
    {
      curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, curl_debug);
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    }

    CURLcode err = curl_easy_perform(curl);
    if(CURLE_OK != err && CURLE_PARTIAL_FILE != err)//CURLE_PARTIAL_FILE(18) means curtailed payload compared to HTTP header - QStack does this sometimes. However the payload appears valid (from QStack)!
    {
      QSOS((L"curl_easy_perform return: %d", err));
      if(!errorOccured)
      {
        curl_slist_free_all(slist);
        errorCurlHandle(curl, couchAccess);
        curl = 0;
        errorOccured = true;
      }
    }

    long responseCode;
    err = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    if(CURLE_OK != err)
    {
      QSOS((L"curl_easy_getinfo return: %d", err));
      if(!errorOccured)
      {
        curl_slist_free_all(slist);
        errorCurlHandle(curl, couchAccess);
        curl = 0;
        errorOccured = true;
      }
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, 0);
    curl_slist_free_all(slist);
    freeCurlHandle(curl, couchAccess);

    bool benignError = false;
    if(responseCode >= 400)//eg 500: internal Server Error, 404 is not found
    {
      if(responseCode == 404)//fine - its not there - don't retry.
      {
        numOfRetries = 0;
        benignError = true;
      }
      else
      {
        QSOS((L"Found responseCode %d. Response:%S", responseCode, pointer->FirstHeader.c_str()));
      }
      errorOccured = true;
    }

    if(log)
      QTRACE((L"HTTP Get: %S - Response %d", url.c_str(), responseCode));
    if(log && responseCode != 200)
      QTRACE((L"processed %d bytes, err = %d, responseCode %d. Response:%S", pointer->Offset, err, responseCode, pointer->FirstHeader.c_str()));

    if(!errorOccured)
    {
      std::string search("200");
      std::string::size_type off = pointer->FirstHeader.find(search);
      if(std::string::npos == off)
      {
        //if(log)
        QSOS((L"HTTP Get: %S - ERROR %S err %d", url.c_str(), pointer->FirstHeader.c_str(), err));
        errorOccured = true;
      }
    }
    if(errorOccured && numOfRetries <= 0)
    {
      if(!benignError)
      {
        QSOS((L"HTTP_Get_Basic_V2 retried %d times but failed with url %s", kNumOfRetries, widen(url).c_str()));
      }
      throw cHTTPError(__FILE__, __LINE__, responseCode, cMemoryView::Ptr(), L"URL %S generated: curl_easy_perform return: %d.", url.c_str(), err);
    }
  } while(errorOccured && numOfRetries--);

  if(pHeaders)
  {
    *pHeaders = pointer->Headers;
  }

  if(pointer->Data.isValid() && (pointer->Offset < pointer->Data->getSize()))
  {
    //QTRACE((L"? Transfer encoding chunked ? Either way trimming payload to %d bytes.", pointer->Offset));
    size_t contentLength(pointer->Offset);
    if(contentLength % iMemoryManager::singleton().getFastIOAlignment() != 0)
      contentLength = (size_t)ceil((double)contentLength / (double)iMemoryManager::singleton().getFastIOAlignment()) * iMemoryManager::singleton().getFastIOAlignment();
    pointer->Data = pointer->Data->range(0, contentLength);
  }

  return pointer->Data;
}

struct read_data_param_basic
{
  read_data_param_basic() : Offset(0) {}
  cMemoryView::Ptr Data;
  size_t Offset;
};

size_t read_data_basic(void *buffer, size_t size, size_t nmemb, void *userp)
{
  size_t length = size * nmemb;
  read_data_param_basic* pointer = (read_data_param_basic*)userp;

  if(pointer->Data.isNull())
    return 0;

  //QTRACE((L"read_data: Offset:%d, Length:%d",pointer->Offset,length));
  if(pointer->Offset >= pointer->Data->getSize())
  {
    //QTRACE((L"read_data: returned zero: %d >= %d", pointer->Offset, pointer->Data->getSize()));
    return 0;
  }
  if(pointer->Offset + length > pointer->Data->getSize())
  {
    //QTRACE((L"read_data: clamping length: %d > %d", pointer->Offset + length, pointer->Data->getSize()));
    length = pointer->Data->getSize() - pointer->Offset;
  }
  memcpy(buffer, pointer->Data->getBytes() + pointer->Offset, length);
  pointer->Offset += length;

  return length;
}



vfs::cMemoryView::Ptr HTTPDeliverWithReply(const std::string& url, cMemoryView::Ptr payload, const std::list<std::string>& headers, bool usePut, bool log)
{
  bool payloadValid = payload.isValid();

  if(log)
    QTRACE((L"HTTP Deliver With reply:%S %d bytes", url.c_str(), payloadValid ? payload->getSize() : 0));

  std::auto_ptr<write_data_param_basic> pointerReply;

  int numOfRetries = kNumOfRetries;
  bool errorOccured = false;
  bool dataError = false;
  do
  {
    errorOccured = false;
    dataError = false;

    struct curl_slist *slist = NULL;
    for(std::list<std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
    {
      slist = curl_slist_append(slist, it->c_str());
    }
    std::auto_ptr<read_data_param_basic> pointer(new read_data_param_basic);
    pointer->Data = payload;

    //CURL* curl = curl_easy_init(); 
    CURL* curl = allocateCurlWriteHandle(couchAccess);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1);

    //curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    if(usePut)
    {
      curl_easy_setopt(curl, CURLOPT_POST, 0);
      curl_easy_setopt(curl, CURLOPT_PUT, 1);
    }
    else
    {
      curl_easy_setopt(curl, CURLOPT_PUT, 0);
      curl_easy_setopt(curl, CURLOPT_POST, 1);
    }
    curl_easy_setopt(curl, CURLOPT_URL, escapeUrl(curl, url).c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data_basic);
    curl_easy_setopt(curl, CURLOPT_READDATA, pointer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payloadValid ? payload->getSize() : 0);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

    pointerReply = std::auto_ptr<write_data_param_basic>(new write_data_param_basic);

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header_alligned);
    curl_easy_setopt(curl, CURLOPT_WRITEHEADER, pointerReply);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_basic);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pointerReply);
//    curl_easy_setopt(curl, CURLOPT_USERAGENT, kStrProductNameAndVersion.c_str());

    //Very verbose, but useful for CURL error debugging
    if(mIsLogging)
    {
      curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, curl_debug);
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    }

    int res = curl_easy_perform(curl);
    long responseCode(0);
    if(CURLE_OK != res)
    {
      QSOS((L"HTTPDeliverWithReply: curl_easy_perform return: %d", res));
      curl_slist_free_all(slist);
      errorCurlWriteHandle(curl, couchAccess);
      errorOccured = true;
    }
    else
    {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
      if(log)
        QTRACE((L"Finished HTTP Deliver:%S %d, responseCode:%d", url.c_str(), res, responseCode));

      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, 0);
      curl_slist_free_all(slist);
      //curl_easy_cleanup(curl);
      freeCurlWriteHandle(curl, couchAccess);

      if(payloadValid && pointer->Offset != pointer->Data->getSize())
      {
        //if(log)
        QSOS((L"Failed to read data properly during HTTP Deliver to:%S: %d != %d", url.c_str(), pointer->Offset, pointer->Data->getSize()));
        errorOccured = true;
        dataError = true;

      }
      if((responseCode >= 400) || (responseCode == 0))
      {
        if(409 == responseCode)
        {
          //Don't log or retry on a conflict - hand back to the client ...
          throw cHTTPError(__FILE__, __LINE__, responseCode, pointerReply->Data, L"responseCode %d - never a good sign!", responseCode);
        }
        //if(log)
        QSOS((L"%S responseCode %d!", url.c_str(), responseCode));
        errorOccured = true;
      }
      else if((responseCode == 302) && (url.find("login") == std::string::npos))
      {
        QTRACE((L"302: Using forms based authentication"));
        // MWMWM Can we always guarantee 302 will be for forms login?
        // For now I think the Transformer will not return 302 in any other scenario        
        //if(formsBasedLogin(curl, url.substr(0, url.find("/", 8)), pointerReply->Headers))
        {
          errorOccured = true;
        }
      }
    }

    if(errorOccured && numOfRetries <= 0)
    {
      QSOS((L"HTTPDeliverWithReply retried %d times but failed with URL %s. res %d. responseCode %d", kNumOfRetries, widen(url).c_str(), res, responseCode));
      if(dataError)
        throw cHTTPError(__FILE__, __LINE__, 0, pointerReply->Data, L"Failed to read data properly during HTTP Deliver to:%S: %d != %d", url.c_str(), pointer->Offset, pointer->Data->getSize());
      else
        throw cHTTPError(__FILE__, __LINE__, responseCode, pointerReply->Data, L"responseCode %d - never a good sign!", responseCode);
    }
  } while(errorOccured && numOfRetries--);

  return pointerReply->Data;
}


vfs::cMemoryView::Ptr HTTPPostJSONWithReply(const std::string& url, cMemoryView::Ptr payload, bool log)
{
  std::list<std::string> headers;
  headers.push_back("Content-Type: application/json");
  return HTTPDeliverWithReply(url, payload, headers, false, log);
}

vfs::cMemoryView::Ptr HTTPPutJSONWithReply(const std::string& url, vfs::cMemoryView::Ptr payload, bool log)
{
  std::list<std::string> headers;
  headers.push_back("Content-Type: application/json");
  return HTTPDeliverWithReply(url, payload, headers, true, log);
}
