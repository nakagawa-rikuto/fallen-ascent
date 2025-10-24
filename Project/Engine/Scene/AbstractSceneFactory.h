#pragma once
/// ===Include=== ///
#include "Engine/Scene/IScene.h"
#include "Engine/DataInfo/SceneType.h"
#include <memory>
#include <string>

///=====================================================/// 
/// シーン工場（概念）
///=====================================================///
class AbstractSceneFactory {
public:
	
	virtual ~AbstractSceneFactory() = default;
	
	/// <summary>
	/// シーン生成処理、純粋仮想関数
	/// </summary>
	/// <param name="type">シーンの種類</param>
	/// <returns>ISceneを返す</returns>
	virtual std::unique_ptr<IScene> CreateScene(SceneType type) = 0;
};

