#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Material
{
	float Roughness = 1.0f;
	float Metallic = 0.0f;
	glm::vec3 Albedo{1.0f};
};

struct Sphere
{
	glm::vec3 Position{0.0f};
	float Radius = 0.5f;

	int MatIndex = 0;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};


