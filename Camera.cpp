#include "Camera.h"

Camera::Camera() : pitch(0.0f), yaw(0.0f), moveSpeed(0.05f), rotationSpeed(0.002f)
{
	Initialize();
}
Camera::~Camera()
{
}

void Camera::Initialize()
{
	//position et vue de la camera 

	position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	lookat = XMFLOAT3(0.0f, 0.0f, 1.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookat), XMLoadFloat3(&up));
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);

	XMStoreFloat4x4(&this->viewMatrix, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&this->projectionMatrix, XMMatrixTranspose(projectionMatrix));

}

void Camera::Update()
{
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookat), XMLoadFloat3(&up));
	XMStoreFloat4x4(&this->viewMatrix, XMMatrixTranspose(viewMatrix));
}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

