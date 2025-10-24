#pragma once
/// ===Inclde=== ///
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

///=====================================================/// 
/// CSVManager
///=====================================================///
class CSVManager {
public:

	CSVManager() = default;
	~CSVManager() = default;

	/// <summary>
	/// CSVファイルの読み込み処理
	/// </summary>
	/// <param name="file_path">CSVファイルの登録名</param>
	void Load(const std::string& file_path);

private:

	std::mutex mtx_;
	std::condition_variable cv_;
	bool data_ready_ = false;
	std::vector<std::vector<int>> map_data_;
};

