/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Intersection.h
Purpose: header of Intersection.cpp (draw shape and calculate normal)
Language: C++, Visual Studio 2019
Platform: Visual Studio 2019 16.11.2, Windows 10
Project: CS350 Project
Author: Hagyeong Kim (hagyeong.kim@digipen.edu, 180002221) //using framework
Creation date: 03/04/2022
End Header --------------------------------------------------------*/
#pragma once

#include "AABB.h"
#include "BoundingSphere.h"
#include "Plane.h"
#include "Ray.h"
#include "Triangle.h"

//point aabb sphere plane triangle ray

class Intersection
{
public:
	bool SphereVsSphere(BoundingSphere sphere1, BoundingSphere sphere2);
	bool AABBVVsSphere(AABB aabb, BoundingSphere sphere);
	bool SphereVsAABB(BoundingSphere sphere, AABB aabb);
	bool AABBVsAABB(AABB aabb1, AABB aabb2);

	bool PointVsSphere(Point3D point, BoundingSphere sphere);
	bool PointVsAABB(Point3D point, AABB aabb);
	bool PointVsTriangle(Point3D point, Triangle triangle);
	bool PointVsPlane(Point3D point, Plane plane);

	bool RayVsPlane(Ray ray, Plane plane);
	bool RayVsAABB(Ray ray, AABB aabb);
	bool RayVsSphere(Ray ray, BoundingSphere sphere);
	bool RayVsTriangle(Ray ray, Triangle triangle);

	bool PlaneVsAABB(Plane plane, AABB aabb);
	bool PlaneVsSphere(Plane plane, BoundingSphere sphere);
	bool PlaneVsTriangle(Plane plane, Triangle triangle);
};

/*
if (intersection.SphereVsSphere(sphere, sphere2))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.AABBVVsSphere(aabb, sphere3))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.SphereVsAABB(sphere4, aabb2))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.AABBVsAABB(aabb, aabb4))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PointVsSphere(point, sphere5))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PointVsAABB(point2, aabb5))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PointVsTriangle(point3, triangle))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PointVsPlane(point4, plane))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.RayVsPlane(ray, plane2))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.RayVsAABB(ray2, aabb6))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.RayVsSphere(ray3, sphere6))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.RayVsTriangle(ray4, triangle2))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PlaneVsAABB(plane3, aabb7))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PlaneVsSphere(plane4, sphere7))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}

if (intersection.PlaneVsTriangle(plane5, triangle3))
{
	newColors = { 0.2f, 0.5f, 0.3f };
}
*/