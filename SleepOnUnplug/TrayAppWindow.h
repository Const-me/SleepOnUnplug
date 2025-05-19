#pragma once
#include "eUnplugAction.h"

// RAII class to subscribe a window for WM_POWERBROADCAST notifications from the OS
class PowerSettingsNotification
{
	HPOWERNOTIFY hpn = nullptr;

public:
	HRESULT registerWindow( HWND wnd ) noexcept;
	HRESULT unregister() noexcept;
	~PowerSettingsNotification() noexcept;
};

using TrayAppWindowTraits = CWinTraits<0, 0>;

// Main window of the application; it's invisible i.e. message-only
class TrayAppWindow: public CWindowImpl<TrayAppWindow, CWindow, TrayAppWindowTraits>
{
	eUnplugAction action;
	PowerSettingsNotification psn;
	HICON icon = nullptr;
	HWND configDialog = nullptr;
	NOTIFYICONDATA nid;

	static constexpr UINT WM_TRAYICON = WM_USER + 1;
	static constexpr UINT ID_TRAY_CONFIG = 1001;
	static constexpr UINT ID_TRAY_EXIT = 1002;

public:
	TrayAppWindow( eUnplugAction act );

	bool create() noexcept;

	BEGIN_MSG_MAP( TrayAppWindow )
		MESSAGE_HANDLER( WM_CREATE, onCreate )
		MESSAGE_HANDLER( WM_TRAYICON, onTrayIcon )
		COMMAND_ID_HANDLER( ID_TRAY_CONFIG, onConfig )
		COMMAND_ID_HANDLER( ID_TRAY_EXIT, onExit )
		MESSAGE_HANDLER( WM_DESTROY, onDestroy )
		MESSAGE_HANDLER( WM_POWERBROADCAST, onPowerBroadcast );
	END_MSG_MAP()

	static LPCWSTR GetWndClassName() { return L"SleepOnUnplug"; }

private:
	LRESULT onCreate( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT onTrayIcon( UINT, WPARAM, LPARAM lParam, BOOL& );
	LRESULT onConfig( WORD, WORD, HWND, BOOL& );
	LRESULT onExit( WORD, WORD, HWND, BOOL& );
	LRESULT onDestroy( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT onPowerBroadcast( UINT, WPARAM wParam, LPARAM, BOOL& );
};