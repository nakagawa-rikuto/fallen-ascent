#include "SpriteManager.h"
// Engine
#include "Service/GraphicsResourceGetter.h"
#include "Service/Render.h"

namespace MiiEngine {

	///-------------------------------------------/// 
	/// 更新処理 
	///-------------------------------------------///
	void SpriteManager::Update() {
		for (auto& sprite : sprites_) {
			sprite->Update();
		}
	}

	///-------------------------------------------/// 
	/// 背景描画処理 
	///-------------------------------------------///
	void MiiEngine::SpriteManager::BackDraw() {
		for (auto& sprite : sprites_) {
			// 早期リターン
			if (sprite->GetGroundType() != GroundType::Back || !sprite->GetIsDraw()) continue;

			/// ===コマンドリストのポインタの取得=== ///
			ID3D12GraphicsCommandList* commandList = Service::GraphicsResourceGetter::GetDXCommandList();

			// PSOの設定
			Service::Render::SetPSO(commandList, PipelineType::BackGround2D, sprite->GetBlendMode());

			// 描画
			sprite->Draw();
		}
	}


	///-------------------------------------------///
	/// 前景描画処理
	///-------------------------------------------///
	void MiiEngine::SpriteManager::FrontDraw() {
		for (auto& sprite : sprites_) {
			// 早期リターン
			if (sprite->GetGroundType() != GroundType::Front || !sprite->GetIsDraw()) continue;

			/// ===コマンドリストのポインタの取得=== ///
			ID3D12GraphicsCommandList* commandList = Service::GraphicsResourceGetter::GetDXCommandList();

			// PSOの設定
			Service::Render::SetPSO(commandList, PipelineType::ForGround2D, sprite->GetBlendMode());

			// 描画
			sprite->Draw();
		}
	}

	///-------------------------------------------/// 
	/// スプライトの追加 
	///-------------------------------------------///
	void SpriteManager::AddSprite(SpriteCommon* sprite) {
		// スプライトを配列に追加
		sprites_.emplace_back(sprite);
	}

	///-------------------------------------------/// 
	/// スプライトのクリア 
	///-------------------------------------------///
	void SpriteManager::ClearSprites() {
		// スプライトの配列をクリア
		sprites_.clear();
	}

	///-------------------------------------------/// 
	///	スプライトの削除
	///-------------------------------------------///
	void SpriteManager::RemoveSprite(SpriteCommon* sprite) {
		// スプライトを配列から削除
		sprites_.erase(std::remove(sprites_.begin(), sprites_.end(), sprite), sprites_.end());
	}
}

