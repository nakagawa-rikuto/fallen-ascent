#pragma once
/// ===Include=== ///
// C++
#include <string>

namespace Service {
	///=====================================================/// 
	/// Load関数
	///=====================================================///
	class Loader {
	public:
		/// <summary>
		/// テクスチャの読み込み処理
		/// </summary>
		/// <param name="key">テクスチャを識別および参照するためのキー。</param>
		/// <param name="filePath">読み込むテクスチャファイルのパス。</param>
		static void LoadTexture(const std::string& key, const std::string& filePath);

		/// <summary>
		/// モデルの読み込み処理
		/// </summary>
		/// <param name="Key">読み込むモデルを識別するためのキー。</param>
		/// <param name="filename">モデルデータが格納されているファイルのパス。</param>
		static void LoadModel(const std::string& Key, const std::string& filename);

		/// <summary>
		/// アニメーションモデルの読み込み処理
		/// </summary>
		/// <param name="Key">読み込んだアニメーションに割り当てる識別キー。</param>
		/// <param name="filename">読み込むアニメーションファイルのパスまたはファイル名。</param>
		static void LoadAnimation(const std::string& Key, const std::string& filename);

		/// <summary>
		/// アニメーションのみの読み込み処理
		/// </summary>
		/// <param name="directorPath">アニメーションおよびモデルファイルが格納されているディレクトリへのパス。</param>
		/// <param name="ModelFilename">読み込むモデルファイルの名前（ファイル名）。</param>
		/// <param name="AnimationFilename">読み込むアニメーションファイルの名前（ファイル名）。</param>
		static void LoadAnimationdifferentModel(const std::string& directorPath, const std::string& ModelFilename, const std::string& AnimationFilename);

		/// <summary>
		/// CSVファイルの読み込み処理
		/// </summary>
		/// <param name="filename">読み込む CSV ファイルのパスまたはファイル名。</param>
		static void LoadCSV(const std::string& filename);

		/// <summary>
		/// Jsonファイルの読み込み処理
		/// </summary>
		/// <param name="filename">読み込むJSONファイルのパスまたは名前。</param>
		static void LoadLevelJson(const std::string& filename);

		/// <summary>
		/// Waveファイルの読み込み処理
		/// </summary>
		/// <param name="key">読み込んだ音声を識別するための一意のキー。</param>
		/// <param name="filename">読み込むWAVファイルのパスまたはファイル名。</param>
		static void LoadWave(const std::string& key, const std::string& filename);

		/// <summary>
		/// MP3ファイルの読み込み処理
		/// </summary>
		/// <param name="key">MP3 リソースを識別するための一意のキー。</param>
		/// <param name="filename">読み込む MP3 ファイルのパスまたはファイル名。</param>
		static void LoadMP3(const std::string& key, const std::string& filename);

		/// <summary>
		/// サウンドデータの解放処理
		/// </summary>
		/// <param name="key">解放するサウンドを識別するキー。</param>
		static void UnloadSound(const std::string& key);

		/// <summary>
		/// サウンドデータの一括開放処理
		/// </summary>
		static void AllUnloadSound();
	};
}
