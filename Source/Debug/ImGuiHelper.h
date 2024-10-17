#pragma once
#include <string>
#include <vector>
#include <imgui.h>

#ifdef _DEBUG

namespace ImGui
{
	/**
	 * �h���b�v�_�E�����j���[�̕\��
	 *
	 * \param caption �R���{�{�b�N�X�̃��x��
	 * \param current_item ���݊J���Ă���R���{�{�b�N�X�̃��x��
	 * \param items �R���{�{�b�N�X�ɕ\�����閼�O�ꗗ
	 * \param index ���ݑI������Ă���{�b�N�X�̃C���f�b�N�X
	 * \return
	 */
	static bool ComboUI(
		const std::string& caption,
		std::string& current_item,
		const std::vector<std::string>& items,
		int& index
	);

	static bool InputSize_t(
		const std::string& label,
		size_t& value
	);

	static bool InputTextString(
		const std::string& label,
		std::string& text
	);
};

#endif // _DEBUG