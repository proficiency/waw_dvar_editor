#include "context.h"

decltype( hooked::o_release ) hooked::o_release;

ul __stdcall hooked::release( IDirect3DDevice9* device )
{
	if ( !hooked::o_endscene )
	{
		hooked::o_endscene = memory::detours.create_hook< decltype( &hooked::endscene ) >( memory::get_vfunc< void* >( device, 42 ), hooked::endscene );
		memory::detours.enable( );
	}

	return hooked::o_release( device );
}