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
	/// <param name="device">初期化に使用する D3D12 デバイスへのポインタ。GPU リソースの作成やコマンド発行に使用されます。</param>
	/// <param name="RenderTexture">初期化で使用するレンダーテクスチャへの std::shared_ptr。レンダリングの出力先や関連リソースを表します。</param>
	void Initialize(ID3D12Device* device, std::shared_ptr<RenderTexture> RenderTexture) override;

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録するための ID3D12GraphicsCommandList へのポインター。GPU コマンドの発行に使用されます。</param>
	/// <param name="dsvHandle">深度ステンシルビュー (DSV) を指す D3D12_CPU_DESCRIPTOR_HANDLE。描画前に DSV をバインドまたは設定するために使用されます。</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録する ID3D12GraphicsCommandList へのポインター。nullptr を渡さないでください。</param>
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