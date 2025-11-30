#include "ParticleDefinition.h"

///-------------------------------------------/// 
/// JSON変換: ToJson
///-------------------------------------------///
nlohmann::json ParticleDefinition::ToJson() const {
	nlohmann::json j;

	// 基本設定
	j["name"] = name;
	j["modelName"] = modelName;
	j["shape"] = shape;
	j["maxInstance"] = maxInstance;

	// 物理設定
	j["physics"] = {
		{"velocityMin", Vector3ToJson(physics.velocityMin)},
		{"velocityMax", Vector3ToJson(physics.velocityMax)},
		{"acceleration", Vector3ToJson(physics.acceleration)},
		{"gravity", physics.gravity},
		{"explosionRadius", physics.explosionRadius},
		{"upwardForce", physics.upwardForce},
		{"useRandomVelocity", physics.useRandomVelocity}
	};

	// 見た目設定
	j["appearance"] = {
		{"startColor", Vector4ToJson(appearance.startColor)},
		{"endColor", Vector4ToJson(appearance.endColor)},
		{"startScaleMin", Vector3ToJson(appearance.startScaleMin)},
		{"startScaleMax", Vector3ToJson(appearance.startScaleMax)},
		{"endScale", Vector3ToJson(appearance.endScale)},
		{"texturePath", appearance.texturePath},
		{"useColorGradient", appearance.useColorGradient},
		{"useScaleAnimation", appearance.useScaleAnimation}
	};

	// 回転設定
	j["rotation"] = {
		{"rotationSpeedMin", Vector3ToJson(rotation.rotationSpeedMin)},
		{"rotationSpeedMax", Vector3ToJson(rotation.rotationSpeedMax)},
		{"randomRotation", rotation.randomRotation},
		{"enableRotation", rotation.enableRotation}
	};

	// 発生設定
	j["emission"] = {
		{"lifetimeMin", emission.lifetimeMin},
		{"lifetimeMax", emission.lifetimeMax},
		{"emissionRate", emission.emissionRate},
		{"burstCount", emission.burstCount},
		{"frequency", emission.frequency},
		{"isBurst", emission.isBurst}
	};

	// 高度設定
	j["advanced"] = {
		{"motion", MotionSettingsToJson(advanced.motion)},
		{"emissionPattern", EmissionPatternToJson(advanced.emissionPattern)},
		{"colorGradient", ColorGradientToJson(advanced.colorGradient)},
		{"scaleAnimation", ScaleAnimationToJson(advanced.scaleAnimation)},
		{"isTrajectoryParticle", advanced.isTrajectoryParticle},
		{"trailSpacing", advanced.trailSpacing},
		{"clearOnStop", advanced.clearOnStop}
	};

	return j;
}

///-------------------------------------------/// 
/// JSON変換: FromJson
///-------------------------------------------///
ParticleDefinition ParticleDefinition::FromJson(const nlohmann::json& json) {
	ParticleDefinition def;

	// 基本設定
	if (json.contains("name")) def.name = json["name"];
	if (json.contains("modelName")) def.modelName = json["modelName"];
	if (json.contains("shape")) def.shape = json["shape"];
	if (json.contains("maxInstance")) def.maxInstance = json["maxInstance"];

	// 物理設定
	if (json.contains("physics")) {
		const auto& p = json["physics"];
		if (p.contains("velocityMin")) def.physics.velocityMin = JsonToVector3(p["velocityMin"]);
		if (p.contains("velocityMax")) def.physics.velocityMax = JsonToVector3(p["velocityMax"]);
		if (p.contains("acceleration")) def.physics.acceleration = JsonToVector3(p["acceleration"]);
		if (p.contains("gravity")) def.physics.gravity = p["gravity"];
		if (p.contains("explosionRadius")) def.physics.explosionRadius = p["explosionRadius"];
		if (p.contains("upwardForce")) def.physics.upwardForce = p["upwardForce"];
		if (p.contains("useRandomVelocity")) def.physics.useRandomVelocity = p["useRandomVelocity"];
	}

	// 見た目設定
	if (json.contains("appearance")) {
		const auto& a = json["appearance"];
		if (a.contains("startColor")) def.appearance.startColor = JsonToVector4(a["startColor"]);
		if (a.contains("endColor")) def.appearance.endColor = JsonToVector4(a["endColor"]);
		if (a.contains("startScaleMin")) def.appearance.startScaleMin = JsonToVector3(a["startScaleMin"]);
		if (a.contains("startScaleMax")) def.appearance.startScaleMax = JsonToVector3(a["startScaleMax"]);
		if (a.contains("endScale")) def.appearance.endScale = JsonToVector3(a["endScale"]);
		if (a.contains("texturePath")) def.appearance.texturePath = a["texturePath"];
		if (a.contains("useColorGradient")) def.appearance.useColorGradient = a["useColorGradient"];
		if (a.contains("useScaleAnimation")) def.appearance.useScaleAnimation = a["useScaleAnimation"];
	}

	// 回転設定
	if (json.contains("rotation")) {
		const auto& r = json["rotation"];
		if (r.contains("rotationSpeedMin")) def.rotation.rotationSpeedMin = JsonToVector3(r["rotationSpeedMin"]);
		if (r.contains("rotationSpeedMax")) def.rotation.rotationSpeedMax = JsonToVector3(r["rotationSpeedMax"]);
		if (r.contains("randomRotation")) def.rotation.randomRotation = r["randomRotation"];
		if (r.contains("enableRotation")) def.rotation.enableRotation = r["enableRotation"];
	}

	// 発生設定
	if (json.contains("emission")) {
		const auto& e = json["emission"];
		if (e.contains("lifetimeMin")) def.emission.lifetimeMin = e["lifetimeMin"];
		if (e.contains("lifetimeMax")) def.emission.lifetimeMax = e["lifetimeMax"];
		if (e.contains("emissionRate")) def.emission.emissionRate = e["emissionRate"];
		if (e.contains("burstCount")) def.emission.burstCount = e["burstCount"];
		if (e.contains("frequency")) def.emission.frequency = e["frequency"];
		if (e.contains("isBurst")) def.emission.isBurst = e["isBurst"];
	}

	// 高度設定
	if (json.contains("advanced")) {
		const auto& adv = json["advanced"];
		if (adv.contains("motion")) def.advanced.motion = JsonToMotionSettings(adv["motion"]);
		if (adv.contains("emissionPattern")) def.advanced.emissionPattern = JsonToEmissionPattern(adv["emissionPattern"]);
		if (adv.contains("colorGradient")) def.advanced.colorGradient = JsonToColorGradient(adv["colorGradient"]);
		if (adv.contains("scaleAnimation")) def.advanced.scaleAnimation = JsonToScaleAnimation(adv["scaleAnimation"]);
		if (adv.contains("isTrajectoryParticle")) def.advanced.isTrajectoryParticle = adv["isTrajectoryParticle"];
		if (adv.contains("trailSpacing")) def.advanced.trailSpacing = adv["trailSpacing"];
		if (adv.contains("clearOnStop")) def.advanced.clearOnStop = adv["clearOnStop"];
	}

	return def;
}

///-------------------------------------------/// 
/// Vector3のJSON変換
///-------------------------------------------///
nlohmann::json ParticleDefinition::Vector3ToJson(const Vector3& vec) {
	return nlohmann::json::array({ vec.x, vec.y, vec.z });
}
Vector3 ParticleDefinition::JsonToVector3(const nlohmann::json& json) {
	if (json.is_array() && json.size() >= 3) {
		return Vector3{ json[0], json[1], json[2] };
	}
	return Vector3{ 0.0f, 0.0f, 0.0f };
}

///-------------------------------------------/// 
/// Vector4のJSON変換
///-------------------------------------------///
nlohmann::json ParticleDefinition::Vector4ToJson(const Vector4& vec) {
	return nlohmann::json::array({ vec.x, vec.y, vec.z, vec.w });
}
Vector4 ParticleDefinition::JsonToVector4(const nlohmann::json& json) {
	if (json.is_array() && json.size() >= 4) {
		return Vector4{ json[0], json[1], json[2], json[3] };
	}
	return Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
}

///-------------------------------------------/// 
/// MotionSettingsのJson変換
///-------------------------------------------///
nlohmann::json ParticleDefinition::MotionSettingsToJson(const ParticleMotionSettings& motion) {
	return {
		{"enableSwirling", motion.enableSwirling},
		{"swirlingSpeed", motion.swirlingSpeed},
		{"expansionRate", motion.expansionRate},
		{"followEmitter", motion.followEmitter},
		{"followStrength", motion.followStrength},
		{"useRotationInfluence", motion.useRotationInfluence},
		{"rotationInfluence", motion.rotationInfluence},
		{"velocityDamping", motion.velocityDamping},
		{"enableBillboardRotation", motion.enableBillboardRotation},
		{"billboardRotationSpeed", motion.billboardRotationSpeed}
	};
}
ParticleMotionSettings ParticleDefinition::JsonToMotionSettings(const nlohmann::json& json) {
	ParticleMotionSettings motion;
	if (json.contains("enableSwirling")) motion.enableSwirling = json["enableSwirling"];
	if (json.contains("swirlingSpeed")) motion.swirlingSpeed = json["swirlingSpeed"];
	if (json.contains("expansionRate")) motion.expansionRate = json["expansionRate"];
	if (json.contains("followEmitter")) motion.followEmitter = json["followEmitter"];
	if (json.contains("followStrength")) motion.followStrength = json["followStrength"];
	if (json.contains("useRotationInfluence")) motion.useRotationInfluence = json["useRotationInfluence"];
	if (json.contains("rotationInfluence")) motion.rotationInfluence = json["rotationInfluence"];
	if (json.contains("velocityDamping")) motion.velocityDamping = json["velocityDamping"];
	if (json.contains("enableBillboardRotation")) motion.enableBillboardRotation = json["enableBillboardRotation"];
	if (json.contains("billboardRotationSpeed")) motion.billboardRotationSpeed = json["billboardRotationSpeed"];
	return motion;
}

///-------------------------------------------/// 
/// EmissionPatternのJson変換
///-------------------------------------------///
nlohmann::json ParticleDefinition::EmissionPatternToJson(const ParticleEmissionPattern& pattern) {
	return {
	   {"pattern", pattern.pattern},
	   {"patternRadius", pattern.patternRadius},
	   {"patternAngle", pattern.patternAngle},
	   {"particlesPerEmit", pattern.particlesPerEmit}
	};
}
ParticleEmissionPattern ParticleDefinition::JsonToEmissionPattern(const nlohmann::json& json) {
	ParticleEmissionPattern pattern;
	if (json.contains("pattern")) pattern.pattern = json["pattern"];
	if (json.contains("patternRadius")) pattern.patternRadius = json["patternRadius"];
	if (json.contains("patternAngle")) pattern.patternAngle = json["patternAngle"];
	if (json.contains("particlesPerEmit")) pattern.particlesPerEmit = json["particlesPerEmit"];
	return pattern;
}

///-------------------------------------------/// 
/// ColorGradientのJson変換
///-------------------------------------------///
nlohmann::json ParticleDefinition::ColorGradientToJson(const ParticleColorGradient& gradient) {
	return {
		{"useGradient", gradient.useGradient},
		{"primaryColor", Vector4ToJson(gradient.primaryColor)},
		{"secondaryColor", Vector4ToJson(gradient.secondaryColor)},
		{"colorBlendCurve", gradient.colorBlendCurve},
		{"oscillateColor", gradient.oscillateColor}
	};
}
ParticleColorGradient ParticleDefinition::JsonToColorGradient(const nlohmann::json& json) {
	ParticleColorGradient gradient;
	if (json.contains("useGradient")) gradient.useGradient = json["useGradient"];
	if (json.contains("primaryColor")) gradient.primaryColor = JsonToVector4(json["primaryColor"]);
	if (json.contains("secondaryColor")) gradient.secondaryColor = JsonToVector4(json["secondaryColor"]);
	if (json.contains("colorBlendCurve")) gradient.colorBlendCurve = json["colorBlendCurve"];
	if (json.contains("oscillateColor")) gradient.oscillateColor = json["oscillateColor"];
	return gradient;
}

///-------------------------------------------/// 
/// ScaleAnimationのJson変換
///-------------------------------------------///
nlohmann::json ParticleDefinition::ScaleAnimationToJson(const ParticleScaleAnimation animation) {
	return {
		{"type", animation.type},
		{"startScale", animation.startScale},
		{"maxScale", animation.maxScale},
		{"endScale", animation.endScale}
	};
}
ParticleScaleAnimation ParticleDefinition::JsonToScaleAnimation(const nlohmann::json& json) {
	ParticleScaleAnimation animation;
	if (json.contains("type")) animation.type = json["type"];
	if (json.contains("startScale")) animation.startScale = json["startScale"];
	if (json.contains("maxScale")) animation.maxScale = json["maxScale"];
	if (json.contains("endScale")) animation.endScale = json["endScale"];
	return animation;
}
