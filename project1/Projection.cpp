/*
*	hakyung.kim
*	Assigment 4
*	CS250
*	spring 2020
*/

#include "Projection.h"

Affine CameraToWorld(const Camera& cam)
{
	Affine cameraToWorld;

	cameraToWorld.row[0].x = cam.Right().x;
	cameraToWorld.row[0].y = cam.Up().x;
	cameraToWorld.row[0].z = cam.Back().x;
	cameraToWorld.row[0].w = cam.Eye().x;

	cameraToWorld.row[1].x = cam.Right().y;
	cameraToWorld.row[1].y = cam.Up().y;
	cameraToWorld.row[1].z = cam.Back().y;
	cameraToWorld.row[1].w = cam.Eye().y;

	cameraToWorld.row[2].x = cam.Right().z;
	cameraToWorld.row[2].y = cam.Up().z;
	cameraToWorld.row[2].z = cam.Back().z;
	cameraToWorld.row[2].w = cam.Eye().z;

	cameraToWorld.row[3].x = 0;
	cameraToWorld.row[3].y = 0;
	cameraToWorld.row[3].z = 0;
	cameraToWorld.row[3].w = 1;

	return cameraToWorld;
}

Affine WorldToCamera(const Camera& cam)
{
	Affine worldToCamera;

	worldToCamera = inverse(CameraToWorld(cam));

	return worldToCamera;
}

Matrix CameraToNDC(const Camera& cam)
{
	Matrix cameraToNCD;

	cameraToNCD.row[0].x = (2 * cam.ViewportGeometry().z) / cam.ViewportGeometry().x;
	cameraToNCD.row[1].y = (2 * cam.ViewportGeometry().z) / cam.ViewportGeometry().y;
	cameraToNCD.row[2].z = (cam.NearDistance() + cam.FarDistance()) / (cam.NearDistance() - cam.FarDistance());
	cameraToNCD.row[2].w = (2 * cam.NearDistance() * cam.FarDistance()) / (cam.NearDistance() - cam.FarDistance());
	cameraToNCD.row[3].z = -1;

	return cameraToNCD;
}
