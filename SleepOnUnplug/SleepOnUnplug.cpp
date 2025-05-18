#include <stdafx.h>
#include "TrayAppWindow.h"

CComModule _AtlModule;

int __stdcall wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	TrayAppWindow wnd;
	wnd.Create( nullptr, CWindow::rcDefault, L"Sleep on Unplug", WS_OVERLAPPEDWINDOW );
	if( !wnd )
		return -1;
	::ShowWindow( wnd.m_hWnd, SW_HIDE );

	MSG msg;
	while( GetMessage( &msg, nullptr, 0, 0 ) ) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return (int)msg.wParam;
}