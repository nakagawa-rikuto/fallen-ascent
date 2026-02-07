#pragma once
/// ===Include=== ///

namespace Service {
	///=====================================================/// 
	/// DeltaTimeService
	///=====================================================///
	class DeltaTimeSevice {
	public:

		/// <summary>
		/// 前フレームからの経過時間（デルタタイム）を取得
		/// </summary>
		/// <returns>前フレームからの経過時間を表す const float 型への参照。通常は秒単位で格納された内部値への参照を返します（変更しないでください）。</returns>
		static const float& GetDeltaTime();

		/// <summary>
		/// 静的関数。フレーム間の経過時間（デルタタイム）を設定
		/// </summary>
		/// <param name="deltaTime">設定する経過時間（float の const 参照）。通常は秒単位で指定します。</param>
		static void SetDeltaTime(const float& deltaTime);
	};
}
