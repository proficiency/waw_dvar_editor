#include <windows.h>
#include "context.h"

decltype( hooked::o_wnd_proc ) hooked::o_wnd_proc;

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
long __stdcall hooked::wnd_proc( HWND hwnd, uint32_t msg, uint32_t uparam, long param )
{
	// menu open/ close		
	if ( msg == WM_KEYUP && uparam == VK_INSERT )
	{
		ctx.m_menu_open = !ctx.m_menu_open;
		ImGui::GetIO( ).MouseDrawCursor = ctx.m_menu_open;
		engine::set_mousestate( ctx.m_menu_open );
	}

	ctx.m_wndproc_ran = true;
	
	if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, uparam, param ) && ctx.m_menu_open )
		return true;

	return CallWindowProcA( hooked::o_wnd_proc, hwnd, msg, uparam, param );
}