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
	/// 
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	virtual std::unique_ptr<IScene> CreateScene(SceneType type) = 0;
};

