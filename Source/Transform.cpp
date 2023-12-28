#include "PreCompiledHeaders.h"
#include "Transform.h"
#include <iostream>
#include <fstream>
#include <random>


namespace Angles
{
	const XMVECTOR UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DIR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
} 

void PrintQ(std::string name, XMVECTOR& Q) {

	std::ofstream logFile("PrintQ.txt", std::ios_base::app);

	float x = XMVectorGetX(Q);
	float y = XMVectorGetY(Q);
	float z = XMVectorGetZ(Q);
	float w = XMVectorGetW(Q);

	if (logFile.is_open()) {
		// Écrire des commentaires ou des informations dans le fichier de logs
		logFile << name << " " << x << " " << y << " " << z << " " << w << std::endl;
	}
}

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
	UpdateRotationFromQuaternion();
}

void Transform::UpdateMatrix()
{
	matrix = XMMatrixScalingFromVector(vSca) * mRot * XMMatrixTranslationFromVector(vPos);
}

void RotateLocalFromRotationQ(Transform& transform, XMVECTOR rotationQuat) {

	XMVECTOR qConjugate = XMQuaternionConjugate(rotationQuat);

	transform.qRot = XMQuaternionMultiply(rotationQuat, transform.qRot);
	transform.qRot = XMQuaternionMultiply(transform.qRot, qConjugate);

	transform.UpdateRotationFromQuaternion();
}

void Transform::Rotate(float yaw, float pitch, float roll)
{
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	RotateLocalFromRotationQ(*this,rotationQuat);
}

void Transform::RotateYaw(float angle)
{
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(Angles::UP, angle);
	RotateLocalFromRotationQ(*this, rotationQuat);
}

void Transform::RotatePitch(float angle)
{
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(Angles::RIGHT, angle);
	RotateLocalFromRotationQ(*this, rotationQuat);
}


void Transform::RotateRoll(float angle)
{
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(Angles::DIR, angle);
	RotateLocalFromRotationQ(*this, rotationQuat);
}

void Transform::RotateWorld(XMMATRIX* pMatrix)
{
	mRot = XMMatrixMultiply(mRot, *pMatrix);
	UpdateRotationFromMatrix();
}

void RotateWorldFromRotationQ(Transform& transform, XMVECTOR rotationQuat) {

	XMVECTOR qConjugate = XMQuaternionConjugate(rotationQuat);

	transform.qRot = XMQuaternionMultiply(qConjugate, transform.qRot);
	transform.qRot = XMQuaternionMultiply(transform.qRot, rotationQuat);

	transform.UpdateRotationFromQuaternion();
}

void Transform::RotateWorldX(float angle)
{
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(Angles::RIGHT, angle);
	RotateWorldFromRotationQ(*this, rotationQuat);
}

void Transform::RotateWorldY(float angle)
{
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(Angles::UP, angle);
	RotateWorldFromRotationQ(*this, rotationQuat);
}

void Transform::RotateWorldZ(float angle)
{
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(Angles::DIR, angle);
	RotateWorldFromRotationQ(*this, rotationQuat);
}

void Transform::TransformTest() {

	// XMMATRIX randomTransformMatrix = GenerateRandomTransformMatrix();

	qRot = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	//FromMatrix(&randomTransformMatrix);

	float x = XMVectorGetX(qRot);
	float y = XMVectorGetY(qRot);
	float z = XMVectorGetZ(qRot);
	float w = XMVectorGetW(qRot);

	std::ofstream logFile("logfile.txt");

	if (logFile.is_open()) {
		logFile << "Quaternion Base : " << x << " " << y << " " << z << " " << w << std::endl;
	}
	
	RotateRoll(1.5708);

	x = XMVectorGetX(qRot);
	y = XMVectorGetY(qRot);
	z = XMVectorGetZ(qRot);
	w = XMVectorGetW(qRot);


	if (logFile.is_open()) {
		logFile << "Quaternion After : " << x << " " << y << " " << z << " " << w << std::endl;
	}

	//RotateRoll(-1.5708);

	x = XMVectorGetX(qRot);
	y = XMVectorGetY(qRot);
	z = XMVectorGetZ(qRot);
	w = XMVectorGetW(qRot);


	if (logFile.is_open()) {
		logFile << "Quaternion AfterAfter : " << x << " " << y << " " << z << " " << w << std::endl;

		logFile.close();
	}

}
//
//float Transform::RandomFloat(float min, float max)
//{
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_real_distribution<float> dis(min, max);
//	return dis(gen);
//}
//
//XMMATRIX Transform::GenerateRandomTransformMatrix()
//{
//	XMVECTOR position = XMVectorSet(RandomFloat(-10.0f, 10.0f), RandomFloat(-10.0f, 10.0f), RandomFloat(-10.0f, 10.0f), 1.0f);
//	XMVECTOR rotationAxis = XMVectorSet(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0.0f);
//	XMVECTOR rotation = XMQuaternionRotationAxis(rotationAxis, RandomFloat(0.0f, XM_2PI));
//	XMVECTOR scale = XMVectorSet(RandomFloat(0.5f, 2.0f), RandomFloat(0.5f, 2.0f), RandomFloat(0.5f, 2.0f), 1.0f);
//
//	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(position);
//	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
//	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
//
//	XMMATRIX transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;
//
//	return transformMatrix;
//}
