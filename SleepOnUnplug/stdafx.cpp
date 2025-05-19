#include "stdafx.h"

namespace
{
	// Utility class to call FormatMessage WinAPI; owns the allocated UTF-16 buffer with the message
	class FormattedMessage
	{
		wchar_t* buffer = nullptr;
		DWORD len = 0;

	public:
		FormattedMessage() noexcept = default;
		FormattedMessage( const FormattedMessage& ) noexcept = delete;

		~FormattedMessage()
		{
			// If the hMem parameter is NULL, LocalFree ignores the parameter and returns NULL.
			// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-localfree
			LocalFree( buffer );
		}

		bool tryFormat( HRESULT status ) noexcept
		{
			assert( nullptr == buffer );

			// Try FormatMessage with US English first
			constexpr DWORD langId = MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US );
			constexpr DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			len = FormatMessageW( flags, nullptr, status, langId, (LPWSTR)&buffer, 0, nullptr );
			if( len != 0 && buffer != nullptr )
				return true;

			LocalFree( buffer );
			buffer = nullptr;

			// Use OS default locale
			len = FormatMessageW( flags, nullptr, status, 0, (LPWSTR)&buffer, 0, nullptr );
			if( len != 0 && buffer != nullptr )
				return true;

			// No message is available for the provided status code
			LocalFree( buffer );
			buffer = nullptr;
			return false;
		}

		// Trim whitespace characters at the end of the message
		// Returns false is the message became empty string as the result
		bool trimEnd() noexcept
		{
			const wchar_t* end = buffer + len;
			while( true )
			{
				if( end <= buffer )
				{
					len = 0;
					return false;
				}

				const wchar_t c = *( end - 1 );
				switch( c )
				{
				case L' ':
				case L'\t':
				case L'\r':
				case L'\n':
					break;

				default:
					len = (DWORD)( end - buffer );
					return true;
				}

				end--;
			}
		}

		DWORD length() const noexcept { return len; }
		const wchar_t* c_str() const noexcept { return buffer; }
	};
}

HRESULT __declspec( noinline ) formatErrorMessage( std::wstring& result, const char* what, HRESULT status ) noexcept
{
	try
	{
		// Convert 'what' string from UTF-8 to UTF-16
		int wlen = MultiByteToWideChar( CP_UTF8, 0, what, -1, nullptr, 0 );
		std::wstring message( wlen, L'\0' );
		MultiByteToWideChar( CP_UTF8, 0, what, -1, message.data(), wlen );
		message.pop_back(); // remove null terminator

		if( FAILED( status ) )
		{
			// Try to convert HRESULT status into English message
			FormattedMessage fm;
			if( fm.tryFormat( status ) && fm.trimEnd() )
			{
				// Compose final result
				message.reserve( message.length() + fm.length() + 3 );
				using namespace std::string_view_literals;
				message.append( L"\r\n"sv );
				message.append( fm.c_str(), fm.length() );
			}
		}

		result = std::move( message );
		return S_OK;
	}
	catch( const std::bad_alloc& )
	{
		return E_OUTOFMEMORY;
	}
}

const LPCTSTR messageTitle = L"Sleep on Unplug";