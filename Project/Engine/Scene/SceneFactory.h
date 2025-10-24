#pragma once
/// ===Include=== ///
#include "Engine/Scene/AbstractSceneFactory.h"

///=====================================================/// 
/// シーン工場
///=====================================================///
class SceneFactory : public AbstractSceneFactory {
public:
	/// <summary>
	/// シーン生成処理
	/// </summary>
	/// <param name="type">シーンタイプ</param>
	/// <returns>ISceneを返す</returns>
	std::unique_ptr<IScene> CreateScene(SceneType type) override;
};

