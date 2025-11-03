#include "DeltaTimeSevice.h"
// DeltaTime
#include "Engine/System/DeltraTime/DeltaTime.h"
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void DeltaTimeSevice::SetDeltaTime(const float& deltaTime) {ServiceLocator::GetDeltaTime()->SetDeltaTime(deltaTime);}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
const float& DeltaTimeSevice::GetDeltaTime() {return ServiceLocator::GetDeltaTime()->GetDeltaTime();}




