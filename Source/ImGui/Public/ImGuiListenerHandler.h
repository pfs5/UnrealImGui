#pragma once

#include "Tickable.h"

namespace ImGui
{
	class IImGuiListener;
}

class FImGuiListenerHandler : public FTickableGameObject
{
public:
	void Init();
	void Shutdown();

	void Tick(float deltaSeconds) final; 
	bool IsTickable() const final { return _initialized; }
	bool IsTickableInEditor() const final;
	TStatId GetStatId() const final;

	void RegisterListener(ImGui::IImGuiListener& listener);
	void UnregisterListener(ImGui::IImGuiListener& listener);

private:
	struct FListenerData
	{
		FListenerData(ImGui::IImGuiListener& listener);

		ImGui::IImGuiListener* Listener;
		FString Name;
		bool IsOpen = false;

		friend bool operator==(const FListenerData& lhs, const FListenerData& rhs) { return lhs.Listener == rhs.Listener; }
		friend bool operator!=(const FListenerData& lhs, const FListenerData& rhs) { return lhs.Listener != rhs.Listener; }
	};

	struct FWindowData
	{
		FString Title;
		uint32 TitleHash;

		FWindowData(const FString& title);

	};

private:
	uint64 _lastTickedFrameNumber = INDEX_NONE;

	TArray<FListenerData> _listeners;
	TArray<FWindowData> _cachedWindows;

#if WITH_EDITOR
	FDelegateHandle _endPIEDelegateHandle;
#endif //WITH_EDITOR

	bool _initialized = false;
	bool _sortListeners = true;
	bool _cacheWindows = true;

private:
#if !UE_BUILD_SHIPPING
	void Draw();
#endif //!UE_BUILD_SHIPPING

	void OnEndPIE(const bool isSimulating);
	void BuildWindowCache();
};