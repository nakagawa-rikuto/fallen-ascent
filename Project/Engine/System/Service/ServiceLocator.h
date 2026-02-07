#pragma once
/// ===Include=== ///
#include <memory>

/// ===前方宣言=== ///
namespace MiiEngine {
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
}

namespace Service {
	/// ===サービス候補=== ///
	struct ServiceRegister {
		MiiEngine::WinApp* winApp = nullptr;
		MiiEngine::DXCommon* dxCommon = nullptr;
		MiiEngine::SRVManager* srvManager = nullptr;
		MiiEngine::RTVManager* rtvManager = nullptr;
		MiiEngine::DSVManager* dsvManager = nullptr;
		MiiEngine::PipelineManager* pipelineManager = nullptr;
		MiiEngine::TextureManager* textureManager = nullptr;
		MiiEngine::ModelManager* modelManager = nullptr;
		MiiEngine::AnimationManager* animationManager = nullptr;
		MiiEngine::OffScreenRenderer* offScreenRenderer = nullptr;
		MiiEngine::AudioManager* audioManager = nullptr;
		MiiEngine::CSVManager* csvManager = nullptr;
		MiiEngine::LevelManager* levelManager = nullptr;
		MiiEngine::LineObject3D* lineObject3D = nullptr;
		MiiEngine::Keyboard* keyboard = nullptr;
		MiiEngine::Mouse* mouse = nullptr;
		MiiEngine::Controller* controller = nullptr;
		MiiEngine::CameraManager* cameraManager = nullptr;
		MiiEngine::ParticleManager* particleManager = nullptr;
		MiiEngine::ColliderManager* colliderManager = nullptr;
		MiiEngine::DeltaTime* deltaTime = nullptr;
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
		static MiiEngine::WinApp* GetWinApp();
		// DXCommon
		static MiiEngine::DXCommon* GetDXCommon();
		// SRVManager
		static MiiEngine::SRVManager* GetSRVManager();
		// RTVManager 
		static MiiEngine::RTVManager* GetRTVManager();
		// DSVManager
		static MiiEngine::DSVManager* GetDSVManager();
		// PipelineManager
		static MiiEngine::PipelineManager* GetPipelineManager();
		// TextureManager
		static MiiEngine::TextureManager* GetTextureManager();
		// ModelManager
		static MiiEngine::ModelManager* GetModelManager();
		// AnimationManager
		static MiiEngine::AnimationManager* GetAnimationManager();
		// OffScreenRenderer
		static MiiEngine::OffScreenRenderer* GetOffScreenRenderer();
		// CSVManager
		static MiiEngine::CSVManager* GetCSVManager();
		// LevelLoader
		static MiiEngine::LevelManager* GetLevelManager();
		// AudioManager
		static MiiEngine::AudioManager* GetAudioManager();
		// LineObject3D
		static MiiEngine::LineObject3D* GetLineObject3D();
		// Input (Keyboard, Mouse, Controller)
		static MiiEngine::Keyboard* GetKeyboard();
		static MiiEngine::Mouse* GetMouse();
		static MiiEngine::Controller* GetController();
		// CameraManager
		static MiiEngine::CameraManager* GetCameraManager();
		// ParticleManager
		static MiiEngine::ParticleManager* GetParticleManager();
		// ColliderManager
		static MiiEngine::ColliderManager* GetColliderManager();
		// DeltaTime
		static MiiEngine::DeltaTime* GetDeltaTime();

	private:
		// Core
		static inline MiiEngine::WinApp* winApp_ = nullptr;
		static inline MiiEngine::DXCommon* dxCommon_ = nullptr;
		static inline MiiEngine::SRVManager* srvManager_ = nullptr;
		static inline MiiEngine::RTVManager* rtvManager_ = nullptr;
		static inline MiiEngine::DSVManager* dsvManager_ = nullptr;
		// Graphics
		static inline MiiEngine::PipelineManager* pipelineManager_ = nullptr;
		static inline MiiEngine::TextureManager* textureManager_ = nullptr;
		static inline MiiEngine::ModelManager* modelManager_ = nullptr;
		static inline MiiEngine::AnimationManager* animationManager_ = nullptr;
		static inline MiiEngine::OffScreenRenderer* offScreenRenderer_ = nullptr;
		// Level
		static inline MiiEngine::CSVManager* csvManager_ = nullptr;
		static inline MiiEngine::LevelManager* levelManager_ = nullptr;
		// Audio
		static inline MiiEngine::AudioManager* audioManager_ = nullptr;
		// Game
		static inline MiiEngine::LineObject3D* lineObject3D_ = nullptr;
		static inline MiiEngine::Keyboard* keyboard_ = nullptr;
		static inline MiiEngine::Mouse* mouse_ = nullptr;
		static inline MiiEngine::Controller* controller_ = nullptr;
		static inline MiiEngine::CameraManager* cameraManager_ = nullptr;
		static inline MiiEngine::ParticleManager* particleManager_ = nullptr;
		static inline MiiEngine::ColliderManager* colliderManager_ = nullptr;
		static inline MiiEngine::DeltaTime* deltaTime_ = nullptr;
	};
}