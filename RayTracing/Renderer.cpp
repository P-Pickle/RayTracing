#include "Renderer.h"

#include "Walnut/Random.h"

namespace Utils {
	
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (color.r * 255.0f);
		uint8_t g = (color.g * 255.0f);
		uint8_t b = (color.b * 255.0f);
		uint8_t a = (color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

void Renderer::Render(glm::vec3 LightDirection, const Camera& camera, const Scene& scene)
{
	const glm::vec3& cameraPosition = camera.GetPosition();
	Ray ray;
	ray.Origin = cameraPosition;

	//Render every pixel
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{

		for (int x = 0; x < m_FinalImage->GetWidth(); x++)
		{

			//glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			//coord = coord * 2.0f - 1.0f; // -1 -> 1
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];



			glm::vec4 color = TraceRay(ray, scene, LightDirection);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x+y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);

		}
	}

	m_FinalImage->SetData(m_ImageData);

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		//No resize needed
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

}

glm::vec4 Renderer::TraceRay(const Ray& ray, const Scene& scene, glm::vec3 LightDirection)
{

	if (scene.Spheres.size() == 0)
	{
		return glm::vec4(0, 0, 0, 1);
	}

	const Sphere& sphere = scene.Spheres[0];

	// (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 -r^2)
	// a = ray origin
	// b = ray directrion
	// r = radius
	// t = hit distance



	glm::vec3 Origin = ray.Origin - sphere.Position;

	float A = glm::dot(ray.Direction, ray.Direction); //the first part of the quadratic equation
	float B = 2.0f * glm::dot(Origin, ray.Direction);
	float C = glm::dot(Origin, Origin) - sphere.Radius * sphere.Radius;


	// quadratic formula discriminant: b^2 - 4ac

	float discriminant = B * B - 4.0f * A * C;


	if (discriminant < 0)//if the discriminant is less than zero return black as the pixel is not part of the sphere
	{
		return glm::vec4(0, 0, 0, 1);
	}


	//Quadratic formula: (-b +- sqrt(discriminant)) / 2a

	float t0 = (-B + sqrt(discriminant)) / (2 * A);//furthest t
	float t1 = (-B - sqrt(discriminant)) / (2 * A);//closest t

	glm::vec3 h0 = Origin + ray.Direction * t0;
	glm::vec3 h1 = Origin + ray.Direction * t1;

	glm::vec3 normal = glm::normalize(h1);

	glm::vec3 lightdir = glm::normalize(LightDirection);
	
	//d = intensity of the light
	float d = glm::max(glm::dot(normal, -lightdir), 0.0f); //== cos(angle)

	//glm::vec3 spherecolor(0, 1, 1);
	//if (d > 0.85f)
	//{
	//	spherecolor = glm::vec3((d-0.85f)/0.2f, d, d);
	//}
	//else
	//{
	//	spherecolor *= d;
	//}
	
	glm::vec3 spherecolor;
	spherecolor = sphere.Albedo;
	spherecolor *= d;
	return glm::vec4(spherecolor, 1.0f);

}
