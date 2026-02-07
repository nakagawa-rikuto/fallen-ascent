#pragma once
/// ===Include=== ///
// buffer
#include "Engine/Graphics/3d/Base/VertexBuffer3D.h"
#include "Engine/Graphics/3d/Base/IndexBuffer3D.h"
#include "Engine/Graphics/3d/Base/ObjectCommon.h"
// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>

/// ===前方宣言=== ///
class GameCamera;

namespace MiiEngine {
	///=====================================================/// 
	/// SkyBox
	///=====================================================///
	class SkyBox {
	public:

		SkyBox() = default;
		~SkyBox();

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="fileName">初期化に使用するファイルの名前またはパス。</param>
		/// <param name="type">初期化時に使用するライトの種類を表す LightType の値。</param>
		void Initialize(const std::string& fileName, LightType type);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="mode">描画時に使用するブレンドモード。省略した場合は BlendMode::kBlendModeNone（ブレンドなし）が使用されます。</param>
		void Draw(BlendMode mode = BlendMode::kBlendModeNone);

	public: /// ===Getter=== ///
		// モデル座標
		const Vector3& GetTranslate() const;
		// モデル回転
		const Quaternion& GetRotate() const;
		// モデル拡縮
		const Vector3& GetScale() const;
		// モデルカラー
		const Vector4& GetColor() const;

	public: /// ===Setter=== ///
		// モデルTransform
		void SetTranslate(const Vector3& position);
		void SetRotate(const Quaternion& rotate);
		void SetScale(const Vector3& scale);
		// モデルカラー
		void SetColor(const Vector4& color);
		// Light
		void SetLight(LightType type);
		// LightData
		void SetLightData(LightInfo light);
		// 環境マップ
		void SetEnvironmentMapData(bool flag, float string);

	private:
		/// ===バッファリソース=== ///
		std::unique_ptr<VertexBuffer3D> vertex_;
		std::unique_ptr<IndexBuffer3D> index_;
		std::unique_ptr<ObjectCommon> common_;

		/// ===バッファリソース内のデータを指すポインタ=== ///
		VertexData3D* vertexData_ = nullptr;
		uint32_t* indexData_ = nullptr;

		/// ===バッファビュー=== ///
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		/// ===定数=== ///
		static constexpr int kVertexCount = 24;
		static constexpr int kIndexCount = 36;

		/// ===UV=== ///
		EulerTransform uvTransform_;

		/// ===カメラ=== ///
		GameCamera* camera_ = nullptr;

		/// ===プリミティブ情報=== ///
		QuaternionTransform worldTransform_;
		Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// ===ファイルパス=== ///
		std::string textureFilePath_;

		/// ===Light=== ///
		LightInfo light_ = {
			40.0f,
			{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, -1.0f, 0.0f } ,1.0f},
			{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 1.0f, 0.0f, 0.0f},
			{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 0.0f, 0.0f } , 0.0f, { 0.0f, 0.0f, 0.0f } , 0.0f, 0.0f, 0.0f}
		};

		/// ===環境マップ=== ///
		EnvironmentMapInfo environmentMapInfo_;

	private: /// ===Functions(関数)=== ///

		/// <summary>
		/// VertexDataの書き込み処理
		/// </summary>
		void VertexDataWrite();

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

