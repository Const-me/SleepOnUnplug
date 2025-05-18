#include <stdafx.h>
#include "TrayAppWindow.h"

CComModule _AtlModule;

// Utility class to ensure this application only runs in a single instance
class SingleInstanceCheck
{
	CHandle mutex;
public:
	SingleInstanceCheck() = default;

	bool initialize() noexcept
	{
		LPCTSTR mutexName = L"{e5f3c6b2-99c8-40fd-a897-a9d6671af3c3}";
		const HANDLE h = CreateMutex( nullptr, TRUE, mutexName );
		if( nullptr == h )
		{
			// TODO: include information why
			MessageBox( nullptr, L"Failed to create mutex.", L"Error", MB_ICONERROR | MB_OK );
			return false;
		}
		mutex.Attach( h );

		if( GetLastError() == ERROR_ALREADY_EXISTS ) 
		{
			MessageBox( nullptr, L"Another instance of the application is already running.", L"Sleep on Unplug", MB_ICONINFORMATION | MB_OK );
			mutex.Close();
			return false;
		}

		return true;
	}

	~SingleInstanceCheck() noexcept
	{
		mutex.Close();
	}
};

int __stdcall wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	SingleInstanceCheck sic;
	if( !sic.initialize() )
		return -1;

	TrayAppWindow wnd;
	wnd.Create( nullptr, CWindow::rcDefault, L"Sleep on Unplug", WS_OVERLAPPEDWINDOW );
	if( !wnd )
		return -2;
	::ShowWindow( wnd.m_hWnd, SW_HIDE );

	MSG msg;
	while( GetMessage( &msg, nullptr, 0, 0 ) ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return (int)msg.wParam;
}