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
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;



	//Render every pixel
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{

		for (int x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			/*PerPixel(x,y);*/


			glm::vec4 color = PerPixel(x,y);
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

Renderer::HitPayLoad Renderer::TraceRay(const Ray& ray)
{

	//if (m_ActiveScene->Spheres.size() == 0)
	//{
	//	return glm::vec4(0, 0, 0, 1);
	//}

	int closestSphere = -1;
	float hitDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		// (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 -r^2)
		// a = ray origin
		// b = ray directrion
		// r = radius
		// t = hit distance

		const Sphere& sphere = m_ActiveScene->Spheres[i];

		glm::vec3 Origin = ray.Origin - sphere.Position;

		float A = glm::dot(ray.Direction, ray.Direction); //the first part of the quadratic equation
		float B = 2.0f * glm::dot(Origin, ray.Direction);
		float C = glm::dot(Origin, Origin) - sphere.Radius * sphere.Radius;


		// quadratic formula discriminant: b^2 - 4ac

		float discriminant = B * B - 4.0f * A * C;


		if (discriminant < 0.0f)//if the discriminant is less than zero return black as the pixel is not part of the sphere
			continue;


		//Quadratic formula: (-b +- sqrt(discriminant)) / 2a

		float t0 = (-B + sqrt(discriminant)) / (2 * A);//furthest t
		float t1 = (-B - sqrt(discriminant)) / (2 * A);//closest t

		if (t1 > 0.0f && t1 < hitDistance)
		{
			hitDistance = t1;
			closestSphere = i;
		}
	}

	if (closestSphere < 0)
		return Miss(ray);

	return ClosestHit(ray, closestSphere, hitDistance);


	
	

}

Renderer::HitPayLoad Renderer::ClosestHit(const Ray& ray, int ObjectIndex, float HitDistance)
{
	Renderer::HitPayLoad payload;
	payload.HitDistance = HitDistance;
	payload.ObjectIndex = ObjectIndex;
	 
	const Sphere& closestSphere = m_ActiveScene->Spheres[ObjectIndex];

	glm::vec3 Origin = ray.Origin - closestSphere.Position;
	//glm::vec3 h0 = Origin + ray.Direction * HitDistance;
	payload.WorldPosition = Origin + ray.Direction * HitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);
	payload.WorldPosition += closestSphere.Position;

	return payload;
}

Renderer::HitPayLoad Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayLoad payload;
	payload.HitDistance = -1.0f;
	return payload;
}

glm::vec4 Renderer::PerPixel(uint32_t x,uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 color(0.0f);
	float multiplier = 1.0f;

	int bounces = 2;
	for (int i = 0; i < bounces; i++)
	{
		Renderer::HitPayLoad payload = TraceRay(ray);

		if (payload.HitDistance < 0.0f)
		{
			glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.0f);
			color += skyColor * multiplier;
			break;
		}

		glm::vec3 lightdir = glm::normalize(glm::vec3(-1, -1, -1));

		//d = intensity of the light
		float d = glm::max(glm::dot(payload.WorldNormal, -lightdir), 0.0f); //== cos(angle)

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];

		glm::vec3 spherecolor;
		spherecolor = sphere.Albedo;
		spherecolor *= d;
		color += spherecolor * multiplier;

		multiplier *= 0.7f;

		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal);
	}
	
	return glm::vec4(color, 1.0f);
}

