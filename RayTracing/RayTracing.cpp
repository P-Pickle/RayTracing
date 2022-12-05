#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "glm/glm.hpp"
#include "Renderer.h"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
private: 
	Renderer m_Renderer;
	float m_LastRenderTime = 0.0f;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	glm::vec3 m_SphereColor;
	glm::vec3 m_LightDirection;
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::Text("Sphere Color:");
		static float r = 0.0f;
		static float g = 1.0f;
		static float b = 1.0f;
		ImGui::InputFloat("r", &r, 0.01f, 1.0f, "%.3f");
		ImGui::InputFloat("g", &g, 0.01f, 1.0f, "%.3f");
		ImGui::InputFloat("b", &b, 0.01f, 1.0f, "%.3f");

		ImGui::Text("Light Direction:");
		static float x = -1.0f;
		static float y = -1.0f;
		static float z = -1.0f;
		ImGui::InputFloat("x", &x, 0.01f, 1.0f, "%.3f");
		ImGui::InputFloat("y", &y, 0.01f, 1.0f, "%.3f");
		ImGui::InputFloat("z", &z, 0.01f, 1.0f, "%.3f");

		m_SphereColor = glm::vec3(r, g, b);
		m_LightDirection = glm::vec3(x, y, z);

		ImGui::End();



		
		/*glm::vec3 SphereColor = Color;*/
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0,1), ImVec2(1,0));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_SphereColor , m_LightDirection);

		m_LastRenderTime = timer.ElapsedMillis();
	}

	
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
