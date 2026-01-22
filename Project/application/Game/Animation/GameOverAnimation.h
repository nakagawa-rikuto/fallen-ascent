#pragma once
/// ===Include=== ///
#include "application/Drawing/2d/Sprite.h"
#include <memory>

/// ===前方宣言=== ///
class GameCamera;

///-------------------------------------------/// 
/// GameOverAnimation 
///-------------------------------------------///
class GameOverAnimation {
public:
	GameOverAnimation() = default;
	~GameOverAnimation() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="camera">初期化する GameCamera オブジェクトへのポインタ。</param>
	void Initialize(GameCamera* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット処理
	/// </summary>
	void Reset();

public: /// ===Getter=== ///
	// アニメーション完了フラグの取得
	bool IsCompleted() const { return isCompleted_; }

private:
	/// ===ポインタ=== ///
	GameCamera* camera_ = nullptr;  // カメラ

	/// ===Sprite=== ///
	std::unique_ptr<Sprite> gameOverSprite_;  // ゲームオーバースプライト
	std::unique_ptr<Sprite> blackSprite_;     // ブラックアウト用スプライト

	/// ===アニメーション制御=== ///
	bool isCompleted_ = false;
	float phaseTimer_ = 0.0f;        // フェーズ内のタイマー

	/// ===スプライト制御=== ///
	float spriteAlpha_ = 0.0f;       // スプライトの透明度
	float blackAlpha_ = 0.0f;        // ブラックアウトの透明度
	Vector2 shakeOffset_ = { 0.0f, 0.0f };      // シェイクのオフセット
	Vector2 originalSpritePos_ = { 0.0f, 0.0f }; // スプライトの元の位置

	/// ===カメラ制御=== ///
	Vector3 initialCameraOffset_ = { 0.0f, 2.0f, -15.0f }; // カメラの初期オフセット

	/// ===フェーズ管理=== ///
	enum class Phase {
		AooroachCamera, // カメラを近づける
		DrawSprite,    // スプライトを表示する
		ShakeSprite    // スプライトをシェイクさせる
	};
	Phase phase_ = Phase::AooroachCamera;

private:

	/// <summary>
	/// ApproachCameraフェーズの更新処理
	/// </summary>
	void UpdateApproachCamera();

	/// <summary>
	/// DrawSpriteフェーズの更新処理
	/// </summary>
	void UpdateDrawSprite();

	/// <summary>
	/// ShakeSpriteフェーズの更新処理
	/// </summary>
	void UpdateShakeSprite();
};