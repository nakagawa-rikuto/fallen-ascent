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
#ifdef USE_IMGUI
// Service
#include "Service/Input.h"
#endif // USE_IMGUI


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
	// Cameraの解放
	Service::Camera::Remove("Title");
	camera_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TitleScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===Camera=== ///
	camera_ = std::make_unique<MiiEngine::FollowCamera>();
	camera_->Initialize();
	camera_->SetFollowCamera(MiiEngine::FollowCameraType::Orbiting);
	camera_->SetOrbitingOffset(cameraOrbitingOffset_);
	camera_->SetRotate(cameraRotation_);
	// カメラの設定
	Service::Camera::AddCamera("Title", camera_.get());
	Service::Camera::SetActiveCamera("Title");

	/// ===Stage=== ///	
	stage_ = std::make_unique<GameStage>();
	stage_->Initialize("Level/StageData2.json");

	/// ===Playerの生成=== ///
	player_ = std::make_unique<Player>();
	SpawnPlayer("Level/EntityData2.json");
	player_->SetCameraTargetPlayer();

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
	ImGui::End();

	// Camera
	if (Service::Camera::GetActiveCamera() == camera_.get()) {
		camera_->ImGuiUpdate();
		camera_->DebugUpdate();
	} else {
		defaultCamera_->ImGuiUpdate();
		defaultCamera_->DebugUpdate();
	}

	// デバッグカメラの切り替え
	if (Service::Input::TriggerKey(DIK_TAB)) {
		if (Service::Camera::GetActiveCamera() == camera_.get()) {
			Service::Camera::SetActiveCamera("Default");
		} else {
			Service::Camera::SetActiveCamera("Game");
		}
	}

	// Cameraの情報表示
	camera_->ImGuiUpdate();

	// Playerの情報表示
	player_->Information();

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

	/// ===stage=== ///
	stage_->Draw();

	/// ===Player=== ///
	player_->Draw();

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
			player_->InitGame(obj.translation, camera_.get());
			player_->SetRotate(Math::QuaternionFromVector(obj.rotation));
			break;
		}
	}
}
