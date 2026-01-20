#pragma once
/// ===Include=== ///
// Collider
#include "Base/Collider.h"
// C++
#include <type_traits>

/// ===Concepts=== ///
template<typename T>
concept IsCollider = std::is_base_of_v<Collider, T>;