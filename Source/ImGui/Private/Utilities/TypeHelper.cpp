#include "ImGui/Private/Utilities/TypeHelper.h"

#include "GameFramework/Actor.h"
#include "imgui.h"
#include "UObject/Object.h"

void ImGui::TTypeHelper<UObject>::Draw(UObject& value)
{
	if (ImGui::CollapsingHeader("UObject"))
	{
		ImGui::Indent();

		const FString className = value.GetClass()->GetName();
		ImGui::Text("Class: %s", TCHAR_TO_UTF8(*className));

		ImGui::Unindent();
	}
}

FString ImGui::TTypeHelper<UObject>::GetName(const UObject& value)
{
	return value.GetFName().ToString();
}

void ImGui::TTypeHelper<AActor>::Draw(AActor& value)
{
	ImGui::TTypeHelper<UObject>::Draw(value);

	if (ImGui::CollapsingHeader("AActor"))
	{
		ImGui::Indent();

		// Transform
		if (ImGui::TreeNodeEx("Transform:"))
		{
			const FVector location = value.GetActorLocation();
			const FRotator rotation = value.GetActorRotation();
			const FVector scale = value.GetActorScale();

			if (ImGui::BeginTable("##TransformTable", 2, ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableNextColumn();

				ImGui::Text("Location: ");
				ImGui::TableNextColumn();
				ImGui::Text("%s", TCHAR_TO_UTF8(*location.ToString()));
				ImGui::TableNextColumn();

				ImGui::Text("Rotation: ");
				ImGui::TableNextColumn();
				ImGui::Text("%s", TCHAR_TO_UTF8(*rotation.ToString()));
				ImGui::TableNextColumn();

				ImGui::Text("Scale: ");
				ImGui::TableNextColumn();
				ImGui::Text("%s", TCHAR_TO_UTF8(*scale.ToString()));
				ImGui::TableNextColumn();

				ImGui::EndTable();
			}

			ImGui::TreePop();
		}

		ImGui::Unindent();
	}
}

FString ImGui::TTypeHelper<AActor>::GetName(const AActor& value)
{
	return FString{ value.GetActorNameOrLabel() };
}
