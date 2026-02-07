#pragma once
/// ===Include=== ///

namespace MiiEngine {
	///=====================================================/// 
	/// DeltaTime
	///=====================================================///
	class DeltaTime {
	public:
		DeltaTime() = default;
		~DeltaTime() = default;

	public: /// ===Getter=== ///
		// デルタタイムの取得
		const float& GetDeltaTime() const;

	public: /// ===Setter=== ///
		// デルタタイムの設定
		void SetDeltaTime(const float& deltaTime);

	private:

		// DeltaTime
		float deltaTime_ = 1.0f / 60.0f;
	};
}

