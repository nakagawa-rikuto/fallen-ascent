#include "Line.h"
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/Graphics/3d/Line/LineObject3D.h"

#include "Math/MatrixMath.h"
#include "Math/EasingMath.h"
#include "Math/sMath.h"

///-------------------------------------------/// 
/// Line
///-------------------------------------------///
void Line::DrawLine(const Vector3& start, const Vector3& end, const Vector4& color) {
	Service::ServiceLocator::GetLineObject3D()->CreateLine(start, end, color);
}

///-------------------------------------------/// 
/// OBB
///-------------------------------------------///
void Line::DrawOBB(const MiiEngine::OBB& obb, const Vector4& color) {
	// OBBの各頂点を定義（ローカル座標）
	Vector3 localVertices[8] = {
		{-obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z}, {obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z},
		{obb.halfSize.x,  obb.halfSize.y, -obb.halfSize.z}, {-obb.halfSize.x,  obb.halfSize.y, -obb.halfSize.z},
		{-obb.halfSize.x, -obb.halfSize.y,  obb.halfSize.z}, {obb.halfSize.x, -obb.halfSize.y,  obb.halfSize.z},
		{obb.halfSize.x,  obb.halfSize.y,  obb.halfSize.z}, {-obb.halfSize.x,  obb.halfSize.y,  obb.halfSize.z}
	};

	// ワールド座標に変換（回転適用 & 平行移動）
	Vector3 worldVertices[8] = {};
	for (int i = 0; i < 8; i++) {
		worldVertices[i] =
			obb.center +
			obb.axis[0] * localVertices[i].x +
			obb.axis[1] * localVertices[i].y +
			obb.axis[2] * localVertices[i].z;
	}

	// OBBのエッジを結ぶ
	int edges[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, // 底面
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, // 上面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 側面
	};

	for (int i = 0; i < 12; i++) {
		DrawLine(worldVertices[edges[i][0]], worldVertices[edges[i][1]], color);
	}
}

///-------------------------------------------/// 
/// AABB
///-------------------------------------------///
void Line::DrawAABB(const MiiEngine::AABB & aabb, const Vector4& color) {
	// グリッドを6面描画
	Vector3 size = aabb.max - aabb.min;
	Vector3 halfSize = size * 0.5f;
	uint32_t division = 1;

	// AABBの中心を算出
	Vector3 offset = (aabb.min + aabb.max) * 0.5f;

	// 前面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);

	// 後面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);

	// 左面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);

	// 右面
	DrawGridLines(offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);

	// 上面
	DrawGridLines(offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);

	// 下面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);
}

///-------------------------------------------/// 
/// Sphere
///-------------------------------------------///
void Line::DrawSphere(const MiiEngine::Sphere& sphere, const Vector4 & color) {
	// 分割数
	const uint32_t div = 8;
	// 緯度・経度のステップ角度
	const float lonStep = 2.0f * Math::Pi() / float(div);
	const float latStep = Math::Pi() / float(div);

	// 緯度・経度に沿って線を描画
	for (uint32_t lat = 0; lat < div; ++lat) {
		for (uint32_t lon = 0; lon < div; ++lon) {
			Vector3 a, b, c;

			float lat1 = -Math::Pi() / 2.0f + latStep * lat;
			float lon1 = lonStep * lon;

			a = {
				cos(lat1) * cos(lon1),
				sin(lat1),
				cos(lat1) * sin(lon1)
			};
			b = {
				cos(lat1 + latStep) * cos(lon1),
				sin(lat1 + latStep),
				cos(lat1 + latStep) * sin(lon1)
			};
			c = {
				cos(lat1) * cos(lon1 + lonStep),
				sin(lat1),
				cos(lat1) * sin(lon1 + lonStep)
			};

			// スケール & 中心変換
			a = a * sphere.radius + sphere.center;
			b = b * sphere.radius + sphere.center;
			c = c * sphere.radius + sphere.center;

			DrawLine(a, b, color);
			DrawLine(a, c, color);
		}
	}
}

///-------------------------------------------/// 
/// Grid
///-------------------------------------------///
void Line::DrawGrid(const Vector3 & center, const Vector3 & size, uint32_t division, const Vector4& color) {
	Vector3 halfSize = size * 0.5f;
	Vector3 start = center + Vector3(-halfSize.x, 0.0f, -halfSize.z);
	Vector3 end = center + Vector3(halfSize.x, 0.0f, -halfSize.z);

	// 横線
	for (uint32_t i = 0; i <= division; i++) {
		float t = float(i) / float(division);
		Vector3 s = Math::Lerp(start, end, t);
		Vector3 e = s + Vector3(0.0f, 0.0f, size.z);
		DrawLine(s, e, color);
	}
	start = center + Vector3(-halfSize.x, 0.0f, -halfSize.z);
	end = center + Vector3(-halfSize.x, 0.0f, halfSize.z);
	// 縦線
	for (uint32_t i = 0; i <= division; i++) {
		float t = float(i) / float(division);
		Vector3 s = Math::Lerp(start, end, t);
		Vector3 e = s + Vector3(size.x, 0.0f, 0.0f);
		DrawLine(s, e, color);
	}
}

///-------------------------------------------/// 
/// GridBox
///-------------------------------------------///
void Line::DrawGirdBox(const MiiEngine::AABB& aabb, uint32_t division, const Vector3& center, const Vector4& color) {
	// グリッドを6面描画
	Vector3 size = aabb.max - aabb.min;
	Vector3 halfSize = size * 0.5f;

	// 中心を基準に座標を調整
	Vector3 offset = center;

	// 前面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);

	// 後面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);

	// 左面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);

	// 右面
	DrawGridLines(offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, halfSize.z),
		Vector3(0.0f, halfSize.y * 2, 0.0f), division, color);

	// 上面
	DrawGridLines(offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, halfSize.y, halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);

	// 下面
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(halfSize.x, -halfSize.y, -halfSize.z),
		Vector3(0.0f, 0.0f, halfSize.z * 2), division, color);
	DrawGridLines(offset + Vector3(-halfSize.x, -halfSize.y, -halfSize.z),
		offset + Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		Vector3(halfSize.x * 2, 0.0f, 0.0f), division, color);
}

///-------------------------------------------/// 
/// GridLine
///-------------------------------------------///
void Line::DrawGridLines(const Vector3 & start, const Vector3 & end, const Vector3 & offset, uint32_t division, const Vector4 & color) {
	// 線を描画
	for (uint32_t i = 0; i <= division; i++) {
		float t = float(i) / float(division);
		Vector3 startPos = Math::Lerp(start, end, t);
		Vector3 endPos = startPos + offset;
		DrawLine(startPos, endPos, color);
	}
}


///-------------------------------------------/// 
/// 2次ベジェ曲線の作成
///-------------------------------------------///
void Line::CreateQuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& color, int segments) {
	// セグメント数が0以下の場合は早期リターン
	if (segments <= 0) {
		return;
	}

	// 前の点を保存
	Vector3 previousPoint = p0;

	// 曲線を分割して描画
	for (int i = 1; i <= segments; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(segments);
		Vector3 currentPoint = CalculateQuadraticBezier(p0, p1, p2, t);

		// 前の点から現在の点まで線を引く
		DrawLine(previousPoint, currentPoint, color);

		// 次のループのために更新
		previousPoint = currentPoint;
	}
}

///-------------------------------------------/// 
/// 3次ベジェ曲線の作成
///-------------------------------------------///
void Line::CreateCubicBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector4& color, int segments) {
	// セグメント数が0以下の場合は早期リターン
	if (segments <= 0) {
		return;
	}

	// 前の点を保存
	Vector3 previousPoint = p0;

	// 曲線を分割して描画
	for (int i = 1; i <= segments; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(segments);
		Vector3 currentPoint = CalculateCubicBezier(p0, p1, p2, p3, t);

		// 前の点から現在の点まで線を引く
		DrawLine(previousPoint, currentPoint, color);

		// 次のループのために更新
		previousPoint = currentPoint;
	}
}

///-------------------------------------------/// 
/// 滑らかな曲線の作成
///-------------------------------------------///
void Line::CreateSmoothCurve(const std::vector<MiiEngine::BezierControlPointData>& controlPoints, const Vector4& color, int segments) {
	// 制御点が2つ未満の場合は描画しない
	if (controlPoints.size() < 2) {
		return;
	}

	// 制御点の数に応じて処理を分岐
	size_t n = controlPoints.size();

	// 2点の場合は直線
	if (n == 2) {
		DrawLine(controlPoints[0].position, controlPoints[1].position, color);
		return;
	}

	// 3点の場合は2次ベジェ曲線
	if (n == 3) {
		CreateQuadraticBezier(
			controlPoints[0].position,
			controlPoints[1].position,
			controlPoints[2].position,
			color,
			segments
		);
		return;
	}

	// 4点の場合は3次ベジェ曲線
	if (n == 4) {
		CreateCubicBezier(
			controlPoints[0].position,
			controlPoints[1].position,
			controlPoints[2].position,
			controlPoints[3].position,
			color,
			segments
		);
		return;
	}

	// 5点以上の場合はDe Casteljauのアルゴリズムを使用
	Vector3 previousPoint = CalculateBezierPointDeCasteljau(controlPoints, 0.0f);

	for (int i = 1; i <= segments; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(segments);
		Vector3 currentPoint = CalculateBezierPointDeCasteljau(controlPoints, t);

		DrawLine(previousPoint, currentPoint, color);

		previousPoint = currentPoint;
	}
}

///-------------------------------------------///  
/// ベジェ曲線の計算
///-------------------------------------------///
Vector3 Line::CalculateQuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {
	float u = 1.0f - t;
	float uu = u * u;
	float tt = t * t;

	// (1-t)^2 * P0 + 2(1-t) * t * P1 + t^2 * P2
	Vector3 point = {};
	point.x = uu * p0.x + 2.0f * u * t * p1.x + tt * p2.x;
	point.y = uu * p0.y + 2.0f * u * t * p1.y + tt * p2.y;
	point.z = uu * p0.z + 2.0f * u * t * p1.z + tt * p2.z;

	return point;
}

///-------------------------------------------/// 
/// ベジェ曲線の計算
///-------------------------------------------///
Vector3 Line::CalculateCubicBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	float u = 1.0f - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	// (1-t)^3 * P0 + 3(1-t)^2 * t * P1 + 3(1-t) * t^2 * P2 + t^3 * P3
	Vector3 point = {};
	point.x = uuu * p0.x + 3.0f * uu * t * p1.x + 3.0f * u * tt * p2.x + ttt * p3.x;
	point.y = uuu * p0.y + 3.0f * uu * t * p1.y + 3.0f * u * tt * p2.y + ttt * p3.y;
	point.z = uuu * p0.z + 3.0f * uu * t * p1.z + 3.0f * u * tt * p2.z + ttt * p3.z;

	return point;
}

///-------------------------------------------/// 
/// ベジェ曲線の計算（De Casteljauのアルゴリズム）
///-------------------------------------------///
Vector3 Line::CalculateBezierPointDeCasteljau(const std::vector<MiiEngine::BezierControlPointData>& controlPoints, float t) {
	// 制御点の位置のみを抽出
	std::vector<Vector3> temp;
	for (const auto& cp : controlPoints) {
		temp.push_back(cp.position);
	}

	// De Casteljauのアルゴリズム
	while (temp.size() > 1) {
		std::vector<Vector3> newTemp;
		for (size_t i = 0; i < temp.size() - 1; ++i) {
			newTemp.push_back(Math::Lerp(temp[i], temp[i + 1], t));
		}
		temp = newTemp;
	}

	return temp[0];
}
