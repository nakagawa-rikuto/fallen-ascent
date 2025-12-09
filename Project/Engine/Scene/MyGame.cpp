#include "MyGame.h"
// シーンファクトリー
#include "Engine/Scene/SceneFactory.h"
#include "Engine/System/Service/Loader.h"
#include "Engine/Core/Logger.h"
// c++
#include <iostream>
#include <thread>
#include <chrono>

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
MyGame::MyGame() = default;
MyGame::~MyGame() {
	sceneFactory_.reset();
	sceneManager_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void MyGame::Initialize(const wchar_t* title) {
	// 基底クラスの初期化
	Framework::Initialize(title);
	
	/// ===時間計測=== ///
	auto start = std::chrono::high_resolution_clock::now();

	/// ===読み込み処理=== ///
	// 各読み込み処理用のスレッドを生成
	LoadTexture();
	std::vector<std::thread> loadingThreads;
	loadingThreads.emplace_back([this] { LoadAudio(); });
	loadingThreads.emplace_back([this] { LoadModel(); });
	loadingThreads.emplace_back([this] { LoadAnimation(); });
	// すべてのスレッドの終了を待機
	for (auto& thread : loadingThreads) {
		thread.join();
	}

	// 処理時間を計測（end）
	auto end = std::chrono::high_resolution_clock::now();
	// 経過時間をミリ秒単位で出力
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	Log("time: " + std::to_string(duration.count()) + " ms\n");

	/// ===シーンの作成=== ///
	// シーンファクトリーの生成
	sceneFactory_ = std::make_unique<SceneFactory>();
	// シーンマネージャの初期化
	sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->Initialize(sceneFactory_.get());
	sceneManager_->ChangeScene(SceneType::Title);   //　スタートシーンの設定
}
 
///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void MyGame::Finalize() {
	// シーンの解放処理
	sceneManager_.reset();
	sceneFactory_.reset();
	// 基底クラスの終了処理
	Framework::Finalize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void MyGame::Update() {
	// 基底クラスの更新処理
	Framework::Update();
	// シーンの更新
	sceneManager_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void MyGame::Draw() {
	// 描画前処理
	Framework::PreDraw();
	// シーンの描画
	sceneManager_->Draw();
	// 描画後処理
	Framework::PostDraw();
}

///-------------------------------------------/// 
/// 読み込み関数
///-------------------------------------------///
// 音
void MyGame::LoadAudio() {
#pragma region Wave
	Loader::LoadWave("fanfare", "fanfare.wav");
#pragma endregion
#pragma region MP3
	Loader::LoadMP3("clear", "clear.mp3");
#pragma endregion
}
// テクスチャ
void MyGame::LoadTexture() {
	// DebugSceneで使用。
	Loader::LoadTexture("uvChecker", "uvChecker.png");
	Loader::LoadTexture("monsterBall", "monsterBall.png");
	Loader::LoadTexture("White", "WhiteTexture.png");
	// Particleで使用。
	Loader::LoadTexture("circle", "Particle/circle.png");
	Loader::LoadTexture("circle2", "Particle/circle2.png");
	Loader::LoadTexture("gradationLine", "Particle/gradationLine.png");
	// OffScreenで使用
	Loader::LoadTexture("noise0", "OffScreen/noise0.png");
	Loader::LoadTexture("noise1", "OffScreen/noise1.png");
	// DDS
	Loader::LoadTexture("skyBox", "DDS/rostock_laage_airport_4k.dds");
	// Game(Title)
	Loader::LoadTexture("TitleBG", "TitleUI/SkyBG.png");
	Loader::LoadTexture("TitleBGKiri", "TitleUI/BGsecond.png");
	Loader::LoadTexture("Title", "TitleUI/Title.png");
	Loader::LoadTexture("Start", "TitleUI/Start.png");
	Loader::LoadTexture("Option", "TitleUI/Option.png");
	Loader::LoadTexture("Exit", "TitleUI/Exit.png");
	Loader::LoadTexture("OverLay", "TitleUI/OverLay.png");
	// GameAnimaiton
	Loader::LoadTexture("GameOvverAnimation", "Animation/GameOverAnimation.png");
}
// モデル
void MyGame::LoadModel() {
	// DebugSceneで使用
	Loader::LoadModel("MonsterBall", "MonsterBall/MonsterBall.obj");
	Loader::LoadModel("terrain", "terrain/terrain.obj");
	// Particleで使用
	Loader::LoadModel("plane", "Particle/Plane/ParticlePlane.gltf");
	Loader::LoadModel("sphere", "Particle/Sphere/ParticleSphere.obj");
	Loader::LoadModel("cube", "Particle/Cube/ParticleCube.obj");
	Loader::LoadModel("triangle", "Particle/Triangle/ParticleTriangle.obj");
	// Game
	Loader::LoadModel("player", "player/player.gltf");
	Loader::LoadModel("Ground", "Ground/Ground.obj");
	Loader::LoadModel("Object1", "Object1/Object1.obj");
	Loader::LoadModel("Object2", "Object2/Object2.obj");
	Loader::LoadModel("PlayerWeapon", "PlayerWeapon/PlayerWeapon.obj");
}
// アニメーション
void MyGame::LoadAnimation() {
	// DebugSceneで使用。
	Loader::LoadAnimation("simpleSkin", "simpleSkin/simpleSkin.gltf");
	Loader::LoadAnimation("human", "human/sneakWalk.gltf");
}