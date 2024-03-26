#include "ImGuiListener.h"

#include "ImGuiModule.h"

void ImGui::IImGuiListener::RegisterListener()
{
	FImGuiListenerHandler* handler = FImGuiModule::GetListenerHandler();
	if (handler == nullptr)
	{
		ensureAlways(false);
		return;
	}

	handler->RegisterListener(*this);
}

void ImGui::IImGuiListener::UnregisterListener()
{
	FImGuiListenerHandler* handler = FImGuiModule::GetListenerHandler();
	if (handler == nullptr)
	{
		ensureAlways(false);
		return;
	}

	handler->UnregisterListener(*this);
}
