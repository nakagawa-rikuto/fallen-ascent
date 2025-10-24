#pragma once
/// ===Include=== ///
#include <string>

///=====================================================/// 
/// AudioService
///=====================================================///
class AudioService {
public:
	
	/// <summary>
	/// 指定されたキーに対応する音声を再生する静的関数。
	/// </summary>
	/// <param name="key">再生する音声を識別するキーを表す文字列。</param>
	/// <param name="loop">再生をループするかどうかを指定します。true の場合はループ再生、false の場合は一度だけ再生します。</param>
	static void PlayeSound(const std::string& key, bool loop);

	/// <summary>
	/// 指定したキーに対応する再生中のサウンドを停止します。
	/// </summary>
	/// <param name="key">停止するサウンドを識別する文字列キー。</param>
	static void StopSound(const std::string& key);

	/// <summary>
	/// 指定したキーに対応するサウンドの音量を設定します。
	/// </summary>
	/// <param name="key">サウンドを識別する文字列キー。</param>
	/// <param name="volume">設定する音量を表す浮動小数点値。範囲や単位は実装に依存します。</param>
	static void VolumeSound(const std::string& key, float volume);

	/// <summary>
	/// 指定されたキーに対応するサウンドを指定したピッチで再生します。
	/// </summary>
	/// <param name="key">再生するサウンドを識別する文字列キー。通常はサウンドリソースやエフェクトの名前を指定します。</param>
	/// <param name="pitch">再生時のピッチを表す浮動小数点値。値の解釈（倍率やセミトーンなど）は実装によって異なります。</param>
	static void PitchSound(const std::string& key, float pitch);
};

