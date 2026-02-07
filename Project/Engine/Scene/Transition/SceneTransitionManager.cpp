#include "SceneTransitionManager.h"
// Transition
#include "Engine/Scene/Transition/ShatterGlassTransition.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	SceneTransitionManager::~SceneTransitionManager() {
		currentTransition_.reset();
	}

	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	bool SceneTransitionManager::IsFading() const { return currentTransition_->IsFading(); }
	bool SceneTransitionManager::IsFinished() const { return currentTransition_->IsFinished(); }
	TransitionType SceneTransitionManager::GetType() const { return currentType_; }
	FadeState SceneTransitionManager::GetState() const { return currentTransition_->GetState(); }

	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void SceneTransitionManager::Initialize() {
		// トランジションタイプの初期化
		currentType_ = TransitionType::None;

		// トランジションの作成
		currentTransition_ = std::move(std::make_unique<SceneTransitionBase>());
	}

	///-------------------------------------------/// 
	/// 更新
	///-------------------------------------------///
	void SceneTransitionManager::Update() {
		// フェードイン更新
		currentTransition_->FadeInUpdate();
		// フェードアウト更新
		currentTransition_->FadeOutUpdate();
	}

	///-------------------------------------------/// 
	/// 描画
	///-------------------------------------------///
	void SceneTransitionManager::Draw() {
		// 描画
		currentTransition_->Draw();
	}

	///-------------------------------------------/// 
	/// リセット
	///-------------------------------------------///
	void SceneTransitionManager::Reset() {
		currentTransition_->Reset();
	}

	///-------------------------------------------/// 
	/// NewMake
	///-------------------------------------------///
	void SceneTransitionManager::NewMake() {
		// 既存のトランジションを破棄
		currentTransition_.reset();
		// 新しく作成
		currentType_ = TransitionType::None;
		currentTransition_ = std::make_unique<SceneTransitionBase>();
	}

	///-------------------------------------------/// 
	/// 遷移フェードインを開始
	///-------------------------------------------///
	void SceneTransitionManager::StartFadeIn(TransitionType type, const float duration) {
		// 既存のトランジションを変更
		CreateTransition(type);
		// フェードイン開始
		currentTransition_->StartFadeIn(duration);
	}

	///-------------------------------------------/// 
	/// 遷移フェードアウトを開始
	///-------------------------------------------///
	void SceneTransitionManager::StartFadeOut(TransitionType type, const float duration) {
		// 既存のトランジションを変更
		CreateTransition(type);
		// フェードアウト開始
		currentTransition_->StartFadeOut(duration);
	}

	///-------------------------------------------/// 
	/// 指定した種類のシーン遷移を作成
	///-------------------------------------------///
	void SceneTransitionManager::CreateTransition(TransitionType type) {
		// トランジションタイプが異なる場合のみ作成
		if (currentType_ != type) {
			switch (type) {
			case TransitionType::ShatterGlass:
				// 既存のトランジションを破棄
				currentTransition_.reset();
				// 新しいトランジションを作成
				currentTransition_ = std::move(std::make_unique<ShatterGlassTransition>());
				// 現在のトランジションタイプを更新
				currentType_ = type;
				break;
			default:
				break;
			}
		}
	}
}