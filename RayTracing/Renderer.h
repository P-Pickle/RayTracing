#pragma once

#include "Walnut/Image.h"
#include <glm/glm.hpp>

#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

#include <memory>

class Renderer
{
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
public:
	Renderer() = default; 

	void Render(glm::vec3 LightDirection, const Camera& camera, const Scene& scene);

	void OnResize(uint32_t width, uint32_t height);

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private: 

	struct HitPayLoad
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	HitPayLoad TraceRay(const Ray& ray);
	HitPayLoad ClosestHit(const Ray& ray, int ObjectIndex, float HitDistance);
	HitPayLoad Miss(const Ray& ray);

	glm::vec4 PerPixel(uint32_t x, uint32_t y);// Ray Gen

};


