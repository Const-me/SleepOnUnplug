#include "stdafx.h"
#include "ConfigDialog.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

ConfigDialog::ConfigDialog( UnplugAction act, HICON hIcon, HWND* pWindowHandle ) :
	action( act ), icon( hIcon ), windowHandle( pWindowHandle )
{
}

LRESULT ConfigDialog::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ ) noexcept
{
	SendMessage( m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)icon );
	CenterWindow();

	CWindow combo = GetDlgItem( IDC_ACTION );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Show message" );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Sleep" );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Hibernate" );

	int curSel = 1;	// Default to Sleep
	switch( action.action() )
	{
	case eUnplugAction::Unspecified:
	default:
		break;
	case eUnplugAction::Message:
		curSel = 0;
		break;
	case eUnplugAction::Hibernate:
		curSel = 2;
		break;
	}
	combo.SendMessageW( CB_SETCURSEL, curSel );

	CWindow cbWakeEvents = GetDlgItem( IDC_WAKE_EVENTS );
	cbWakeEvents.SendMessageW( BM_SETCHECK, action.wakeupEventsDisabled() ? BST_UNCHECKED : BST_CHECKED );

	if( nullptr != windowHandle )
		*windowHandle = m_hWnd;

	toolTip = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, nullptr,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		0, 0, 0, 0, m_hWnd, nullptr, _AtlBaseModule.GetModuleInstance(), nullptr );

	if( nullptr != toolTip )
	{
		::SetWindowPos( toolTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );

		TOOLINFO toolInfo = { 0 };
		toolInfo.cbSize = sizeof( toolInfo );
		toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		toolInfo.hwnd = m_hWnd;
		toolInfo.uId = (UINT_PTR)combo.m_hWnd;
		toolInfo.lpszText = (wchar_t*)L"What to do when the AC power is unplugged.";

		LRESULT status = ::SendMessage( toolTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo );
		assert( status == TRUE );

		toolInfo.uId = (UINT_PTR)cbWakeEvents.m_hWnd;
		toolInfo.lpszText = (wchar_t*)L"If unchecked, only the power button wakes the PC; mouse and keyboard won’t.";
		status = ::SendMessage( toolTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo );
		assert( status == TRUE );

		toolInfo.uId = (UINT_PTR)( ::GetDlgItem( m_hWnd, IDOK ) );
		toolInfo.lpszText = (wchar_t*)L"Save preferences to registry, and close this dialog.";
		status = ::SendMessage( toolTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo );
		assert( status == TRUE );
	}

	return 1;
}

LRESULT ConfigDialog::OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ ) noexcept
{
	EndDialog( IDCANCEL );
	return 0;
}

static inline bool isHibernationEnabled()
{
	DWORD dwResult = 0, dwSize = 4;
	LONG ret = RegGetValue( HKEY_LOCAL_MACHINE,
		LR"(SYSTEM\CurrentControlSet\Control\Power)", L"HibernateEnabled",
		RRF_RT_REG_DWORD, nullptr, &dwResult, &dwSize );
	return ( ret == ERROR_SUCCESS && dwResult != 0 );
}

LRESULT ConfigDialog::OnBnClickedOk( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ ) noexcept
{
	int curState = (int)SendDlgItemMessageW( IDC_ACTION, CB_GETCURSEL );
	eUnplugAction act;
	switch( curState )
	{
	case 0:
		act = eUnplugAction::Message;
		break;
	case 1:
		act = eUnplugAction::Sleep;
		break;
	case 2:
		if( !isHibernationEnabled() )
		{
			MessageBox( L"Hibernation is disabled in the OS preferences", messageTitle, MB_OK | MB_ICONWARNING );
			return 0;
		}
		act = eUnplugAction::Hibernate;
		break;
	default:
		EndDialog( IDCANCEL );
		return 0;
	}

	curState = (int)SendDlgItemMessage( IDC_WAKE_EVENTS, BM_GETCHECK );
	const bool enableWakeupEvents = ( curState == BST_CHECKED );

	UnplugAction action{ act, enableWakeupEvents };
	const HRESULT hr = action.store();
	if( SUCCEEDED( hr ) )
	{
		this->action = action;
		EndDialog( IDOK );
		return 0;
	}

	// Report an error
	std::wstring message;
	formatErrorMessage( message, "Unable to save the preferences", hr );
	MessageBox( message.c_str(), messageTitle, MB_ICONWARNING | MB_OK );
	return 0;
}