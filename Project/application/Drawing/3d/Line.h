#pragma once
/// ===Include=== ///
// Data
#include "Engine/DataInfo/ColliderData.h"
#include "Engine/DataInfo/LineObjectData.h"
// C++
#include <vector>
#include <cstdint>

///=====================================================/// 
/// Line
///=====================================================///
class Line {
public:
	/// <summary>
	/// 線分の描画。
	/// </summary>
	/// <param name="start">線分の始点</param>
	/// <param name="end">線分の終点</param>
	/// <param name="color">線分の色（RGBA）</param>
	void DrawLine(const Vector3& start, const Vector3& end, const Vector4& color);

	/// <summary>
	/// OBBを線で描画
	/// </summary>
	/// <param name="obb">描画するOBB</param>
	/// <param name="color">線の色（RGBA）</param>
	void DrawOBB(const OBB& obb, const Vector4& color);

	/// <summary>
	/// AABBを線で描画
	/// </summary>
	/// <param name="aabb">描画するAABB</param>
	/// <param name="color">線の色（RGBA）</param>
	void DrawAABB(const AABB& aabb, const Vector4& color);

	/// <summary>
	/// 球体を線で描画
	/// </summary>
	/// <param name="sphere">描画する球体</param>
	/// <param name="color">線の色（RGBA）</param>
	void DrawSphere(const Sphere& sphere, const Vector4& color);

	/// <summary>
	/// グリッドを描画
	/// </summary>
	/// <param name="center">グリッドの中心座標</param>
	/// <param name="size">グリッド全体のサイズ</param>
	/// <param name="division">分割数</param>
	/// <param name="color">線の色（RGBA）</param>
	void DrawGrid(const Vector3& center, const Vector3& size, uint32_t division, const Vector4& color);

	/// <summary>
	/// AABBを基準としたグリッドボックスを描画
	/// </summary>
	/// <param name="aabb">基準となるAABB</param>
	/// <param name="division">分割数</param>
	/// <param name="center">グリッドの中心座標</param>
	/// <param name="color">線の色（RGBA）</param>
	void DrawGirdBox(const AABB& aabb, uint32_t division, const Vector3& center, const Vector4& color);

	/// <summary>
	/// 1方向に対して分割されたグリッド線を描画するユーティリティ関数
	/// </summary>
	/// <param name="start">開始座標</param>
	/// <param name="end">終了座標</param>
	/// <param name="offset">各線のオフセット量</param>
	/// <param name="division">分割数</param>
	/// <param name="color">線の色（RGBA）</param>
	void DrawGridLines(const Vector3& start, const Vector3& end, const Vector3& offset, uint32_t division, const Vector4& color);

	/// <summary>
	/// 2次ベジェ曲線を描画（3つの制御点）
	/// </summary>
	/// <param name="p0">開始点</param>
	/// <param name="p1">制御点</param>
	/// <param name="p2">終了点</param>
	/// <param name="color">線の色</param>
	/// <param name="segments">曲線の分割数（滑らかさ）</param>
	void CreateQuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& color, int segments = 20);

	/// <summary>
	/// 3次ベジェ曲線を描画（4つの制御点）
	/// </summary>
	/// <param name="p0">開始点</param>
	/// <param name="p1">制御点1</param>
	/// <param name="p2">制御点2</param>
	/// <param name="p3">終了点</param>
	/// <param name="color">線の色</param>
	/// <param name="segments">曲線の分割数（滑らかさ）</param>
	void CreateCubicBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector4& color, int segments = 20);

	/// <summary>
	/// 複数の制御点から滑らかな曲線を描画
	/// </summary>
	/// <param name="controlPoints">制御点のリスト</param>
	/// <param name="color">線の色</param>
	/// <param name="segments">各セグメントの分割数</param>
	void CreateSmoothCurve(const std::vector<BezierControlPointData>& controlPoints, const Vector4& color, int segments = 20);

private:

	/// <summary>
	/// 2次ベジェ曲線上の点を計算
	/// </summary>
	Vector3 CalculateQuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

	/// <summary>
	/// 3次ベジェ曲線上の点を計算
	/// </summary>
	Vector3 CalculateCubicBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);
};

