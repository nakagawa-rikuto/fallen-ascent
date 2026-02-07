#pragma once
/// ===include=== ///
// Engine
#include "Engine/Graphics/2d/Base/Object2DCommon.h"
#include "Engine/Graphics/2d/Base/VertexBuffer2D.h"
#include "Engine/Graphics/2d/Base/IndexBuffer2D.h"
// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"

namespace MiiEngine {
	/// ===描画位置の種類=== ///
	enum class GroundType {
		Front,
		Back
	};

	///=====================================================/// 
	/// スプライト共通部
	///=====================================================///
	class SpriteCommon {
	public: /// ===基本的な関数=== ///

		SpriteCommon() = default;
		~SpriteCommon();

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="textureFilePath">読み込むテクスチャファイルへのパス。関数はこのファイルからテクスチャを読み込み初期化を行います。</param>
		virtual void Initialize(const std::string textureFilePath);

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="type">描画する地面の種類を指定します（GroundType）。</param>
		/// <param name="mode">描画に使用するブレンドモードを指定します（BlendMode）。</param>
		virtual void Draw(GroundType type, BlendMode mode);

	private:/// ===Variables(変数)=== ///

		// バッファリソース
		std::unique_ptr<VertexBuffer2D> vertex_;
		std::unique_ptr<IndexBuffer2D> index_;
		std::unique_ptr<Object2DCommon> common_;

		// サイズ
		uint32_t vertexSize_ = 6;
		uint32_t indexSize_ = 6;
		uint32_t materialSize_ = 3;

		// バッファリソース内のデータを指すポインタ
		VertexData2D* vertexData_ = nullptr;
		uint32_t* indexData_ = nullptr;

		// バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	protected:

		// WorldTransform
		EulerTransform worldTransform_; // Transform(scale, rotate, transform)

		/// ===スプライト情報=== ///
		std::string filePath_;                       // テクスチャファイルパス
		Vector2 position_ = { 0.0f, 0.0f };          // 座標
		float rotation_ = 0.0f;                      // 回転
		Vector2 size_ = { 640.0f, 360.0f };          // サイズ
		Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色
		Vector2 anchorPoint_ = { 0.0f, 0.0f };       // アンカーポイント
		bool isFlipX_ = false;                       // 左右フリップ
		bool isFlipY_ = false;                       // 上下フリップ
		Vector2 textureLeftTop_ = { 0.0f, 0.0f };    // テクスチャ左上
		Vector2 textureSize_ = { 100.0f, 100.0f };   // テクスチャ切り出しサイズ

	private:/// ===Functions(関数)=== ///

		/// <summary>
		/// VertexResourceへの書き込み処理
		/// </summary>
		void VertexDataWrite();

		/// <summary>
		/// IndexResourceへの書き込み処理
		/// </summary>
		void IndexDataWrite();

		/// <summary>
		/// MaterialDataへの書き込み処理
		/// </summary>
		void MaterialDataWrite();

		/// <summary>
		/// Transform情報の書き込み処理
		/// </summary>
		void TransformDataWrite();

		/// <summary>
		/// 頂点データの書き込み更新処理
		/// </summary>
		void UpdateVertexDataWrite();

		/// <summary>
		/// テクスチャの範囲指定処理
		/// </summary>
		void SpecifyRange();

		/// <summary>
		/// 指定されたテクスチャファイルのサイズを調整処理
		/// </summary>
		/// <param name="filePath">サイズを調整するテクスチャファイルへのパス。</param>
		void AdjustTextureSize(const std::string& filePath);
	};
}

