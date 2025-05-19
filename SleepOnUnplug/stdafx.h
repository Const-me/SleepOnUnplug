#pragma once

#include "targetver.h"

// C header files
#include <stdint.h>
#include <assert.h>

// C++ header files
#include <string>

// Windows Header Files
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>

// ATL header files
#include <atlbase.h>
#include <atlwin.h>

// Retrieve the calling thread's last-error code value, pack into HRESULT status code
inline HRESULT getLastHr() { return HRESULT_FROM_WIN32( ::GetLastError() ); }

// Produce UTF-16 error message which combines UTF-8 reason string, and a message formatted from the failed HRESULT status
HRESULT formatErrorMessage( std::wstring& result, const char* what, HRESULT status ) noexcept;

extern const LPCTSTR messageTitle;