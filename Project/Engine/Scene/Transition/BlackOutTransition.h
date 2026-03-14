#pragma once
/// ===Include=== ///
#include "Engine/Scene/Transition/Base/SceneTransitionBase.h"
#include "application/Drawing/2d/Object2d.h"

namespace MiiEngine {
	///=====================================================/// 
	/// BlackOutTransition
	///=====================================================///
	class BlackOutTransition : public SceneTransitionBase {
	public:

		BlackOutTransition() = default;
		~BlackOutTransition();

		/// <summary>
        /// フェードイン開始処理
        /// </summary>
        /// <param name="duration">フェードインにかかる時間（秒）。省略時は1.2秒が使用されます。</param>
        void StartFadeIn(float duration = 1.0f) override;

        /// <summary>
        /// フェードアウト開始処理
        /// </summary>
        /// <param name="duration">フェードアウトの継続時間（秒）。省略した場合の既定値は1.0秒です。</param>
        void StartFadeOut(float duration = 1.2f) override;

        /// <summary>
        /// 描画処理
        /// </summary>
        void Draw() override;

    private:

        // Sprite
		std::unique_ptr<Object2d> sprite_;

        /// <summary>
        /// フェードイン固有の更新処理
        /// </summary>
        void OnFadeInUpdate() override;

        /// <summary>
        /// フェードアウト固有の更新処理
        /// </summary>
        void OnFadeOutUpdate() override;
	};
}


