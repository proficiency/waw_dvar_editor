#include "context.h"

c_context ctx;

void setup( HINSTANCE instance )
{
	ctx.fill_filearray( ctx.m_files, xors( "./" ) );

	ctx.m_patterns.m_dvar_array      = memory::pattern_search( xors( "89 1D ? ? ? ? 89 1D ? ? ? ? 89 1D ? ? ? ? E8 ? ? ? ? 83 C4 0C B8 ? ? ? ?" ), 2 );
	ctx.m_patterns.m_dvar_arraysize  = memory::pattern_search( xors( "3B 35 ? ? ? ? 7C E7 5E 5B C3" ), 2 );
	ctx.m_patterns.m_mousedisabled   = memory::pattern_search( xors( "80 3D ? ? ? ? ? 53 8B 5C 24 08 74 52 57 8B 3D" ), 2 );
	ctx.m_patterns.m_writepacket     = memory::pattern_search( xors( "81 EC ? ? ? ? A1 ? ? ? ? 53 8B 1D" ) );
	ctx.m_patterns.m_reset           = *memory::pattern_search< i8*** >( xors( "FF 15 ? ? ? ? 8B F8 85 FF 78 18 85 F6" ), 2, GetModuleHandle( xors( "GameOverlayRenderer.dll" ) ) );
	ctx.m_patterns.m_release         = *memory::pattern_search< i8*** >( xors( "FF 15 ? ? ? ? 57 B9 ? ? ? ? E8" ), 2, GetModuleHandle( xors( "GameOverlayRenderer.dll" ) ) );
	ctx.m_patterns.m_sendcmd         = memory::pattern_search< decltype( ctx.m_patterns.m_sendcmd ) >( xors( "A1 ? ? ? ? 85 C0 53 55 8B 6C 24 14 56 57" ) );

	hooked::o_wnd_proc        = ( decltype( &hooked::wnd_proc ) ) SetWindowLong( FindWindow( xors( "CoD-WaW" ), 0 ), -4, ( long ) hooked::wnd_proc );
	hooked::o_cl_writepacket  = memory::detours.create_hook< decltype( &hooked::cl_writepacket ) >( ( void* ) ctx.m_patterns.m_writepacket, hooked::cl_writepacket );

	// FF 15 50 96 10 10     call    g_reset
	{
		hooked::o_reset = ( decltype( &hooked::reset ) ) ( *ctx.m_patterns.m_reset );
		*ctx.m_patterns.m_reset = ( i8* ) ( &hooked::reset );

		hooked::o_release = ( decltype( &hooked::release ) ) ( *ctx.m_patterns.m_release );
		*ctx.m_patterns.m_release = ( i8* ) ( &hooked::release );
	}

	ExitThread( 0 );
}

bool __stdcall DllMain( HINSTANCE instance, unsigned long reason, void* reserved )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( instance );

	#ifdef TESTBUILD
		if ( AllocConsole( ) )
		{
			SetConsoleTitle( xors( "waw hack" ) );
			SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
			SetLayeredWindowAttributes( GetConsoleWindow( ), 0, 230, 2 );
			freopen( xors( "CONOUT$" ), xors( "w" ), stdout );

			printf( xors( " /$$                                          /$$                           /$$      \n" ) );
			printf( xors( "| $$                                         | $$                          | $$      \n" ) );
			printf( xors( "| $$$$$$$   /$$$$$$   /$$$$$$$ /$$$$$$$      | $$$$$$$   /$$$$$$   /$$$$$$$| $$   /$$\n" ) );
			printf( xors( "| $$__  $$ /$$__  $$ /$$_____//$$_____/      | $$__  $$ |____  $$ /$$_____/| $$  /$$/\n" ) );
			printf( xors( "| $$  \\ $$| $$  \\ $$|  $$$$$$|  $$$$$$       | $$  \\ $$  /$$$$$$$| $$      | $$$$$$/ \n" ) );
			printf( xors( "| $$  | $$| $$  | $$ \\____  $$\\____  $$      | $$  | $$ /$$__  $$| $$      | $$_  $$ \n" ) );
			printf( xors( "| $$$$$$$/|  $$$$$$/ /$$$$$$$//$$$$$$$/      | $$  | $$|  $$$$$$$|  $$$$$$$| $$ \\  $$\n" ) );
			printf( xors( "|_______/  \\______/ |_______/|_______/       |__/  |__/ \\_______/ \\_______/|__/  \\__/\n" ) );
			printf( xors( "\n" ) );
		}
	#endif

		CreateThread( 0, 0, LPTHREAD_START_ROUTINE( setup ), instance, 0, 0 );

		return true;
	}

	return false;
}