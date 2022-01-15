#pragma once
#include "detours.h"

namespace memory
{
	template <typename t>
	__forceinline t get_vfunc( void* base, u32 index )
	{
		uintptr_t* vfunc_table = *( uintptr_t** ) ( base );
		return ( t ) ( vfunc_table[index] );
	}
	class c_vmt
	{
		u32* m_table{ };
		u32* m_original{ };
		std::vector< u32 > m_new{ };

	public:
		c_vmt( void* vmt )
		{
			if ( !vmt )
				return;

			m_table = ( u32* ) vmt;
			m_original = *( u32** ) m_table;

			// count vmt length and fill table
			while ( m_original[m_new.size( )] )
				m_new.push_back( m_original[m_new.size( ) - 1] );

			if ( !m_new.empty( ) )
				*m_table = ( u32 ) ( &m_new.data( )[1] );
		}

		u32 hook_count( )
		{
			return m_new.size( );
		}

		// get function address and maybe hook it
		template< typename t = u32 > t get_function( const u32& index, const void* new_func = 0 )
		{
			// if we pass a func, hook it
			if ( new_func > 0 )
				hook( index, new_func );

			// return virtual func at index
			return ( t ) m_original[index];
		}

		// hook a function but don't return address
		void hook( const u32& index, const void* new_func )
		{
			m_new[index + 1] = ( u32 ) new_func;
		}

		// unhook a specific function
		void unhook( const u32& index )
		{
			m_new[index + 1] = m_original[index];
		}

		// unhook the whole instance
		void restore( )
		{
			*m_table = ( u32 ) m_original;
		}
	};

	template < typename t = uintptr_t >
	static t pattern_search(
		// pattern of bytes to search for
		const char* pattern,

		// offset in bytes from address obtained by pattern searching
		const int& offset = 0,

		// module to search within
		const HMODULE& module = GetModuleHandle( 0 ),

		// search region
		const uintptr_t& region_start = 4096,
		const uintptr_t& region_end = 0
	)
	{
		auto get_pattern = []( const char* raw_pattern ) -> std::vector< uintptr_t >
		{
			std::vector< uintptr_t > pattern;
			char* start = ( char* ) raw_pattern;
			char* end = ( char* ) ( raw_pattern ) +strlen( raw_pattern );

			for ( char* byte = start; byte < end; ++byte )
			{
				// handle ida sttyle signatures, like a1 ? ? ? ? or a1 ?? ??
				if ( *byte == '?' )
				{
					// jump past ? in string, see if we land on another ?
					if ( *( ++byte ) == '?' )
						byte++;

					pattern.push_back( std::numeric_limits< uintptr_t >::max( ) );
				}
				else
				{
					// we have a valid byte to push back to our new pattern
					pattern.push_back( strtoul( byte, &byte, 16 ) );
				}
			}

			return pattern;
		};

		auto get_pattern_fast = []( uint8_t* bytes, uint8_t* mask, const char* raw_pattern ) -> size_t
		{
			std::vector< uintptr_t > pattern;
			char* start = ( char* ) raw_pattern;
			char* end = ( char* ) ( raw_pattern ) +strlen( raw_pattern );

			size_t counter = 0;

			for ( char* byte = start; byte < end; ++byte )
			{
				// handle ida sttyle signatures, like a1 ? ? ? ? or a1 ?? ??
				if ( *byte == '?' )
				{
					// jump past ? in string, see if we land on another ?
					if ( *( ++byte ) == '?' )
						byte++;

					bytes[counter] = 0;
					mask[counter] = 0;
					counter++;
				}
				else
				{
					// we have a valid byte to push back to our new pattern
					bytes[counter] = ( uint8_t ) ( strtoul( byte, &byte, 16 ) );
					mask[counter] = 1;
					counter++;
				}
			}

			return counter;
		};

		PIMAGE_DOS_HEADER dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( module );
		PIMAGE_NT_HEADERS nt_headers{ };

		if ( !dos_header || dos_header->e_magic != IMAGE_DOS_SIGNATURE )
			return 0;

		nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( ( uintptr_t ) dos_header + dos_header->e_lfanew );

		if ( !nt_headers || nt_headers->Signature != IMAGE_NT_SIGNATURE )
			return 0;

		const auto bin = reinterpret_cast< uint8_t* >( module );
		const uintptr_t start = region_start != std::numeric_limits< uintptr_t >::max( ) ? region_start : 0;
		const uintptr_t end = region_end > 0 ? region_end : nt_headers->OptionalHeader.SizeOfImage;

		// before u cry, check this
		// > pattern scan took: 312 ms
		// > pattern scan took: 5234 ms
		// also please reconsider life if u want to use a signature over 512 bytes
		static uint8_t bytes[512]{ };
		static uint8_t mask[512]{ };
		size_t size = get_pattern_fast( bytes, mask, pattern );

		//const auto bytes = get_pattern( pattern );

		for ( uintptr_t address = start; address < end - size; ++address )
		{
			bool found_pattern = true;

			for ( uintptr_t token = 0; token < size; ++token )
			{
				if ( bytes[token] != bin[address + token] && mask[token] == 1 )
				{
					// either invalid portion of signature or the current bytes don't match
					// break and invalidate return value for now
					found_pattern = false;
					break;
				}
			}

			if ( found_pattern )
			{
				// success, return address
				return ( t ) ( &bin[address] + offset );
			}
		}

	#ifdef TESTBUILD
		printf( xors( "OUTDATED SIG: %s \n" ), pattern );
	#endif

		// didn't find pattern
		return ( t ) 0;
	}
}