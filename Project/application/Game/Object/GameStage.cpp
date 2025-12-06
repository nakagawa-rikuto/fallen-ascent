#include "GameStage.h"
// Service
#include "Engine/System/Service/GraphicsResourceGetter.h"
// Math
#include "Math/sMath.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// 初期化　
///-------------------------------------------///
void GameStage::Initialize(const std::string& levelData) {

	// ステージデータを読み込む
	LoadStageData(levelData);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameStage::Update() {

	// Groundの更新
	for (const auto& ground : oshans_) {
		if (ground) {
#ifdef USE_IMGUI
			ground->ShowImGui();
#endif // USE_IMGUI
			ground->Update();
		}
	}

	// オブジェクトの更新
	for (const auto& obj : objects_) {
		if (obj) {
			obj->Update();
		}
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameStage::Draw(BlendMode mode) {

	// Groundの更新
	for (const auto& ground : oshans_) {
		if (ground) {
			ground->Draw(mode);
		}
	}

	// オブジェクトの描画
	for (const auto& obj : objects_) {
		if (obj) {
			obj->Draw(mode);
		}
	}
}

///-------------------------------------------/// 
/// ステージデータを読み込む関数
///-------------------------------------------///
void GameStage::LoadStageData(const std::string& stageData) {
	// レベルデータの取得
	LevelData* levelData = GraphicsResourceGetter::GetLevelData(stageData);

	// オブジェクト分回す
	for (const auto& stage : levelData->objects) {
		if (stage.classType == LevelData::ClassTypeLevel::Ground1) {
			// Object3dの生成
			std::shared_ptr<GroundOshan> ground = std::make_shared<GroundOshan>();
			// 初期化
			ground->Initialize();

			// AABB設定
			Vector3 min = stage.translation + stage.colliderInfo1;
			Vector3 max = stage.translation + stage.colliderInfo2;
			ground->SetAABB({ min, max });

			// 座標設定
			ground->SetTranslate(stage.translation);
			ground->SetRotate(Math::QuaternionFromVector(stage.rotation));
			ground->SetScale(stage.scaling);

			// 一回更新を入れる
			ground->Update();

			// 配列に追加
			oshans_.emplace_back(ground);
		} else if (stage.classType == LevelData::ClassTypeLevel::Object1) {

			// Object3dの生成
			std::shared_ptr<StageObject> object = std::make_shared<StageObject>();
			// 初期化
			object->GameInit(stage.fileName);

			// AABB設定
			Vector3 min = stage.translation + stage.colliderInfo1;
			Vector3 max = stage.translation + stage.colliderInfo2;
			object->SetAABB({ min, max });

			// 座標設定
			object->SetTranslate(stage.translation);
			object->SetRotate(Math::QuaternionFromVector(stage.rotation));
			object->SetScale(stage.scaling);

			// 一回更新を入れる
			object->Update();

			// 配列に追加
			objects_.emplace_back(object);
		} else if (stage.classType == LevelData::ClassTypeLevel::Object2){

			// Object3dの生成
			std::shared_ptr<StageObject> object = std::make_shared<StageObject>();
			// 初期化
			object->GameInit(stage.fileName);
			Vector3 min = stage.translation + stage.colliderInfo1;
			Vector3 max = stage.translation + stage.colliderInfo2;
			object->SetAABB({ min, max });

			// 座標設定
			object->SetTranslate(stage.translation);
			object->SetRotate(Math::QuaternionFromVector(stage.rotation));
			object->SetScale(stage.scaling);

			// 一回更新を入れる
			object->Update();

			// 配列に追加
			objects_.emplace_back(object);
		} else {
			// その他のクラスは無視
			continue;
		}
	}
}
