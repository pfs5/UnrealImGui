#pragma once

namespace ImGuiHelpers
{
	// Functionality is present in imgui_internal so we create this wrapper function to avoid having a direct dependency.
	bool WindowExists(const char* winName);
	
};
