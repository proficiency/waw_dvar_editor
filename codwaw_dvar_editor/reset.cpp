#include "context.h"

decltype( hooked::o_reset ) hooked::o_reset;

long __stdcall hooked::reset( void* device, void* params )
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const long ret = hooked::o_reset( device, params );

	ImGui_ImplDX9_CreateDeviceObjects( );

	return ret;
}