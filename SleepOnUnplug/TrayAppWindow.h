#pragma once
#include "eUnplugAction.h"

class PowerSettingsNotification
{
	HPOWERNOTIFY hpn = nullptr;

public:
	HRESULT registerWindow( HWND wnd ) noexcept;
	HRESULT unregister() noexcept;
	~PowerSettingsNotification() noexcept;
};

class TrayAppWindow: public CWindowImpl<TrayAppWindow>
{
	eUnplugAction action;
	PowerSettingsNotification psn;
	NOTIFYICONDATA nid;

	static constexpr UINT WM_TRAYICON = WM_USER + 1;
	static constexpr UINT ID_TRAY_EXIT = 1001;

public:
	TrayAppWindow( eUnplugAction act );

	BEGIN_MSG_MAP( TrayAppWindow )
		MESSAGE_HANDLER( WM_CREATE, onCreate )
		MESSAGE_HANDLER( WM_TRAYICON, onTrayIcon )
		COMMAND_ID_HANDLER( ID_TRAY_EXIT, onExit )
		MESSAGE_HANDLER( WM_DESTROY, onDestroy )
		MESSAGE_HANDLER( WM_POWERBROADCAST, onPowerBroadcast );
	END_MSG_MAP()

	static LPCWSTR GetWndClassName() { return L"TrayAppWindow"; }

private:
	LRESULT onCreate( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT onTrayIcon( UINT, WPARAM, LPARAM lParam, BOOL& );
	LRESULT onExit( WORD, WORD, HWND, BOOL& );
	LRESULT onDestroy( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT onPowerBroadcast( UINT, WPARAM wParam, LPARAM, BOOL& );
};