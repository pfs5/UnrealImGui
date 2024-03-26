#pragma once

class AActor;
class UObject;

namespace ImGui
{
	// Generic implementation.
	template <typename T, typename Enable = void>
	struct TTypeHelper
	{
		static void Draw(T& value) {}
		static FString GetName(const T& value) { return FString{ TEXT("N/A") }; }
	};

	// UObject specialization.
	template<>
	struct IMGUI_API TTypeHelper<UObject>
	{
		static void Draw(UObject& value);
		static FString GetName(const UObject& value);
	};


	// AActor specialization.
	template<>
	struct IMGUI_API TTypeHelper<AActor>
	{
		static void Draw(AActor& value);
		static FString GetName(const AActor& value);
	};

	// UObject hierarchy specialization.
	template<typename T>
	struct TTypeHelper<T, 
		typename std::enable_if<
			std::is_base_of_v<UObject, T> &&
			!std::is_base_of_v<AActor, T>	// AActor inherits UObject so we need to handle this since we want a specialization for AActor as well.
		>::type
	>
	{
		static void Draw(T& value) { TTypeHelper<UObject>::Draw(value); }
		static FString GetName(const T& value) { return TTypeHelper<UObject>::GetName(value); }
	};

	// AActor hierarchy specialization.
	template<typename T>
	struct TTypeHelper<T, 
		typename std::enable_if<
			std::is_base_of_v<AActor, T>
		>::type
	>
	{
		static void Draw(T& value) { TTypeHelper<AActor>::Draw(value); }
		static FString GetName(const T& value) { return TTypeHelper<AActor>::GetName(value); }
	};

};