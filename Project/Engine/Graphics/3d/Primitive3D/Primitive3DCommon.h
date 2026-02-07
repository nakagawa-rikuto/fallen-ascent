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
	/// プリミティブコモン
	///=====================================================///
	class Primitive3DCommon {
	public:
		Primitive3DCommon() = default;
		~Primitive3DCommon();

		/// <summary>
		/// 初期化処理、純粋仮想関数
		/// </summary>
		/// <param name="modelName">初期化に使用するモデルの名前を表す文字列（const std::string&）。</param>
		/// <param name="type">ライトの種類を示す LightType 型の値。</param>
		virtual void Initialize(const std::string& modelName, LightType type) = 0;

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画処理、純粋仮想関数
		/// </summary>
		/// <param name="mode">描画に使用するブレンドモードを指定します。</param>
		virtual void Draw(BlendMode mode) = 0;

		/// <summary>
		/// 生成処理
		/// </summary>
		/// <param name="device">ライトを作成するために使用する ID3D12Device へのポインター（Direct3D 12 デバイス）。</param>
		/// <param name="type">作成するライトの種類を示す値（LightType）。</param>
		void Create(ID3D12Device* device, LightType type);

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="commandList">バインド操作を実行する Direct3D 12 のグラフィックスコマンドリストへのポインター。</param>
		void Bind(ID3D12GraphicsCommandList* commandList);

	public:/// ===Setter=== ///
		// Light
		void SetLightType(LightType type);

	protected: /// ===継承先で使用する変数=== ///

		// バッファリソース
		std::shared_ptr<VertexBuffer3D> vertex_;
		std::shared_ptr<IndexBuffer3D> index_;

		/// ===バッファビュー=== ///
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

		/// ===バッファリソース内のデータを指すポインタ=== ///
		VertexData3D* vertexData_ = nullptr;
		uint32_t* indexData_ = nullptr;

		/// ===プリミティブ情報=== ///
		EulerTransform uvTransform_;
		QuaternionTransform worldTransform_;
		Vector4 color_;

		/// ===ファイルパス=== ///
		std::string textureFilePath_;

		/// ===Light=== ///
		LightInfo light_;

		/// ===Camera=== ///
		GameCamera* camera_ = nullptr;

		/// ===環境マップ=== ///
		EnvironmentMapInfo environmentMapInfo_;

	private:/// ===Variables(変数)=== ///

		// Common
		std::shared_ptr<ObjectCommon> common_;

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

