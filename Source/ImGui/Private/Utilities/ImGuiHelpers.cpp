#include "ImGuiHelpers.h"

#include <imgui_internal.h>

bool ImGuiHelpers::WindowExists(const char* winName)
{
	return ImGui::FindWindowByName(winName) != nullptr;
}
