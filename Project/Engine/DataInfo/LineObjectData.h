#pragma once
/// ===Include=== ///
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// LineVertexData3D 
	///-------------------------------------------///
	struct LineVertexData3D {
		Vector3 position;
		Vector4 color;
	};

	///-------------------------------------------/// 
	/// LineTransformMatrixData3D
	///-------------------------------------------///
	struct LineTransformMatrixData3D {
		Matrix4x4 WVP;
	};

	///-------------------------------------------/// 
	/// BezierControlPointData
	///-------------------------------------------///
	struct BezierControlPointData {
		Vector3 position;
		Quaternion rotation;
		float time;
	};
}
