#pragma once
#include "DirectXMath.h"

using namespace DirectX;
class Camera
{
public:
	Camera();
	~Camera();
	
	void Initialize();

	void Update();

	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetProjectionMatrix() const;

	void HandleInput();
private:
	XMFLOAT3 position;
	XMFLOAT3 lookat;
	XMFLOAT3 up;

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	float pitch;
	float yaw;

	float moveSpeed;
	float rotationSpeed;
};

