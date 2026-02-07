#pragma once
/// ===include=== ///
// Buffer
#include "Engine/Graphics/3d/Base/VertexBuffer3D.h"
#include "Engine/Graphics/3d/Base/IndexBuffer3D.h"
#include "Engine/Graphics/3d/Base/ObjectCommon.h"
// Data
#include "Engine/DataInfo/BlendModeData.h"
// c++
#include <memory>
#include <string>

/// ===前方宣言=== ///
class GameCamera;

namespace MiiEngine {
	///=====================================================/// 
	/// モデル共通部
	///=====================================================///
	class ModelCommon {
	public:
		ModelCommon() = default;
		~ModelCommon();

		/// <summary>
		/// 初期化処理、順数仮想関数
		/// </summary>
		/// <param name="modelName">初期化に使用するモデル名。</param>
		/// <param name="type">ライトの種類を表す値（LightType）。</param>
		virtual void Initialize(const std::string& modelName, LightType type) = 0;

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画処理、純粋仮想関数
		/// </summary>
		/// <param name="mode">描画に使用するブレンドモード。BlendMode 列挙型で合成方法を指定します。</param>
		virtual void Draw(BlendMode mode) = 0;

		/// <summary>
		/// 生成処理
		/// </summary>
		/// <param name="device">リソースの作成に使用するID3D12Deviceへのポインター。nullであってはなりません。</param>
		/// <param name="type">作成するライトの種類を示すLightTypeの値。</param>
		void Create(ID3D12Device* device, LightType type);

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="commandList">バインドおよび操作に使用する ID3D12GraphicsCommandList へのポインター。</param>
		void Bind(ID3D12GraphicsCommandList* commandList);

	public: /// ===親子関係=== ///
		/// <summary>
		/// 親オブジェクトを設定
		/// </summary>
		/// <param name="parent">親となる ModelCommon 型のポインタ。設定する親オブジェクトを指します。nullptr の取り扱い（親の解除を意味するかどうかなど）は実装に依存します。</param>
		void SetParent(ModelCommon* parent);
		/// <summary>
		///  親の解除
		/// </summary>
		void ClearParent();
		// Offset
		void SetParentOffset(const Vector3& offset);
		const Vector3& GetParentOffset() const;

	public:/// ===Setter=== ///
		// Translate
		void SetTranslate(const Vector3& position);
		// Rotate
		void SetRotate(const Quaternion& rotate);
		// Scale
		void SetScale(const Vector3& scale);
		// Color
		void SetColor(const Vector4& color);
		// Light
		void SetLightType(LightType type);
		// LightData
		void SetLightData(LightInfo light);
		// 環境マップ
		void SetEnvironmentMapData(bool flag, float string);

	public: /// ===Getter=== ///
		// Transform（位置、回転、拡縮）を取得
		const Vector3& GetWorldTranslate() const;
		const Quaternion& GetWorldRotate() const;
		const Vector3& GetWorldScale() const;
		const QuaternionTransform& GetWorldTransform() const;
		// Color（色）を取得
		const Vector4& GetColor() const;

	protected: /// ===継承先で使用する変数=== ///

		/// ===バッファビュー=== ///
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		/// ===バッファリソース内のデータを指すポインタ=== ///
		VertexData3D* vertexData_ = nullptr;
		uint32_t* indexData_ = nullptr;

		/// ===モデル情報=== ///
		ModelData modelData_;
		EulerTransform uvTransform_;
		QuaternionTransform worldTransform_;
		Vector4 color_;

		/// ===親子関係=== ///
		ModelCommon* parent_ = nullptr;
		Vector3 parentOffset_ = { 0.0f,0.0f,0.0f };

		/// ===Light=== ///
		LightInfo light_;

		/// ===Camera=== ///
		GameCamera* camera_ = nullptr;

		/// ===環境マップ=== ///
		EnvironmentMapInfo environmentMapInfo_;

	private:/// ===Variables(変数)=== ///

		// バッファリソース
		std::unique_ptr<VertexBuffer3D> vertex_;
		std::unique_ptr<IndexBuffer3D> index_;
		std::unique_ptr<ObjectCommon> common_;

		// ワールド行列
		Matrix4x4 worldMatrix_;

		// Getter用
		mutable QuaternionTransform cachedWorldTransform_;
		struct WorldTransformCacheDirtyTag {
			mutable bool translate = false;
			mutable bool rotate = false;
			mutable bool scale = false;
		};
		WorldTransformCacheDirtyTag worldTransformCacheDirtyTag_;

	private:
		/// <summary>
		/// MaterialDataの書き込み処理
		/// </summary>
		void MaterialDataWrite();

		/// <summary>
		/// Transform情報の書き込み処理
		/// </summary>
		void TransformDataWrite();

		/// <summary>
		/// LightData書き込み処理
		/// </summary>
		void LightDataWrite();

		/// <summary>
		/// CameraData書き込み処理
		/// </summary>
		void CameraDataWrite();

		/// <summary>
		/// EnvironmentMapData書き込み処理
		/// </summary>
		void EnvironmentMapDataWrite();
	};
}

