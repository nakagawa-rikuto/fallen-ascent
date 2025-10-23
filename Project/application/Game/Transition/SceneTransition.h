#pragma once
#include "Engine/Graphics/OffScreen/Effect/ShatterGlassEffect.h"
#include "application/Drawing/2d/Sprite.h"

enum class FadeState {
    None,
    FadeIn,   
    FadeOut,  
    Finished
};

///=====================================================/// 
/// SceneTransition
///=====================================================///
class SceneTransition {
public: 

    SceneTransition() = default;
    ~SceneTransition();

    /// <summary>
    /// フェードイン開始処理
    /// </summary>
    /// <param name="duration">フェードインにかかる時間（秒）。省略時は1.2秒が使用されます。</param>
    void StartFadeIn(float duration = 1.2f);

    /// <summary>
    /// フェードアウト開始処理
    /// </summary>
    /// <param name="duration">フェードアウトの継続時間（秒）。省略した場合の既定値は1.0秒です。</param>
    void StartFadeOut(float duration = 1.0f);  // 通常→真っ暗

    /// <summary>
    /// フェードインの更新処理
    /// </summary>
    void FadeInUpdate();

    /// <summary>
    /// フェードアウトの更新処理
    /// </summary>
    void FadeOutUpdate();

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw();

public: /// ===Getter=== ///

    // フェード中か
    bool IsFading() const;

    // 完了したか
    bool IsFinished() const;

    // 現在の状態
    FadeState GetState() const;

private: 

    // Sprite
	std::unique_ptr<Sprite> sprite_;

    // Data
    ShatterGlassData data_;

    // アニメーション制御
    bool isPlaying_ = false;
    bool isFinished_ = false;
    float currentTime_ = 0.0f;
    float duration_ = 1.2f; // エフェクトの総時間（秒）

    FadeState currentState_ = FadeState::None;
};

