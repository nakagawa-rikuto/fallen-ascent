#pragma once
/// ===Include=== ///
// C++
#include <iostream>
#include <fstream>
#include <map>
// JSON
#include <json.hpp>
// Data
#include "Engine/DataInfo/LevelData.h"

namespace MiiEngine {
	///=====================================================/// 
	/// LevelManager
	///=====================================================///
	class LevelManager {
	public:

		LevelManager() = default;
		~LevelManager() = default;

		/// <summary>
		/// Jsonファイルの読み込み処理
		/// </summary>
		/// <param name="basePath">読み込み時の基準となるベースディレクトリのパス。</param>
		/// <param name="file_path">読み込む JSON ファイルへのパス（basePath を基準とする相対パス、または絶対パス）。</param>
		void LoadLevelJson(const std::string& basePath, const std::string& file_path);

	public: /// ===Getter=== ///
		// レベルデータの取得
		LevelData* GetLevelData(const std::string& file_path);

	private:

		// 格納データ
		std::map<std::string, LevelData*> m_objectMap;

	private:

		/// <summary>
		/// nlohmann::json のオブジェクトを再帰的に読み込み、LevelData に反映する。
		/// </summary>
		/// <param name="object">読み込む JSON データ（nlohmann::json）。関数はこの JSON を再帰的に走査して内容を処理する。</param>
		/// <param name="levelData">読み込み先の LevelData 構造体へのポインタ。関数はこのポインタが指すデータを更新する。</param>
		void LoadobjectRecursive(nlohmann::json& obj, LevelData* levelData);

		/// <summary>
		/// 文字列を LevelData::ClassType に変換する静的メンバー関数。
		/// </summary>
		/// <param name="str">変換対象の文字列（クラスタイプを表す）。</param>
		/// <returns>引数の文字列に対応する LevelData::ClassType の値。</returns>
		static LevelData::ClassTypeLevel StringToClassType(const std::string& str);

		/// <summary>
		/// 文字列を LevelData::ColliderTypeLevel 型の値に変換します。
		/// </summary>
		/// <param name="str">変換する入力文字列。</param>
		/// <returns>入力文字列に対応する LevelData::ColliderTypeLevel の値。対応する値がない場合の挙動は実装依存です。</returns>
		static LevelData::ColliderTypeLevel StringToColliderType(const std::string& str);
	};
}

