#include "Transform.h"

#include <numbers>

#include "Vector3.h"

FlowEngineDX11::Vector3 FlowEngineDX11::Transform::GetRight()
{
	float yaw = rotation.y + DirectX::XM_PIDIV2;

	Vector3 right = { sin(yaw),
	0,
	cos(yaw) };

	return right;
}

FlowEngineDX11::Vector3 FlowEngineDX11::Transform::GetForward()
{
	float yaw = rotation.y + DirectX::XM_PIDIV2;
	float pitch = rotation.x;

	Vector3 forward = { -cos(pitch) * cos(yaw),
		-sin(pitch),
		 cos(pitch) * sin(yaw) };

	return forward;
}

FlowEngineDX11::Vector3 FlowEngineDX11::Transform::GetUp()
{
	float yaw = rotation.y + DirectX::XM_PIDIV2;
	float pitch = rotation.x - DirectX::XM_PIDIV2;

	Vector3 up = { 
		-cos(pitch) * cos(yaw),
		-sin(pitch),
		 cos(pitch) * sin(yaw) };

	return up;
}

FlowEngineDX11::Vector3 FlowEngineDX11::Transform::GetLeft()
{
	return GetRight() * -1;
}

FlowEngineDX11::Vector3 FlowEngineDX11::Transform::GetBackward()
{
	return GetForward() * -1;
}

FlowEngineDX11::Vector3 FlowEngineDX11::Transform::GetDown()
{
	return GetUp() * -1;
}

DirectX::XMMATRIX FlowEngineDX11::Transform::CreateMatrix()
{
	auto scaleMatrix =
		DirectX::XMMatrixScaling(
			scale.x,
			scale.y,
			scale.z);

	auto xRot = DirectX::XMMatrixRotationX(rotation.x);
	auto yRot = DirectX::XMMatrixRotationY(rotation.y);
	auto zRot = DirectX::XMMatrixRotationZ(rotation.z);

	auto rot = xRot * yRot * zRot;

	auto positionMatrix =
		DirectX::XMMatrixTranslation(
			position.x,
			position.y,
			position.z);

	return scaleMatrix * rot * positionMatrix;
}

DirectX::XMMATRIX FlowEngineDX11::Transform::CreateInverseMatrix()
{
	auto scaleMatrix =
		DirectX::XMMatrixScaling(
			-scale.x,
			-scale.y,
			-scale.z);

	auto xRot = DirectX::XMMatrixRotationX(-rotation.x);
	auto yRot = DirectX::XMMatrixRotationY(-rotation.y);
	auto zRot = DirectX::XMMatrixRotationZ(-rotation.z);

	auto rot = xRot * yRot * zRot;

	auto positionMatrix =
		DirectX::XMMatrixTranslation(
			-position.x,
			-position.y,
			-position.z);

	return scaleMatrix * rot * positionMatrix;
}
