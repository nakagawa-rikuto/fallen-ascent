#pragma once

namespace Easing {
	/// <summary>
	/// 線形イージング関数。入力値 t をそのまま返す
	/// </summary>
	/// <param name="t">補間パラメータ。通常は 0 から 1 の範囲の値を取ります。</param>
	/// <returns>入力と同じ浮動小数点値（t をそのまま返す）。</returns>
	float Linear(float t);

	/// <summary>
	/// 二次（Quad）のイーズイン（Ease-In）を計算する関数。入力に基づいて加速するイージング値を返す
	/// </summary>
	/// <param name="t">時間または進行度を表す値。通常は 0（開始）から 1（終了）の範囲で与えます。</param>
	/// <returns>イージング後の値。入力 t に対する二次的なイーズイン結果（通常 0 から 1 の範囲）。</returns>
	float EaseInQuad(float t);

	/// <summary>
	/// 二次イージングの Ease-Out 関数。進行度 t に対して減速する補間値を計算する
	/// </summary>
	/// <param name="t">アニメーションの進行度または正規化された時刻（通常 0〜1 の範囲）。</param>
	/// <returns>イージングを適用した補間値（float）。t に対して減速した出力を返します。</returns>
	float EaseOutQuad(float t);

	/// <summary>
	/// 二次のイーズイン・イーズアウト補間を行うイージング関数。進行度に応じた補間値を返す
	/// </summary>
	/// <param name="t">経過時間または進行度。通常は 0.0〜1.0 の範囲で、0 が開始、1 が終了を表します。</param>
	/// <returns>指定した進行度 t に対する補間結果（float）。イーズイン・イーズアウトの二次補間値を返します。</returns>
	float EaseInOutQuad(float t);

	/// <summary>
	/// イーズイン（加速）する3次イージング関数。進捗に応じて加速する補間値を計算する
	/// </summary>
	/// <param name="t">補間の進捗を表す値。通常は 0〜1 の範囲で指定します（0 が開始、1 が終了）。</param>
	/// <returns>時刻 t におけるイージング後の値。通常は 0〜1 の範囲の補間結果です。</returns>
	float EaseInCubic(float t);

	/// <summary>
	/// 立方イージングのイーズアウト関数。入力の進行値に対して減速する補間値を返す
	/// </summary>
	/// <param name="t">経過時間または進行比率。通常は 0.0（開始）から 1.0（終了）の範囲で使用します。</param>
	/// <returns>入力 t にイーズアウト（立方関数による減速）を適用した補間値。通常は 0.0〜1.0 の範囲で変化します。</returns>
	float EaseOutCubic(float t);

	/// <summary>
	/// 3次（キュービック）イーズイン／イーズアウトの補間を行う関数。入力に応じて開始で加速し終了で減速する補間値を返す
	/// </summary>
	/// <param name="t">正規化された時刻または進行度（通常は 0〜1 の範囲）。0 が開始、1 が終了を表す。</param>
	/// <returns>補間（イーズ）後の値を float 型で返す。通常は入力 t と同様に 0〜1 の範囲の値になる。</returns>
	float EaseInOutCubic(float t);

	/// <summary>
	/// 四乗のイーズイン（EaseInQuart）イージング関数を計算する
	/// </summary>
	/// <param name="t">進行度を表す入力値。通常は 0.0 から 1.0 の範囲で、開始から終了への正規化された時間を表します。</param>
	/// <returns>イージング適用後の値（float）。t の増加に応じて加速する四乗イージング結果を返します。</returns>
	float EaseInQuart(float t);

	/// <summary>
	/// 4次のイーズアウト（Ease-Out Quart）イージング関数。入力 t に対して減速するイージング値を計算して返す
	/// </summary>
	/// <param name="t">進行度を表す値。通常は 0 から 1 の範囲で、0 が開始、1 が終了を示します。</param>
	/// <returns>イージングが適用された値（通常は 0 ～ 1 の範囲の float）。</returns>
	float EaseOutQuart(float t);

	/// <summary>
	/// 四乗（quartic）イージングの Ease-In-Out を計算します。前半で加速し後半で減速する滑らかな補間を提供する
	/// </summary>
	/// <param name="t">正規化された時間値（通常 0.0 から 1.0）。0 が開始、1 が終了を表します。範囲外の値も受け取れますが、その場合の結果は通常の [0,1] 区間から外れる可能性があります。</param>
	/// <returns>イージング適用後の値（通常 0.0 から 1.0）。t=0 のとき 0、t=1 のとき 1 になります。</returns>
	float EaseInOutQuart(float t);

	/// <summary>
	/// イーズイン（5次）関数。入力値 t の5乗に基づいて、開始時に遅く徐々に加速するイージングを計算する
	/// </summary>
	/// <param name="t">正規化された時刻または進捗（通常は 0 から 1 の範囲）の値。</param>
	/// <returns>イージング後の値（通常は 0 から 1 の範囲）。</returns>
	float EaseInQuint(float t);

	/// <summary>
	/// イーズアウト（5次）イージング関数。時間パラメータ t に基づき、減速する補間値を返す
	/// </summary>
	/// <param name="t">補間の時間（通常は 0.0 から 1.0）。0 が開始、1 が終了を表す正規化された進行度。</param>
	/// <returns>t に対する 5次イーズアウト曲線に基づく補間値。入力範囲に応じて通常は 0.0〜1.0 の範囲になります。</returns>
	float EaseOutQuint(float t);

	/// <summary>
	/// 5次イージングのイーズインアウト補間を行う関数
	/// </summary>
	/// <param name="t">進行度を表す正規化された値（通常は 0.0 〜 1.0）。</param>
	/// <returns>指定した t に対応するイーズインアウト（5次）補間後の値（通常は 0.0 〜 1.0）。</returns>
	float EaseInOutQuint(float t);

	/// <summary>
	/// サイン曲線に基づくイーズイン関数。入力の進行率に応じて開始時にゆっくり加速する値を返す
	/// </summary>
	/// <param name="t">進行率を表す値。通常は 0.0（開始）から 1.0（終了）の範囲で使用します。</param>
	/// <returns>入力 t に対してイージングが適用された値。通常は 0.0 から 1.0 の範囲となりますが、入力の範囲に依存します。</returns>
	float EaseInSine(float t);

	/// <summary>
	/// イーズアウト（Sine）イージング関数。時間 t に基づいてイーズアウトされた値を返す
	/// </summary>
	/// <param name="t">進行時間または正規化された時間（通常は 0〜1 の範囲）。</param>
	/// <returns>イーズアウトされた値（通常は 0〜1 の範囲）。</returns>
	float EaseOutSine(float t);

	/// <summary>
	/// サイン関数に基づくイーズイン・イーズアウト補間の値を計算する
	/// </summary>
	/// <param name="t">補間のパラメータとなる正規化された時刻。通常は 0 から 1 の範囲で使用します。</param>
	/// <returns>指定した時刻 t における補間値（通常は 0 から 1 の範囲）。</returns>
	float EaseInOutSine(float t);

	/// <summary>
	/// 指数関数的なイージング（Ease-In）を計算する
	/// </summary>
	/// <param name="t">経過時間または進行率。通常は0から1の範囲で渡し、0で開始して1で終了します。</param>
	/// <returns>指定した進行に対するイージング値（通常0から1の範囲）。</returns>
	float EaseInExpo(float t);

	/// <summary>
	/// 指数関数的なイーズアウト（Ease-Out-Expo）イージング関数の値を計算します。時間の経過に伴い急速に減速する補間を提供する
	/// </summary>
	/// <param name="t">経過時間または進捗を表す値。通常は 0（開始）から 1（終了）の範囲で指定します。</param>
	/// <returns>指定された時点におけるイージング出力。通常は 0 から 1 の範囲で、進行に伴って指数関数的に減速する値を返します。</returns>
	float EaseOutExpo(float t);

	/// <summary>
	/// 指数関数的なイーズイン・イーズアウト補間を行う関数
	/// </summary>
	/// <param name="t">進行時間または正規化された時刻。通常は 0.0（開始）から 1.0（終了）までの範囲で指定する。</param>
	/// <returns>時刻 t におけるイージング後の値。通常は 0.0（開始）から 1.0（終了）までの範囲の値を返す。</returns>
	float EaseInOutExpo(float t);

	/// <summary>
	/// 円形（circular）イージングの Ease-In 値を計算する
	/// </summary>
	/// <param name="t">正規化された時間パラメーター（通常は 0 から 1）。0 が開始、1 が終了を表します。</param>
	/// <returns>指定した時間 t に対応するイージング済みの値を返します。通常は 0〜1 の範囲の補間値です。</returns>
	float EaseInCirc(float t);

	/// <summary>
	/// 円形（circ）イージングの Ease Out 関数。進行度 t に応じて終端で減速するイージング値を計算する
	/// </summary>
	/// <param name="t">正規化された時間・進行度（通常 0.0 から 1.0 の範囲）。0 が開始、1 が終了を表します。</param>
	/// <returns>入力 t に対するイージング後の値。通常は 0.0 から 1.0 の範囲で、t が増加するほど値も増加し、終了時に緩やかになります。</returns>
	float EaseOutCirc(float t);

	/// <summary>
	/// 円形のイーズイン・イーズアウト補間を計算する
	/// </summary>
	/// <param name="t">正規化された時間または進行度。通常は 0～1 の範囲で、イージング計算の入力となります。</param>
	/// <returns>イージング後の値。進行度に応じて加速・減速する補間結果（通常 0～1 の範囲）。</returns>
	float EaseInOutCirc(float t);

	/// <summary>
	/// 開始時にオーバーシュートするバックイージングを適用して進捗値を変換する
	/// </summary>
	/// <param name="t">進行時間または正規化された時刻。通常は 0（開始）から 1（終了）の範囲の値を取ります。</param>
	/// <returns>イージング後の値を表す浮動小数点数。オーバーシュートにより 0–1 の範囲を超えることがあります。</returns>
	float EaseInBack(float t);

	/// <summary>
	/// バック（オーバーシュート）効果を持つイーズアウト関数。時間パラメータに応じて減速しつつ目標を超えて戻る補間値を計算する
	/// </summary>
	/// <param name="t">正規化された時間パラメータ（通常は 0〜1）。0 が開始、1 が終了を表します。</param>
	/// <returns>指定時点の補間後の浮動小数点値。通常は 0〜1 の範囲ですが、オーバーシュートにより範囲を超える場合があります。</returns>
	float EaseOutBack(float t);

	/// <summary>
	/// バック（オーバーシュート）を伴うイーズイン・イーズアウト補間を計算する関数、アニメーションや補間処理で使用する
	/// </summary>
	/// <param name="t">正規化された進行度（時間）を示す値。通常は 0.0（開始）から 1.0（終了）の範囲で使用します。</param>
	/// <returns>引数 t に対応する補間値。バック効果により返り値が 0.0〜1.0 の範囲を一時的に超えることがあります。</returns>
	float EaseInOutBack(float t);

	/// <summary>
	/// バウンス（跳ね返り）効果を持つイーズイン関数。時間の進行に応じて跳ね返るような補間値を生成する
	/// </summary>
	/// <param name="t">正規化された時刻パラメーター（通常は 0.0 から 1.0）。0 が開始、1 が終了を表します。</param>
	/// <returns>イーズされた浮動小数点値を返します。通常は 0.0 から 1.0 の範囲で、バウンス効果を伴う補間結果となります。</returns>
	float EaseInBounce(float t);

	/// <summary>
	/// バウンス（弾む）効果のイージング関数。アニメーションの進行に応じて弾むような補間値を返す
	/// </summary>
	/// <param name="t">進行時間または正規化された位置（通常は 0.0 から 1.0 の範囲）。</param>
	/// <returns>バウンス効果を適用した補間結果（通常は 0.0 から 1.0 の範囲）。</returns>
	float EaseOutBounce(float t);

	/// <summary>
	/// バウンス（跳ね返り）効果のあるイーズイン・イーズアウト補間を計算する
	/// </summary>
	/// <param name="t">補間の進行を表す値（通常は正規化された時間、0.0〜1.0）。</param>
	/// <returns>バウンス効果を適用した補間値（通常は0.0〜1.0の範囲）。</returns>
	float EaseInOutBounce(float t);
}
