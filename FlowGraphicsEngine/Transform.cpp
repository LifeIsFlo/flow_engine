#include "Transform.h"

DirectX::XMMATRIX FlowEngineDX11::Transform::CreateMatrix()
{
	auto scaleMatrix =
		DirectX::XMMatrixScaling(
			scale.x,
			scale.y,
			1.0f);

	auto rotationMatrix =
		DirectX::XMMatrixRotationZ(
			rotation);

	auto positionMatrix =
		DirectX::XMMatrixTranslation(
			position.x,
			position.y,
			0.0f);

	return scaleMatrix * rotationMatrix * positionMatrix;
}

DirectX::XMMATRIX FlowEngineDX11::Transform::CreateInverseMatrix()
{
	auto scaleMatrix =
		DirectX::XMMatrixScaling(
			-scale.x,
			-scale.y,
			1.0f);

	auto rotationMatrix =
		DirectX::XMMatrixRotationZ(
			-rotation);

	auto positionMatrix =
		DirectX::XMMatrixTranslation(
			-position.x,
			-position.y,
			0.0f);

	return scaleMatrix * rotationMatrix * positionMatrix;
}
