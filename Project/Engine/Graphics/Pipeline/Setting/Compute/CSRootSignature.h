#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// C++
#include <d3d12.h>
#include <unordered_map>
#include <functional>

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// ルートシグネイチャ（CS用）
///=====================================================///
class CSRootSignature {
	public:
	CSRootSignature() = default;
	~CSRootSignature();
	
	/// <summary>
	/// ルートシグネイチャの生成処理
	/// </summary>
	/// <param name="dxCommon">DXCommon インスタンスへのポインタ。デバイスやコマンドキューなどの共通 DirectX リソースとコンテキストを提供します。</param>
	/// <param name="Type">作成するパイプラインの種類を指定する PipelineType 値（例: グラフィックス、コンピュート）。</param>
	void Create(DXCommon* dxCommon, PipelineType Type);
	/// <summary>
	/// ルートシグネイチャの取得
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* GetRootSignature()const;

private:
	ComPtr<ID3D12RootSignature> rootSignature_; // ルートシグネイチャ
};

