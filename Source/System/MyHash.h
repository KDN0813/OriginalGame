#pragma once

#include <utility>
#include <string>

//std::string�Ƃ��̃n�b�V���l�����L����N���X
class MyHash final
{
public:
	MyHash()noexcept : str(), hash(0) {}
	explicit MyHash(const std::string& str)noexcept :str(str), hash(std::hash<std::string>{}(str)) {}

	// ��������蓮�Őݒ肵�A���̃n�b�V���l�𐶐�����֐�
	// �ʏ�̓R���X�g���N�^�Ńn�b�V���l�𐶐����܂����A
	// �R���X�g���N�^�ȊO�ŕ������ύX�����ۂɂ��̊֐����Ăяo���܂��B
	void GenerateHash(const std::string& str)
	{
		this->str = str;
		this->hash = std::hash<std::string>{}(str);
	}

	constexpr const std::string& GetString()const noexcept { return str; }
	constexpr const size_t& GetHash()const noexcept { return hash; }

	// �Փ˂��Ȃ�==���Z�q
	[[nodiscard]] bool PerfectEqual(const MyHash& other)const noexcept
	{
		return hash==other.hash&& str == other.str;
	}

	// 43������1�̊m���ŏՓ˂���
	[[nodiscard]] bool operator==(const MyHash& other)const noexcept
	{
		return hash == other.hash;
	}

	// 43������1�̊m���ŏՓ˂���
	[[nodiscard]] bool operator!=(const MyHash& other)const noexcept
	{
		return hash != other.hash;
	}

	// 43������1�̊m���ŏՓ˂���
	[[nodiscard]] bool operator==(const size_t& other)const noexcept
	{
		return hash == other;
	}

	// 43������1�̊m���ŏՓ˂���
	[[nodiscard]] bool operator!=(const size_t& other)const noexcept
	{
		return hash != other;
	}
private:
	size_t hash;
	std::string str;
};
