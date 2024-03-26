#include "ImGuiListenerHandler.h"

#include "ImGuiListener.h"
#include "Templates/TypeHash.h"

FImGuiListenerHandler::FListenerData::FListenerData(ImGui::IImGuiListener& listener) :
	Listener{ &listener },
	Name{ listener.GetName() }
{

}

FImGuiListenerHandler::FWindowData::FWindowData(const FString& title) :
	Title{ title },
	TitleHash{ GetTypeHashHelper<FString>(title) }
{

}

void FImGuiListenerHandler::Init()
{
	ensure(!_initialized);

	_listeners.Empty();

#if WITH_EDITOR
	_endPIEDelegateHandle = FEditorDelegates::EndPIE.AddRaw(this, &FImGuiListenerHandler::OnEndPIE);
#endif //WITH_EDITOR

	_initialized = true;
}

void FImGuiListenerHandler::Shutdown()
{
	_initialized = false;

#if WITH_EDITOR
	FEditorDelegates::EndPIE.Remove(_endPIEDelegateHandle);
#endif //WITH_EDITOR
}

void FImGuiListenerHandler::Tick(float deltaSeconds)
{
	if (_lastTickedFrameNumber == GFrameCounter)
	{
		return;
	}

	_lastTickedFrameNumber = GFrameCounter;

	Draw();
}

bool FImGuiListenerHandler::IsTickableInEditor() const
{
	return false;
}

TStatId FImGuiListenerHandler::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FImGuiListenerHandler, STATGROUP_Tickables);
}

void FImGuiListenerHandler::RegisterListener(ImGui::IImGuiListener& listener)
{
	auto index = _listeners.AddUnique(listener);
	if (_listeners.IsValidIndex(index))
	{
		FListenerData& data = _listeners[index];

		// Use hash to avoid string compares.
		const uint32 nameHash = GetTypeHashHelper<FString>(data.Name);
		if (_cachedWindows.FindByPredicate([nameHash](const FWindowData& window) { return window.TitleHash == nameHash; }))
		{
			data.IsOpen = true;
		}
	}
}

void FImGuiListenerHandler::UnregisterListener(ImGui::IImGuiListener& listener)
{
	_listeners.Remove(listener);
}

void FImGuiListenerHandler::Draw()
{
	// Draw menu

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.f, 10.f});

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Listeners"))
		{
			static TArray<FListenerData*> listeners;
			listeners.Empty();

			for (FListenerData& listener : _listeners)
			{
				ensure(listener.Listener != nullptr);
				listeners.Add(&listener);
			}

			if (_sortListeners)
			{
				listeners.Sort([](const FListenerData& lhs, const FListenerData& rhs) { return lhs.Name < rhs.Name; });
			}

			for (FListenerData* listener : listeners)
			{
				ImGui::MenuItem(TCHAR_TO_UTF8(*listener->Name), nullptr, &listener->IsOpen);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Sort listeners", nullptr, &_sortListeners);
			ImGui::MenuItem("Keep windows", nullptr, &_cacheWindows);

			ImGui::EndMenu();
		}

		ImGui::Text(" | ");
		ImGui::Text("[ Press ALT + T for mouse control ]");

		ImGui::EndMainMenuBar();
	}

	ImGui::PopStyleVar();

	// Draw listeners
	for (FListenerData& listener : _listeners)
	{
		if (listener.IsOpen)
		{
			ImGui::Begin(TCHAR_TO_UTF8(*listener.Name));
			listener.Listener->Draw();
			ImGui::End();
		}
	}
}

void FImGuiListenerHandler::OnEndPIE(const bool isSimulating)
{
	BuildWindowCache();
}

void FImGuiListenerHandler::BuildWindowCache()
{
	if (!_cacheWindows)
	{
		_cachedWindows.Empty();
		return;
	}

	for (const FListenerData& listener : _listeners)
	{
		if (!listener.IsOpen)
		{
			continue;
		}

		// Cache window data
		if (const ImGuiWindow* window = ImGui::FindWindowByName(TCHAR_TO_UTF8(*listener.Name)))
		{
			_cachedWindows.Emplace(listener.Name);
		}
	}
}
