#pragma once
/// ===Include=== ///
// ComPtr
#include "Engine/Core/ComPtr.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// directX
#include <d3d12.h>
#include <dxcapi.h>
// c++
#include <string>
#include <unordered_map>
#include <vector>
// dxcompiler.libのリンク指定
#pragma comment(lib, "dxcompiler.lib")

namespace MiiEngine {
	/// ===前方宣言=== ///
	class DXCommon;

	///=====================================================/// 
	/// CSCompiler
	///=====================================================///
	class CSCompiler {
	public:

		CSCompiler() = default;
		~CSCompiler();

		/// <summary>
		/// 指定されたパイプラインタイプでコンピュートシェーダーを初期化します。
		/// </summary>
		/// <param name="dxCommon">DirectX共通リソースへのポインタ。</param>
		/// <param name="type">初期化するコンピュートシェーダーパイプラインのタイプ。</param>
		void Initialize(DXCommon* dxCommon, CSPipelineType type);

	public: /// ===Getter=== ///
		/// <summary>
		/// カーネル名を指定してBlobを取得する
		/// </summary>
		/// <param name="entryPoint">カーネル関数名（例: L"InitSpectrum"）</param>
		IDxcBlob* GetKernel(const std::wstring& entryPoint);

		/// <summary>
		/// 登録されている全カーネル名のリストを取得する
		/// </summary>
		const std::vector<std::wstring>& GetKernelNames() const;

	private:
		// カーネル名 → Blob のマップ
		std::unordered_map<std::wstring, ComPtr<IDxcBlob>> kernels_;
		// 登録順を保持するリスト（ディスパッチ順管理用）
		std::vector<std::wstring> kernelNames_;

	private:
		/// <summary>
		/// シェーダーカーネルをコンパイルします。
		/// </summary>
		/// <param name="filePath">コンパイルするシェーダーファイルのパス。</param>
		/// <param name="entryPoint">シェーダーのエントリーポイント関数名。</param>
		/// <param name="dxcUtils">DXCユーティリティインターフェイスへのポインタ。</param>
		/// <param name="dxcCompiler">DXCコンパイラインターフェイスへのポインタ。</param>
		/// <param name="includeHandler">インクルードファイルを処理するハンドラーへのポインタ。</param>
		/// <returns>コンパイルされたシェーダーバイトコードを含むBlobオブジェクト。</returns>
		ComPtr<IDxcBlob> CompileKernel(
			// CompilerするShaderファイルへのパス
			const std::wstring& filePath,
			// シェーダーのエントリーポイント関数名
			const std::wstring& entryPoint,
			// DXC関連のインターフェイス
			IDxcUtils* dxcUtils,
			IDxcCompiler3* dxcCompiler,
			IDxcIncludeHandler* includeHandler);
	};
}

