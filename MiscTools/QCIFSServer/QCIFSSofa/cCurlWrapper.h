//cCurlWrapper.h

#pragma once

vfs::cMemoryView::Ptr HTTPGet_basic(const std::string& url, bool log, std::list<std::string>* pHeaders);

vfs::cMemoryView::Ptr HTTPPostJSONWithReply(const std::string& url, vfs::cMemoryView::Ptr payload, bool log);
vfs::cMemoryView::Ptr HTTPPutJSONWithReply(const std::string& url, vfs::cMemoryView::Ptr payload, bool log);


class cHTTPError : public vfs::cRecoverable
{
public:
  unsigned int ResponseCode;
  vfs::cMemoryView::Ptr Payload;
  virtual ~cHTTPError() {};
  cHTTPError(const char* const  SrcFilename, const unsigned long SrcLineNumber, unsigned int responseCode, vfs::cMemoryView::Ptr payload, const vfs::Char* MsgFormat, ...) :
    cRecoverable(SrcFilename, SrcLineNumber, MsgFormat, (va_list)&MsgFormat + _INTSIZEOF(MsgFormat)), ResponseCode(responseCode), Payload(payload)
  {
  }
};