#include "OshanCommon.h"
// Service
#include "Engine/System/Service/Render.h"
#include "Engine/System/Service/CameraService.h"
// camera
#include "application/Game/Camera/GameCamera.h"

// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OshanCommon::~OshanCommon() {
	material_.reset();
	wvp_.reset();
	directionallight_.reset();
	camera3D_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
const QuaternionTransform& OshanCommon::GetWorldTransform() const { return worldTransform_; }
const Vector4& OshanCommon::GetColor() const { return color_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void OshanCommon::SetTranslate(const Vector3& position) { worldTransform_.translate = position; }
void OshanCommon::SetRotate(const Quaternion& rotate) { worldTransform_.rotate = rotate; }
void OshanCommon::SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
void OshanCommon::SetColor(const Vector4& color) { color_ = color; }
void OshanCommon::SetLightData(LightInfo light) { light_ = light; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OshanCommon::Initialize(ID3D12Device* device) {
	/// ===生成=== ///
	material_ = std::make_unique<Material3D>();
	wvp_ = std::make_unique<Transform3D>();
	//  Camera
	camera3D_ = std::make_unique<BufferBase>();
	// Light
	directionallight_ = std::make_unique<BufferBase>();

	/// ===worldTransform=== ///
	worldTransform_ = { { 10.0f, 1.0f, 10.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
	uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	/// ===Material=== ///
	material_->Create(device, sizeof(MaterialData3D));
	material_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// Data書き込み
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = 1;
	materialData_->shininess = 10.0f;
	materialData_->uvTransform = Math::MakeIdentity4x4();

	/// ===wvp=== ///
	wvp_->Create(device, sizeof(TransformationMatrix3D));
	wvp_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&wvpMatrixData_));
	// Dataの書き込み
	wvpMatrixData_->WVP = Math::MakeIdentity4x4();
	wvpMatrixData_->World = Math::MakeIdentity4x4();
	wvpMatrixData_->WorldInverseTranspose = Math::Inverse4x4(wvpMatrixData_->World);

	/// ===DirectionalLight=== ///
	directionallight_->Create(device, sizeof(DirectionalLight));
	directionallight_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// Data書き込み
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };  // 真上から
	directionalLightData_->intensity = 1.5f;  // 少し明るく

	/// ===Camera=== ///
	camera3D_->Create(device, sizeof(CameraForGPU));
	camera3D_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	// Data書き込み
	cameraData_->worldPosition = { 0.0f, 4.0f, -10.0f };
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void OshanCommon::Update() {
	camera_ = CameraService::GetActiveCamera().get();

	MaterialDataWrite();
	TransformDataWrite();
	LightDataWrite();
	CameraDataWrite();
}

///-------------------------------------------/// 
/// 描画準備
///-------------------------------------------///
void OshanCommon::Bind(ID3D12GraphicsCommandList* commandList) {
	/// ===コマンドリストに設定=== ///
	// MaterialBufferの設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
	// wvpMatrixBufferの設定
	commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
	// DirectionlLightの設定
	commandList->SetGraphicsRootConstantBufferView(2, directionallight_->GetBuffer()->GetGPUVirtualAddress());
	// CameraBufferの設定
	commandList->SetGraphicsRootConstantBufferView(3, camera3D_->GetBuffer()->GetGPUVirtualAddress());
}

///-------------------------------------------/// 
/// MaterialDataの書き込み処理
///-------------------------------------------///
void OshanCommon::MaterialDataWrite() {
	// UV変換行列の計算
	Matrix4x4 uvTransformMatrix = Math::MakeScaleMatrix(uvTransform_.scale);
	Matrix4x4 uvTransformMatrixMultiply = Multiply(uvTransformMatrix, Math::MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrixMultiply = Multiply(uvTransformMatrixMultiply, Math::MakeTranslateMatrix(uvTransform_.translate));

	// データの書き込み
	materialData_->color = color_;
	materialData_->shininess = light_.shininess;
	materialData_->uvTransform = uvTransformMatrixMultiply;
}

///-------------------------------------------/// 
/// Transform情報の書き込み処理
///-------------------------------------------///
void OshanCommon::TransformDataWrite() {
	// ワールド行列の計算
	Matrix4x4 worldMatrix = Math::MakeAffineQuaternionMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	// ビュー射影行列の取得と掛け算
	const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
	worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

	// データの書き込み
	wvpMatrixData_->WVP = worldViewProjectionMatrix;
	wvpMatrixData_->World = worldMatrix;
	wvpMatrixData_->WorldInverseTranspose = Math::Inverse4x4(worldMatrix);
}

///-------------------------------------------/// 
/// LightData書き込み処理
///-------------------------------------------///
void OshanCommon::LightDataWrite() {
	// 平行光源のデータ書き込み
	directionalLightData_->color = light_.directional.color;
	directionalLightData_->direction = light_.directional.direction;
	directionalLightData_->intensity = light_.directional.intensity;
}

///-------------------------------------------/// 
/// CameraData書き込み処理
///-------------------------------------------///
void OshanCommon::CameraDataWrite() {
	// カメラのワールド位置を書き込み
	cameraData_->worldPosition = camera_->GetTranslate();
}
