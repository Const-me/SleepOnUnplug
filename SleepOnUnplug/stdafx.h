#pragma once

#include "targetver.h"

// C header files
#include <stdint.h>

// Windows Header Files
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>

// ATL header files
#include <atlbase.h>
#include <atlwin.h>

inline HRESULT getLastHr() { return HRESULT_FROM_WIN32( ::GetLastError() ); }