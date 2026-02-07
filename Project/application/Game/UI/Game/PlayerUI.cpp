#include "PlayerUI.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// SpriteBaseScale 
#include "application/Game/UI/SpriteBaseScale.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
PlayerUI::~PlayerUI() {
	moveUI_.reset();
	cameraUI_.reset();
	attackUI_.reset();
	avoidanceUI_.reset();
	xButton_.reset();
	aButton_.reset();
	leftStick_.reset();
	rightStick_.reset();
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void PlayerUI::Initialize() {
	// windowサイズの取得
	Vector2 windowSize = {
		static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()),
		static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight())
	};
	
	/// ===基準スケール=== ///
	scale_ = GetBaseScale();

	/// ===位置=== ///
	// 基準点
	Vector2 point = {
		windowSize.x * (3.0f / 4.0f) - (20.0f * scale_.x),
		windowSize.y * (7.0f / 8.0f)
	};
	// 間隔
	float spaceX = (windowSize.x - point.x) / 8.0f + (2.0f * scale_.x);
	// コントローラーUIのY座標
	float controllerPosY = point.y + (90.0f * scale_.y);

	// 移動UIの初期化
	moveUI_ = std::make_shared<Sprite>();
	moveUI_->Initialize("MoveUI");
	moveUI_->SetPosition({ point.x + spaceX, point.y });
	moveUIPos_ = moveUI_->GetPosition();
	moveUI_->SetSize({ 80.0f * scale_.x, 80.0f * scale_.y });
	moveUI_->SetAnchorPoint({ 0.5f, 0.5f });
	moveUI_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	// 左スティックUIの初期化
	leftStick_ = std::make_shared<Sprite>();
	leftStick_->Initialize("leftStick");
	leftStick_->SetPosition({ moveUI_->GetPosition().x, controllerPosY });
	leftStick_->SetSize({ 50.0f * scale_.x, 50.0f * scale_.y });
	leftStick_->SetAnchorPoint({ 0.5f, 0.5f });

	// カメラUIの初期化
	cameraUI_ = std::make_shared<Sprite>();
	cameraUI_->Initialize("CameraUI");
	cameraUI_->SetPosition({ point.x + (spaceX * 3.0f), point.y });
	cameraUI_->SetSize({ 80.0f * scale_.x, 80.0f * scale_.y });
	cameraUI_->SetAnchorPoint({ 0.5f, 0.5f });
	cameraUI_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	// 右スティックUI初期化
	rightStick_ = std::make_shared<Sprite>();
	rightStick_->Initialize("rightStick");
	rightStick_->SetPosition({ cameraUI_->GetPosition().x, controllerPosY });
	rightStick_->SetSize({ 50.0f * scale_.x, 50.0f * scale_.y });
	rightStick_->SetAnchorPoint({ 0.5f, 0.5f });

	// 攻撃UIの初期化
	attackUI_ = std::make_shared<Sprite>();
	attackUI_->Initialize("AttackUI");
	attackUI_->SetPosition({ point.x + (spaceX * 5.0f), point.y });
	attackUI_->SetSize({ 80.0f * scale_.x, 80.0f * scale_.y });
	attackUI_->SetAnchorPoint({ 0.5f, 0.5f });
	attackUI_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	// XボタンUIの初期化
	xButton_ = std::make_shared<Sprite>();
	xButton_->Initialize("xButton");
	xButton_->SetPosition({ attackUI_->GetPosition().x,  controllerPosY });
	xButton_->SetSize({ 50.0f * scale_.x, 50.0f * scale_.y });
	xButton_->SetAnchorPoint({ 0.5f, 0.5f });

	// 回避UIの初期化
	avoidanceUI_ = std::make_shared<Sprite>();
	avoidanceUI_->Initialize("AvoidanceUI");
	avoidanceUI_->SetPosition({ point.x + (spaceX * 7.0f), point.y });
	avoidanceUI_->SetSize({ 80.0f * scale_.x, 80.0f * scale_.y });
	avoidanceUI_->SetAnchorPoint({ 0.5f, 0.5f });
	avoidanceUI_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	// AボタンUIの初期化
	aButton_ = std::make_shared<Sprite>();
	aButton_->Initialize("aButton");
	aButton_->SetPosition({ avoidanceUI_->GetPosition().x, controllerPosY });
	aButton_->SetSize({ 50.0f * scale_.x, 50.0f * scale_.y });
	aButton_->SetAnchorPoint({ 0.5f, 0.5f });

	// UIの位置の保存
	moveUIPos_ = moveUI_->GetPosition();
	cameraUIPos_ = cameraUI_->GetPosition();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void PlayerUI::Update() {
	// フラグと色のリセット
	if (colorChange_.attackUI) {
		colorChange_.attackUI = false;
		attackUI_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	} else if (colorChange_.avoidanceUI) {
		colorChange_.avoidanceUI = false;
		avoidanceUI_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	}

	// スプライトの更新処理
	SpriteUpdate();

	// 色の更新処理
	ColorUpdate();

	// 移動UIの更新
	moveUI_->Update();
	// カメラUIの更新
	cameraUI_->Update();
	// 攻撃UIの更新
	attackUI_->Update();
	// 回避UIの更新
	avoidanceUI_->Update();
	// XボタンUiの更新
	xButton_->Update();
	// AボタンUIの更新
	aButton_->Update();
	// 左スティックUIの更新
	leftStick_->Update();
	// 右スティックUIの更新
	rightStick_->Update();
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void PlayerUI::Draw() {
	// 移動UIの描画
	moveUI_->Draw();
	// カメラUIの描画
	cameraUI_->Draw();
	// 攻撃UIの描画
	attackUI_->Draw();
	// 回避UIの描画
	avoidanceUI_->Draw();
	// XButtonUIの描画
	xButton_->Draw();
	// AButtonUIの描画
	aButton_->Draw();
	// 左スティックUIの描画
	leftStick_->Draw();
	// 右スティックUIの描画
	rightStick_->Draw();
}

///-------------------------------------------/// 
/// スプライトの更新処理
///-------------------------------------------///
void PlayerUI::SpriteUpdate() {

	/// ===スティックの取得=== ///
	Vector2 leftStick = player_->GetLeftStickState();
	Vector2 rightStick = player_->GetRightStickState();
	leftStick.y *= -1.0f;
	rightStick.y *= -1.0f;

	/// ===位置の更新=== ///
	// 移動UI
	moveUI_->SetPosition(moveUIPos_ + leftStick * 15.0f);
	// カメラUI
	cameraUI_->SetPosition(cameraUIPos_ + rightStick * 15.0f);

	/// ===サイズの管理=== ///
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		// サイズを大きくする
		moveUI_->SetSize({ 100.0f * scale_.x, 100.0f * scale_.y });
	} else {
		// サイズを元に戻す
		moveUI_->SetSize({ 80.0f * scale_.x, 80.0f * scale_.y });
	}
	if (std::abs(rightStick.x) > 0.1f || std::abs(rightStick.y) > 0.1f) {
		// サイズを大きくする
		cameraUI_->SetSize({ 100.0f * scale_.x, 100.0f * scale_.y });
	} else {
		// サイズを元に戻す
		cameraUI_->SetSize({ 80.0f * scale_.x, 80.0f * scale_.y });
	}
}

///-------------------------------------------/// 
/// 色の更新処理
///-------------------------------------------///
void PlayerUI::ColorUpdate() {
	/// ===色変更のフラグ管理=== ///
	if (player_->GetAttackComponent()->GetState().isActive && !colorChange_.attackUI) {
		// 攻撃UI
		colorChange_.attackUI = true;
	}
	if (player_->GetAvoidanceComponent()->GetState().isActive && !colorChange_.avoidanceUI) {
		// 回避UI
		colorChange_.avoidanceUI = true;
	}

	/// ===UIの色更新=== ///
	if (colorChange_.attackUI) {
		// 攻撃UI
		attackUI_->SetColor(activeColor_);
	}
	if (colorChange_.avoidanceUI) {
		// 回避UI
		avoidanceUI_->SetColor(activeColor_);
	}
}
