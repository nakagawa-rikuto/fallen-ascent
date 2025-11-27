#pragma once
/// ===include=== ///
#include "Engine/DataInfo/BlendModeData.h"
// C++
#include <cassert>

///-------------------------------------------/// 
/// PipelineType
///-------------------------------------------///
enum PipelineType {
	// 前景2D
	ForGround2D,
	// 背景2D
	BackGround2D,
	// 3D
	Obj3D,
	// SkyBox
	PrimitiveSkyBox,
	// Oshan
	PrimitiveOshan,
	// Particle
	Particle,
	// Skinning3D
	Skinning3D,
	// Line3D
	Line3D,
	// OffScreen
	OffScreen,
	// Grayscale
	Grayscale,
	// Vignette
	Vignette,
	// Dissolve
	Dissolve,
	// BoxFilter3x3
	BoxFilter3x3,
	// BoxFilter5x5
	BoxFilter5x5,
	// RadiusBlur
	RadiusBlur,
	// OutLine
	OutLine,
	// ShatterGlass
	ShatterGlass,

	// 総数(これは最後にしなければいけない)
	CountOfPipelineType,
};

// 列挙型の全値を取得する関数
constexpr std::array<PipelineType, static_cast<size_t>(PipelineType::CountOfPipelineType)> AllPipelineTypes() {
	return {
		PipelineType::ForGround2D,
		PipelineType::BackGround2D,
		PipelineType::Obj3D,
		PipelineType::PrimitiveSkyBox,
		PipelineType::Particle ,
		PipelineType::Skinning3D,
		PipelineType::PrimitiveOshan,
		PipelineType::Line3D,
		PipelineType::OffScreen,
		PipelineType::Grayscale,
		PipelineType::Vignette,
		PipelineType::Dissolve,
		PipelineType::BoxFilter3x3,
		PipelineType::BoxFilter5x5,
		PipelineType::RadiusBlur,
		PipelineType::OutLine,
		PipelineType::ShatterGlass
	};
}

// Computeパイプラインかどうかを判定する関数
inline bool IsComputePipeline(PipelineType type) {
	switch (type) {
	/// ===Compute Pipelines=== ///
	case PipelineType::Skinning3D:
		return true;

	/// ===Graphics Pipelines=== ///
	case PipelineType::ForGround2D:
	case PipelineType::BackGround2D:
	case PipelineType::Obj3D:
	case PipelineType::PrimitiveSkyBox:
	case PipelineType::Particle:
	case PipelineType::PrimitiveOshan:
	case PipelineType::Line3D:
	case PipelineType::OffScreen:
	case PipelineType::Grayscale:
	case PipelineType::Vignette:
	case PipelineType::Dissolve:
	case PipelineType::BoxFilter3x3:
	case PipelineType::BoxFilter5x5:
	case PipelineType::RadiusBlur:
	case PipelineType::OutLine:
	case PipelineType::ShatterGlass:
		return false;

	default:
		assert(false && "Unknown PipelineType");
		return false;
	}
}