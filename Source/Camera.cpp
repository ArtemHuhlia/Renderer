#include "Camera.h"

#include "Math/MathUtlity.h"

using namespace DirectX;

const DirectX::XMFLOAT3 UpVector(0.f, 1.f, 0.f);
const DirectX::XMFLOAT3 ForwardVector(0.f, 0.f, 1.f);

CameraHandler::CameraHandler()
{
	Rotation.x = 0;
	Rotation.y = 0;
	Rotation.z = 0;

	Position.x = 0;
	Position.y = 0;
	Position.z = 0;

	Direction = DirectX::XMLoadFloat3(&ForwardVector);
}

void CameraHandler::SetPosition(float InX, float InY, float InZ)
{
	Position.x = InX;
	Position.y = InY;
	Position.z = InZ;

	LOG("%s: %i, %i, %i", "NewPos", InX, InY, InZ);
}

void CameraHandler::SetRotation(float InX, float InY, float InZ)
{
	Rotation.x = InX;
	Rotation.y = InY;
	Rotation.z = InZ;
}

DirectX::XMFLOAT3 CameraHandler::GetPosition() const 
{
	return Position;
}

DirectX::XMFLOAT3 CameraHandler::GetRotation() const
{
	return Rotation;
}

void CameraHandler::UpdateViewMatrix()
{
	DirectX::XMFLOAT3 UpVector;
	DirectX::XMFLOAT3 PositionVector;
	DirectX::XMFLOAT3 LookAtVector;
	float Yaw, Pitch, Roll;
	DirectX::XMMATRIX RotationMatrix;


	// Setup the vector that points upwards.
	UpVector.x = 0.0f;
	UpVector.y = 1.0f;
	UpVector.z = 0.0f;

	// Setup the position of the camera in the world.
	PositionVector.x = Position.x;
	PositionVector.y = Position.y;
	PositionVector.z = Position.z;

	// Setup where the camera is looking by default.
	LookAtVector.x = 0.0f;
	LookAtVector.y = 0.0f;
	LookAtVector.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	Pitch = Rotation.x;
	Yaw   = Rotation.y;
	Roll  = Rotation.z;
	
	DirectX::XMVECTOR ForwardSIMD = XMLoadFloat3(&ForwardVector);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	Direction = DirectX::XMVector3TransformCoord(ForwardSIMD, RotationMatrix); 
	Direction = DirectX::XMVector3Normalize(Direction);
	
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	DirectX::XMVECTOR UpSIMD = XMLoadFloat3(&UpVector);
	DirectX::XMVECTOR PostionSIMD = XMLoadFloat3(&PositionVector);

	// Translate the rotated camera position to the location of the viewer.
	DirectX::XMVECTOR TransformedDirection = DirectX::XMVectorAdd(PostionSIMD, Direction);

	// Finally create the view matrix from the three updated vectors.
	ViewMatrix = DirectX::XMMatrixLookAtLH(PostionSIMD, TransformedDirection, UpSIMD);
}

DirectX::XMMATRIX CameraHandler::GetViewMatrix() const
{
	return ViewMatrix;
}

void CameraHandler::MoveForward(float InUnits)
{
	Direction = DirectX::XMVector3Normalize(Direction);
	DirectX::XMVECTOR MovingVector = DirectX::XMVectorScale(Direction,InUnits);
	DirectX::XMVECTOR NewPositionSIMD = DirectX::XMVectorAdd(GetSIMDPosition(),MovingVector);
	DirectX::XMFLOAT3 NewPosition;
	DirectX::XMStoreFloat3(&NewPosition, NewPositionSIMD);
	SetPosition(NewPosition.x,NewPosition.y,NewPosition.z);
}

void CameraHandler::MoveBackward(float InUnits)
{
	Direction = DirectX::XMVector3Normalize(Direction);
	DirectX::XMVECTOR MovingVector = DirectX::XMVectorScale(Direction,InUnits);
	DirectX::XMVECTOR NewPositionSIMD = DirectX::XMVectorSubtract(GetSIMDPosition(),MovingVector);
	DirectX::XMFLOAT3 NewPosition;
	DirectX::XMStoreFloat3(&NewPosition, NewPositionSIMD);
	SetPosition(NewPosition.x,NewPosition.y,NewPosition.z);
}

void CameraHandler::MoveRight(float InUnits)
{
	Direction = DirectX::XMVector3Normalize(Direction);
	XMVECTOR UpVectorSIMD = DirectX::XMLoadFloat3(&UpVector);

	XMVECTOR RightVector = DirectX::XMVector3Cross(UpVectorSIMD, Direction);
	RightVector = DirectX::XMVector3Normalize(RightVector);
	DirectX::XMVECTOR MovingVector = DirectX::XMVectorScale(RightVector,InUnits);
	DirectX::XMVECTOR NewPositionSIMD = DirectX::XMVectorAdd(GetSIMDPosition(),MovingVector);

	DirectX::XMFLOAT3 NewPosition;
	DirectX::XMStoreFloat3(&NewPosition, NewPositionSIMD);
	SetPosition(NewPosition.x,NewPosition.y,NewPosition.z);
}

void CameraHandler::MoveLeft(float InUnits)
{
	Direction = DirectX::XMVector3Normalize(Direction);
	XMVECTOR UpVectorSIMD = DirectX::XMLoadFloat3(&UpVector);

	XMVECTOR RightVector = DirectX::XMVector3Cross(UpVectorSIMD, Direction);
	RightVector = DirectX::XMVector3Normalize(RightVector);
	DirectX::XMVECTOR MovingVector = DirectX::XMVectorScale(RightVector,InUnits);
	DirectX::XMVECTOR NewPositionSIMD = DirectX::XMVectorSubtract(GetSIMDPosition(),MovingVector);

	DirectX::XMFLOAT3 NewPosition;
	DirectX::XMStoreFloat3(&NewPosition, NewPositionSIMD);
	SetPosition(NewPosition.x,NewPosition.y,NewPosition.z);
}

void CameraHandler::AddYaw(float Angle)
{
	float NewValue = Math::ToDegrees(Rotation.y) + Angle;
	if (NewValue > 0)
	{
		NewValue = NewValue >= 360.f ? NewValue - 360.f : NewValue;
	}
	else
	{
		NewValue = NewValue <= -360.f ? NewValue + 360.f : NewValue;
	}

	SetYaw(Math::ToRadians(NewValue));
}

void CameraHandler::AddPitch(float Angle)
{
	const float ClampedDegrees = Math::Clamp( -89.f, 89.f,Angle);
	SetPitch(Math::ToRadians(Math::Clamp(-89.f, 89.f, Math::ToDegrees(Rotation.x) + Angle)));
}

void CameraHandler::SetYaw(float Radians)
{
	LOG("%s: %d", "NewYaw", Radians);
	Rotation.y = Radians;
}

void CameraHandler::SetPitch(float Radians)
{
	LOG("%s: %f", "NewPitch", Radians);
	Rotation.x = Radians;
}

