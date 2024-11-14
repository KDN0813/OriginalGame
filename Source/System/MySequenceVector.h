#pragma once
#include <vector>
#include "Misc.h"

// �|�C���^�[�^���󂯎��Ȃ��悤�ɐ�����������
template <typename T>
concept NotPointer = !std::is_pointer_v<T>;

// �����I�ɗv�f�𖄂߂Ă���std::vector�^�̃��b�p�[�N���X
template<NotPointer T>  // T���|�C���^�[�^�łȂ����Ƃ�v������
class MySequenceVector
{
public:
    /**
     * �R���X�g���N�^
     * 
     * \param num �ő�̗v�f��
     * \param initial_value �������Ɏg���l
     */
    MySequenceVector(ize_t num, T initial_value = T())
        : max_size(num), write_index(0), initial_value(initial_value)
    {
        vec.resize(num, initial_value);
    };

    ~MySequenceVector() {};  // �f�X�g���N�^�i���ݓ��ɃN���[���A�b�v�����Ȃ��j

    // �f�[�^��ǉ�����֐�
    void AddData(const T& data)
    {
        _ASSERT_EXPR_W(this->write_index < this->max_size, L"�v�f�����ő�ł�");    // �G���[�������s��
        if (this->max_size <= this->write_index) return;   // �������݃C���f�b�N�X���ő�ɒB���Ă���Ή������Ȃ�

        this->vec[this->write_index] = data;    // �w�肳�ꂽ�C���f�b�N�X�Ƀf�[�^����������
        ++this->write_index;                    // ���̏������݃C���f�b�N�X�ɐi�߂�
    }

    // �f�[�^������������֐�
    void DataClear()
    {
        // vector�̂��ׂĂ̗v�f��initial_value�ŏ�����
        std::fill(vec.begin(), vec.end(), this->initial_value);  // initial_value�ŏ�����
    }

    // ���݂�vector���擾����֐�
    const std::vector<T>& GetVector() { return this->vec; }

private:
    std::vector<T> vec;         // �v�f���i�[����std::vector
    T initial_value;            // �������Ɏg���l
    const size_t max_size;      // �ő�T�C�Y
    size_t write_index;         // ���ɏ������ރC���f�b�N�X
};