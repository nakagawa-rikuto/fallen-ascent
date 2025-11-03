#pragma once
/// ===Include=== ///
#include <memory>

/// ===前方宣言=== ///
/// <Engine>
// Core
class WinApp;
class DXCommon;
class SRVManager;
class RTVManager;
class DSVManager;
// Graphics
class PipelineManager;
class TextureManager;
class ModelManager;
class AnimationManager;
class OffScreenRenderer;
// Audio
class AudioManager;
// Level
class CSVManager;
class LevelManager;
/// <Game>
// Line
class LineObject3D;
// Input
class Keyboard;
class Mouse;
class Controller;
// Camera
class CameraManager;
// Particle
class ParticleManager;
// ColliderManager
class ColliderManager;
// DeltaTime
class DeltaTime;

/// ===サービス候補=== ///
struct ServiceRegister {
	WinApp* winApp = nullptr;
	DXCommon* dxCommon = nullptr;
	SRVManager* srvManager = nullptr;
	RTVManager* rtvManager = nullptr;
	DSVManager* dsvManager = nullptr;
	PipelineManager* pipelineManager = nullptr;
	TextureManager* textureManager = nullptr;
	ModelManager* modelManager = nullptr;
	AnimationManager* animationManager = nullptr;
	OffScreenRenderer* offScreenRenderer = nullptr;
	AudioManager* audioManager = nullptr;
	CSVManager* csvManager = nullptr;
	LevelManager* levelManager = nullptr;
	LineObject3D* lineObject3D = nullptr;
	Keyboard* keyboard = nullptr;
	Mouse* mouse = nullptr;
	Controller* controller = nullptr;
	CameraManager* cameraManager = nullptr;
	ParticleManager* particleManager = nullptr;
	ColliderManager* colliderManager = nullptr;
	DeltaTime* deltaTime = nullptr;
};

///=====================================================/// 
/// サービスロケータ
///=====================================================///
class ServiceLocator {
public:
	// 全てのサービスを提供
	static void ProvideAll(const ServiceRegister& registry);
	// 全てのサービスを終了
	static void Finalize();

	// WinApp
	static WinApp* GetWinApp();
	// DXCommon
	static DXCommon* GetDXCommon();
	// SRVManager
	static SRVManager* GetSRVManager();
	// RTVManager 
	static RTVManager* GetRTVManager();
	// DSVManager
	static DSVManager* GetDSVManager();
	// PipelineManager
	static PipelineManager* GetPipelineManager();
	// TextureManager
	static TextureManager* GetTextureManager();
	// ModelManager
	static ModelManager* GetModelManager();
	// AnimationManager
	static AnimationManager* GetAnimationManager();
	// OffScreenRenderer
	static OffScreenRenderer* GetOffScreenRenderer();
	// CSVManager
	static CSVManager* GetCSVManager();
	// LevelLoader
	static LevelManager* GetLevelManager();
	// AudioManager
	static AudioManager* GetAudioManager();
	// LineObject3D
	static LineObject3D* GetLineObject3D();
	// Input (Keyboard, Mouse, Controller)
	static Keyboard* GetKeyboard();
	static Mouse* GetMouse();
	static Controller* GetController();
	// CameraManager
	static CameraManager* GetCameraManager();
	// ParticleManager
	static ParticleManager* GetParticleManager();
	// ColliderManager
	static ColliderManager* GetColliderManager();
	// DeltaTime
	static DeltaTime* GetDeltaTime();

private:
	// Core
	static inline WinApp* winApp_ = nullptr;
	static inline DXCommon* dxCommon_ = nullptr;
	static inline SRVManager* srvManager_ = nullptr;
	static inline RTVManager* rtvManager_ = nullptr;
	static inline DSVManager* dsvManager_ = nullptr;
	// Graphics
	static inline PipelineManager* pipelineManager_ = nullptr;
	static inline TextureManager* textureManager_ = nullptr;
	static inline ModelManager* modelManager_ = nullptr;
	static inline AnimationManager* animationManager_ = nullptr;
	static inline OffScreenRenderer* offScreenRenderer_ = nullptr;
	// Level
	static inline CSVManager* csvManager_ = nullptr;
	static inline LevelManager* levelManager_ = nullptr;
	// Audio
	static inline AudioManager* audioManager_ = nullptr;
	// Game
	static inline LineObject3D* lineObject3D_ = nullptr;
	static inline Keyboard* keyboard_ = nullptr;
	static inline Mouse* mouse_ = nullptr;
	static inline Controller* controller_ = nullptr;
	static inline CameraManager* cameraManager_ = nullptr;
	static inline ParticleManager* particleManager_ = nullptr;
	static inline ColliderManager* colliderManager_ = nullptr;
	static inline DeltaTime* deltaTime_ = nullptr;
};

