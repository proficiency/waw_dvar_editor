#pragma once
#pragma once

#include <cstdio>
#include <vector>
#include <stdint.h>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <Shlwapi.h>
#include <Psapi.h>
#include <winternl.h>
#include <memory>
#include <fstream>
#include <intrin.h>
#include <time.h>
#include <ctime>
#include <chrono>
#include <Shlobj.h>
#include <map>

#include <d3d9.h>
#include <d3dx9.h>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"

// integral constant overflow
#pragma warning( disable : 4307 )

// conversion from x to x possible loss of data
#pragma warning( disable : 4244 )

// signed unsigned mismatch
#pragma warning( disable : 4018 )

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "Shlwapi.lib" )
#pragma comment( lib, "Shell32.lib" )

#define __VARIABLE__( v ) #v

#undef interface

typedef HMODULE  module_t;
typedef HANDLE   handle_t;
typedef FILE     file_t;
typedef HWND     hwnd_t;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned long ul;

#include "hash.h"

#include "memory.h"
#include "sdk.h"
#include "math.h"

class c_patterns
{
public:
	i8** m_release{ };
	i8** m_reset{ };
	
	u32 m_dvar_array{};
	u32 m_dvar_arraysize{ };
	u32 m_mousedisabled{ };
	u32 m_writepacket{};

	void( __cdecl* m_sendcmd )( int, int, const char* );
};

class c_context
{
public:
	c_patterns          m_patterns = { };

	std::vector< dvar_s* > m_dvars = {};
	std::vector< std::string > m_files = {};

	bool m_wndproc_ran = {};
	bool m_menu_open = {};

	inline void fill_filearray( std::vector<std::string>& out, const std::string& directory )
	{
		HANDLE dir;
		WIN32_FIND_DATA file_data;

		if ( ( dir = FindFirstFile( ( directory + "/*" ).c_str( ), &file_data ) ) == INVALID_HANDLE_VALUE )
			return;

		do
		{
			const std::string file_name = file_data.cFileName;
			std::string full_file_name = directory + "/" + file_name;
			const bool is_directory = ( file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;

			if ( file_name[0] == '.' || !strstr( file_name.c_str( ), ".cfg" ) )
				continue;

			if ( is_directory )
				continue;

			out.push_back( full_file_name );
		} while ( FindNextFile( dir, &file_data ) );

		FindClose( dir );
	}
}; extern c_context ctx;

namespace hooked
{
	extern ul __stdcall release( IDirect3DDevice9* device );
	extern decltype( &release ) o_release;

	extern long __stdcall reset( void* device, void* params );
	extern decltype( &reset ) o_reset;

	extern long __stdcall endscene( void* device );
	extern decltype( &endscene ) o_endscene;

	extern long __stdcall wnd_proc( HWND hwnd, uint32_t msg, uint32_t uparam, long param );
	extern decltype( &wnd_proc ) o_wnd_proc;
}

namespace engine
{
	inline void set_mousestate( bool enable )
	{
		**( bool** ) ctx.m_patterns.m_mousedisabled = !enable;
	}

	inline dvar_s* find_dvar( u32 name )
	{
		for ( u32 i = 0; i < ctx.m_dvars.size( ); ++i )
		{
			dvar_s* dvar = ctx.m_dvars[i];

			if ( !dvar || !dvar->name )
				continue;

			if ( fnvr( dvar->name ) == name )
				return dvar;
		}

		return nullptr;
	}

}