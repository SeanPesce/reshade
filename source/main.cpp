/**
 * Copyright (C) 2014 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/reshade#license
 */

#include "log.hpp"
#include "filesystem.hpp"
#include "input.hpp"
#include "runtime.hpp"
#include "hook_manager.hpp"
#include "version.h"
#include <Windows.h>


HMODULE g_module_handle = nullptr;
std::vector<std::string> g_chained_dlls; // Holds names of wrapper DLLs to load in place of real DLLs


BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(lpvReserved);

	using namespace reshade;

	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			g_module_handle = hModule;
			runtime::s_reshade_dll_path = filesystem::get_module_path(hModule);
			runtime::s_target_executable_path = filesystem::get_module_path(nullptr);
			const filesystem::path system_path = filesystem::get_special_folder_path(filesystem::special_folder::system);

			log::open(filesystem::path(runtime::s_reshade_dll_path).replace_extension(".log"));

#ifdef WIN64
#define VERSION_PLATFORM "64-bit"
#else
#define VERSION_PLATFORM "32-bit"
#endif
			LOG(INFO) << "Initializing crosire's ReShade version '" VERSION_STRING_FILE "' (" << VERSION_PLATFORM << ") built on '" VERSION_DATE " " VERSION_TIME "' loaded from " << runtime::s_reshade_dll_path << " to " << runtime::s_target_executable_path << " ...";
			//LOG(INFO) << "Initializing crosire's ReShade version '" VERSION_STRING_FILE "' (" << VERSION_PLATFORM << "), re-built by Sean Pesce with compatibility for DLL wrapper chaining on '" VERSION_DATE " " VERSION_TIME "' loaded from " << runtime::s_reshade_dll_path << " to " << runtime::s_target_executable_path << " ...";


			// Check for chained wrapper DLL definitions in config file:

			/*
				This is a quick & sloppy implementation that assumes each wrapper DLL specified in the configuration is in the immediate working directory.
				It's mostly a proof of concept so that someone can implement it properly in the future.

				@TODO: Check whether each entry specifies a relative or absolute filepath, and handle them accordingly.
				@TODO: Turn repetitive code into functions
			*/

			// d3d9.dll wrapper
			const filesystem::path current_dir = filesystem::get_module_path(NULL).remove_filename(); // Working directory
			const auto cfg_filename = filesystem::get_module_path(g_module_handle).filename_without_extension() + ".ini"; // Configuration file
			char dll_chain_buffer[MAX_PATH + 1]; // Buffer for filename of wrapper DLL to be loaded
			dll_chain_buffer[0] = '\0'; // Clear buffer
			GetPrivateProfileStringA("Wrapper DLLs", "d3d9Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "d3d9.dll");
				
			// d3d10.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "d3d10Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "d3d10.dll");

			// d3d10_1.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "d3d10_1Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "d3d10_1.dll");

			// d3d11.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "d3d11Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "d3d11.dll");

			// dxgi.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "dxgiChain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "dxgi.dll");

			// opengl32.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "opengl32Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "opengl32.dll");

			// user32.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "user32Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "user32.dll");

			// ws2_32.dll wrapper
			dll_chain_buffer[0] = '\0';
			GetPrivateProfileStringA("Wrapper DLLs", "ws2_32Chain", NULL, dll_chain_buffer, MAX_PATH, std::string(".\\").append(cfg_filename.string()).c_str());
			if (dll_chain_buffer[0] != '\0')
			{
				g_chained_dlls.push_back(dll_chain_buffer);
				hooks::register_module(current_dir / (const char*)dll_chain_buffer);
			}
			else
				hooks::register_module(system_path / "ws2_32.dll");

			LOG(INFO) << "Initialized.";
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			LOG(INFO) << "Exiting ...";

			input::uninstall();
			hooks::uninstall();

			LOG(INFO) << "Exited.";
			break;
		}
	}

	return TRUE;
}
