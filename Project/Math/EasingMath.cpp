#include "EasingMath.h"
#define _USE_MATH_DEFINES
#include <cmath>

namespace Easing {
    // 線形補間（等速で変化）
    float Linear(float t) {
        return t;
    }

    // 2次関数イージング（加速）
    float EaseInQuad(float t) {
        // 始めはゆっくり、後半で加速
        return t * t;
    }

    float EaseOutQuad(float t) {
        // 始めは速く、後半で減速
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    float EaseInOutQuad(float t) {
        // 前半は加速、後半は減速
        if (t < 0.5f) {
            return 2.0f * t * t;
        }
        return 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
    }

    // 3次関数イージング（より急な加速/減速）
    float EaseInCubic(float t) {
        // 始めは非常にゆっくり、後半で急加速
        return t * t * t;
    }

    float EaseOutCubic(float t) {
        // 始めは急加速、後半でゆっくり
        return 1.0f - std::pow(1.0f - t, 3.0f);
    }

    float EaseInOutCubic(float t) {
        // 前半は急加速、後半は急減速
        if (t < 0.5f) {
            return 4.0f * t * t * t;
        }
        return 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    // 4次関数イージング（さらに急な加速/減速）
    float EaseInQuart(float t) {
        return t * t * t * t;
    }

    float EaseOutQuart(float t) {
        return 1.0f - std::pow(1.0f - t, 4.0f);
    }

    float EaseInOutQuart(float t) {
        if (t < 0.5f) {
            return 8.0f * t * t * t * t;
        }
        return 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
    }

    // 5次関数イージング（最も急な加速/減速）
    float EaseInQuint(float t) {
        return t * t * t * t * t;
    }

    float EaseOutQuint(float t) {
        return 1.0f - std::pow(1.0f - t, 5.0f);
    }

    float EaseInOutQuint(float t) {
        if (t < 0.5f) {
            return 16.0f * t * t * t * t * t;
        }
        return 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
    }

    // サインカーブイージング（滑らかな加減速）
    float EaseInSine(float t) {
        // 始めはゆっくり、後半で加速
        return 1.0f - std::cos(t * static_cast<float>(M_PI) / 2.0f);
    }

    float EaseOutSine(float t) {
        // 始めは速く、後半でゆっくり
        return std::sin(t * static_cast<float>(M_PI) / 2.0f);
    }

    float EaseInOutSine(float t) {
        // 前半は加速、後半は減速
        return -(std::cos(static_cast<float>(M_PI) * t) - 1.0f) / 2.0f;
    }

    // 指数関数イージング（急激な加速/減速）
    float EaseInExpo(float t) {
        // 始めは非常にゆっくり、後半で急加速
        if (t == 0.0f) return 0.0f;
        return std::pow(2.0f, 10.0f * t - 10.0f);
    }

    float EaseOutExpo(float t) {
        // 始めは急加速、後半でゆっくり
        if (t == 1.0f) return 1.0f;
        return 1.0f - std::pow(2.0f, -10.0f * t);
    }

    float EaseInOutExpo(float t) {
        // 前半は急加速、後半は急減速
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        if (t < 0.5f) {
            return std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f;
        }
        return (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    // 円運動イージング（円弧のような動き）
    float EaseInCirc(float t) {
        // 始めはゆっくり、後半で加速
        return 1.0f - std::sqrt(1.0f - t * t);
    }

    float EaseOutCirc(float t) {
        // 始めは速く、後半でゆっくり
        return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
    }

    float EaseInOutCirc(float t) {
        // 前半は加速、後半は減速
        if (t < 0.5f) {
            return (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f;
        }
        return (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    // バックイージング（オーバーシュートする動き）
    float EaseInBack(float t) {
        // 始めに少し逆方向に動いてから加速
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return c3 * t * t * t - c1 * t * t;
    }

    float EaseOutBack(float t) {
        // 終わりに少しオーバーシュートしてから戻る
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
    }

    float EaseInOutBack(float t) {
        // 両端でオーバーシュートする動き
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        if (t < 0.5f) {
            return (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
        }
        return (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    // バウンドイージング（跳ねる動き）
    float EaseOutBounce(float t) {
        // 跳ねるような減速
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1.0f / d1) {
            return n1 * t * t;
        } else if (t < 2.0f / d1) {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        } else if (t < 2.5f / d1) {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        } else {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }

    float EaseInBounce(float t) {
        // 跳ねるような加速
        return 1.0f - EaseOutBounce(1.0f - t);
    }

    float EaseInOutBounce(float t) {
        // 前半は加速、後半は減速しながら跳ねる
        if (t < 0.5f) {
            return (1.0f - EaseOutBounce(1.0f - 2.0f * t)) / 2.0f;
        }
        return (1.0f + EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
    }
}