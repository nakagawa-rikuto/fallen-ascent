#include "TitleScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Service/GraphicsResourceGetter.h"
#include "Service/Camera.h"
#include "Service/Collision.h"
#include "Service/PostEffect.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
TitleScene::~TitleScene() {
	// ISceneのデストラクタ
	IScene::~IScene();
	// Colliderのリセット
	Service::Collision::Reset();
	// TitleUIのリセット
	titleUI_.reset();
	// TitleSceneAnimationのリセット
	animation_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TitleScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===Camera=== ///
	camera_ = std::make_shared<GameCamera>();
	camera_->Init(CameraType::Follow);
	camera_->SetFollowCamera(FollowCameraType::Orbiting);
	camera_->SetOrbitingOffset(cameraOrbitingOffset_);
	camera_->SetRotate(cameraRotation_);
	// カメラの設定
	Service::Camera::AddCamera("Title", camera_);
	Service::Camera::SetActiveCamera("Title");

	/// ===Playerの生成=== ///
	player_ = std::make_unique<Player>();
	SpawnPlayer("Level/EntityData2.json");
	player_->SetCameraTargetPlayer();

	/// ===Stage=== ///	
	stage_ = std::make_unique<GameStage>();
	stage_->Initialize("Level/StageData2.json");

	/// ===TitleUI=== ///
	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Initialize();

	/// ===TitleSceneAnimation=== ///
	animation_ = std::make_unique<TitleSceneAnimation>();
	animation_->Initialize(camera_.get());

	/// ===Fadeの設定=== ///
	isStartFadeOut_ = false;
	currentFade_ = FadeState::Selecting; // デバッグ用にフェードインをスキップ
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void TitleScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("TitleScene");
	ImGui::Text("Fade State: %d", static_cast<int>(currentFade_));

	camera_->ImGuiUpdate();
	//camera_->DebugUpdate();

	ImGui::End();
#endif // USE_IMGUI

	/// ===Player=== ///
	player_->UpdateAnimation();

	/// ===Stage=== ///
	stage_->Update();

	/// ===カメラアニメーション=== ///
	animation_->UpdateCameraAnimation(player_->GetTransform().translate);

	/// ===フェーズ別更新=== ///
	switch (currentFade_) {
	case TitleScene::FadeState::FadeIn:
		UpdateFadeIn();
		break;
	case TitleScene::FadeState::Selecting:
		titleUI_->Update();
		if (titleUI_->IsGameStart()) {
			currentFade_ = FadeState::FadeOut;
			Service::PostEffect::SetOffScreenType(MiiEngine::OffScreenType::RadiusBlur);
		}
		break;
	case TitleScene::FadeState::FadeOut:
		UpdateFadeOut();
		break;
	}

	/// ===ISceneの更新=== ///
	IScene::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void TitleScene::Draw() {
	/// ===Player=== ///
	player_->Draw();

	/// ===stage=== ///
	stage_->Draw();

	/// ===TitleUIの描画=== ///
	titleUI_->Draw();

	/// ===ISceneの描画=== ///
	IScene::Draw();
}

///-------------------------------------------/// 
/// フェードインの更新
///-------------------------------------------///
void TitleScene::UpdateFadeIn() {

	/// ===Fadeの変更=== ///
	if (sceneManager_->GetTransitionFinished()) {
		currentFade_ = FadeState::Selecting;
	}
}

///-------------------------------------------/// 
/// フェードアウトの更新
///-------------------------------------------///
void TitleScene::UpdateFadeOut() {
	// ラジアスブラーの更新
	animation_->UpdateRadiusBlurAnimation();

	// アニメーション終了後にフェードアウトへ移行
	if (animation_->GetStartFadeOut() && !isStartFadeOut_) {
		isStartFadeOut_ = true;
		sceneManager_->StartFadeOut(TransitionType::ShatterGlass, 1.0f);
	}

	/// ===シーンの切り替え=== ///
	if (sceneManager_->GetTransitionFinished()) {
		// ゲームシーンへ遷移
		sceneManager_->ChangeScene(MiiEngine::SceneType::Game);
	}
}

///-------------------------------------------/// 
/// プレイヤーの生成
///-------------------------------------------///
void TitleScene::SpawnPlayer(const std::string& json_name) {
	LevelData* levelData = Service::GraphicsResourceGetter::GetLevelData(json_name);

	// オブジェクト分回す
	for (const auto& obj : levelData->objects) {
		// OBBの半分の大きさを計算
		Vector3 obbHalfSize = obj.colliderInfo2 / 2.0f;

		/// ===クラス名で分岐=== ///
		switch (obj.classType) {
		case LevelData::ClassTypeLevel::Player:
			// 初期化と座標設定
			player_->Initialize();
			player_->SetTranslate(obj.translation);
			player_->SetRotate(Math::QuaternionFromVector(obj.rotation));
			break;
		}
	}
}
