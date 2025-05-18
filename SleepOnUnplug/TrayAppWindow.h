#pragma once

class TrayAppWindow: public CWindowImpl<TrayAppWindow>
{
	NOTIFYICONDATA nid = {};

	static constexpr UINT WM_TRAYICON = WM_USER + 1;
	static constexpr UINT ID_TRAY_EXIT = 1001;

public:
	BEGIN_MSG_MAP( TrayAppWindow )
		MESSAGE_HANDLER( WM_CREATE, OnCreate )
		MESSAGE_HANDLER( WM_TRAYICON, OnTrayIcon )
		COMMAND_ID_HANDLER( ID_TRAY_EXIT, OnExit )
		MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
	END_MSG_MAP()

	static LPCWSTR GetWndClassName() { return L"TrayAppWindow"; }

private:
	LRESULT OnCreate( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnTrayIcon( UINT, WPARAM, LPARAM lParam, BOOL& );
	LRESULT OnExit( WORD, WORD, HWND, BOOL& );
	LRESULT OnDestroy( UINT, WPARAM, LPARAM, BOOL& );
};