#include "GameTime.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	const float& GameTime::GetGameTime() const { return gameTime_; }

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	void GameTime::SetGameTime(const float& deltaTime) { gameTime_ = deltaTime; }
}