#include "ParticleService.h"
// Manager
#include "Engine/System/Managers/ParticleManager.h"
// ServiceLocator
#include "ServiceLocator.h"
// Definition
#include "Engine/Graphics/Particle/ParticleDefinition.h"
#include "Engine/Graphics/Particle/ParticleParameter.h"

///-------------------------------------------/// 
/// 定義の管理
///-------------------------------------------///

// JSONから定義を読み込み
bool ParticleService::LoadParticleDefinition(const std::string& jsonPath) {
	// ベースのディレクトリパス
	const std::string& baseDirectiorPath = "./Resource/Json/Particles";
	return ServiceLocator::GetParticleManager()->LoadParticleDefinition(baseDirectiorPath + "/" + jsonPath);
}

// 定義を直接追加
void ParticleService::AddParticleDefinition(const std::string& name, const ParticleDefinition& definition) {
	ServiceLocator::GetParticleManager()->AddParticleDefinition(name, definition);
}

// 定義を削除
void ParticleService::RemoveParticleDefinition(const std::string& name) {
	ServiceLocator::GetParticleManager()->RemoveParticleDefinition(name);
}

///-------------------------------------------/// 
/// パーティクルの操作
///-------------------------------------------///

// パーティクルを発生
bool ParticleService::Emit(const std::string& name, const Vector3& translate) {
	return ServiceLocator::GetParticleManager()->Emit(name, translate);
}

// テクスチャを設定
void ParticleService::SetTexture(const std::string& name, const std::string& textureName) {
	ServiceLocator::GetParticleManager()->SetTexture(name, textureName);
}

// パラメータを設定（型安全版）
void ParticleService::SetParameter(const std::string& name, ParticleParameter param, float value) {
	ServiceLocator::GetParticleManager()->SetParameter(name, param, value);
}

// エミッタ位置を設定
void ParticleService::SetEmitterPosition(const std::string& name, const Vector3& position, int groupIndex) {
	ServiceLocator::GetParticleManager()->SetEmitterPosition(name, position, groupIndex);
}

// エミッタ位置を移動
void ParticleService::MoveEmitterPosition(const std::string& name, const Vector3& offset, int groupIndex) {
	ServiceLocator::GetParticleManager()->MoveEmitterPosition(name, offset, groupIndex);
}

// エミッタの回転を設定
void ParticleService::SetEmitterRotate(const std::string& name, const Vector3& rotate, int groupIndex) {
	ServiceLocator::GetParticleManager()->SetEmitterRotation(name, rotate, groupIndex);
}

// パーティクルを停止
void ParticleService::StopParticle(const std::string& name) {
	ServiceLocator::GetParticleManager()->StopParticle(name);
}

// 全パーティクルを停止
void ParticleService::StopAllParticles() {
	ServiceLocator::GetParticleManager()->StopAllParticles();
}

///-------------------------------------------/// 
/// 情報取得
///-------------------------------------------///

// 定義を取得
const ParticleDefinition* ParticleService::GetDefinition(const std::string& name) {
	return ServiceLocator::GetParticleManager()->GetDefinition(name);
}

// 定義が存在するか確認
bool ParticleService::HasDefinition(const std::string& name) {
	return ServiceLocator::GetParticleManager()->HasDefinition(name);
}

// アクティブなパーティクル数を取得
uint32_t ParticleService::GetActiveParticleCount(const std::string& name) {
	return ServiceLocator::GetParticleManager()->GetActiveParticleCount(name);
}

// 定義名の一覧を取得
std::vector<std::string> ParticleService::GetDefinitionNames() {
	return ServiceLocator::GetParticleManager()->GetDefinitionNames();
}

// アクティブなグループ数を取得
size_t ParticleService::GetActiveGroupCount(const std::string& name) {
	return ServiceLocator::GetParticleManager()->GetActiveGroupCount(name);
}