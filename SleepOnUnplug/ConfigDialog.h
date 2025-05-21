#pragma once
#include "resource.h"
#include "UnplugAction.h"

class ConfigDialog: public CDialogImpl<ConfigDialog>
{
	UnplugAction action;
	const HICON icon;
	HWND* const windowHandle;
public:
	UnplugAction unplugAction() const noexcept { return action; }

	ConfigDialog( UnplugAction act, HICON hIcon, HWND* pWindowHandle );

	static constexpr UINT IDD = IDD_CONFIG;

	BEGIN_MSG_MAP( ConfigDialog )
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		COMMAND_HANDLER( IDOK, BN_CLICKED, OnBnClickedOk )
		COMMAND_HANDLER( IDCANCEL, BN_CLICKED, OnBnClickedCancel )
	END_MSG_MAP()

private:
	LRESULT OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
	LRESULT OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
	LRESULT OnBnClickedOk( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
};