#pragma once
/// ===include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>
#include <array>
#include <cassert>
// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

namespace MiiEngine {
	/// ===前方宣言=== ///
	class DXCommon;
	class Compiler;

	///=====================================================/// 
	/// PipelineStateObjectBase
	///=====================================================///
	class PSOBase {
	public:
		virtual ~PSOBase() = default;
	};
}

