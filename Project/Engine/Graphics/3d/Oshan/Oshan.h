#pragma once
/// ===Include=== ///
// buffer
#include "Engine/Graphics/3d/Base/VertexBuffer3D.h"
#include "Engine/Graphics/3d/Base/IndexBuffer3D.h"
// Common
#include "OshanCommon.h"

// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"
// Data
#include "Engine/DataInfo/OshanData.h"
#include "Engine/DataInfo/LightData.h"
// c++
#include <memory>
#include <vector>
#include <array>

/// ===前方宣言=== ///
class GameCamera;

///=====================================================/// 
/// Oshan - オーシャンオブジェクト管理クラス
///=====================================================///
class Oshan : public OshanCommon {
public:
	Oshan() = default;
	~Oshan();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="gridSize">グリッドのサイズ（縦横の分割数）</param>
	void Initialize(int gridSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">ブレンドモード</param>
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// 波紋を追加
	/// </summary>
	/// <param name="worldPosition">ワールド座標</param>
	/// <param name="intensity">強度</param>
	void AddRipple(const Vector3& worldPosition, float intensity = 1.0f);

	/// <summary>
	/// 指定座標に物体が当たったことを検知して波紋を生成
	/// </summary>
	/// <param name="worldPosition">衝突位置</param>
	/// <param name="objectID">物体の識別ID（同じ物体からの連続衝突を防ぐため）</param>
	void OnObjectHit(const Vector3& worldPosition, int objectID);

	/// <summary>
	/// 衝突時に呼び出す関数（ゲーム側から使用）
	/// </summary>
	/// <param name="collisionPosition">衝突位置（ワールド座標）</param>
	/// <param name="collisionTime">衝突時刻</param>
	/// <param name="intensity">波紋の強度（デフォルト: 1.0f）</param>
	void OnCollision(const Vector3& collisionPosition, float collisionTime, float intensity = 1.0f);

	/// <summary>
	/// すべての波紋をクリア
	/// </summary>
	void ClearRipples();

public: /// ===Setter=== ///
	// Ocean Wave Parameters
	void SetWaveInfo(int waveIndex, const Vector3& direction, float amplitude, float length, float speed);
	// Ocean Color Parameters
	void SetColorInfo(const OceanColorInfo& colorInfo);
	// Ripple Parameters
	void SetRippleSpeed(float speed);
	void SetRippleDecay(float decay);

private:
	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<IndexBuffer3D> index_;
	// Ocean専用バッファ
	std::unique_ptr<BufferBase> oceanShaderInfo_[3];  // 3つの波情報
	std::unique_ptr<BufferBase> rippleBuffer_;        // 波紋バッファ
	std::unique_ptr<BufferBase> oceanColorBuffer_;    // 色情報バッファ

	/// ===バッファリソース内のデータを指すポインタ=== ///
	VertexData3D* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	// Ocean専用データ
	OceanShaderInfo* oceanShaderData_[3] = { nullptr, nullptr, nullptr };
	RippleBufferForGPU* rippleBufferData_ = nullptr;
	OceanColorInfo* oceanColorData_ = nullptr;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	/// ===グリッド情報=== ///
	int gridSize_ = 64;
	int vertexCount_ = 0;
	int indexCount_ = 0;

	/// ===波紋管理=== ///
	std::array<RippleData, 8> ripples_;
	float currentTime_ = 0.0f;
	float rippleSpeed_ = 4.0f;
	float rippleDecay_ = 1.0f;

	// 衝突検知用（同じ物体からの連続衝突を防ぐ）
	struct HitRecord {
		int objectID;
		Vector3 lastPosition;
		bool wasHitting;
	};
	std::vector<HitRecord> hitRecords_;

	/// ===Ocean Color情報=== ///
	OceanColorInfo colorInfo_ = {
		{ 0.1f, 0.4f, 0.6f }, 0.0f,      // seaBase（基本の海の色：深い青）
		{ 0.2f, 0.7f, 0.8f }, 0.0f,      // seaShallow（浅瀬：明るいシアン）
		{ 0.4f, 0.6f, 0.9f }, 0.0f,      // sky（空の色）
		{ 0.0f, 0.05f, 0.2f },           // deepWater（深海：非常に暗い青）
		1.2f,                            // baseStrength（色の強度：少し強めに）
		0.3f,                            // heightOffset
		0.4f,                            // waterClarity（透明度）
		0.5f                             // foamThreshold（泡の閾値：少し控えめに）
	};

private: /// ===Functions=== ///
	/// <summary>
	/// グリッドメッシュの生成
	/// </summary>
	void CreateGridMesh();

	/// <summary>
	/// OceanShaderData書き込み処理
	/// </summary>
	void OceanShaderDataWrite();

	/// <summary>
	/// RippleData書き込み処理
	/// </summary>
	void RippleDataWrite();

	/// <summary>
	/// ColorData書き込み処理
	/// </summary>
	void ColorDataWrite();

	/// <summary>
	/// 波紋の更新処理
	/// </summary>
	void UpdateRipples();

	/// <summary>
	/// 衝突記録を探す
	/// </summary>
	HitRecord* FindHitRecord(int objectID);
};