#pragma once

#include "ImGui/Private/Utilities/TypeHelper.h"

namespace ImGui
{
	class IMGUI_API IImGuiListener
	{
	public:
		virtual void Draw() = 0;
		virtual FString GetName() const = 0;
		virtual FString GetCategory() const = 0;

	protected:
		void RegisterListener();
		void UnregisterListener();

	};

	template <typename CRTP>
	class TImGuiListener : public IImGuiListener
	{
	public:
		void Draw() override
		{
			TTypeHelper<CRTP>::Draw(static_cast<CRTP&>(*this));
		}

		FString GetName() const override
		{
			return TTypeHelper<CRTP>::GetName(static_cast<const CRTP&>(*this));
		}

		FString GetCategory() const override { return FString{ TEXT("Other") }; }
	};
};