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
    // �f�t�H���g�R���X�g���N�^
    // ����̏����l�������Ȃ��A��̏�Ԃō쐬�����
    MySequenceVector()
        : max_size(0), write_index(0), initial_value() {}

    /**
     * @brief �R���X�g���N�^
     *
     * �ő�v�f���Ə������l���w�肵�ăx�N�^�[���쐬���܂��B
     *
     * @param num �ő�̗v�f��
     * @param initial_value �������Ɏg���l�i�f�t�H���g�l: T()�j
     */
    MySequenceVector(size_t num, T initial_value = T())
        : max_size(num), write_index(0), initial_value(initial_value)
    {
        this->vec.resize(num, initial_value); // vec�̃T�C�Y��num�ɐݒ肵�A�����l�Ŗ��߂�
    }

    // �f�X�g���N�^
    ~MySequenceVector() {}

    /**
     * @brief �R���X�g���N�^�Œl��ݒ�ł��Ȃ����p�̏������֐�
     *
     * �R���X�g���N�^�ȊO�ŏ��������s���ꍇ�ɌĂ΂�܂��B
     * ���ɏ���������Ă���ꍇ�̓G���[��Ԃ��A�ēx�������ł��Ȃ��悤�ɂ��܂��B
     *
     * @param num �ő�̗v�f��
     * @param initial_value �������Ɏg���l�i�f�t�H���g�l: T()�j
     */
    void Initialize(size_t num, T initial_value = T())
    {
        _ASSERT_EXPR_W(this->vec.empty(), L"���ɏ����ݒ肪����Ă��܂�");   // �G���[�������s��
        if (!this->vec.empty()) return;

        this->max_size = num;                   // �ő�v�f���̐ݒ�
        this->initial_value = initial_value;    // �������l�̐ݒ�
        this->vec.resize(num, initial_value);   // �x�N�^�[�̃T�C�Y��num�ɕύX���A�����l�Ŗ��߂�
    }

    /**
     * @brief �f�[�^��ǉ�����֐�
     *
     * ���̃C���f�b�N�X�Ɏw�肳�ꂽ�f�[�^��ǉ����܂��B
     * �������A�v�f�����ő�̎��͉������܂���B
     *
     * @param data �ǉ�����f�[�^
     */
    void AddData(const T& data)
    {
        //_ASSERT_EXPR_W(this->write_index < this->max_size, L"�v�f�����ő�ł�");    // �G���[�������s��
        if (this->max_size <= this->write_index) return;   // �������݃C���f�b�N�X���ő�ɒB���Ă���Ή������Ȃ�

        this->vec[this->write_index] = data;    // �w�肳�ꂽ�C���f�b�N�X�Ƀf�[�^����������
        ++this->write_index;                    // ���̏������݃C���f�b�N�X�ɐi�߂�
    }

    /**
     * @brief �f�[�^������������֐�
     *
     * vector�̂��ׂĂ̗v�f��initial_value�ŏ��������܂��B
     */
    void DataClear()
    {
        // vector�̂��ׂĂ̗v�f��initial_value�ŏ�����
        std::fill(this->vec.begin(), this->vec.end(), this->initial_value);  // initial_value�ŏ�����
        this->write_index = 0;
    }

    /**
     * @brief �v�f����Ԃ�
     *
     * @return ���݂�vector�̃T�C�Y
     */
    size_t size() { return this->vec.size(); }
    
    /**
     * @brief �z��̃|�C���^��Ԃ�
     *
     * @return vector�����̃f�[�^�ւ̃|�C���^
     */
    const T* data() { return this->vec.data(); }

    /**
     * @brief ���݂�vector���擾����֐�
     *
     * @return vector�̎Q��
     */
    const std::vector<T>& GetVector() { return this->vec; }
private:
    std::vector<T> vec;         // �v�f���i�[����std::vector
    T initial_value;            // �������Ɏg���l
    size_t max_size;            // �ő�T�C�Y
    size_t write_index;         // ���ɏ������ރC���f�b�N�X
};