#include "EasingMath.h"
#define _USE_MATH_DEFINES
#include <cmath>

namespace Easing {
    // リニア（線形補間）
    float Linear(float t) {
        return t;
    }

    // Quad（2次関数）
    float EaseInQuad(float t) {
        return t * t;
    }

    float EaseOutQuad(float t) {
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    float EaseInOutQuad(float t) {
        if (t < 0.5f) {
            return 2.0f * t * t;
        }
        return 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
    }

    // Cubic（3次関数）
    float EaseInCubic(float t) {
        return t * t * t;
    }

    float EaseOutCubic(float t) {
        return 1.0f - std::pow(1.0f - t, 3.0f);
    }

    float EaseInOutCubic(float t) {
        if (t < 0.5f) {
            return 4.0f * t * t * t;
        }
        return 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    // Quart（4次関数）
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

    // Quint（5次関数）
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

    // Sine（正弦波）
    float EaseInSine(float t) {
        return 1.0f - std::cos(t * static_cast<float>(M_PI) / 2.0f);
    }

    float EaseOutSine(float t) {
        return std::sin(t * static_cast<float>(M_PI) / 2.0f);
    }

    float EaseInOutSine(float t) {
        return -(std::cos(static_cast<float>(M_PI) * t) - 1.0f) / 2.0f;
    }

    // Expo（指数関数）
    float EaseInExpo(float t) {
        if (t == 0.0f) return 0.0f;
        return std::pow(2.0f, 10.0f * t - 10.0f);
    }

    float EaseOutExpo(float t) {
        if (t == 1.0f) return 1.0f;
        return 1.0f - std::pow(2.0f, -10.0f * t);
    }

    float EaseInOutExpo(float t) {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        if (t < 0.5f) {
            return std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f;
        }
        return (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    // Circ（円運動）
    float EaseInCirc(float t) {
        return 1.0f - std::sqrt(1.0f - t * t);
    }

    float EaseOutCirc(float t) {
        return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
    }

    float EaseInOutCirc(float t) {
        if (t < 0.5f) {
            return (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f;
        }
        return (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    // Back（バックオーバーシュート）
    float EaseInBack(float t) {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return c3 * t * t * t - c1 * t * t;
    }

    float EaseOutBack(float t) {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
    }

    float EaseInOutBack(float t) {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        if (t < 0.5f) {
            return (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
        }
        return (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    // Bounce（バウンド）
    float EaseOutBounce(float t) {
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
        return 1.0f - EaseOutBounce(1.0f - t);
    }

    float EaseInOutBounce(float t) {
        if (t < 0.5f) {
            return (1.0f - EaseOutBounce(1.0f - 2.0f * t)) / 2.0f;
        }
        return (1.0f + EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
    }
}