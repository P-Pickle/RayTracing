#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"
#include "glm/glm.hpp"
#include "Renderer.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
private: 
	Camera m_Camera;
	Scene m_Scene;
	Renderer m_Renderer;
	float m_LastRenderTime = 0.0f;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	glm::vec3 m_SphereColor;
	glm::vec3 m_LightDirection{ -1.0f, -1.0f, -1.0f };
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.f)
	{
		{
			Sphere sphere;
			sphere.Position = { 0.0f,0.0f,0.0f };
			sphere.Radius = 0.5f;
			sphere.Albedo = { 0.0f,1.0f,1.0f };
			m_Scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Position = { 1.0f,0.0f,-5.0f };
			sphere.Radius = 1.5f;
			sphere.Albedo = { 1.0f,0.0f,1.0f };
			m_Scene.Spheres.push_back(sphere);
		}
	}
	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Scene");
		for (size_t x = 0; x < m_Scene.Spheres.size(); x++)
		{
			ImGui::PushID(x);

			Sphere& sphere = m_Scene.Spheres[x];
			ImGui::Text("Sphere %d:", x+1);
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::ColorEdit3("Color", glm::value_ptr(sphere.Albedo));
			ImGui::DragFloat3("Light Direction", glm::value_ptr(m_LightDirection), 0.1f);

			ImGui::PopID();
		}
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
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_LightDirection, m_Camera, m_Scene);

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
