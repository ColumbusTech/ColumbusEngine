#include <Editor/PanelScene.h>
#include <Lib/imgui/imgui.h>
#include <Graphics/OpenGL/TextureOpenGL.h>

#include <cstdio>

namespace Columbus
{

	void EditorPanelScene::Draw()
	{
		if (Opened)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			if (ImGui::Begin(Name.c_str(), &Opened, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::Checkbox("Stats", &Stats);
					ImGui::EndMenuBar();
				}

				Selected = ImGui::IsWindowFocused();
				Hover = ImGui::IsWindowHovered();

				Position = iVector2((int)ImGui::GetCursorScreenPos().x, (int)ImGui::GetCursorScreenPos().y);

				auto Tex = ((TextureOpenGL*)FramebufferTexture)->GetID();
				auto Cursor = ImGui::GetCursorPos();
				auto Size = ImGui::GetWindowSize();
				Size.y -= Cursor.y;
				ImGui::Image((void*)(intptr_t)Tex, ImVec2(Size.x, Size.y), ImVec2(0, 1), ImVec2(1, 0));
				SizeOfRenderWindow = { (int)Size.x, (int)Size.y };
			}
			ImGui::End();
			ImGui::PopStyleVar(1);
		}
	}

}

