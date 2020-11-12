#include <Editor/PanelScene.h>
#include <Editor/FontAwesome.h>
#include <Editor/CommonUI.h>
#include <Lib/imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <Graphics/OpenGL/TextureOpenGL.h>

namespace Columbus::Editor
{

	PanelScene::PanelScene() :
		Panel(ICON_FA_GLOBE" Scene")
	{
		SetMenuBar(true);
		SetPadding(false);
	}

	void PanelScene::DrawInternal()
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			FlagButton(ICON_FA_IMAGE, _Renderer->DrawIcons, "Icons");
			FlagButton(ICON_FA_BORDER_ALL, _Renderer->DrawGrid, "Grid");
			FlagButton(ICON_FA_DICE_D6, _Gizmo.Enable, "Gizmo");

			ImGui::Dummy({ 10,0 });

			ToolButton(ICON_FA_ARROWS_ALT, (int*)&_Gizmo._Operation, Gizmo::Operation::Translate, {}, "Translate");
			ToolButton(ICON_FA_SYNC_ALT, (int*)&_Gizmo._Operation, Gizmo::Operation::Rotate, {}, "Rotate");
			ToolButton(ICON_FA_EXTERNAL_LINK_ALT, (int*)&_Gizmo._Operation, Gizmo::Operation::Scale, {}, "Scale");

			ImGui::Dummy({ 10,0 });

			FlagButton(_Scene->EnablePhysicsSimulation ? ICON_FA_PAUSE : ICON_FA_PLAY, _Scene->EnablePhysicsSimulation);
			ShowTooltipDelayed(CommonUISettings.TooltipDelay, _Scene->EnablePhysicsSimulation ? "Disable physics simulation" : "Enable physics simulation");
			ImGui::PopStyleVar();

			ImGui::EndMenuBar();
		}

		Selected = ImGui::IsWindowFocused();

		Position = iVector2((int)ImGui::GetCursorScreenPos().x, (int)ImGui::GetCursorScreenPos().y);

		if (FramebufferTexture != nullptr)
		{
			auto Tex = ((TextureOpenGL*)FramebufferTexture)->GetID();
			auto Cursor = ImGui::GetCursorPos();
			auto Size = ImGui::GetWindowSize();
			Size.y -= Cursor.y;
			ImGui::Image((void*)(intptr_t)Tex, ImVec2(Size.x, Size.y), ImVec2(0, 1), ImVec2(1, 0));
			Hover = ImGui::IsItemHovered();
			SizeOfRenderWindow = { (int)Size.x, (int)Size.y };

			if (_PickedObject != nullptr)
			if (_PickedObject != nullptr)
			{
				Vector4 rect(Position.X, Position.Y, Size.x, Size.y);
				_Gizmo.SetCamera(_Renderer->GetMainCamera());
				_Gizmo.Draw(_PickedObject->transform, rect);
			}
		}
	}

}
