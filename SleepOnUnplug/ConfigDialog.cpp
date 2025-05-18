#include "stdafx.h"
#include "ConfigDialog.h"

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
	return 1;
}

LRESULT ConfigDialog::OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	EndDialog( IDCANCEL );
	return 0;
}

LRESULT ConfigDialog::OnBnClickedOk( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	EndDialog( IDCANCEL );
	return 0;
}