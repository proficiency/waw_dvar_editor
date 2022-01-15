#pragma once
#define fnvr( str ) ( fnv32::hash( str ) )
#define fnvc( str ) ( [&]( ){ constexpr u32 out = fnv32::hash( str ); return out; }( ) )

inline constexpr u32 strlen_ct( const char* str )
{
	u32 out = 1;

	for ( ; str[out] != '\0'; ++out );

	return out;
}

namespace fnv32
{
	constexpr u32 fnv32_prime = 0x1000193;
	constexpr u32 fnv32_basis = 0x811c9dc5;

	inline constexpr u32 hash( const char* str )
	{
		const u32 len = strlen_ct( str );

		u32 out = fnv32_basis;
		for ( u32 i = 0; i < len; ++i )
			out = ( out ^ str[i] ) * fnv32_prime;

		return out;
	}
}