/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Intersection.h
Purpose: calculate Intersection (draw shape and calculate normal)
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/
#include "Intersection.h"

bool Intersection::SphereVsSphere(BoundingSphere sphere1, BoundingSphere sphere2)
{
	return PointVsSphere(sphere2.center, BoundingSphere((sphere1.r + sphere2.r), sphere1.center));
}

bool Intersection::AABBVVsSphere(AABB aabb, BoundingSphere sphere)
{
	glm::vec3 point = sphere.center;

	point.x = std::min(point.x, aabb.max.x);
	point.x = std::max(point.x, aabb.min.x);

	point.y = std::min(point.y, aabb.max.y);
	point.y = std::max(point.y, aabb.min.y);

	point.z = std::min(point.z, aabb.max.z);
	point.z = std::max(point.z, aabb.min.z);

	float distance = glm::length(sphere.center - point) * glm::length(sphere.center - point);
	float radius = sphere.r * sphere.r;

	return distance < radius;
}

bool Intersection::SphereVsAABB(BoundingSphere sphere, AABB aabb)
{
	glm::vec3 point = sphere.center;

	point.x = std::min(point.x, aabb.max.x);
	point.x = std::max(point.x, aabb.min.x);

	point.y = std::min(point.y, aabb.max.y);
	point.y = std::max(point.y, aabb.min.y);

	point.z = std::min(point.z, aabb.max.z);
	point.z = std::max(point.z, aabb.min.z);

	float distance = glm::length(sphere.center - point) * glm::length(sphere.center - point);
	float radius = sphere.r * sphere.r;

	return distance < radius;
}

bool Intersection::AABBVsAABB(AABB aabb1, AABB aabb2)
{
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x)
	{
		return false;
	}

	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y)
	{
		return false;
	}

	if (aabb1.max.z < aabb2.min.z || aabb1.min.z > aabb2.max.z)
	{
		return false;
	}


	return true;
}

bool Intersection::PointVsSphere(Point3D point, BoundingSphere sphere)
{
	float distacne = glm::distance(point.coordinate, sphere.center);
	float radius = sphere.r * sphere.r;

	return (distacne * distacne) <= radius;

}

bool Intersection::PointVsAABB(Point3D point, AABB aabb)
{
	if (aabb.max.x < point.coordinate.x || aabb.min.x > point.coordinate.x)
	{
		return false;
	}

	if (aabb.max.y < point.coordinate.y || aabb.min.y > point.coordinate.y)
	{
		return false;
	}

	if (aabb.max.z < point.coordinate.z || aabb.min.z > point.coordinate.z)
	{
		return false;
	}


	return true;

	/*for (int i = 0; i < 3; ++i)
	{
		if (aabb.min[i] > point.coordinate[i] || point.coordinate[i] > aabb.max[i])
		{
			return false;
		}
	}

	return true;*/
}

bool Intersection::PointVsTriangle(Point3D point, Triangle triangle)
{
	glm::vec3 v21 = triangle.v2 - triangle.v1;
	glm::vec3 v31 = triangle.v3 - triangle.v1;

	glm::vec3 vp1 = point.coordinate - triangle.v1;

	float v21v21 = glm::dot(v21, v21);
	float v21v31 = glm::dot(v21, v31);
	float v31v31 = glm::dot(v31, v31);
	float v31v21 = glm::dot(v31, v21);
	float vp1v21 = glm::dot(vp1, v21);
	float vp1v31 = glm::dot(vp1, v31);

	float det = v21v31 * v31v21 - v21v21 * v31v31;

	float det1, det2;

	det1 = (v31v21 * vp1v31 - v31v31 * vp1v21) / det;

	if (det1 < 0.f || det1 > 1.f)
	{
		return false;
	}

	det2 = (v21v31 * vp1v21 - v21v21 * vp1v31) / det;

	if (det2 < 0.f || (det1 + det2 > 1.f))
	{
		return false;
	}

	/*if (1.f - det1 - det2 > 0.f || 1.f - det1 - det2 < 1.f)
	{
		return false;
	}*/

	return true;
}

bool Intersection::PointVsPlane(Point3D point, Plane plane)
{
	float dot = glm::dot(plane.normal, point.coordinate);

	return dot == plane.distance;

}

bool Intersection::RayVsPlane(Ray ray, Plane plane)
{
	float dot1 = glm::dot(plane.normal, ray.start - plane.distance);
	float dot2 = glm::dot(plane.normal, ray.direction);

	float t = -dot1 / dot2;

	return t >= 0.f;
}

bool Intersection::RayVsAABB(Ray ray, AABB aabb)
{
	glm::vec3 min = (aabb.min - ray.start) / ray.direction;
	glm::vec3 max = (aabb.max - ray.start) / ray.direction;

	float minX = std::min(min.x, max.x);
	float minY = std::min(min.y, max.y);
	float minZ = std::min(min.z, max.z);

	float maxX = std::max(min.x, max.x);
	float maxY = std::max(min.y, max.y);
	float maxZ = std::max(min.z, max.z);

	float mins = std::max(std::max(minX, minY), minZ);
	float maxs = std::min(std::min(maxX, maxY), maxZ);

	if (mins > maxs)
	{
		return false;
	}

	if (maxs < 0.f)
	{
		return false;
	}

	return true;
}

bool Intersection::RayVsSphere(Ray ray, BoundingSphere sphere)
{
	glm::vec3 distance = ray.start - sphere.center;

	float a = (sphere.r * sphere.r);
	float b = glm::dot(distance, ray.direction);
	float c = glm::dot(distance, distance) - a;

	float bsqrt = b * b;

	float discriminant = bsqrt - c;

	if (discriminant < 0.f)
	{
		return false;
	}

	return true;
}

bool Intersection::RayVsTriangle(Ray ray, Triangle triangle)
{
	glm::vec3 v21 = triangle.v2 - triangle.v1;
	glm::vec3 v31 = triangle.v3 - triangle.v1;
	glm::vec3 vr1 = ray.start - triangle.v1;

	glm::vec3 cross = glm::cross(v21, v31);

	if (cross == glm::vec3{0.f, 0.f, 0.f})
	{
		return false;
	}

	float cross1 = glm::dot(cross, vr1);
	float dot = glm::dot(cross, ray.direction);

	float t = -cross1 / dot;

	if (fabs(dot) < FLT_EPSILON)
	{
		if (cross1 != 0.f)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}

	if (t < 0.f)
	{
		return false;
	}

	glm::vec3 rays = ray.start + ray.direction * t;
	glm::vec3 distance = rays - triangle.v1;

	float v2121 = glm::dot(v21, v21);
	float v2131 = glm::dot(v21, v31);
	float v3121 = glm::dot(v31, v21);
	float v3131 = glm::dot(v31, v31);

	float vp21 = glm::dot(distance, v21);
	float vp31 = glm::dot(distance, v31);

	float det = v2131 * v3121 - v2121 * v3131;

	float det1 = (v2131 * vp31 - v3131 * vp21) / det;

	if (det1 < 0.f || det1 > 1.f)
	{
		return false;
	}

	float det2 = (v3121 * vp21 - v2121 * vp31) / det;
	
	if (det2 < 0.f || det2 > 1.f)
	{
		return false;
	}

	//if (1.f - det1 - det2)
	//{
	//	return false;
	//}

	return true;
}

bool Intersection::PlaneVsAABB(Plane plane, AABB aabb)
{
	glm::vec3 point = aabb.max - ((aabb.max + aabb.min) / 2.f);

	float xNormal = point.x * abs(plane.normal.x);
	float yNormal = point.y * abs(plane.normal.y);
	float zNormal = point.z * abs(plane.normal.z);

	float normal = xNormal + yNormal + zNormal;

	float distance = abs(glm::dot(plane.normal, ((aabb.max + aabb.min) / 2.f)) - plane.distance);

	return distance <= normal;
}

bool Intersection::PlaneVsSphere(Plane plane, BoundingSphere sphere)
{
	glm::vec3 pointNormal = sphere.center - glm::dot(plane.normal, sphere.center);
	pointNormal = pointNormal * plane.normal;

	float distance = glm::distance(sphere.center, pointNormal);

	return distance <= sphere.r;
}

bool Intersection::PlaneVsTriangle(Plane plane, Triangle triangle)
{
	float dot1 = glm::dot(plane.normal, triangle.v1);
	float dot2 = glm::dot(plane.normal, triangle.v2);
	float dot3 = glm::dot(plane.normal, triangle.v3);

	if (dot1 > 0.f && dot2 > 0.f && dot3 > 0.f)
	{
		return false;
	}

	if (dot1 < 0.f && dot2 < 0.f && dot3 < 0.f)
	{
		return false;
	}

	return true;
}
