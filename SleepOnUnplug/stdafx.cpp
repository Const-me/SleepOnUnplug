#include "stdafx.h"

HRESULT formatErrorMessage( std::wstring& result, const char* what, HRESULT status ) noexcept
{
	try 
	{
		// Convert 'what' from UTF-8 to UTF-16
		int wlen = MultiByteToWideChar( CP_UTF8, 0, what, -1, nullptr, 0 );
		std::wstring whatW( wlen, L'\0' );
		MultiByteToWideChar( CP_UTF8, 0, what, -1, &whatW[ 0 ], wlen );
		whatW.pop_back(); // remove null terminator

		// Try FormatMessage with US English first
		wchar_t* msgBuffer = nullptr;
		constexpr DWORD langId = MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US );
		constexpr DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		DWORD size = FormatMessageW( flags, nullptr, status, langId, (LPWSTR)&msgBuffer, 0, nullptr );
		if( size == 0 ) 
			size = FormatMessageW( flags, nullptr, status, 0, (LPWSTR)&msgBuffer, 0, nullptr );
		if( size == 0 || msgBuffer == nullptr )
		{
			result = std::move( whatW );
			return S_OK;
		}

		// Compose final result
		std::wstring messageW = std::wstring( msgBuffer, size );
		result = whatW + L"\r\n" + messageW;
		return S_OK;
	}
	catch( const std::bad_alloc& ) 
	{
		return E_OUTOFMEMORY;
	}
}