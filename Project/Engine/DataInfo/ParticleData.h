#pragma once
#include "Engine/DataInfo/CData.h"

///=====================================================/// 
/// パーティクル形状タイプ
///=====================================================///
enum class shapeType {
	kCircle,    // 円
	kCylinder,  // 円柱
	kNone,      // なし（通常のプレーン）
};

///=====================================================/// 
/// パーティクルブレンドモード
/// ※既存のBlendModeと重複する場合は削除
///=====================================================///
enum class ParticleBlendMode {
	kNormal,    // 通常
	kAdd,       // 加算
	kSubtract,  // 減算
	kMultiply,  // 乗算
};

///=====================================================/// 
/// パーティクルデータ（実行時）
///=====================================================///
struct ParticleData {
	EulerTransform transform;  // トランスフォーム
	Vector3 velocity;          // 速度
	Vector4 color;             // 色
	float lifeTime;            // 寿命
	float currentTime;         // 経過時間
	Vector3 rotationSpeed;     // 回転速度
	Vector3 initialScale;      // 初期スケール
	Vector4 initialColor;      // 初期色
};

///=====================================================/// 
/// パーティクルGPU送信用データ
///=====================================================///
struct ParticleForGPU {
	Matrix4x4 WVP;    // ワールドビュープロジェクション行列
	Matrix4x4 World;  // ワールド行列
	Vector4 color;    // 色
};

///=====================================================/// 
/// パーティクル物理設定
///=====================================================///
struct ParticlePhysicsSettings {
	Vector3 velocityMin = { -1.0f, 1.0f, -1.0f };      // 速度の最小値
	Vector3 velocityMax = { 1.0f, 5.0f, 1.0f };        // 速度の最大値
	Vector3 acceleration = { 0.0f, 0.0f, 0.0f };       // 加速度
	float gravity = 0.0f;                               // 重力
	float explosionRadius = 1.0f;                       // 爆発半径
	float upwardForce = 0.0f;                          // 上方向の力
	bool useRandomVelocity = true;                      // ランダム速度を使用
};

///=====================================================/// 
/// パーティクル見た目設定
///=====================================================///
struct ParticleAppearanceSettings {
	Vector4 startColor = { 1.0f, 1.0f, 1.0f, 1.0f };  // 開始色
	Vector4 endColor = { 1.0f, 1.0f, 1.0f, 0.0f };    // 終了色
	Vector3 startScaleMin = { 1.0f, 1.0f, 1.0f };     // 開始スケール最小
	Vector3 startScaleMax = { 1.0f, 1.0f, 1.0f };     // 開始スケール最大
	Vector3 endScale = { 0.5f, 0.5f, 0.5f };          // 終了スケール
	std::string texturePath = "";                      // テクスチャパス
	bool useColorGradient = false;                     // 色のグラデーション使用
	bool useScaleAnimation = false;                    // スケールアニメーション使用
};

///=====================================================/// 
/// パーティクル回転設定
///=====================================================///
struct ParticleRotationSettings {
	Vector3 rotationSpeedMin = { 0.0f, 0.0f, 0.0f };  // 回転速度最小
	Vector3 rotationSpeedMax = { 0.0f, 0.0f, 0.0f };  // 回転速度最大
	bool randomRotation = false;                       // ランダム回転を使用
	bool enableRotation = false;                       // 回転を有効化
};

///=====================================================/// 
/// パーティクル発生設定
///=====================================================///
struct ParticleEmissionSettings {
	float lifetimeMin = 1.0f;                          // 寿命最小値
	float lifetimeMax = 2.0f;                          // 寿命最大値
	float emissionRate = 10.0f;                        // 発生レート（秒間発生数）
	uint32_t burstCount = 0;                           // バースト数（0なら連続発生）
	float frequency = 0.5f;                            // 発生頻度（秒）
	bool isBurst = false;                              // バーストモード
};