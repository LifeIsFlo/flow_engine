#pragma once
#include <DirectXMath.h>

#include "FWD.h"
#include "Vector3.h"
#include "Object.h"
#include "Component.h"

namespace FlowEngineDX11 {
	class Transform : public Component {
	public:
		Vector3 scale{1,1, 1};
		Vector3 position{0,0, 0};
		Vector3 rotation{0,0,0};

		Vector3 GetRight();
		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetLeft();
		Vector3 GetBackward();
		Vector3 GetDown();

		DirectX::XMMATRIX CreateMatrix();
		DirectX::XMMATRIX CreateInverseMatrix();
	};
}