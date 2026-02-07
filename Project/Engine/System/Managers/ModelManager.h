#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
#include "Engine/DataInfo/CData.h"
// DirectXTex
#include "DirectXTex.h"
// C++
#include <string>
#include <map>
// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MiiEngine {
	/// ===前方宣言=== ///
	class TextureManager;

	///=====================================================///  
	/// モデルマネージャ
	///=====================================================///
	class ModelManager {
	public:/// ===基本的な関数=== ///

		ModelManager() = default;
		~ModelManager();

	public:/// ===Functions(関数)=== ///

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="texture">初期化に使用する TextureManager へのポインタ。</param>
		void Initialize(TextureManager* texture);

		/// <summary>
		/// モデルデータの読み込み処理
		/// </summary>
		/// <param name="Key">読み込むエントリまたはリソースを識別するキー（const std::string&）。</param>
		/// <param name="baseDirectoryPath">ファイル探索の基点となるディレクトリのパス（const std::string&）。</param>
		/// <param name="filename">基準ディレクトリ内で読み込むファイルの名前（const std::string&）。</param>
		void Load(const std::string& Key, const std::string& baseDirectoryPath, const std::string& filename);

		/// <summary>
		/// モデルデータの取得
		/// </summary>
		/// <param name="filename">読み込むモデルファイルのパスまたはファイル名（const std::string&）。</param>
		/// <returns>読み込まれたモデル情報を含む ModelData オブジェクト。読み込み失敗時の挙動（例: 例外を投げる、空の ModelData を返すなど）は実装依存です。</returns>
		ModelData GetModelData(const std::string& filename);

	private:/// ===Variables(変数)=== ///

		// テクスチャマネージャ
		TextureManager* textureManager_ = nullptr;

		// モデルデータ
		std::map<std::string, ModelData> modelDates_;

	private:/// ===Functions(関数)=== ///

		/// <summary>
		/// 指定したディレクトリとファイル名からマテリアルテンプレートファイルを読み込み、MaterialData を返します。
		/// </summary>
		/// <param name="directorPath">テンプレートファイルが格納されているディレクトリへのパス。</param>
		/// <param name="filename">読み込むテンプレートファイルの名前（ファイル名）。</param>
		/// <returns>読み込まれたテンプレートに対応する MaterialData。読み込みに失敗した場合の動作（例: 空の MaterialData を返す、例外を投げるなど）は実装に依存します。</returns>
		MaterialData LoadMaterialTemplateFile(const std::string& directorPath, const std::string& filename);

		/// <summary>
		/// 指定したディレクトリとファイル名からWavefront OBJファイルを読み込み、ModelDataとして返します。
		/// </summary>
		/// <param name="directoryPath">OBJファイルが格納されているディレクトリのパス。</param>
		/// <param name="filename">読み込むOBJファイルのファイル名（拡張子を含む）。</param>
		/// <returns>読み込まれたモデルデータを格納したModelDataオブジェクト。読み込み失敗時の挙動（例：例外を投げる、空のデータを返すなど）は実装によります。</returns>
		ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

		/// <summary>
		/// aiNode ポインタから Node を読み取り、対応する Node オブジェクトを生成して返します。
		/// </summary>
		/// <param name="node">読み取る aiNode へのポインタ。関数はこのノードのデータを基に Node を構築します。</param>
		/// <returns>読み取られたノードを表す Node オブジェクト。</returns>
		Node ReadNode(aiNode* node);
	};
}

