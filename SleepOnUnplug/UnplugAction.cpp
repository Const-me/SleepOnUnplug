#include <stdafx.h>
#include "UnplugAction.h"

namespace
{
	static const LPCWSTR kRegKeyPath = LR"(SOFTWARE\const.me\SleepOnUnplug)";
	static const LPCWSTR kValueName = L"action";

	inline uint8_t packAction( eUnplugAction act, bool enableWakeupEvents ) noexcept
	{
		uint8_t v = (uint8_t)act;
		v |= ( enableWakeupEvents ? (uint8_t)0x80 : (uint8_t)0 );
		return v;
	}
}

UnplugAction::UnplugAction( DWORD loaded ) noexcept :
	packed( (uint8_t)loaded )
{
	assert( 0 == ( loaded >> 8 ) );
}

UnplugAction::UnplugAction( eUnplugAction act, bool enableWakeupEvents ) noexcept :
	packed( packAction( act, enableWakeupEvents ) )
{
}

HRESULT UnplugAction::store() const noexcept
{
	CRegKey key;
	LSTATUS res = key.Create( HKEY_CURRENT_USER, kRegKeyPath );
	if( res != ERROR_SUCCESS )
		return HRESULT_FROM_WIN32( res );

	res = key.SetDWORDValue( kValueName, (DWORD)packed );
	if( res != ERROR_SUCCESS )
		return HRESULT_FROM_WIN32( res );

	return S_OK;
}

UnplugAction UnplugAction::load() noexcept
{
	DWORD dwResult = 0, dwSize = 4;
	RegGetValue( HKEY_CURRENT_USER, kRegKeyPath, kValueName,
		RRF_RT_REG_DWORD | RRF_ZEROONFAILURE, nullptr, &dwResult, &dwSize );
	return UnplugAction{ dwResult };
}