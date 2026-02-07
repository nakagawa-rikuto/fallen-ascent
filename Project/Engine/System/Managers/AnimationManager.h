#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/AnimationData.h"
#include "Engine/Core/ComPtr.h"
// DirectXTex
#include "DirectXTex.h"
// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MiiEngine {
	///=====================================================/// 
	/// アニメーションマネージャ
	///=====================================================///
	class AnimationManager {
	public:

		AnimationManager() = default;
		~AnimationManager();

	public:
		/// <summary>
		/// アニメーションデータの読み込み処理
		/// </summary>
		/// <param name="Key">読み込むデータを識別するためのキー。</param>
		/// <param name="baseDirectoryPath">ファイルが存在する基本ディレクトリのパス。</param>
		/// <param name="filename">読み込むファイルの名前またはディレクトリに対する相対パス。</param>
		void Load(const std::string& Key, const std::string& baseDirectoryPath, const std::string& filename);

		/// <summary>
		/// 格納しているアニメーションの取得
		/// </summary>
		/// <param name="filename">読み込むアニメーションファイルのパスまたはファイル名。</param>
		/// <returns>ファイルに含まれる各アニメーションを、名前（std::string）をキー、対応する Animation オブジェクトを値とする std::map。</returns>
		std::map<std::string, Animation> GetAnimation(const std::string& filename);

	private: /// ===Variables(変数)=== ///

		// アニメーションデータ	
		std::map<std::string, std::map<std::string, Animation>> animationDatas_;

	private: /// ===Functions(関数)=== ///

		/// <summary>
		/// アニメーションデータの読み込み処理
		/// </summary>
		/// <param name="directorPath">アニメーションファイルが格納されているディレクトリのパス。</param>
		/// <param name="filename">読み込むアニメーションファイルの名前（必要に応じて拡張子を含む）。</param>
		/// <returns>アニメーション名をキー、対応する Animation オブジェクトを値とする std::map。</returns>
		std::map<std::string, Animation> LoadAnimation(const std::string& directorPath, const std::string& filename);
	};
}

