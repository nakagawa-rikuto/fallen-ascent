#pragma once
/// ===Include=== ///
// Data
#include "Engine/DataInfo/LightData.h"

/// ===前方宣言=== ///
namespace MiiEngine {
	class Collider;
}

namespace Service {
	///-------------------------------------------/// 
	/// Collision
	///-------------------------------------------///
	class Collision {
	public:

		/// <summary>
		/// 内部状態をリセットします。
		/// </summary>
		static void Reset();

		/// <summary>
		/// 指定されたコライダーを削除します。
		/// </summary>
		/// <param name="collider">削除対象のコライダーへのポインタ。呼び出す前にポインタが有効であることを確認してください。</param>
		static void RemoveCollider(MiiEngine::Collider* collider);

		/// <summary>
		/// 指定されたコライダーを追加します。
		/// </summary>
		/// <param name="collider">追加するコライダーへのポインタ。</param>
		static void AddCollider(MiiEngine::Collider* collider);

		/// <summary>
		/// ライトの種類を設定します。
		/// </summary>
		/// <param name="type">設定するライトの種類を指定します。</param>
		static void SetLight(MiiEngine::LightType type);

		/// <summary>
		/// ライトのデータを設定します。
		/// </summary>
		/// <param name="light">設定するライト情報を格納した LightInfo オブジェクト。</param>
		static void SetLightData(MiiEngine::LightInfo light);
	};
}

