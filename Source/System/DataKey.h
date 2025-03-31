#pragma once
#include "System\MyHash.h"

// データ検索用のキー
struct DataKey
{
	static const size_t INVALID_INDEX = SIZE_MAX;

	DataKey() = delete;
	explicit DataKey(MyHash h) : hash(h), index(INVALID_INDEX) {}

	MyHash hash;  // ハッシュ値
	size_t index; // データストア内のインデックス
};

// テンプレートデータストアクラス
template<class T>
class DataStore
{
private:
	// データエントリー
	struct Entry
	{
		Entry() = default;
		Entry(MyHash h, T d) : hash(h), data(d) {}

		MyHash hash; // データ識別用のハッシュ
		T data;      // 保持するデータ
	};

private:
	std::vector<Entry> entries; // データのリスト

public:
	// キーを使用してデータを取得
	T& GetData(DataKey& key)
	{
		if (key.index == DataKey::INVALID_INDEX)
		{
			key.index = GetIndex(key);
		}
		_ASSERT_EXPR_W((key.index != DataKey::INVALID_INDEX), L"キーに対応するデータが見つかりません");
		_ASSERT_EXPR_W((key.index < entries.size()), L"インデックスが範囲外です");
		return entries[key.index].data;
	}

	// 新しいデータを追加
	Entry& AddData(MyHash hash, T data)
	{
		return entries.emplace_back(hash, data);
	}

private:
	// キーに対応するインデックスを取得
	size_t GetIndex(const DataKey& key) const
	{
		for (size_t i = 0; i < entries.size(); ++i)
		{
			if (entries[i].hash.PerfectEqual(key.hash))
			{
				return i;
			}
		}
		return DataKey::INVALID_INDEX;
	}
};