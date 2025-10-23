#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// directX
#include <d3d12.h>
#include <dxcapi.h>
// c++
#include <string>
#include <unordered_map>

#pragma comment(lib, "dxcompiler.lib")

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// Compiler
///=====================================================///
class Compiler {
public:

	Compiler() = default;
	~Compiler();

	/// <summary>
	///shaderをコンパイルする処理
	/// </summary>
	/// <param name="dxCommon">初期化に使用する DXCommon オブジェクトへのポインタ。DirectX に関連する共有リソースや状態を提供する。</param>
	/// <param name="Type">初期化対象のパイプラインの種類を示す値（PipelineType）。</param>
	void Initialize(DXCommon* dxCommon, PipelineType Type);

	/// ===ShaderBlobの取得=== ///
	// VertexShader
	IDxcBlob* GetObjVS();
	// PixelShader
	IDxcBlob* GetObjPS();

private:

	//　ObjectShader
	ComPtr<IDxcBlob> objVSBlob_;
	ComPtr<IDxcBlob> objPSBlob_;

private:
	/// <summary>
	/// CompileShader関数
	/// </summary>
	/// <param name="filePath">コンパイル対象のシェーダーファイルへのパス（std::wstring）。</param>
	/// <param name="profile">コンパイルに使用するプロファイル文字列（例: L"vs_6_0"、L"ps_6_0"）。</param>
	/// <param name="dxcUtils">初期化済みのIDxcUtilsインターフェイス。ファイル読み込みやユーティリティ操作に使用されます。</param>
	/// <param name="dxcCompiler">初期化済みのIDxcCompiler3インターフェイス。実際のコンパイル処理を行います。</param>
	/// <param name="includeHandler">初期化済みのIDxcIncludeHandlerインターフェイス。#includeの解決を担当します。</param>
	/// <returns>コンパイル結果を保持するComPtr<IDxcBlob>。コンパイル成功時はシェーダーバイナリが格納され、失敗時はエラーメッセージを含むBlobが返される場合があります。</returns>
	ComPtr<IDxcBlob> CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,

		// Compilerに使用するProfile
		const wchar_t* profile,

		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};