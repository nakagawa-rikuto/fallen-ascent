#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
#include "Engine/DataInfo/CData.h"
// DirectXTex
#include "DirectXTex.h"
// C++
#include <string>
#include <unordered_map>
// DirectX
#include <d3dx12.h>

/// ===前方宣言=== ///
class DXCommon;
class SRVManager;

///=====================================================/// 
/// テクスチャマネージャ
///=====================================================///
class TextureManager {

public:/// ===基本的な関数=== ///
	
	TextureManager() = default;
	~TextureManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="dxCommon">DirectX に関連する共通データやコンテキストを表す DXCommon オブジェクトへのポインタ。初期化処理で使用される有効なインスタンスを渡します。</param>
	/// <param name="srvManager">シェーダーリソースビュー（SRV）を管理する SRVManager オブジェクトへのポインタ。SRV の登録や管理に使用される有効なインスタンスを渡します。</param>
	void Initialize(DXCommon* dxCommon, SRVManager* srvManager);

public:/// ===Getter=== ///

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(const std::string& Key);
	// メタデータ
	const DirectX::TexMetadata& GetMetaData(const std::string& Key);

public:/// ==Setter==== ///

	// グラフィックスルートデスクリプタテーブルの設定
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex, std::string Key);

public:/// ===Variables(変数)=== ///

	/// ===ImGui対応=== ///
	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop_;

public:/// ===Functions(関数)=== ///

	/// <summary>
	/// テクスチャの読み込み処理
	/// </summary>
	/// <param name="key">読み込んだテクスチャを識別・参照するためのキー。</param>
	/// <param name="filePath">読み込むテクスチャファイルのパス（相対パスまたは絶対パス）。</param>
	void LoadTexture(const std::string& key, const std::string& filePath);

private:/// ===Variables(変数)=== ///

	// DXCommonのポインタ
	DXCommon* dxCommon_ = nullptr; 
	// SRVManagerのポインタ
	SRVManager* srvManager_ = nullptr; 

	/// ===テクスチャデータの構造体=== ///
	// テクスチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metadata;
		ComPtr<ID3D12Resource> resource;
		ComPtr<ID3D12Resource> intermediateResource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	/// ===テクスチャデータコンテナ=== ///
	// テクスチャデータ
	std::unordered_map<std::string, TextureData> textureDates_;

private:/// ===Functions(関数)=== ///
	
	/// <summary>
	/// 指定されたファイルからイメージを読み込み、DirectX::ScratchImage オブジェクトとして返す
	/// </summary>
	/// <param name="key">読み込み操作を識別するためのキー。キャッシュの識別子などに使用されることがあります。</param>
	/// <param name="filePath">読み込むイメージファイルへのパス。</param>
	/// <returns>読み込まれたイメージを保持する DirectX::ScratchImage オブジェクト。読み込みに失敗した場合の挙動（例: 空の画像や例外）は実装に依存します。</returns>
	DirectX::ScratchImage Load(const std::string& key, const std::string& filePath);

	/// <summary>
	/// 指定された DirectX::TexMetadata に基づいてテクスチャ用の ID3D12Resource を作成し、ComPtr<ID3D12Resource> として返す
	/// </summary>
	/// <param name="metadata">テクスチャの幅・高さ・ミップレベル・ピクセル形式などを含むメタデータ。作成するリソースの特性を指定します。</param>
	/// <returns>作成された ID3D12Resource を保持する ComPtr。作成に失敗した場合は空の ComPtr（null 相当）を返す可能性があります。</returns>
	ComPtr<ID3D12Resource> CreateTextureResource( const DirectX::TexMetadata& metadata);

	/// <summary>
	/// ミップイメージをGPUにアップロードし、アップロード済みのテクスチャリソースを返す
	/// </summary>
	/// <param name="texture">データをアップロードするターゲットのテクスチャリソースを保持する ComPtr<ID3D12Resource>。</param>
	/// <param name="mipImages">各ミップレベルの画像データを格納した DirectX::ScratchImage の参照。</param>
	/// <returns>アップロードされたテクスチャを表す ComPtr<ID3D12Resource>。戻り値は無視しないでください。</returns>
	[[nodiscard]]
	ComPtr<ID3D12Resource> UploadTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);
};

