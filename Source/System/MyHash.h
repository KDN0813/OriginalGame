#pragma once

#include <utility>
#include <string>

//std::stringとそのハッシュ値を所有するクラス
class MyHash final
{
public:
	MyHash()noexcept : str(), hash(0) {}
	explicit MyHash(const std::string& str)noexcept :str(str), hash(std::hash<std::string>{}(str)) {}

	// 文字列を手動で設定し、そのハッシュ値を生成する関数
	// 通常はコンストラクタでハッシュ値を生成しますが、
	// コンストラクタ以外で文字列を変更した際にこの関数を呼び出します。
	void GenerateHash(const std::string& str)
	{
		this->str = str;
		this->hash = std::hash<std::string>{}(str);
	}

	constexpr const std::string& GetString()const noexcept { return str; }
	constexpr const size_t& GetHash()const noexcept { return hash; }

	// 衝突しない==演算子
	[[nodiscard]] bool PerfectEqual(const MyHash& other)const noexcept
	{
		return hash==other.hash&& str == other.str;
	}

	// 43億分の1の確率で衝突する
	[[nodiscard]] bool operator==(const MyHash& other)const noexcept
	{
		return hash == other.hash;
	}

	// 43億分の1の確率で衝突する
	[[nodiscard]] bool operator!=(const MyHash& other)const noexcept
	{
		return hash != other.hash;
	}

	// 43億分の1の確率で衝突する
	[[nodiscard]] bool operator==(const size_t& other)const noexcept
	{
		return hash == other;
	}

	// 43億分の1の確率で衝突する
	[[nodiscard]] bool operator!=(const size_t& other)const noexcept
	{
		return hash != other;
	}
private:
	size_t hash;
	std::string str;
};
