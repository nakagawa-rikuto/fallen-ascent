#pragma once
/// ===Include=== ///
// c++
#include <vector>
// Offscreen
#include "RenderTexture.h"
// Effect
#include "Effect/CopyImageEffect.h"
#include "Effect/GrayscaleEffect.h"
#include "Effect/VignetteEffect.h"
#include "Effect/OutLineEffect.h"
#include "Effect/BoxFilter3x3Effect.h"
#include "Effect/BoxFilter5x5Effect.h"
#include "Effect/RadiusBlurEffect.h"
#include "Effect/DissolveEffect.h"
#include "Effect/ShatterGlassEffect.h"

// Data
#include "Engine/DataInfo/OffScreenData.h"

///=====================================================/// 
/// OffScreenRenderer
///=====================================================///
class OffScreenRenderer {
public:
	OffScreenRenderer() = default;
	~OffScreenRenderer();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">リソース作成やコマンド実行に使用される ID3D12Device へのポインタ。</param>
	/// <param name="srv">シェーダーリソースビュー (SRV) を管理する SRVManager へのポインタ。</param>
	/// <param name="rtv">レンダーターゲットビュー (RTV) を管理する RTVManager へのポインタ。</param>
	/// <param name="width">初期化するレンダーターゲットの幅（ピクセル単位）。</param>
	/// <param name="height">初期化するレンダーターゲットの高さ（ピクセル単位）。</param>
	/// <param name="clearColor">レンダーターゲットをクリアする際に使用する色（Vector4、通常は RGBA）。</param>
	void Initialize(ID3D12Device* device, SRVManager* srv, RTVManager* rtv, uint32_t width, uint32_t height, const Vector4& clearColor);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録する ID3D12GraphicsCommandList へのポインタ。操作対象の有効なコマンドリストを渡します。</param>
	/// <param name="dsvHandle">バインドする深度ステンシルビュー (DSV) を指す D3D12_CPU_DESCRIPTOR_HANDLE。</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録・発行するための ID3D12GraphicsCommandList へのポインター。呼び出す前にコマンドリストが適切に初期化および設定されている必要があります。</param>
	void Draw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	/// <param name="commandList">後処理コマンドを記録するための ID3D12GraphicsCommandList へのポインター。</param>
	void PostDraw(ID3D12GraphicsCommandList* commandList);

#ifdef USE_IMGUI
	void DrawImGui();
#endif

public: /// ===Getter=== ///
	// RTVHandleの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetResultRTV() const;
	// SRVの取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetResultSRV() const;
	// RTVのインデックス番号の取得
	uint32_t GetRTVHandleIndex() const;
	// SRVのインデックス番号の取得
	uint32_t GetSRVHandleIndex() const;
	// Resourceの取得
	ID3D12Resource* GetBuffer() const;

public: /// ===Getter=== ///
	// コピーイメージエフェクトの取得
	CopyImageEffect* GetCopyImage();
	// グレースケールエフェクトの取得
	GrayscaleEffect* GetGrayscale();
	// ビネットエフェクトの取得
	VignetteEffect* GetVignette();
	// アウトラインエフェクトの取得
	OutLineEffect* GetOutLine();
	// 3x3ボックスフィルターエフェクトの取得
	BoxFilter3x3Effect* GetBoxFilter3x3();
	// 5x5ボックスフィルターエフェクトの取得
	BoxFilter5x5Effect* GetBoxFilter5x5();
	// 半径ぼかしエフェクトの取得
	RadiusBlurEffect* GetRadiusBlur();
	// ディゾルブエフェクトの取得
	DissolveEffect* GetDissolve();
	// 破片ガラスエフェクトの取得
	ShatterGlassEffect* GetShatterGlass();
	// Piplineのタイプの取得
	OffScreenType GetType();

public: /// ===Setter=== ///
	// Typeの設定
	void SetType(OffScreenType type);

private:
	std::shared_ptr<RenderTexture> renderTexture_;

	// 各パス（あらかじめ生成済み）
	std::shared_ptr<CopyImageEffect> copyImage_;
	std::shared_ptr<GrayscaleEffect> grayscale_;
	std::shared_ptr<VignetteEffect> vignette_;
	std::shared_ptr<OutLineEffect> outLine_;
	std::shared_ptr<BoxFilter3x3Effect> boxFilter3x3_;
	std::shared_ptr<BoxFilter5x5Effect> boxFilter5x5_;
	std::shared_ptr<RadiusBlurEffect> radiusBlur_;
	std::shared_ptr<DissolveEffect> dissolve_;
	std::shared_ptr<ShatterGlassEffect> shatterGlass_;

	// Piplineのタイプ
	OffScreenType type_ = OffScreenType::CopyImage;
};

