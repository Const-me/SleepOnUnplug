#include "stdafx.h"
#include "Registry.h"

namespace
{
	static const LPCWSTR kRegKeyPath = LR"(SOFTWARE\const.me\SleepOnUnplug)";
	static const LPCWSTR kValueName = L"action";
}

eUnplugAction actionLoad() noexcept
{
	CRegKey key;
	LSTATUS res = key.Open( HKEY_CURRENT_USER, kRegKeyPath, KEY_QUERY_VALUE );
	if( res != ERROR_SUCCESS )
		return eUnplugAction::Unspecified;

	DWORD value = 0;
	res = key.QueryDWORDValue( kValueName, value );
	if( res != ERROR_SUCCESS )
		return eUnplugAction::Unspecified;

	return (eUnplugAction)(uint8_t)value;
}

HRESULT actionStore( eUnplugAction act ) noexcept
{
	CRegKey key;
	LSTATUS res = key.Create( HKEY_CURRENT_USER, kRegKeyPath );
	if( res != ERROR_SUCCESS )
		return HRESULT_FROM_WIN32( res );

	res = key.SetDWORDValue( kValueName, (DWORD)(uint8_t)( act ) );
	if( res != ERROR_SUCCESS )
		return HRESULT_FROM_WIN32( res );

	return S_OK;
}