#pragma once
/// ===Include=== ///
// SpriteCommon
#include "Engine/Graphics/2d/Sprite/SpriteCommon.h"
// C++
#include <memory>
#include <vector>

namespace MiiEngine {
	///=====================================================/// 
	/// スプライトマネージャ
	///=====================================================///
	class SpriteManager {
	public:

		SpriteManager() = default;
		~SpriteManager() = default;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理（背景）
		/// </summary>
		void BackDraw();

		/// <summary>
		/// 描画処理（前景）
		/// </summary>
		void FrontDraw();

		/// <summary>
		/// スプライトを追加します。
		/// </summary>
		/// <param name="sprite">追加するスプライトへのポインタ。</param>
		void AddSprite(SpriteCommon* sprite);

		/// <summary>
		/// すべてのスプライトをクリアします。
		/// </summary>
		void ClearSprites();

		/// <summary>
		/// スプライトを削除します。
		/// </summary>
		/// <param name="sprite">削除するスプライトへのポインタ。</param>
		void RemoveSprite(SpriteCommon* sprite);

	private: /// ===メンバ変数=== ///
		// スプライトの配列
		std::vector<MiiEngine::SpriteCommon*> sprites_;
	};

}

