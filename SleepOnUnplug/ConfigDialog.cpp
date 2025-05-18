#include "stdafx.h"
#include "ConfigDialog.h"
#include "Registry.h"

ConfigDialog::ConfigDialog( eUnplugAction act ):
	action( act ) 
{
}

LRESULT ConfigDialog::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CenterWindow();

	CWindow combo = GetDlgItem( IDC_ACTION );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Show message" );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Sleep" );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Hibernate" );

	int curSel = 1;	// Default to Sleep
	switch( action )
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
	return 1;
}

LRESULT ConfigDialog::OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
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

LRESULT ConfigDialog::OnBnClickedOk( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	CWindow combo = GetDlgItem( IDC_ACTION );
	const int curSel = (int)combo.SendMessageW( CB_GETCURSEL );
	switch( curSel )
	{
	case 0:
		action = eUnplugAction::Message;
		break;
	case 1:
		action = eUnplugAction::Sleep;
		break;
	case 2:
		if( !isHibernationEnabled() )
		{
			MessageBox( L"Hibernation is disabled in the OS preferences", messageTitle, MB_OK | MB_ICONWARNING );
			return 0;
		}
		action = eUnplugAction::Hibernate;
		break;
	default:
		EndDialog( IDCANCEL );
		return 0;
	}

	const HRESULT hr = actionStore( action );
	if( SUCCEEDED( hr ) )
	{
		EndDialog( IDOK );
		return 0;
	}

	// TODO: report an error
	return 0;
}