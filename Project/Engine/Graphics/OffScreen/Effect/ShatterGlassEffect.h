#pragma once
/// ===Include=== ///
// BufferBase
#include "Engine/Graphics/Base/BufferBase.h"
// RenderPass
#include "Engine/Graphics/OffScreen/RenderPass.h"

/// ===ShatterGlassエフェクトのデータ構造=== ///
struct ShatterGlassData {
	float progress;      // 割れる進行度 (0.0 ~ 1.0)
	float impactX;       // 衝撃点のX座標 (0.0 ~ 1.0)
	float impactY;       // 衝撃点のY座標 (0.0 ~ 1.0)
	float crackDensity;  // ひび割れの密度
	float dispersion;    // 破片の飛散度
	float rotation;      // 破片の回転量
	float fadeOut;       // フェードアウト効果
	float randomSeed;	 // ランダムナシード値
	float padding;       // パディング
};

///=====================================================/// 
/// ShatterGlassEffect
///=====================================================///
class ShatterGlassEffect : public RenderPass {
public:
	ShatterGlassEffect() = default;
	~ShatterGlassEffect() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">Direct3D 12 デバイスへのポインタ。リソースやコマンド作成など、初期化処理でデバイスを使用します。</param>
	/// <param name="RenderTexture">初期化対象のレンダーテクスチャを保持する std::shared_ptr。レンダーテクスチャの作成や設定に用いられます。</param>
	void Initialize(ID3D12Device* device, std::shared_ptr<RenderTexture> RenderTexture) override;

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録するための ID3D12GraphicsCommandList へのポインタ。コマンドの発行先として使用されます。</param>
	/// <param name="dsvHandle">深度ステンシルビュー（DSV）を示す D3D12_CPU_DESCRIPTOR_HANDLE。深度／ステンシル関連の設定やバインドに使用されます。</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList">描画コマンドや状態設定を記録する ID3D12GraphicsCommandList へのポインター。コマンドリストに対して描画操作を記録します。</param>
	void Draw(ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void ImGuiInfo() override;

public: /// ===Setter=== ///
	// データ
	void SetData(ShatterGlassData data);

	// 新しいランダムパターンを生成
	void GenerateNewPattern();

	// テクスチャの設定
	void SetGlassTexture(const std::string& textureName);

private:
	// Buffer
	std::unique_ptr<BufferBase> buffer_;

	// テクスチャ名
	std::string textureKeyName_;

	// Data
	ShatterGlassData* data_ = nullptr;
};