#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// DirectXTex
#include "DirectXTex.h"
// Engine
#include "Engine/DataInfo/CData.h"
#include "Engine/DataInfo/AnimationData.h"
#include "Engine/DataInfo/LevelData.h"

namespace Service {
	///=====================================================/// 
	/// GraphicsResourceGetter
	///=====================================================///
	class GraphicsResourceGetter {
	public: /// ===DescriptorHandle=== ///
		/// <summary>
		///  RTVの指定番号のCPUでスクリプタハンドルを取得する
		/// </summary>
		/// <param name="index">RTV ディスクリプタヒープ内の対象ディスクリプタの 0 ベースのインデックス。</param>
		/// <returns>指定したインデックスに対応する D3D12_CPU_DESCRIPTOR_HANDLE。CPU 側の RTV ディスクリプタへのハンドルを表します。</returns>
		static D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);

		/// <summary>
		/// RTVの指定番号のGPUでスクリプタハンドルを取得する
		/// </summary>
		/// <param name="index">取得する RTV のインデックス（配列やヒープ内の位置）。</param>
		/// <returns>指定した RTV のディスクリプタを指す D3D12_GPU_DESCRIPTOR_HANDLE（GPU が使用するディスクリプタハンドル）。</returns>
		static D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

		/// <summary>
		/// DSVの指定番号のCPUでスクリプタハンドルを取得する
		/// </summary>
		/// <param name="index">取得する DSV 用 CPU デスクリプタのインデックス。</param>
		/// <returns>指定したインデックスに対応する D3D12_CPU_DESCRIPTOR_HANDLE。</returns>
		static D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

		/// <summary>
		/// DSVの指定番号のGPUでスクリプタハンドルを取得する
		/// </summary>
		/// <param name="index">取得するDSVディスクリプタのインデックス。</param>
		/// <returns>指定したインデックスに対応するD3D12_GPU_DESCRIPTOR_HANDLE（GPU側で使用されるディスクリプタハンドル）。</returns>
		static D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

		/// <summary>
		/// SRVの指定番号のCPUでスクリプタハンドルを取得する
		/// </summary>
		/// <param name="index">取得する SRV の CPU ディスクリプタのインデックス（ディスクリプタ配列内の位置）。</param>
		/// <returns>指定されたインデックスに対応する D3D12_CPU_DESCRIPTOR_HANDLE を返します。</returns>
		static D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

		/// <summary>
		/// SRVの指定番号のGPUでスクリプタハンドルを取得する
		/// </summary>
		/// <param name="index">取得する SRV ディスクリプタのインデックス（配列オフセット、通常は 0 から始まります）。</param>
		/// <returns>指定したインデックスに対応する D3D12_GPU_DESCRIPTOR_HANDLE（GPU 側のディスクリプタハンドル）を返します。</returns>
		static D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	public:/// ===Window=== ///

		/// <summary>
		/// ウィンドウの幅を取得する
		/// </summary>
		/// <returns>ウィンドウの幅を表す定数の整数値。</returns>
		static const int GetWindowWidth();

		/// <summary>
		/// ウィンドウの高さを取得する
		/// </summary>
		/// <returns>ウィンドウの高さを表す整数値（ピクセル単位）。</returns>
		static const int GetWindowHeight();

	public:/// ===Data=== ///

		/// <summary>
		/// TextureDataの取得
		/// </summary>
		/// <param name="key">検索するメタデータに対応するキーを表す文字列。</param>
		/// <returns>指定したキーに対応するDirectX::TexMetadataオブジェクトへのconst参照。</returns>
		static const DirectX::TexMetadata& GetMetaData(const std::string& key);

		/// <summary>
		/// ModelDataの取得
		/// </summary>
		/// <param name="directorPath">モデル データを読み込むディレクトリのパス。</param>
		/// <returns>取得した ModelData オブジェクト。指定したディレクトリに基づくモデル情報を含みます。</returns>
		static MiiEngine::ModelData GetModelData(const std::string& directorPath);

		/// <summary>
		/// AnimationDataの取得
		/// </summary>
		/// <param name="directorPath">アニメーションデータが格納されているディレクトリまたはリソースへのパス。データの探索／読み込みに使用されます。</param>
		/// <returns>キーが文字列、値が Animation の std::map。各エントリは識別子（例: アニメーション名）と対応する Animation オブジェクトを表します。</returns>
		static std::map<std::string, MiiEngine::Animation> GetAnimationData(const std::string& directorPath);

		/// <summary>
		/// LevelDataの取得
		/// </summary>
		/// <param name="file_name">読み込むレベルデータのファイル名（フルパスまたは相対パスを指定）。</param>
		/// <returns>読み込まれた LevelData へのポインタ。読み込みに失敗した場合やファイルが存在しない場合は nullptr を返すことがあります。</returns>
		static LevelData* GetLevelData(const std::string& file_name);

	public:/// ===ID3D12=== ///

		/// <summary>
		///	Deviceの取得
		/// </summary>
		/// <returns>Deviceを返す</returns>
		static ID3D12Device* GetDXDevice();

		/// <summary>
		/// CommandListの取得
		/// </summary>
		/// <returns>CommandListを返す</returns>
		static ID3D12GraphicsCommandList* GetDXCommandList();
	};
}