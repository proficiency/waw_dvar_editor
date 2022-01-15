#pragma once

enum dvar_flag : uint16_t
{
	df_rom    = 0x40,
	df_cheat  = 0x80,
};

enum class dvar_type : uint8_t
{
	dvar_boolean = 0,
	dvar_float,
	dvar_2d_vector,
	dvar_3d_vector,
	dvar_4d_vector,
	dvar_int,
	dvar_select,
	dvar_text,
	dvar_color,
};

#pragma pack(push, 1)
union dvar_value
{
	uint8_t enabled;
	int32_t integer;
	uint32_t uint;
	float flt;
	float vector[4];
	const char* string;
	uint8_t color[4];
};

union dvar_limits
{
	struct
	{
		int string_count;
		char** strings;
	} enumeration;

	struct
	{
		int min;
		int max;
	} integer;

	struct
	{
		float min;
		float max;
	} decimal;
};

struct dvar_s
{
	const char* name;         // 0     
	const char* description;     // 4  
	u16 flags;               // 6
	dvar_type type; // 7
	bool modified; // 8 
	u8 pad[4]; // 9
	dvar_value current; // 0xd
	dvar_value latched; // 0x1D
	dvar_value default_value; // 0x2D     

	bool toggle_flag( const uint16_t flag )
	{
		if ( this->flags & flag )
			this->flags &= ~flag;
		else
			this->flags |= flag;
		return this->flags & flag;
	}
	
	dvar_limits get_limits( )
	{
		return *( dvar_limits* ) ( ( u32 ) this + 0x50 );
	}

	const char* get_typename( )
	{
		switch ( ( i32 ) type )
		{
		case 0:
			return xors( "bool" );
		case 1:
			return xors( "float" );
		case 2:
			return xors( "vec2" );
		case 3:
			return xors( "vec3" );
		case 4:
			return xors( "vec4" );
		case 5:
			return xors( "int" );
		case 6:
			return xors( "select" );
		case 7:
			return xors( "string" );
		case 8:
			return xors( "color" );
		}
		
		return "";
	}
};
#pragma pack(pop)