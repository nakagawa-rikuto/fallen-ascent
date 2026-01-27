#pragma once
/// ===Include=== ///
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

struct LineVertexData3D {
	Vector3 position;
	Vector4 color;
};

struct LineTransformMatrixData3D {
	Matrix4x4 WVP;
};

struct BezierControlPointData {
	Vector3 position;
	Quaternion rotation;
	float time;
};
