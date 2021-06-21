#include "PCH.h"

#include "CameraController.h"
#include "Input/InputManager.h"

CameraController::CameraController(CameraHandler * InCameraObject) :
	PossesedCameraObject(InCameraObject),
	CameraTranslationSpeed(0.01f),
	CameraRotationSpeed(1.f)
{}

void CameraController::Update(float DeltaTime)
{
	if (InputManager::IsKeyPressed(SDLK_w))
	{
		PossesedCameraObject->MoveForward(CameraTranslationSpeed * DeltaTime);
	}
	if (InputManager::IsKeyPressed(SDLK_s))
	{
		PossesedCameraObject->MoveBackward(CameraTranslationSpeed * DeltaTime);
	}
	if (InputManager::IsKeyPressed(SDLK_d))
	{
		PossesedCameraObject->MoveRight(CameraTranslationSpeed * DeltaTime);
	}
	if (InputManager::IsKeyPressed(SDLK_a))
	{
		PossesedCameraObject->MoveLeft(CameraTranslationSpeed * DeltaTime);
	}
	
	PossesedCameraObject->AddPitch(InputManager::GetMouseMotionInfo().RelY * DeltaTime * CameraRotationSpeed);
	PossesedCameraObject->AddYaw(InputManager::GetMouseMotionInfo().RelX * DeltaTime * CameraRotationSpeed);
	InputManager::ResetMouseMotionInfo();

	PossesedCameraObject->UpdateViewMatrix();
}

const CameraHandler * CameraController::GetPossedCamera() const
{
	return PossesedCameraObject;
}