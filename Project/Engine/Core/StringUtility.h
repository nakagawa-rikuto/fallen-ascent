#pragma once
#include <string>

namespace MiiEngine {
	///=====================================================/// 
	/// 文字コードユーティリティ
	///=====================================================///
	namespace StringUtility {
		/// <summary>
		/// stringをwstringに変換する
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		std::wstring ConvertString(const std::string& str);

		/// <summary>
		/// wstringをstringに変換する
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		std::string ConvertString(const std::wstring& str);
	}
}