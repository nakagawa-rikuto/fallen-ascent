#include "GameOverAnimation.h"
#include "application/Game/Camera/GameCamera.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include <cmath>

// 定数定義
namespace {
    // カメラアプローチフェーズ
    constexpr float CAMERA_ZOOM_SPEED = 0.05f;          // カメラのズーム速度
    constexpr float CAMERA_TARGET_OFFSET_Z = -18.0f;     // 目標オフセットZ

    // スプライト表示フェーズ
    constexpr float SPRITE_FADE_IN_SPEED = 0.03f;       // フェードイン速度
    constexpr float SPRITE_DISPLAY_TIME = 90.0f;        // スプライト表示時間(フレーム)

    // スプライトシェイクフェーズ
    constexpr float SHAKE_INTENSITY_START = 1.0f;       // シェイクの初期強さ
    constexpr float SHAKE_INTENSITY_MAX = 20.0f;        // シェイクの最大強さ
    constexpr float SHAKE_FREQUENCY = 1.2f;             // シェイクの周波数
    constexpr float FADE_OUT_SPEED = 0.01f;             // フェードアウト速度
    constexpr float SHAKE_DURATION = 140.0f;            // シェイク時間(フレーム)
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameOverAnimation::Initialize(GameCamera* camera) {
    camera_ = camera;

    // スプライトの初期化
    gameOverSprite_ = std::make_unique<Sprite>();
    gameOverSprite_->Initialize("GameOverAnimation");

    // ウィンドウサイズの取得
    float windowWidth = static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth());
    float windowHeight = static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight());

    // スプライトの初期設定（画面中央）
    gameOverSprite_->SetPosition({ windowWidth / 2.0f, windowHeight / 2.0f });
    gameOverSprite_->SetSize({ windowWidth + 100.0f, windowHeight + 100.0f }); // お好みのサイズに調整
    gameOverSprite_->SetAnchorPoint({ 0.5f, 0.5f }); // 中心を基準点に
    gameOverSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 初期は透明

    // ブラックアウト用スプライトの初期化
    blackSprite_ = std::make_unique<Sprite>();
    blackSprite_->Initialize("White"); // 1x1の白テクスチャ
    blackSprite_->SetPosition({ windowWidth / 2.0f, windowHeight /2.0f });
    Service::GraphicsResourceGetter::GetWindowWidth();
    blackSprite_->SetSize({ windowWidth, windowHeight }); // 画面全体
    blackSprite_->SetAnchorPoint({ 0.5f, 0.5f });
    blackSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f }); // 黒、初期は透明

    // 状態の初期化
    phase_ = Phase::AooroachCamera;
    isCompleted_ = false;
    phaseTimer_ = 0.0f;
    spriteAlpha_ = 0.0f;
    blackAlpha_ = 0.0f;
    shakeOffset_ = { 0.0f, 0.0f };
    originalSpritePos_ = gameOverSprite_->GetPosition();

    // カメラの初期オフセットを保存
    if (camera_) {
        initialCameraOffset_ = camera_->GetOffset();
    }
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameOverAnimation::Update() {
    if (isCompleted_) {
        return;
    }

    switch (phase_) {
    case Phase::AooroachCamera:
        UpdateApproachCamera();
        break;

    case Phase::DrawSprite:
        UpdateDrawSprite();
        break;

    case Phase::ShakeSprite:
        UpdateShakeSprite();
        break;
    }

    // スプライトの更新
    if (gameOverSprite_) {
        gameOverSprite_->Update();
    }
    if (blackSprite_) {
        blackSprite_->Update();
    }
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameOverAnimation::Draw() {
    // スプライト描画フェーズ以降でゲームオーバースプライトを描画
    if (phase_ == Phase::DrawSprite || phase_ == Phase::ShakeSprite) {
        if (gameOverSprite_) {
            gameOverSprite_->Draw(MiiEngine::GroundType::Front, MiiEngine::BlendMode::KBlendModeNormal);
        }
    }

    // ブラックアウトの描画（最前面）
    if (phase_ == Phase::ShakeSprite && blackAlpha_ > 0.0f) {
        if (blackSprite_) {
            blackSprite_->Draw(MiiEngine::GroundType::Front, MiiEngine::BlendMode::KBlendModeNormal);
        }
    }
}

///-------------------------------------------/// 
/// リセット
///-------------------------------------------///
void GameOverAnimation::Reset() {
    phase_ = Phase::AooroachCamera;
    isCompleted_ = false;
    phaseTimer_ = 0.0f;
    spriteAlpha_ = 0.0f;
    blackAlpha_ = 0.0f;
    shakeOffset_ = { 0.0f, 0.0f };

    if (gameOverSprite_) {
        gameOverSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
        gameOverSprite_->SetPosition(originalSpritePos_);
    }

    if (blackSprite_) {
        blackSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
    }

    // カメラのシェイクを停止
    if (camera_) {
        camera_->StopShake();
        // オフセットを初期値に戻す
        camera_->SetOffset(initialCameraOffset_);
    }
}

///-------------------------------------------/// 
/// ApproachCameraフェーズの更新
///-------------------------------------------///
void GameOverAnimation::UpdateApproachCamera() {
    if (!camera_) {
        phase_ = Phase::DrawSprite;
        phaseTimer_ = 0.0f;
        return;
    }

    // 現在のオフセットを取得
    Vector3 currentOffset = camera_->GetOffset();

    // 目標オフセット（プレイヤーに近づく）
    Vector3 targetOffset = { 0.0f, 20.0f, CAMERA_TARGET_OFFSET_Z };

    // オフセットを補間
    currentOffset.x += (targetOffset.x - currentOffset.x) * CAMERA_ZOOM_SPEED;
    currentOffset.y += (targetOffset.y - currentOffset.y) * CAMERA_ZOOM_SPEED;
    currentOffset.z += (targetOffset.z - currentOffset.z) * CAMERA_ZOOM_SPEED;

    // オフセットを設定
    camera_->SetOffset(currentOffset);

    // 目標距離に到達したかチェック
    if (std::abs(currentOffset.z - targetOffset.z) < 0.3f) {
        phase_ = Phase::DrawSprite;
        phaseTimer_ = 0.0f;
    }
}

///-------------------------------------------/// 
/// DrawSpriteフェーズの更新
///-------------------------------------------///
void GameOverAnimation::UpdateDrawSprite() {
    // スプライトをフェードイン
    if (spriteAlpha_ < 1.0f) {
        spriteAlpha_ += SPRITE_FADE_IN_SPEED;
        if (spriteAlpha_ > 1.0f) {
            spriteAlpha_ = 1.0f;
        }
        gameOverSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    }

    // 一定時間表示
    phaseTimer_++;
    if (phaseTimer_ >= SPRITE_DISPLAY_TIME) {
        phase_ = Phase::ShakeSprite;
        phaseTimer_ = 0.0f;
    }
}

///-------------------------------------------/// 
/// ShakeSpriteフェーズの更新
///-------------------------------------------///
void GameOverAnimation::UpdateShakeSprite() {
    phaseTimer_++;

    // シェイクの強度を時間とともに増加
    float progress = phaseTimer_ / SHAKE_DURATION;
    float currentIntensity = SHAKE_INTENSITY_START +
        (SHAKE_INTENSITY_MAX - SHAKE_INTENSITY_START) * progress;

    // スプライトをシェイクさせる
    float shakeX = currentIntensity * std::sin(phaseTimer_ * SHAKE_FREQUENCY);
    float shakeY = currentIntensity * std::cos(phaseTimer_ * SHAKE_FREQUENCY * 1.5f);
    shakeOffset_ = { shakeX, shakeY };

    Vector2 newPos = originalSpritePos_;
    newPos.x += shakeOffset_.x;
    newPos.y += shakeOffset_.y;
    gameOverSprite_->SetPosition(newPos);

    // ブラックアウト
    blackAlpha_ += FADE_OUT_SPEED;
    if (blackAlpha_ > 1.0f) {
        blackAlpha_ = 1.0f;
    }
    blackSprite_->SetColor({ 0.0f, 0.0f, 0.0f, blackAlpha_ });

    // 完了判定
    if (phaseTimer_ >= SHAKE_DURATION || blackAlpha_ >= 1.0f) {
        isCompleted_ = true;
    }
}

