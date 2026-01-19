#pragma once
/// ===Include=== ///
// Collider
#include "Engine/Collider/OBBCollider.h"
#include "Engine/Collider/SphereCollider.h"
// C++
#include <type_traits>

/// ===Concepts=== ///
template<typename T>
concept IsCollider = std::is_base_of<Collider, T>::value;