#include "ParticleManager.h"
#include "Engine/System/Service/CameraService.h"
// c++
#include <fstream>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ParticleManager::~ParticleManager() {
	definitions_.clear();
	activeParticles_.clear();
}

///-------------------------------------------/// 
/// パーティクル定義の追加（JSONから）
///-------------------------------------------///
bool ParticleManager::LoadParticleDefinition(const std::string& jsonPath) {
	try {
		std::ifstream file(jsonPath);
		if (!file.is_open()) {
			return false;
		}

		nlohmann::json j;
		file >> j;
		file.close();

		ParticleDefinition def = ParticleDefinition::FromJson(j);
		definitions_[def.name] = def;

		return true;
	} catch (const std::exception& e) {
		// エラーハンドリング（ログ出力など）
		e;
		return false;
	}
}

///-------------------------------------------/// 
/// パーティクル定義の追加（直接指定）
///-------------------------------------------///
void ParticleManager::AddParticleDefinition(const std::string& name, const ParticleDefinition& definition) {
	definitions_[name] = definition;
}

///-------------------------------------------/// 
/// 発生
///-------------------------------------------///
bool ParticleManager::Emit(const std::string& name, const Vector3& translate) {
	// 定義を検索
	auto defIt = definitions_.find(name);
	if (defIt == definitions_.end()) {
		return false; // 定義が見つからない
	}

	// 新しいパーティクルグループを生成
	std::unique_ptr<ParticleGroup> newParticle = std::make_unique<ParticleGroup>();
	newParticle->Initialze(translate, defIt->second);
	activeParticles_[name].push_back(std::move(newParticle));

	return true;
}

///-------------------------------------------/// 
/// 全てのParticleの更新
///-------------------------------------------///
void ParticleManager::Update() {
	for (auto& [name, list] : activeParticles_) {
		for (auto it = list.begin(); it != list.end();) {
			(*it)->Update();
			if ((*it)->IsFinish()) {
				it = list.erase(it);
			} else {
				++it;
			}
		}
	}
}

///-------------------------------------------/// 
/// 全てのParticleの描画
///-------------------------------------------///
void ParticleManager::Draw(BlendMode mode) {
	for (auto& [name, list] : activeParticles_) {
		for (const auto& particle : list) {
			particle->Draw(mode);
		}
	}
}

///-------------------------------------------/// 
/// 停止処理
///-------------------------------------------///
void ParticleManager::StopParticle(const std::string& name) {
	activeParticles_.erase(name);
}

///-------------------------------------------/// 
/// パーティクル削除
///-------------------------------------------///
void ParticleManager::RemoveParticleDefinition(const std::string& name) {
	// 定義から削除
	definitions_.erase(name);

	// アクティブなパーティクルも削除
	activeParticles_.erase(name);
}
void ParticleManager::RemoveAllParticles() {
	activeParticles_.clear();
	definitions_.clear();
}



///-------------------------------------------/// 
/// テクスチャ設定
///-------------------------------------------///
void ParticleManager::SetTexture(const std::string& name, const std::string& textureName) {
	// 定義を更新
	auto defIt = definitions_.find(name);
	if (defIt != definitions_.end()) {
		defIt->second.appearance.texturePath = textureName;
	}

	// アクティブなパーティクルにも適用
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) return;

	for (auto& particle : it->second) {
		if (particle) {
			particle->SetTexture(textureName);
		}
	}
}

///-------------------------------------------/// 
/// パラメータ設定（型安全版）
///-------------------------------------------///
void ParticleManager::SetParameter(const std::string& name, ParticleParameter param, float value) {
	// 定義を更新
	auto defIt = definitions_.find(name);
	if (defIt != definitions_.end()) {
		// 定義内のパラメータを更新（次回発生時に反映）
		switch (param) {
			// 物理パラメータ
		case ParticleParameter::Gravity:
			defIt->second.physics.gravity = value;
			break;
		case ParticleParameter::ExplosionRadius:
			defIt->second.physics.explosionRadius = value;
			break;
		case ParticleParameter::UpwardForce:
			defIt->second.physics.upwardForce = value;
			break;
		case ParticleParameter::AccelerationX:
			defIt->second.physics.acceleration.x = value;
			break;
		case ParticleParameter::AccelerationY:
			defIt->second.physics.acceleration.y = value;
			break;
		case ParticleParameter::AccelerationZ:
			defIt->second.physics.acceleration.z = value;
			break;

			// 発生パラメータ
		case ParticleParameter::EmissionRate:
			defIt->second.emission.emissionRate = value;
			break;
		case ParticleParameter::Frequency:
			defIt->second.emission.frequency = value;
			break;
		case ParticleParameter::LifetimeMin:
			defIt->second.emission.lifetimeMin = value;
			break;
		case ParticleParameter::LifetimeMax:
			defIt->second.emission.lifetimeMax = value;
			break;
		case ParticleParameter::BurstCount:
			defIt->second.emission.burstCount = static_cast<uint32_t>(value);
			break;

			// 見た目パラメータ
		case ParticleParameter::StartColorR:
			defIt->second.appearance.startColor.x = value;
			break;
		case ParticleParameter::StartColorG:
			defIt->second.appearance.startColor.y = value;
			break;
		case ParticleParameter::StartColorB:
			defIt->second.appearance.startColor.z = value;
			break;
		case ParticleParameter::StartColorA:
			defIt->second.appearance.startColor.w = value;
			break;
		case ParticleParameter::EndColorR:
			defIt->second.appearance.endColor.x = value;
			break;
		case ParticleParameter::EndColorG:
			defIt->second.appearance.endColor.y = value;
			break;
		case ParticleParameter::EndColorB:
			defIt->second.appearance.endColor.z = value;
			break;
		case ParticleParameter::EndColorA:
			defIt->second.appearance.endColor.w = value;
			break;

			// 回転パラメータ
		case ParticleParameter::RotationSpeedX:
			defIt->second.rotation.rotationSpeedMin.x = value;
			break;
		case ParticleParameter::RotationSpeedY:
			defIt->second.rotation.rotationSpeedMin.y = value;
			break;
		case ParticleParameter::RotationSpeedZ:
			defIt->second.rotation.rotationSpeedMin.z = value;
			break;

			// 速度パラメータ
		case ParticleParameter::VelocityMinX:
			defIt->second.physics.velocityMin.x = value;
			break;
		case ParticleParameter::VelocityMinY:
			defIt->second.physics.velocityMin.y = value;
			break;
		case ParticleParameter::VelocityMinZ:
			defIt->second.physics.velocityMin.z = value;
			break;
		case ParticleParameter::VelocityMaxX:
			defIt->second.physics.velocityMax.x = value;
			break;
		case ParticleParameter::VelocityMaxY:
			defIt->second.physics.velocityMax.y = value;
			break;
		case ParticleParameter::VelocityMaxZ:
			defIt->second.physics.velocityMax.z = value;
			break;
		}
	}

	// アクティブなパーティクルにも適用
	auto activeIt = activeParticles_.find(name);
	if (activeIt != activeParticles_.end()) {
		for (auto& particle : activeIt->second) {
			if (particle) {
				particle->SetParameter(param, value);
			}
		}
	}
}

///-------------------------------------------/// 
/// エミッタ位置の設定
///-------------------------------------------///
void ParticleManager::SetEmitterPosition(const std::string& name, const Vector3& position, int groupIndex) {
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) return;

	// 全グループに適用
	if (groupIndex == -1) {
		for (auto& particle : it->second) {
			if (particle) {
				particle->SetEmitterPosition(position);
			}
		}
	}
	// 特定のグループに適用
	else if (groupIndex >= 0 && groupIndex < static_cast<int>(it->second.size())) {
		if (it->second[groupIndex]) {
			it->second[groupIndex]->SetEmitterPosition(position);
		}
	}
}

///-------------------------------------------/// 
/// エミッタ位置の移動
///-------------------------------------------///
void ParticleManager::MoveEmitterPosition(const std::string& name, const Vector3& offset, int groupIndex) {
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) return;

	// 全グループに適用
	if (groupIndex == -1) {
		for (auto& particle : it->second) {
			if (particle) {
				particle->MoveEmitterPosition(offset);
			}
		}
	}
	// 特定のグループに適用
	else if (groupIndex >= 0 && groupIndex < static_cast<int>(it->second.size())) {
		if (it->second[groupIndex]) {
			it->second[groupIndex]->MoveEmitterPosition(offset);
		}
	}
}

///-------------------------------------------/// 
/// エミッタ回転の設定
///-------------------------------------------///
void ParticleManager::SetEmitterRotation(const std::string& name, const Vector3& rotation, int groupIndex) {
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) return;

	// 全グループに適用
	if (groupIndex == -1) {
		for (auto& particle : it->second) {
			if (particle) {
				particle->SetEmitterRotate(rotation);
			}
		}
	}
	// 特定のグループに適用
	else if (groupIndex >= 0 && groupIndex < static_cast<int>(it->second.size())) {
		if (it->second[groupIndex]) {
			it->second[groupIndex]->SetEmitterRotate(rotation);
		}
	}
}

///-------------------------------------------/// 
/// 情報取得
///-------------------------------------------///
const ParticleDefinition* ParticleManager::GetDefinition(const std::string& name) const {
	auto it = definitions_.find(name);
	if (it != definitions_.end()) {
		return &it->second;
	}
	return nullptr;
}

bool ParticleManager::HasDefinition(const std::string& name) const {
	return definitions_.find(name) != definitions_.end();
}

uint32_t ParticleManager::GetActiveParticleCount(const std::string& name) const {
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) {
		return 0;
	}

	uint32_t totalCount = 0;
	for (const auto& particle : it->second) {
		if (particle) {
			totalCount += particle->GetActiveParticleCount();
		}
	}
	return totalCount;
}

std::vector<std::string> ParticleManager::GetDefinitionNames() const {
	std::vector<std::string> names;
	names.reserve(definitions_.size());

	for (const auto& [name, def] : definitions_) {
		names.push_back(name);
	}

	return names;
}

size_t ParticleManager::GetActiveGroupCount(const std::string& name) const {
	auto it = activeParticles_.find(name);
	if (it != activeParticles_.end()) {
		return it->second.size();
	}
	return 0;
}

///-------------------------------------------/// 
/// エミッタ位置の取得
///-------------------------------------------///
Vector3 ParticleManager::GetEmitterPosition(const std::string& name, size_t groupIndex) const {
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) {
		return Vector3{ 0.0f, 0.0f, 0.0f };
	}

	if (groupIndex < it->second.size() && it->second[groupIndex]) {
		return it->second[groupIndex]->GetEmitterPosition();
	}

	return Vector3{ 0.0f, 0.0f, 0.0f };
}