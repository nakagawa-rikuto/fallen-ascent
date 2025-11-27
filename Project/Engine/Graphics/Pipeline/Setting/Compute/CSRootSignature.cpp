#include "CSRootSignature.h"
// Engine
#include "Engine/Core/Logger.h"
#include "Engine/Core/DXCommon.h"
// c++
#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
CSRootSignature::~CSRootSignature() {
	rootSignature_.Reset();
}

///-------------------------------------------/// 
/// パイプラインの設定
///-------------------------------------------///
namespace {

	/// ===CSSkinning=== ///
	ComPtr<ID3D12RootSignature> TypeCSSkinning(ID3D12Device* device) {
		// SRV用のDescriptorRange (t0, t1, t2)
		D3D12_DESCRIPTOR_RANGE srvRange = {};
		srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		srvRange.NumDescriptors = 3; // t0:MatrixPalette, t1:InputVertices, t2:Influences
		srvRange.BaseShaderRegister = 0;
		srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// UAV用のDescriptorRange (u0)
		D3D12_DESCRIPTOR_RANGE uavRange = {};
		uavRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		uavRange.NumDescriptors = 1; // u0:OutputVertices
		uavRange.BaseShaderRegister = 0;
		uavRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// RootParameterの生成
		D3D12_ROOT_PARAMETER rootParameters[3] = {};
		// [0] SRV - 入力リソース (t0, t1, t2)
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &srvRange;
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		// [1] UAV - 出力リソース (u0)
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[1].DescriptorTable.pDescriptorRanges = &uavRange;
		rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		// [2] CBV - 定数バッファ (b0:SkinningInformation)
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[2].Descriptor.ShaderRegister = 0;

		// RootSignatureの生成
		D3D12_ROOT_SIGNATURE_DESC desc{};
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE; // ComputeではALLOW_INPUT_ASSEMBLER不要
		desc.pParameters = rootParameters;
		desc.NumParameters = _countof(rootParameters);
		desc.pStaticSamplers = nullptr; // Computeではサンプラー不要
		desc.NumStaticSamplers = 0;

		// --- シリアライズ & 作成 ---
		ComPtr<ID3DBlob> signatureBlob;
		ComPtr<ID3DBlob> errorBlob;
		HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		if (FAILED(hr)) {
			if (errorBlob) {
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}
			assert(false);
			return nullptr;
		}

		ComPtr<ID3D12RootSignature> rootSignature;
		hr = device->CreateRootSignature(
			0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));

		return rootSignature;
	}

	/// ===マップに登録=== ///
	using RootSigGenerator = std::function<ComPtr<ID3D12RootSignature>(ID3D12Device*)>;

	// IsComputePipeline関数を使用して、Compute用のみを登録
	std::unordered_map<PipelineType, RootSigGenerator> CreateCSRootSignatureTable() {
		std::unordered_map<PipelineType, RootSigGenerator> table;

		// 全てのPipelineTypeをチェック
		for (PipelineType type : AllPipelineTypes()) {
			// Computeパイプラインのみ登録
			if (IsComputePipeline(type)) {
				switch (type) {
				case PipelineType::Skinning3D:
					table[type] = TypeCSSkinning;
					break;
				default:
					// Computeと判定されたが、対応する関数がない場合
					assert(false && "CS RootSignature not implemented for this type");
					break;
				}
			}
		}

		return table;
	}

	const std::unordered_map<PipelineType, RootSigGenerator> kCSRootSignatureTable_ = CreateCSRootSignatureTable();
}

///-------------------------------------------/// 
/// ルートシグネイチャの生成
///-------------------------------------------///
void CSRootSignature::Create(DXCommon* dxCommon, PipelineType Type) {
	// Computeパイプラインかチェック
	assert(IsComputePipeline(Type) && "CSRootSignature can only be used for Compute Pipelines");

	auto it = kCSRootSignatureTable_.find(Type);
	if (it != kCSRootSignatureTable_.end()) {
		// 直接 RootSignature を作って返す構成に変更
		rootSignature_ = it->second(dxCommon->GetDevice());
	} else {
		assert(false && "Unknown Compute PipelineType in CSRootSignature");
	}
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ID3D12RootSignature* CSRootSignature::GetRootSignature() const {return rootSignature_.Get();}