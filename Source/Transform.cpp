#include "Transform.h"

Transform::Transform()
{
	Identity();
}

void Transform::Identity()
{
	vSca = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	vDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	vRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	qRot = XMQuaternionIdentity();
	mRot = XMMatrixIdentity();
	vPos = XMVectorZero();
	matrix = XMMatrixIdentity();
}

void Transform::FromMatrix(XMMATRIX* pMat)
{
	XMMatrixDecompose(&vSca, &qRot, &vPos, *pMat);
	XMQuaternionNormalize(qRot);
	XMVECTOR quatRotation = XMQuaternionRotationMatrix(*pMat);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), quatRotation);
	XMVECTOR right = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), quatRotation);
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), quatRotation);
	vDir = XMVector3Normalize(forward);
	vRight = XMVector3Normalize(right);
	vUp = XMVector3Normalize(up);
}

void Transform::UpdateRotationFromVectors()
{
	mRot.r[0] = vRight;
	mRot.r[1] = vUp;
	mRot.r[2] = vDir;
	mRot.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	qRot = XMQuaternionRotationMatrix(mRot);
}

void Transform::UpdateRotationFromQuaternion()
{
	mRot = XMMatrixRotationQuaternion(qRot);
	vDir = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), mRot);
	vRight = XMVector3TransformCoord(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), mRot);
	vUp = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), mRot);
}

void Transform::UpdateRotationFromMatrix()
{
	qRot = XMQuaternionRotationMatrix(mRot);
}

void Transform::UpdateMatrix()
{
	matrix = XMMatrixScalingFromVector(vSca) * mRot * XMMatrixTranslationFromVector(vPos);
}

void Transform::Rotate(float yaw, float pitch, float roll)
{
	qRot *= XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	mRot = XMMatrixRotationQuaternion(qRot);
}

void Transform::RotateYaw(float angle)
{
	qRot *= XMQuaternionRotationAxis(vUp, angle);
	mRot = XMMatrixRotationQuaternion(qRot);
}

void Transform::RotatePitch(float angle)
{
	qRot *= XMQuaternionRotationAxis(vRight, angle);
	mRot = XMMatrixRotationQuaternion(qRot);
}

void Transform::RotateRoll(float angle)
{
	qRot *= XMQuaternionRotationAxis(vDir, angle);
	mRot = XMMatrixRotationQuaternion(qRot);
}

void Transform::RotateWorld(XMMATRIX* pMatrix)
{
	mRot = XMMatrixMultiply(mRot, *pMatrix);
	UpdateRotationFromMatrix();
}

void Transform::RotateWorldX(float angle)
{
	XMMATRIX rotMatrix = XMMatrixRotationX(angle);
	mRot = XMMatrixMultiply(mRot, rotMatrix);
	UpdateRotationFromMatrix();
}

void Transform::RotateWorldY(float angle)
{
	XMMATRIX rotMatrix = XMMatrixRotationY(angle);
	mRot = XMMatrixMultiply(mRot, rotMatrix);
	UpdateRotationFromMatrix();
}

void Transform::RotateWorldZ(float angle)
{
	XMMATRIX rotMatrix = XMMatrixRotationZ(angle);
	mRot = XMMatrixMultiply(mRot, rotMatrix);
	UpdateRotationFromMatrix();
}
