#pragma once
/// ===Include=== ///
// c++
#include <string>
#include <memory>

/// ===前方宣言=== ///
class GameCamera;

namespace Service {
	///=====================================================/// 
	/// CameraService
	///=====================================================///
	class CameraService {
	public:

		/// <summary>
		/// 指定した名前でカメラを登録する静的関数。カメラ管理システムに新しいカメラを追加します。
		/// </summary>
		/// <param name="name">登録するカメラの識別名を示す文字列。</param>
		/// <param name="camera">追加する GameCamera オブジェクトへの std::shared_ptr。管理対象のカメラを指すスマートポインタを渡します。</param>
		static void AddCamera(const std::string& name, std::shared_ptr<GameCamera> camera);

		/// <summary>
		/// 指定された名前の項目を削除する静的メソッドです。
		/// </summary>
		/// <param name="name">削除対象の名前を指定する文字列（const std::string&）。</param>
		static void Remove(const std::string& name);

		/// <summary>
		/// 指定された名前のカメラの存在を確認します。
		/// </summary>
		/// <param name="name">確認対象のカメラ名を表す文字列参照。</param>
		static void HasCamera(const std::string& name);

		/// <summary>
		/// 現在アクティブなカメラへの共有ポインタを取得します（静的メンバ関数）。
		/// </summary>
		/// <returns>アクティブな GameCamera を指す std::shared_ptr<GameCamera>。アクティブなカメラが存在しない場合は nullptr を返します。</returns>
		static std::shared_ptr<GameCamera> GetActiveCamera();

		/// <summary>
		/// 指定した名前のカメラをアクティブに設定します。
		/// </summary>
		/// <param name="name">アクティブにするカメラの名前。</param>
		static void SetActiveCamera(const std::string& name);
	};
}

