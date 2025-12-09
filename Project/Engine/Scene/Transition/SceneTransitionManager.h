#pragma once
/// ===Include=== ///
#include "Engine/Scene/Transition/Base/SceneTransitionBase.h"
// C++
#include <memory>
#include <functional>

/// ===トランジションタイプ=== ///
enum class TransitionType {
	None,
	ShatterGlass,
};

///=====================================================/// 
/// シーントランジションマネージャ
///=====================================================///
class SceneTransitionManager {
public:

	SceneTransitionManager() = default;
	~SceneTransitionManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 遷移（フェード）を開始する関数
	/// </summary>
	/// <param name="type">開始する遷移の種類を示す値（TransitionType）。</param>
	/// <param name="duration">遷移にかかる時間（秒）。float 型で指定する。</param>
	void StartFadeIn(TransitionType type, const float duration);

	/// <summary>
	/// 指定した継続時間でフェードアウトを開始します。
	/// </summary>
	/// <param name="duration">フェードアウトにかける時間（秒）</param>
	void StartFadeOut(TransitionType type, const float duration);

public: /// ===Getter=== ///

	// フェード中か
	bool IsFading() const;
	// 完了したか
	bool IsFinished() const;
	// 現在の状態
	FadeState GetState() const;

private:

	// 現在のトランジションタイプ
	TransitionType currentType_ = TransitionType::None;

	// トランジション
	std::unique_ptr<SceneTransitionBase> currentTransition_;

private:

	/// <summary>
	/// 指定した種類のシーン遷移（SceneTransitionBase 派生）を生成し、その所有権を持つ std::unique_ptr を返します。
	/// </summary>
	/// <param name="type">生成するトランジションの種類を示す値（TransitionType）。</param>
	void CreateTransition(TransitionType type);
};

