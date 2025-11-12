#include "GameStage.h"
// Service
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/ColliderService.h"
// Math
#include "Math/sMath.h"

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
	for (const auto& ground : grounds_) {
		if (ground) {
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
	for (const auto& ground : grounds_) {
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
		if (stage.classType == LevelData::ClassType::Ground1) {
			// Object3dの生成
			std::shared_ptr<StageObject> object = std::make_shared<StageObject>();
			// 初期化
			object->GameInit(stage.fileName, stage.OBBSize / 2.0f);

			// 座標設定
			object->SetTranslate(stage.translation);
			object->SetRotate(Math::QuaternionFromVector(stage.rotation));
			object->SetScale(stage.scaling);

			// 一回更新を入れる
			object->Update();

			// 配列に追加
			objects_.emplace_back(object);
		} else {

			// Object3dの生成
			std::shared_ptr<StageObject> object = std::make_shared<StageObject>();
			// 初期化
			object->GameInit(stage.fileName, stage.OBBSize / 2.0f);

			// 座標設定
			object->SetTranslate(stage.translation);
			object->SetRotate(Math::QuaternionFromVector(stage.rotation));
			object->SetScale(stage.scaling);

			// コライダーサービスに登録
			ColliderService::AddCollider(object.get());

			// 一回更新を入れる
			object->Update();

			// 配列に追加
			objects_.emplace_back(object);
		}
	}
}
