#include <stdafx.h>
#include "TrayAppWindow.h"
#include "Resource.h"

TrayAppWindow::TrayAppWindow()
{
	constexpr size_t cb = sizeof( NOTIFYICONDATA );
	static_assert( 0 == ( cb % 8 ) && 8 == alignof( NOTIFYICONDATA ) );
	__stosq( (DWORD64*)&nid, 0, cb / 8 );
}

LRESULT TrayAppWindow::OnCreate( UINT, WPARAM, LPARAM, BOOL& )
{
	nid.cbSize = sizeof( nid );
	nid.hWnd = m_hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = LoadIcon( _AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE( IDI_SLEEPONUNPLUG ) );
	wcscpy_s( nid.szTip, L"Sleep on Unplug" );

	if( !Shell_NotifyIcon( NIM_ADD, &nid ) )
	{
		// TODO: report error
		return -1;
	}
	return 0;
}

LRESULT TrayAppWindow::OnTrayIcon( UINT, WPARAM, LPARAM lParam, BOOL& )
{
	if( LOWORD( lParam ) == WM_RBUTTONUP ) 
	{
		POINT pt;
		GetCursorPos( &pt );
		HMENU hMenu = CreatePopupMenu();
		AppendMenu( hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit" );
		SetForegroundWindow( m_hWnd );
		TrackPopupMenu( hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, nullptr );
		DestroyMenu( hMenu );
	}
	return 0;
}

LRESULT TrayAppWindow::OnExit( WORD, WORD, HWND, BOOL& )
{
	Shell_NotifyIcon( NIM_DELETE, &nid );
	DestroyWindow();
	return 0;
}

LRESULT TrayAppWindow::OnDestroy( UINT, WPARAM, LPARAM, BOOL& )
{
	Shell_NotifyIcon( NIM_DELETE, &nid );
	PostQuitMessage( 0 );
	return 0;
}