#pragma once
/// ===Include=== ///

/// ===前方宣言=== ///
enum class FadeState {
	None,
	FadeIn,
	FadeOut,
};

///=====================================================/// 
/// シーントランジション基底クラス
///=====================================================///
class SceneTransitionBase {
public:

	virtual ~SceneTransitionBase() = default;

	/// <summary>
	/// 指定した時間でフェードインを開始する
	/// </summary>
	/// <param name="duration">フェードインにかける時間（秒）。</param>
	virtual void StartFadeIn(const float duration);

	/// <summary>
	/// 指定した継続時間でフェードアウトを開始する
	/// </summary>
	/// <param name="duration">フェードアウトにかける時間（秒）。</param>
	virtual void StartFadeOut(const float duration);

	/// <summary>
	/// フェードインの更新処理を行う純粋仮想関数
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// フェードアウトの更新処理を行う純粋仮想関数
	/// </summary>
	void FadeOutUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

public: /// ===Getter=== ///

	// フェード中か
	bool IsFading() const;
	// 完了したか
	bool IsFinished() const;
	// 現在の状態
	FadeState GetState() const;

public:
	// リセット
	void Reset();

protected:

	// アニメーション制御用構造体
	struct TransitionData {
		bool isPlaying = false;
		bool isFinished = false;
		float currentTime = 0.0f;
		float duration = 1.2f;  // エフェクトの総時間
	};
	TransitionData animation_;

	// フェード
	FadeState currentState_ = FadeState::None;

protected:

	/// <summary>
	/// フェードインの更新時に呼び出される処理を行う関数。
	/// </summary>
	virtual void OnFadeInUpdate() {};

	/// <summary>
	/// フェードアウトの進行に応じた更新処理を行います。
	/// </summary>
	virtual void OnFadeOutUpdate() {};
};

