#pragma once

#include <Core/Types.h>
#include <Scene/Scene.h>
#include <Editor/FileDialog.h>
#include <Core/Platform/PlatformFilesystem.h>
#include <Lib/imgui/imgui.h>
#include <Lib/imgui/misc/cpp/imgui_stdlib.h>
#include <functional>
#include <string>

namespace Columbus
{

	static void ResourceViewerDrawLoadMore(const char* Name, std::function<void()> LoadMore);

	template <typename T>
	static void ResourceViewerDrawSelectable(const char* Name, T* Object, T*& Tmp, uint32& Width,
		std::function<bool(const char*, T*)> Button, std::function<void()> DoubleClick);

	template <typename T>
	static void ResourceViewerDrawList(const char* Name, T*& Tmp,
		const ResourceManager<T>& Manager, const std::string& Find,
		std::function<bool(const char*, T*)> Button, std::function<void()> DoubleClick);

	static void ResourceViewerDrawButtons(const char* Name, const void* Dst, std::string& Find, std::function<void()> Close, bool& Opened);

	template <typename T>
	static void ResourceViewerLoadNew(T* New, ResourceManager<T>& Manager, const char* Path, bool Force,
		std::function<bool(const char*, T*)> Load, std::function<void(const char*)> Success,
		std::function<void()> Failure);

	template <typename T>
	static void ResourceViewerLoad(const char* Name, EditorFileDialog& Loader,
		ResourceManager<T>& Manager, MessageBox& BruteLoader,
		std::function<bool(const char*, T*)> Load, std::function<void(const char*)> Success,
		std::function<void()> Failure, std::function<T*()> New);





	void ResourceViewerDrawLoadMore(const char* Name, std::function<void()> LoadMore)
	{
		if (ImGui::BeginChild(Name, ImVec2(ImGui::GetWindowContentRegionWidth(), 30)))
		{
			if (ImGui::Button((std::string("Load More##") + Name).c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 30)))
			{
				LoadMore();
			}
		}
		ImGui::EndChild();
	}

	template <typename T>
	void ResourceViewerDrawSelectable(const char* Name, T* Object, T*& Tmp, uint32& Width,
		std::function<bool(const char*, T*)> Button, std::function<void()> DoubleClick)
	{
		bool Pushed = false;

		// Set color to yellow if this object is selected
		if (Object == Tmp)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.73f, 0.60f, 0.15f, 1.00f));
			Pushed = true;
		}

		if (Button(Name, Object))
			Tmp = Object;

		// Draw a tooltip with the name of object
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(Name);

			if (ImGui::IsMouseDoubleClicked(0))
				DoubleClick();
		}

		// Set color to normal if this object was selected
		if (Pushed) ImGui::PopStyleColor();

		// Texture buttons packing
		if (Width >= 120)
		{
			Width -= 120;
			ImGui::SameLine();
		} else {
			Width = ImGui::GetWindowContentRegionWidth();
		}
	}

	template <typename T>
	void ResourceViewerDrawList(const char* Name, T*& Tmp,
		const ResourceManager<T>& Manager, const std::string& Find,
		std::function<bool(const char*, T*)> Button, std::function<void()> DoubleClick)
	{
		if (ImGui::BeginChild(Name, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30)))
		{
			uint32 Width = ImGui::GetWindowContentRegionWidth();

			ResourceViewerDrawSelectable<T>("None", nullptr, Tmp, Width, Button, DoubleClick);

			std::string MFind = Find;
			std::string MName;

			std::transform(MFind.begin(), MFind.end(), MFind.begin(), ::tolower);

			for (const auto& Elem : Manager.ResourcesMap)
			{
				MName = Elem.first;
				std::transform(MName.begin(), MName.end(), MName.begin(), ::tolower);
				
				if (MName.find(MFind) != std::string::npos)
				{
					T* Object = Manager.Resources[Elem.second].Get();
					ResourceViewerDrawSelectable<T>(Elem.first.c_str(), Object, Tmp, Width, Button, DoubleClick);
				}
			}
		}
		ImGui::EndChild();
	}

	void ResourceViewerDrawButtons(const char* Name, const void* Dst, std::string& Find, std::function<void()> Close, bool& Opened)
	{
		if (ImGui::BeginChild(Name))
		{
			if (Dst != nullptr)
			{
				if (ImGui::Button("Cancel")) Opened = false;
				ImGui::SameLine();
				if (ImGui::Button("Ok")) Close();
				ImGui::SameLine();
			}

			ImGui::InputText("Find##ResourceViewerButtons", &Find);
		}
		ImGui::EndChild();
	}

	template <typename T>
	static void ResourceViewerLoadNew(T* New, ResourceManager<T>& Manager, const char* Path, bool Force,
		std::function<bool(const char*, T*)> Load, std::function<void(const char*)> Success,
		std::function<void()> Failure)
	{
		auto CodeName = Filesystem::RelativePath(Path, Filesystem::GetCurrent());

		if (Manager.IsNameFree(CodeName) || Force)
		{
			SmartPointer<T> Object(New);
			if (Load(Path, New))
			{
				Manager.Add(std::move(Object), CodeName, Force);
				Success(CodeName.c_str());
			}
		} else
		{
			Failure();
		}
	}

	template <typename T>
	void ResourceViewerLoad(const char* Name, EditorFileDialog& Loader,
		ResourceManager<T>& Manager, MessageBox& BruteLoader,
		std::function<bool(const char*, T*)> Load, std::function<void(const char*)> Success,
		std::function<void()> Failure, std::function<T*()> New)
	{
		bool Force = BruteLoader.GetSelected() == 3;
		if (BruteLoader.GetSelected() != 0) BruteLoader.Reset();

		if (Loader.Draw(Name) || Force)
		{
			auto Selected = Loader.GetSelected();
			if (Selected.size() >= 1)
			{
				for (const auto& Elem : Selected)
				{
					ResourceViewerLoadNew<T>(New(), Manager, Elem.Path.c_str(), Force, Load, Success, Failure);
				}
			}
		}
	}

}


