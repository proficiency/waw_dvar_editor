#include "context.h"

decltype( hooked::o_endscene ) hooked::o_endscene;

long __stdcall hooked::endscene( void* device )
{
	// fill dvar list, setup imgui
	static bool init = false;
	if ( !init )
	{
		const u32 size = **( u32** ) ctx.m_patterns.m_dvar_arraysize;
		dvar_s** dvars = *( dvar_s*** ) ctx.m_patterns.m_dvar_array;
		for ( u32 i = 1; i < size; ++i )
		{
			dvar_s* dvar = dvars[i];

			if ( !dvar || !dvar->name || !dvar->description )
				continue;

			if ( fnvr( dvar->name ) == fnvc( "sv_cheats" ) )
				dvar->flags &= ~0x10;

			ctx.m_dvars.push_back( dvar );
		}

		ImGui::CreateContext( );
		ImGui::GetIO( );
		ImGui::GetIO( ).Fonts->AddFontFromFileTTF( xors( "C:\\Windows\\Fonts\\tahoma.ttf" ), 14 );
		{
			ImGuiStyle* style = &ImGui::GetStyle( );
			
			style->WindowPadding = ImVec2( 15, 15 );
			style->WindowRounding = 5.0f;
			style->FramePadding = ImVec2( 5, 5 );
			style->FrameRounding = 4.0f;
			style->ItemSpacing = ImVec2( 3, 8 );
			style->ItemInnerSpacing = ImVec2( 8, 6 );
			style->IndentSpacing = 25.0f;
			style->ScrollbarSize = 15.0f;
			style->ScrollbarRounding = 9.0f;
			style->GrabMinSize = 5.0f;
			style->GrabRounding = 3.0f;

			style->Colors[ImGuiCol_TitleBg] = ImVec4( 0.74f, 0.51f, 0.74f, 0.40f );
			style->Colors[ImGuiCol_TitleBgActive] = ImVec4( 0.74f, 0.51f, 0.74f, 0.40f );
			style->Colors[ImGuiCol_Text] = ImVec4( 0.75f, 0.74f, 0.74f, 1.00f );
			style->Colors[ImGuiCol_TextDisabled] = ImVec4( 0.43f, 0.42f, 0.42f, 1.00f );
			style->Colors[ImGuiCol_WindowBg] = ImVec4( 0.13f, 0.12f, 0.13f, 0.94f );
			style->Colors[ImGuiCol_ChildBg] = ImVec4( 0.20f, 0.19f, 0.19f, 0.00f );
			style->Colors[ImGuiCol_PopupBg] = ImVec4( 0.14f, 0.14f, 0.14f, 0.94f );
			style->Colors[ImGuiCol_Border] = ImVec4( 0.91f, 0.44f, 0.77f, 0.50f );
			style->Colors[ImGuiCol_BorderShadow] = ImVec4( 0.17f, 0.17f, 0.17f, 0.00f );
			style->Colors[ImGuiCol_FrameBg] = ImVec4( 0.14f, 0.11f, 0.15f, 0.54f );
			style->Colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.83f, 0.48f, 0.92f, 0.24f );
			style->Colors[ImGuiCol_FrameBgActive] = ImVec4( 0.90f, 0.59f, 0.98f, 0.67f );
			style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.36f, 0.26f, 0.48f, 0.51f );
			style->Colors[ImGuiCol_MenuBarBg] = ImVec4( 0.24f, 0.22f, 0.26f, 1.00f );
			style->Colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.12f, 0.11f, 0.11f, 0.53f );
			style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.31f, 0.22f, 0.38f, 1.00f );
			style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.48f, 0.33f, 0.54f, 1.00f );
			style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.74f, 0.46f, 0.77f, 1.00f );
			style->Colors[ImGuiCol_CheckMark] = ImVec4( 0.73f, 0.50f, 0.87f, 1.00f );
			style->Colors[ImGuiCol_SliderGrab] = ImVec4( 0.74f, 0.50f, 0.88f, 1.00f );
			style->Colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.73f, 0.48f, 0.81f, 1.00f );
			style->Colors[ImGuiCol_Button] = ImVec4( 0.74f, 0.51f, 0.74f, 0.40f );
			style->Colors[ImGuiCol_ButtonHovered] = ImVec4( 0.91f, 0.53f, 0.83f, 1.00f );
			style->Colors[ImGuiCol_ButtonActive] = ImVec4( 0.57f, 0.36f, 0.58f, 1.00f );
			style->Colors[ImGuiCol_Header] = ImVec4( 0.63f, 0.66f, 0.89f, 0.31f );
			style->Colors[ImGuiCol_HeaderHovered] = ImVec4( 0.77f, 0.60f, 0.75f, 0.80f );
			style->Colors[ImGuiCol_HeaderActive] = ImVec4( 0.72f, 0.59f, 0.72f, 1.00f );
			style->Colors[ImGuiCol_Separator] = ImVec4( 0.59f, 0.53f, 0.62f, 0.50f );
			style->Colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.64f, 0.50f, 0.66f, 0.78f );
			style->Colors[ImGuiCol_SeparatorActive] = ImVec4( 0.69f, 0.49f, 0.66f, 1.00f );
			style->Colors[ImGuiCol_ResizeGrip] = ImVec4( 0.26f, 0.59f, 0.99f, 0.00f );
			style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.26f, 0.59f, 0.98f, 0.00f );
			style->Colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.25f, 0.59f, 0.99f, 0.00f );
		}

		ImGui_ImplDX9_Init( FindWindow( xors( "CoD-WaW" ), 0 ), ( IDirect3DDevice9* ) device );

		//ImGui::StyleColorsDark( );
		ImGui::SetMouseCursor( ImGuiMouseCursor_Arrow );

		init = true;

		return hooked::o_endscene( device );
	}

	// haven't initiated yet, wait
	if ( !ctx.m_wndproc_ran )
		return hooked::o_endscene( device );

	ImGui_ImplDX9_NewFrame( );

	if ( ctx.m_menu_open )
	{
		static bool init2 = false;
		if ( !init2 )
		{
			ImGui::SetNextWindowPosCenter( );
			init2 = true;
		}

		ImGui::Begin( xors( "cod waw dvar editor" ), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize );

		// makes each dvar editable
		if ( ImGui::Button( xors( "make all writable" ) ) )
		{
			for ( u32 i = 0; i < ctx.m_dvars.size( ); ++i )
			{
				const auto dvar = ctx.m_dvars[i];

				if ( !dvar )
					continue;

				dvar->flags &= ~df_cheat;
				dvar->flags &= ~df_rom;
			}
		}

		// buttons to exec each config
		for ( u32 i = 0; i < ctx.m_files.size( ); ++i )
		{
			if ( ImGui::Button( ( xors( "exec " ) + ctx.m_files[i].substr( 3 ) ).c_str( ) ) )
			{
				std::ifstream  file( ctx.m_files[i].c_str( ) );
				std::string    line{};
				std::string    final_cmd{};

				while ( std::getline( file, line ) )
					final_cmd += line + ';';

				ctx.m_patterns.m_sendcmd( 0, 0, final_cmd.c_str( ) );
			}
		}

		static ImGuiTextFilter filter;
		filter.Draw( xors( "search" ) );
		if ( ImGui::ListBoxHeader( "", ctx.m_dvars.size( ) ) )
		{
			for ( u32 i = 0; i < ctx.m_dvars.size( ); i++ )
			{
				dvar_s* dvar = ctx.m_dvars[i];

				if ( !dvar )
					continue;

				const char* item_text = dvar->name;

				if ( !filter.PassFilter( dvar->name ) )
					continue;

				if ( ImGui::IsItemHovered( ) )
					ImGui::SetTooltip( dvar->description );

				ImGui::PushID( i );

				static bool open{};
				if ( ImGui::Selectable( dvar->name ) )
				{
					ImGui::OpenPopup( dvar->name );
					open = true;
				}

				if ( ImGui::BeginPopupModal( dvar->name, &open, ImGuiWindowFlags_AlwaysAutoResize ) )
				{
					char buffer[32];
					sprintf( buffer, xors( "0x%lx" ), ( u32 ) dvar );
					ImGui::Text( ( std::string( dvar->description ) + ", " + dvar->get_typename( ) + ", " + buffer ).c_str( ) );

					ImGui::Separator( );

					ImGui::Text( xors( "cheat" ) );
					ImGui::SameLine( );
					if ( ImGui::Button( ( std::string( xors( "make " ) ) + ( !( dvar->flags & df_cheat ) ? xors( "protected" ) : xors( "unprotected" ) ) + xors( "##cheat" ) ).c_str( ) ) )
						dvar->toggle_flag( df_cheat );

					ImGui::Text( xors( "rom" ) );
					ImGui::SameLine( );
					if ( ImGui::Button( ( std::string( xors( "make " ) ) + ( !( dvar->flags & df_rom ) ? xors( "protected" ) : xors( "unprotected" ) ) + xors( "##rom" ) ).c_str( ) ) )
						dvar->toggle_flag( df_rom );

					switch ( dvar->type )
					{
						case dvar_type::dvar_boolean:
						{
							ImGui::Text( xors( "current" ) );
							ImGui::SameLine( );
							if ( ImGui::Button( ( std::string( xors( "make " ) ) + ( dvar->current.enabled == 0 ? "true" : "false" ) + "##current" ).c_str( ) ) )
								dvar->current.enabled = ( dvar->current.enabled == 0 ) ? 1 : 0;

							ImGui::Text( xors( "latched" ) );
							ImGui::SameLine( );
							if ( ImGui::Button( ( std::string( xors( "make " ) ) + ( dvar->latched.enabled == 0 ? xors( "true" ) : xors( "false" ) ) + xors( "##latched" ) ).c_str( ) ) )
								dvar->latched.enabled = ( dvar->latched.enabled == 0 ) ? 1 : 0;

							ImGui::Text( xors( "default" ) );
							ImGui::SameLine( );
							if ( ImGui::Button( ( std::string( xors( "make " ) ) + ( dvar->default_value.enabled == 0 ? xors( "true" ) : xors( "false" ) ) + xors( "##default" ) ).c_str( ) ) )
								dvar->default_value.enabled = ( dvar->default_value.enabled == 0 ) ? 1 : 0;

							break;
						}

						case dvar_type::dvar_float:
						{
							ImGui::SliderFloat( xors( "value" ), &dvar->current.flt, dvar->get_limits( ).decimal.min, dvar->get_limits( ).decimal.max );

							ImGui::SameLine( );

							if ( ImGui::Button( xors( "reset##float" ) ) )
								dvar->current.flt = dvar->default_value.flt;
							break;
						}

						case dvar_type::dvar_int:
						{
							ImGui::SliderInt( xors( "value" ), &dvar->current.integer, dvar->get_limits( ).integer.min, dvar->get_limits( ).integer.max );
							if ( ImGui::Button( xors( "reset##int" ) ) )
								dvar->current.integer = dvar->default_value.integer;
							ImGui::SameLine( );
							if ( ImGui::Button( xors( "unlatch##int" ) ) )
								dvar->latched.integer = dvar->current.integer;
							break;
						}

						default:
						{
							if ( ImGui::Button( ( std::string( xors( "send " ) ) + dvar->name ).c_str( ) ) )
								ctx.m_patterns.m_sendcmd( 0, 0, dvar->name );
							break;
						}
					}

					ImGui::EndPopup( );
				}

				ImGui::PopID( );
			}
		}

		ImGui::ListBoxFooter( );

		ImGui::Separator( );
		{
			static char cmd_buffer[128] = {};
			ImGui::InputText( xors( "" ), cmd_buffer, 128 );

			ImGui::SameLine( );

			if ( ImGui::Button( xors( "send" ) ) )
			{
				ctx.m_patterns.m_sendcmd( 0, 0, cmd_buffer );
				memset( cmd_buffer, 0, 128 );
			}
		}

		ImGui::End( );
	}

	ImGui::EndFrame( );
	ImGui::Render( );

	return hooked::o_endscene( device );
}