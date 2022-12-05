#pragma once

#include "Walnut/Image.h"
#include <glm/glm.hpp>

#include "Camera.h"
#include "Ray.h"

#include <memory>

class Renderer
{
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

public:
	Renderer() = default; 

	void Render(glm::vec3 ShpereColor, glm::vec3 LightDirection, const Camera& camera);

	void OnResize(uint32_t width, uint32_t height);

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private: 
	glm::vec4 TraceRay(const Ray& ray, glm::vec3 SphereColor, glm::vec3 LightDirection);
};


