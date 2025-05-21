#include <stdafx.h>
#include <powrprof.h>
#include "TrayAppWindow.h"
#include "Resource.h"
#include "ConfigDialog.h"
#pragma comment(lib, "PowrProf.lib")

HRESULT PowerSettingsNotification::registerWindow( HWND wnd ) noexcept
{
	if( nullptr != hpn )
		return HRESULT_FROM_WIN32( ERROR_ALREADY_INITIALIZED );
	hpn = ::RegisterPowerSettingNotification( wnd, &GUID_ACDC_POWER_SOURCE, DEVICE_NOTIFY_WINDOW_HANDLE );
	if( nullptr == hpn )
		return getLastHr();
	return S_OK;
}

HRESULT PowerSettingsNotification::unregister() noexcept
{
	if( nullptr == hpn )
		return S_OK;
	const BOOL success = UnregisterPowerSettingNotification( hpn );
	hpn = nullptr;
	return success ? S_OK : getLastHr();
}

PowerSettingsNotification::~PowerSettingsNotification() noexcept
{
	unregister();
}

TrayAppWindow::TrayAppWindow( UnplugAction act ):
	action( act )
{
	constexpr size_t cb = sizeof( NOTIFYICONDATA );
	static_assert( 0 == ( cb % 8 ) && 8 == alignof( NOTIFYICONDATA ) );
	__stosq( (DWORD64*)&nid, 0, cb / 8 );
}

bool TrayAppWindow::create() noexcept
{
	const HWND wnd = Create( nullptr, 0, messageTitle );
	return nullptr != wnd;
}

LRESULT TrayAppWindow::onCreate( UINT, WPARAM, LPARAM, BOOL& )
{
	// Register for power source notifications
	HRESULT hr = psn.registerWindow( m_hWnd );
	if( FAILED( hr ) )
	{
		std::wstring message;
		formatErrorMessage( message, "Failed to register for power source notifications.", hr );
		MessageBox( message.c_str(), messageTitle, MB_ICONWARNING | MB_OK );
		return -1;
	}

	// Add the tray icon
	nid.cbSize = sizeof( nid );
	nid.hWnd = m_hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON;
	const HICON icon = LoadIcon( _AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE( IDI_SLEEPONUNPLUG ) );
	this->icon = icon;
	nid.hIcon = icon;
	wcscpy_s( nid.szTip, messageTitle );

	if( !Shell_NotifyIcon( NIM_ADD, &nid ) )
	{
		psn.unregister();
		MessageBox( L"Failed to add the tray icon.", messageTitle, MB_ICONWARNING | MB_OK );
		return -1;
	}

	return 0;
}

LRESULT TrayAppWindow::onTrayIcon( UINT, WPARAM, LPARAM lParam, BOOL& )
{
	if( LOWORD( lParam ) == WM_RBUTTONUP )
	{
		POINT pt;
		GetCursorPos( &pt );
		HMENU hMenu = CreatePopupMenu();
		AppendMenu( hMenu, MF_STRING, ID_TRAY_CONFIG, L"Configuration" );
		AppendMenu( hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit" );
		SetForegroundWindow( m_hWnd );
		TrackPopupMenu( hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, nullptr );
		DestroyMenu( hMenu );
	}
	return 0;
}

LRESULT TrayAppWindow::onConfig( WORD, WORD, HWND, BOOL& )
{
	if( nullptr != configDialog && ::IsWindow( configDialog ) )
	{
		SetForegroundWindow( configDialog );
		return 0;
	}

	ConfigDialog dlg{ action, icon, &configDialog };
	const INT_PTR res = dlg.DoModal( nullptr );
	configDialog = nullptr;
	if( res != IDOK )
		return 0;	// User canceled

	action = dlg.unplugAction();
	assert( !action.empty() );
	return 0;
}

LRESULT TrayAppWindow::onExit( WORD, WORD, HWND, BOOL& )
{
	psn.unregister();
	Shell_NotifyIcon( NIM_DELETE, &nid );
	DestroyWindow();
	return 0;
}

LRESULT TrayAppWindow::onDestroy( UINT, WPARAM, LPARAM, BOOL& )
{
	Shell_NotifyIcon( NIM_DELETE, &nid );
	PostQuitMessage( 0 );
	return 0;
}

LRESULT TrayAppWindow::onPowerBroadcast( UINT, WPARAM wParam, LPARAM, BOOL& handled )
{
	handled = false;
	if( wParam != PBT_APMPOWERSTATUSCHANGE )
		return 0;

	SYSTEM_POWER_STATUS sps;
	if( !GetSystemPowerStatus( &sps ) )
	{
		std::wstring message;
		formatErrorMessage( message, "GetSystemPowerStatus() failed", getLastHr() );
		MessageBox( message.c_str(), messageTitle, MB_ICONWARNING | MB_OK );
		return 0;
	}

	if( sps.ACLineStatus != 0 )
		return 0;

	handled = TRUE;
	BOOL hibernate;
	switch( action.action() )
	{
	case eUnplugAction::Message:
		MessageBox( L"Power unplug detected", messageTitle, MB_ICONINFORMATION | MB_OK );
		return TRUE;
	case eUnplugAction::Sleep:
		hibernate = FALSE;
		break;
	case eUnplugAction::Hibernate:
		hibernate = TRUE;
		break;
	default:
		handled = FALSE;
		return FALSE;
	}

	const BOOL bWakeupEventsDisabled = action.wakeupEventsDisabled();
	const BOOL res = SetSuspendState( hibernate, TRUE, bWakeupEventsDisabled );
	if( res == TRUE )
		return TRUE;

	std::wstring message;
	formatErrorMessage( message, "SetSuspendState() failed", getLastHr() );
	MessageBox( message.c_str(), messageTitle, MB_ICONWARNING | MB_OK );
	return TRUE;
}