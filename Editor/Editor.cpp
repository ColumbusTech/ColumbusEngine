#include <Editor/Editor.h>
#include <Lib/imgui/imgui.h>
#include <Editor/FileDialog.h>
#include <Editor/FontAwesome.h>
#include <Editor/CommonUI.h>
#include <Editor/Menu.h>
#include <Core/Filesystem.h>
#include <Editor/ResourcesViewerTexture.h>
#include <Editor/ResourcesViewerShader.h>
#include <Editor/ResourcesViewerMaterial.h>
#include <Editor/ResourcesViewerMesh.h>
#include <Editor/Extension.h>
#include <imgui.h>
#include <map>
#include <functional>

namespace Columbus::Editor
{
	EditorFileDialog SceneLoader("./Data/", {"scene"});
	EditorFileDialog SkyboxLoader("./Data/Skyboxes/", {"hdr", "exr"});

	std::map<std::string_view, MenuItem> MenuItems;
	std::vector<MenuItem> StaticMenuItems =
	{
		MenuItem("File", {},
		{
			{ "Open", MenuItem("Open", []() { SceneLoader.Open(); }, {}).Shortcut("Ctrl+O") },
			{ "New",  MenuItem("New").Shortcut("Ctrl+N") },
			{ "Save", MenuItem("Save").Shortcut("Ctrl+S") },
			{ "Save As", MenuItem("Save As").Shortcut("Ctrl+Shift+S") },
			{ "Quit", MenuItem("Quit") }
		}),
		MenuItem("View", {},
		{
			{ "Scene", MenuItem("Scene") },
			{ "Hierarchy", MenuItem("Hierarchy") },
			{ "Render Settings", MenuItem("Render Settings") },
			{ "Inspector", MenuItem("Inspector") },
			{ "Profiler", MenuItem("Profiler") },
			{ "Console", MenuItem("Console") },
			{ "Assets", MenuItem("Assets") },
			{ "Texture", MenuItem("Texture") }
		}),
		MenuItem("GameObject", {},
		{
			{ "Add Empty", MenuItem("Add Empty") }
		}),
		MenuItem("Resources", {},
		{
			{ "Textures",  MenuItem("Textures",  []() { ResourcesViewerTexture::Open(nullptr); }, {}) },
			{ "Shaders",   MenuItem("Shaders",   []() { ResourcesViewerShader::Open(nullptr); }, {}) },
			{ "Materials", MenuItem("Materials", []() { ResourcesViewerMaterial::Open(nullptr); }, {}) },
			{ "Meshes",    MenuItem("Meshes",    []() { ResourcesViewerMesh::Open(nullptr); }, {}) }
		}),
		MenuItem("Scene", {},
		{
			{ "Skybox", MenuItem("Skybox", []() { SkyboxLoader.Open(); }, {}) }
		})
	};
	
	static void InitializeMenuItems()
	{
		MenuItems.clear();

		for (auto& ext : PrototypeFactory<Extension>::Instance().GetBuilders())
		{
			std::vector<std::string> path = Filesystem::Split(std::string(ext.second->GetPath()));
			if (path.size() < 2)
			{
				Log::Error("Extension path length is too short: %s", ext.first.data());
				continue;
			}

			if (MenuItems.find(path[0]) == MenuItems.end())
				MenuItems[path[0]] = { path[0] };

			MenuItem* current = &MenuItems[path[0]];
			for (int i = 1; i < path.size(); i++)
			{
				current = &current->FindOrCreate(path[i]);
			}

			current->onExecute = [ext] () { ext.second->Execute(); };
		}
	}

	void Editor::DrawMainMenu(Scene& scene)
	{
		if (ImGui::BeginMainMenuBar())
		{
			StaticMenuItems[0]["New"].onExecute = [&scene]() { scene.Clear(); };
			StaticMenuItems[0]["Save"].onExecute = [&scene]() {
				if (scene.GetCurrentSceneFilename().empty())
					SceneLoader.Open(EditorFileDialog::Type_Save);
				else
					scene.Save();
			};
			StaticMenuItems[0]["Save As"].onExecute = []() { SceneLoader.Open(EditorFileDialog::Type_Save); };
			StaticMenuItems[1]["Scene"].selected = &panelScene.Opened;
			StaticMenuItems[1]["Hierarchy"].selected = &panelHierarchy.Opened;
			StaticMenuItems[1]["Render Settings"].selected = &panelRenderSettings.Opened;
			StaticMenuItems[1]["Inspector"].selected = &panelInspector.Opened;
			StaticMenuItems[1]["Profiler"].selected = &panelProfiler.Opened;
			StaticMenuItems[1]["Console"].selected = &panelConsole.Opened;
			StaticMenuItems[1]["Assets"].selected = &panelAssets.Opened;
			StaticMenuItems[1]["Texture"].selected = &panelTexture->Opened;
			StaticMenuItems[2]["Add Empty"].onExecute = [&scene]() { scene.AddEmpty(); };

			for (auto& item : StaticMenuItems)
			{
				item.Draw();
			}

			for (auto& item : MenuItems)
			{
				item.second.Draw();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void Editor::DrawDockSpace(Scene& scene)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		if (ImGui::Begin("DockSpace", nullptr, window_flags))
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			ImGui::PopStyleVar(3);
			DrawMainMenu(scene);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		}
		ImGui::End();

		ImGui::PopStyleVar(3);
	}

	Editor::Editor()
	{
		InitializeMenuItems();
		ApplyDarkTheme();

		ImFontConfig lat, cyr, icons;
		cyr.MergeMode = true;
		icons.MergeMode = true;

		const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("./Data/A.ttf", 14.0f, &lat);
		io.Fonts->AddFontFromFileTTF("./Data/A.ttf", 14.0f, &cyr, io.Fonts->GetGlyphRangesCyrillic());
		io.Fonts->AddFontFromFileTTF("./Data/FontAwesome5.ttf", 12.0f, &icons, icons_ranges);
		io.Fonts->Build();
	}

	void Editor::Draw(Scene& scene, Renderer& Render, iVector2& Size, float RedrawTime)
	{
		DrawDockSpace(scene);

		panelScene.SetFramebufferTexture(Render.GetFramebufferTexture());
		panelHierarchy.SetScene(&scene);
		Size = panelScene.GetSize();
		panelRenderSettings.SetRenderer(&Render);
		panelInspector.SetInspectableObject(panelHierarchy.GetCurrentObject());
		panelScene.SetPickedObject(panelHierarchy.GetCurrentObject());
		panelProfiler.SetRedrawTime(RedrawTime);
		panelAssets.SetTexturePreview(panelTexture);

		panelScene.Draw(scene, Render);
		panelInspector.Draw(scene); // Inspector should be before hierarchy
		panelHierarchy.Draw(); // because in hierarchy there are deleting objects
		panelRenderSettings.Draw();
		panelProfiler.Draw();
		panelConsole.Draw();
		panelAssets.Draw();
		panelTexture->Draw();

		ResourcesViewerTexture::Draw(&scene);
		ResourcesViewerShader::Draw(&scene);
		ResourcesViewerMaterial::Draw(scene);
		ResourcesViewerMesh::Draw(&scene);

		if (SkyboxLoader.Draw("Load Skybox"))
		{
			SmartPointer<Texture> Tex(gDevice->CreateTexture());
			auto Selected = SkyboxLoader.GetSelected();
			if (Selected.size() == 1)
			{
				if (Tex->Load(Selected[0].Path.c_str()))
				{
					delete scene.Sky;
					auto Name = Filesystem::RelativePath(Selected[0].Path, Filesystem::GetCurrent());
					scene.SkyPath = Name;
					scene.Sky = new Skybox(Tex.Get());
					Log::Success("Skybox loaded: %s", Name.c_str());
				}

				SkyboxLoader.Close();
			}
		}

		if (SceneLoader.Draw("Load Scene"))
		{
			auto Selected = SceneLoader.GetSelected();
			if (Selected.size() == 1)
			{
				if (SceneLoader.GetType() == EditorFileDialog::Type_Open)
					scene.Load(Selected[0].Path.c_str());

				if (SceneLoader.GetType() == EditorFileDialog::Type_Save)
					scene.Save(Selected[0].Path.c_str());
			}
			SceneLoader.Close();
		}
	}

	Editor::~Editor() {}

}
