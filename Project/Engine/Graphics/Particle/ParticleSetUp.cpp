#include "ParticleSetUp.h"
// c++
#include <cassert>
#include <fstream>
#include <numbers>
// Service
#include "Service/Locator.h"
#include "Service/GraphicsResourceGetter.h"
#include "Service/Render.h"
// Managers
#include "Engine/System/Managers/SRVManager.h"
// camera
#include "application/Game/Camera/GameCamera.h"
// Math
#include "Math/sMath.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// コンストラクタ・デストラクタ
	///-------------------------------------------///
	ParticleSetUp::~ParticleSetUp() {
		// SRVインデックスを解放
		if (srvData_.srvManager_ != nullptr) {
			srvData_.srvManager_->Free(srvData_.srvIndex);
		}
		vertex_.reset();
		index_.reset();
		common_.reset();
	}


	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// Texture
	void ParticleSetUp::SetTexture(const std::string& fileName) { modelData_.material.textureFilePath = fileName; }
	// Material
	void ParticleSetUp::SetMaterialData(const Vector4& color, const Matrix4x4& uvTransform) { common_->SetMaterialData(color, uvTransform); }
	// instancing
	void ParticleSetUp::SetInstancingData(size_t index, const Vector4& color, const Matrix4x4& WVP, const Matrix4x4& World) { common_->SetInstancingData(index, color, WVP, World); }


	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void ParticleSetUp::Initialize(const std::string& filename, const uint32_t kNumMaxInstance, shapeType type) {

		/// ===コマンドリストのポインタの取得=== ///
		ID3D12Device* device = Service::GraphicsResourceGetter::GetDXDevice();

		// 引数の値を代入
		kNumMaxInstance_ = kNumMaxInstance;

		/// ===モデル読み込み=== ///
		modelData_ = Service::GraphicsResourceGetter::GetModelData(filename); // ファイルパス

		/// ===生成=== ///
		vertex_ = std::make_unique<VertexBuffer3D>();
		index_ = std::make_unique<IndexBuffer3D>();
		common_ = std::make_unique<ParticleCommon>();

		// 円形の場合は必要な頂点数とインデックス数を計算
		size_t vertexCount = modelData_.vertices.size();
		size_t indexCount = modelData_.indices.size();

		if (type == shapeType::kCircle) {
			const uint32_t kRingDivide = 32;
			vertexCount = static_cast<size_t>(kRingDivide) * 4;  // 各セグメント4頂点
			indexCount = static_cast<size_t>(kRingDivide) * 6;   // 各セグメント6インデックス
		} else if (type == shapeType::kCylinder) {
			const uint32_t kCylinderDivide = 32;
			vertexCount = static_cast<size_t>(kCylinderDivide) * 6;  // 各セグメント6頂点
			indexCount = static_cast<size_t>(kCylinderDivide) * 6;   // 各セグメント6インデックス
		}

		/// ===vertex=== ///
		// Buffer
		vertex_->Create(device, sizeof(VertexData3D) * vertexCount);
		vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
		// メモリコピー
		std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData3D) * modelData_.vertices.size());
		// view
		vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
		vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * vertexCount);
		vertexBufferView_.StrideInBytes = sizeof(VertexData3D);

		/// ===index=== ///
		index_->Create(device, sizeof(uint32_t) * indexCount);
		index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
		// メモリコピー
		std::memcpy(indexData_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
		// view
		indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress();
		indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indexCount);
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

		// タイプ次第でVertexBufferを変更する
		if (type == shapeType::kCircle) {
			// 円
			SetVertexBufferCircle();
		} else if (type == shapeType::kCylinder) {
			// cylinder
			SetVertexBufferCylinder();
		}

		// 頂点数とインデックス数を保存
		modelData_.vertices.resize(vertexCount);
		modelData_.indices.resize(indexCount);

		/// ===SetUp=== ///
		common_->Initialize(device, kNumMaxInstance_);

		/// ===SRV=== ///
		srvData_.srvManager_ = Service::Locator::GetSRVManager(); // SRVManagerの取得
		// SRVを作成するDescriptorHeapの場所設定
		srvData_.srvIndex = srvData_.srvManager_->Allocate();
		srvData_.srvHandleCPU = srvData_.srvManager_->GetCPUDescriptorHandle(srvData_.srvIndex);
		srvData_.srvHandleGPU = srvData_.srvManager_->GetGPUDescriptorHandle(srvData_.srvIndex);
		// SRVの生成
		srvData_.srvManager_->CreateSRVForStructuredBuffer(
			srvData_.srvIndex, common_->GetInstancing(), kNumMaxInstance_, sizeof(ParticleForGPU));
	}


	///-------------------------------------------/// 
	/// 描画
	///-------------------------------------------///
	void ParticleSetUp::Draw(const uint32_t instance, BlendMode mode) {

		/// ===コマンドリストのポインタの取得=== ///
		ID3D12GraphicsCommandList* commandList = Service::GraphicsResourceGetter::GetDXCommandList();

		/// ===コマンドリストに設定=== ///
		// PSOの設定
		Service::Render::SetPSO(commandList, PipelineType::Particle, mode);
		// VertexBufferViewの設定
		commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
		// IndexBufferViewの設定
		commandList->IASetIndexBuffer(&indexBufferView_);
		// materialの設定
		common_->Bind(commandList);
		// Instancingの設定
		commandList->SetGraphicsRootDescriptorTable(1, srvData_.srvHandleGPU);
		// テクスチャの設定
		Service::Render::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
		// 描画（Drawコール）
		commandList->DrawIndexedInstanced(UINT(modelData_.indices.size()), instance, 0, 0, 0);
	}

	///-------------------------------------------/// 
	/// VertexBufferの設定
	///-------------------------------------------///
	// 円
	void ParticleSetUp::SetVertexBufferCircle() {
		// 変数の宣言
		const uint32_t kRingDivide = 32;  // 円の分割数
		const float kOuterRadius = 4.5f;  // 外半径
		const float kInnerRadius = 1.0f;  // 内半径（0にすると円盤、値があるとドーナツ）
		const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide);

		// 頂点データの書き込み
		for (uint32_t index = 0; index < kRingDivide; ++index) {
			float currentAngle = index * radianPerDivide;
			float nextAngle = (index + 1) * radianPerDivide;

			float sin = std::sin(currentAngle);
			float cos = std::cos(currentAngle);
			float sinNext = std::sin(nextAngle);
			float cosNext = std::cos(nextAngle);

			float u = static_cast<float>(index) / float(kRingDivide);
			float uNext = static_cast<float>(index + 1) / float(kRingDivide);

			// ★重要：インデックスを正しく計算
			uint32_t baseIndex = index * 4;

			// 外側の頂点1
			vertexData_[baseIndex + 0].position = { cos * kOuterRadius, 0.0f, sin * kOuterRadius, 1.0f };
			vertexData_[baseIndex + 0].texcoord = { u, 0.0f };
			vertexData_[baseIndex + 0].normal = { 0.0f, 1.0f, 0.0f };  // 上向き

			// 外側の頂点2
			vertexData_[baseIndex + 1].position = { cosNext * kOuterRadius, 0.0f, sinNext * kOuterRadius, 1.0f };
			vertexData_[baseIndex + 1].texcoord = { uNext, 0.0f };
			vertexData_[baseIndex + 1].normal = { 0.0f, 1.0f, 0.0f };

			// 内側の頂点1
			vertexData_[baseIndex + 2].position = { cos * kInnerRadius, 0.0f, sin * kInnerRadius, 1.0f };
			vertexData_[baseIndex + 2].texcoord = { u, 1.0f };
			vertexData_[baseIndex + 2].normal = { 0.0f, 1.0f, 0.0f };

			// 内側の頂点2
			vertexData_[baseIndex + 3].position = { cosNext * kInnerRadius, 0.0f, sinNext * kInnerRadius, 1.0f };
			vertexData_[baseIndex + 3].texcoord = { uNext, 1.0f };
			vertexData_[baseIndex + 3].normal = { 0.0f, 1.0f, 0.0f };
		}

		// インデックスデータの書き込み
		for (uint32_t index = 0; index < kRingDivide; ++index) {
			uint32_t baseVertex = index * 4;
			uint32_t baseIndex = index * 6;

			// 三角形1:  (0, 1, 2)
			indexData_[baseIndex + 0] = baseVertex + 0;
			indexData_[baseIndex + 1] = baseVertex + 1;
			indexData_[baseIndex + 2] = baseVertex + 2;

			// 三角形2: (2, 1, 3)
			indexData_[baseIndex + 3] = baseVertex + 2;
			indexData_[baseIndex + 4] = baseVertex + 1;
			indexData_[baseIndex + 5] = baseVertex + 3;
		}
	}

	// Cylinder
	void ParticleSetUp::SetVertexBufferCylinder() {
		// 変数の宣言
		const uint32_t kCylinderDivide = 32;   // 円柱の分割数
		const float kTopRadius = 0.5f;         // 上部の半径
		const float kBottomRadius = 0.5f;      // 下部の半径
		const float kHeight = 3.0f;            // 高さ
		const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kCylinderDivide);

		// 頂点データの書き込み
		for (uint32_t index = 0; index < kCylinderDivide; ++index) {
			float currentAngle = index * radianPerDivide;
			float nextAngle = (index + 1) * radianPerDivide;

			float sin = std::sin(currentAngle);
			float cos = std::cos(currentAngle);
			float sinNext = std::sin(nextAngle);
			float cosNext = std::cos(nextAngle);

			float u = static_cast<float>(index) / static_cast<float>(kCylinderDivide);
			float uNext = static_cast<float>(index + 1) / static_cast<float>(kCylinderDivide);

			// ★重要：インデックスを正しく計算（各セグメント6頂点）
			uint32_t baseIndex = index * 6;

			// 上部の頂点1
			vertexData_[baseIndex + 0].position = { cos * kTopRadius, kHeight, sin * kTopRadius, 1.0f };
			vertexData_[baseIndex + 0].texcoord = { u, 0.0f };
			vertexData_[baseIndex + 0].normal = { cos, 0.0f, sin };

			// 上部の頂点2
			vertexData_[baseIndex + 1].position = { cosNext * kTopRadius, kHeight, sinNext * kTopRadius, 1.0f };
			vertexData_[baseIndex + 1].texcoord = { uNext, 0.0f };
			vertexData_[baseIndex + 1].normal = { cosNext, 0.0f, sinNext };

			// 下部の頂点1
			vertexData_[baseIndex + 2].position = { cos * kBottomRadius, 0.0f, sin * kBottomRadius, 1.0f };
			vertexData_[baseIndex + 2].texcoord = { u, 1.0f };
			vertexData_[baseIndex + 2].normal = { cos, 0.0f, sin };

			// 下部の頂点1（重複 - 三角形2用）
			vertexData_[baseIndex + 3].position = { cos * kBottomRadius, 0.0f, sin * kBottomRadius, 1.0f };
			vertexData_[baseIndex + 3].texcoord = { u, 1.0f };
			vertexData_[baseIndex + 3].normal = { cos, 0.0f, sin };

			// 上部の頂点2（重複 - 三角形2用）
			vertexData_[baseIndex + 4].position = { cosNext * kTopRadius, kHeight, sinNext * kTopRadius, 1.0f };
			vertexData_[baseIndex + 4].texcoord = { uNext, 0.0f };
			vertexData_[baseIndex + 4].normal = { cosNext, 0.0f, sinNext };

			// 下部の頂点2
			vertexData_[baseIndex + 5].position = { cosNext * kBottomRadius, 0.0f, sinNext * kBottomRadius, 1.0f };
			vertexData_[baseIndex + 5].texcoord = { uNext, 1.0f };
			vertexData_[baseIndex + 5].normal = { cosNext, 0.0f, sinNext };
		}

		// インデックスデータの書き込み
		for (uint32_t index = 0; index < kCylinderDivide; ++index) {
			uint32_t baseVertex = index * 6;
			uint32_t baseIndex = index * 6;

			// 三角形1:  (0, 1, 2)
			indexData_[baseIndex + 0] = baseVertex + 0;
			indexData_[baseIndex + 1] = baseVertex + 1;
			indexData_[baseIndex + 2] = baseVertex + 2;

			// 三角形2: (3, 4, 5)
			indexData_[baseIndex + 3] = baseVertex + 3;
			indexData_[baseIndex + 4] = baseVertex + 4;
			indexData_[baseIndex + 5] = baseVertex + 5;
		}
	}
}
