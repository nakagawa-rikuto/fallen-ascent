#include "Sprite.h"
#include "Engine/System/Managers/SpriteManager.h"
// Locator
#include "Locator.h"

namespace Service {
	///-------------------------------------------/// 
	/// スプライトの追加
	///-------------------------------------------///
	void Sprite::AddSprite(MiiEngine::SpriteCommon* sprite) {
		Locator::GetSpriteManager()->AddSprite(sprite);
	}

	///-------------------------------------------/// 
	/// スプライトのクリア
	///-------------------------------------------///
	void Sprite::ClearSprites() {
		Locator::GetSpriteManager()->ClearSprites();
	}

	///-------------------------------------------/// 
	///	スプライトの削除
	///-------------------------------------------///
	void Sprite::RemoveSprite(MiiEngine::SpriteCommon* sprite) {
		Locator::GetSpriteManager()->RemoveSprite(sprite);
	}
}
