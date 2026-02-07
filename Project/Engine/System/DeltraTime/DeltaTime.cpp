#include "DeltaTime.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	const float& DeltaTime::GetDeltaTime() const { return deltaTime_; }

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	void DeltaTime::SetDeltaTime(const float& deltaTime) { deltaTime_ = deltaTime; }
}