#pragma once
#include "System\MyHash.h"

// �f�[�^�����p�̃L�[
struct DataKey
{
	static const size_t INVALID_INDEX = SIZE_MAX;

	DataKey() = delete;
	explicit DataKey(MyHash h) : hash(h), index(INVALID_INDEX) {}

	MyHash hash;  // �n�b�V���l
	size_t index; // �f�[�^�X�g�A���̃C���f�b�N�X
};

// �e���v���[�g�f�[�^�X�g�A�N���X
template<class T>
class DataStore
{
private:
	// �f�[�^�G���g���[
	struct Entry
	{
		Entry() = default;
		Entry(MyHash h, T d) : hash(h), data(d) {}

		MyHash hash; // �f�[�^���ʗp�̃n�b�V��
		T data;      // �ێ�����f�[�^
	};

private:
	std::vector<Entry> entries; // �f�[�^�̃��X�g

public:
	// �L�[���g�p���ăf�[�^���擾
	T& GetData(DataKey& key)
	{
		if (key.index == DataKey::INVALID_INDEX)
		{
			key.index = GetIndex(key);
		}
		_ASSERT_EXPR_W((key.index != DataKey::INVALID_INDEX), L"�L�[�ɑΉ�����f�[�^��������܂���");
		_ASSERT_EXPR_W((key.index < entries.size()), L"�C���f�b�N�X���͈͊O�ł�");
		return entries[key.index].data;
	}

	// �V�����f�[�^��ǉ�
	Entry& AddData(MyHash hash, T data)
	{
		return entries.emplace_back(hash, data);
	}

private:
	// �L�[�ɑΉ�����C���f�b�N�X���擾
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