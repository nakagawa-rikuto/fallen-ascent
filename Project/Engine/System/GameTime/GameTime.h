#pragma once
/// ===Include=== ///

namespace MiiEngine {
	///=====================================================/// 
	/// DeltaTime
	///=====================================================///
	class GameTime {
	public:
		GameTime() = default;
		~GameTime() = default;

	public: /// ===Getter=== ///
		// デルタタイムの取得
		const float& GetGameTime() const;

	public: /// ===Setter=== ///
		// デルタタイムの設定
		void SetGameTime(const float& deltaTime);

	private:
		// GameTime
		float gameTime_ = 1.0f / 60.0f;
	};
}

