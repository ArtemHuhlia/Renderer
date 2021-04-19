#pragma once
#include "Camera.h"

class CameraController
{
	CameraHandler*  PossesedCameraObject;
	float	CameraTranslationSpeed;
	float	CameraRotationSpeed;
public:
	CameraController(CameraHandler * InCameraObject);
	void Update(float DeltaTime);
	const CameraHandler * GetPossedCamera() const;
};
