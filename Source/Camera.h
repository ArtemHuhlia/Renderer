#pragma once
#include <DirectXMath.h>

class CameraHandler
{
public:
	CameraHandler();
	void MoveForward(float InUnits);
	void MoveBackward(float InUnits);
	void MoveRight(float InUnits);
	void MoveLeft(float InUnits);

	void AddYaw(float Angle);
	void AddPitch(float Angle);

	
	DirectX::XMFLOAT3 GetPosition() const ;
	DirectX::XMFLOAT3 GetRotation() const ;

	void UpdateViewMatrix();
	DirectX::XMMATRIX GetViewMatrix() const ;
	void SetPosition(float InX, float InY, float InZ);
	void SetRotation(float InX, float InY, float InZ);


private:
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Rotation;
	DirectX::XMVECTOR Direction;
	DirectX::XMMATRIX ViewMatrix;

	DirectX::XMVECTOR GetSIMDPosition()
	{
		return DirectX::XMLoadFloat3(&Position);
	}

	DirectX::XMVECTOR GetSIMDRotation()
	{
		return DirectX::XMLoadFloat3(&Rotation);
	}


	void CameraHandler::SetYaw(float Radians);
	void CameraHandler::SetPitch(float Radians);

};
