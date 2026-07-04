#include "Menu.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

Menu::Menu()
{
}

Menu::~Menu()
{

}

void Menu::Render()
{
	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
	{
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
		ImGuiID dock_id_left = 0;
		ImGuiID dock_id_main = dockspace_id;
		ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.2f, &dock_id_left, &dock_id_main);

		ImGui::DockBuilderDockWindow("Main", dock_id_main);
		ImGui::DockBuilderDockWindow("Left", dock_id_left);
		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Main");
	ImGui::End();

	ImGui::Begin("Left");
	ImGui::End();
}