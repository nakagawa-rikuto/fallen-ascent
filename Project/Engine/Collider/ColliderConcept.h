#pragma once
/// ===Include=== ///
// Collider
#include "Engine/Collider/Base/Collider.h"
// C++
#include <type_traits>

namespace MiiEngine {
	/// ===Concepts=== ///
	template<typename T>
	concept IsCollider = std::is_base_of_v<Collider, T> && !std::is_abstract_v<T>&& std::is_default_constructible_v<T>;
}