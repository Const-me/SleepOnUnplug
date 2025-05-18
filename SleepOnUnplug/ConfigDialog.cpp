#include "stdafx.h"
#include "ConfigDialog.h"
#include "Registry.h"

ConfigDialog::ConfigDialog( eUnplugAction act ):
	action( act ) 
{
}

LRESULT ConfigDialog::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CWindow combo = GetDlgItem( IDC_ACTION );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Show message" );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Sleep" );
	combo.SendMessageW( CB_ADDSTRING, 0, (LPARAM)L"Hybernate" );

	int curSel = 1;	// Default to Sleep
	switch( action )
	{
	case eUnplugAction::Unspecified:
	default:
		break;
	case eUnplugAction::Message:
		curSel = 0;
		break;
	case eUnplugAction::Hybernate:
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
		action = eUnplugAction::Hybernate;
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