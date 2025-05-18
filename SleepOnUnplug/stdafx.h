#pragma once

#include "targetver.h"

// C header files
#include <stdint.h>
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

inline HRESULT getLastHr() { return HRESULT_FROM_WIN32( ::GetLastError() ); }

HRESULT formatErrorMessage( std::wstring& result, const char* what, HRESULT status ) noexcept;

extern const LPCTSTR messageTitle;