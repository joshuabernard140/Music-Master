#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include <Python.h>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		DownloadWindow();
		MusicWindow();
		//ImGui::ShowDemoWindow();

		UI_DrawAboutModal();
	}

	void UI_DrawAboutModal()
	{
		if (!m_AboutModalOpen)
			return;

		ImGui::OpenPopup("About");
		m_AboutModalOpen = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_AboutModalOpen)
		{

			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::Text("Music Master Application");
			ImGui::Text("     by Joshua Bernard.");
			ImGui::EndGroup();

			if (Walnut::UI::ButtonCentered("Close"))
			{
				m_AboutModalOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}

	void audioDownload(const std::string& url) {
		std::string command = "python audioDownload.py " + url;
		system(command.c_str());
	}

	void DownloadWindow() {
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoDocking;

		ImGui::Begin("Music Downloader", nullptr, window_flags);

		ImGui::Text("Enter Youtube Music Link");
		ImGui::Spacing();

		ImGui::InputText("##YoutubeLink", inputLink, sizeof(inputLink));
		if (ImGui::Button("Download")) {
			url = inputLink;
			audioDownload(url);
		}

		ImGui::End();
	}

	void OpenFileWithNativePlayer(const std::string& filePath) {
		HINSTANCE result = ShellExecuteA(nullptr, "open", filePath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

		if (reinterpret_cast<int>(result) <= 32) {
			std::cerr << "Failed to open file: " << filePath << std::endl;
		}
	}

	void MusicWindow() {
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoDocking;

		ImGui::Begin("Music Library", nullptr, window_flags);

		fs::path folderPath = "downloads";

		if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
			for (const auto& entry : fs::directory_iterator(folderPath)) {
				if (entry.is_regular_file()) {
					std::string fileName = entry.path().filename().string();
					if (ImGui::Button((fileName).c_str())) {
						OpenFileWithNativePlayer(entry.path().string());
					}
				}
			}
		} else {
			std::cerr << "Folder not found." << std::endl;
		}

		ImGui::End();
	}

private:
	bool m_AboutModalOpen = false;
	char inputLink[256];
	std::string url;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Music Master";
	spec.CustomTitlebar = true;

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();
	app->PushLayer(exampleLayer);
	app->SetMenubarCallback([app, exampleLayer]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About"))
				{
					exampleLayer->ShowAboutModal();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}