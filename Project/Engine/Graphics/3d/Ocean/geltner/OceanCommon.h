#pragma once
/// ===include=== ///
#include "Engine/Graphics/3d/Base/Material3D.h"
#include "Engine/Graphics/3d/Base/Transform3D.h"
#include "Engine/Graphics/Base/BufferBase.h"
// Data
#include "Engine/DataInfo/LightData.h"
// c++
#include <memory>

/// ===前方宣言=== ///
class GameCamera;

///-------------------------------------------/// 
/// OceanCommon
///-------------------------------------------///
class OceanCommon {
public:
	OceanCommon() = default;
	~OceanCommon();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">初期化に使用するID3D12Deviceへのポインター。nullptrではない必要があります。</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画準備処理
	/// </summary>
	void Bind(ID3D12GraphicsCommandList* commandList);

public: /// ===Getter=== ///
	// Transform情報取得
	const QuaternionTransform& GetWorldTransform() const;
	// Color取得
	const Vector4& GetColor() const;

public: /// ===Setter=== ///
	// Transform
	void SetTranslate(const Vector3& position);
	void SetRotate(const Quaternion& rotate);
	void SetScale(const Vector3& scale);
	// Color
	void SetColor(const Vector4& color);
	// Light
	void SetLightData(LightInfo light);
protected:

	/// ===バッファリソース=== ///
	std::unique_ptr<Material3D> material_;
	std::unique_ptr<Transform3D> wvp_;
	// Light
	std::unique_ptr<BufferBase> directionallight_;
	// Camera
	std::unique_ptr<BufferBase> camera3D_;

	/// ===バッファリソース内のデータを指すポインタ=== ///
	MaterialData3D* materialData_ = nullptr;
	TransformationMatrix3D* wvpMatrixData_ = nullptr;
	// Light
	DirectionalLight* directionalLightData_ = nullptr;
	// Camera
	CameraForGPU* cameraData_ = nullptr;

	/// ===UV=== ///
	EulerTransform uvTransform_;

	/// ===カメラ=== ///
	GameCamera* camera_ = nullptr;

	/// ===Transform情報=== ///
	QuaternionTransform worldTransform_;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	/// ===Light=== ///
	LightInfo light_ = {
		40.0f,
		{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, 1.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 1.0f, 0.0f, 0.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 0.0f}
	};

private:

	/// <summary>
	/// MaterialDataの書き込み処理
	/// </summary>
	void MaterialDataWrite();

	/// <summary>
	/// Transform情報の書き込み処理
	/// </summary>
	void TransformDataWrite();

	/// <summary>
	/// LightData書き込み処理
	/// </summary>
	void LightDataWrite();

	/// <summary>
	/// CameraData書き込み処理
	/// </summary>
	void CameraDataWrite();
};

