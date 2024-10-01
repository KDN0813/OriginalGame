#pragma once
#include <string>
#include <vector>
#include <imgui.h>

#ifdef _DEBUG

/**
 * �h���b�v�_�E�����j���[�̕\��
 *
 * \param caption �R���{�{�b�N�X�̃��x��
 * \param current_item ���݊J���Ă���R���{�{�b�N�X�̃��x��
 * \param items �R���{�{�b�N�X�ɕ\�����閼�O�ꗗ
 * \param index ���ݑI������Ă���{�b�N�X�̃C���f�b�N�X
 * \return
 */
static bool ImGuiComboUI(
	const std::string& caption,
	std::string& current_item,
	const std::vector<std::string>& items,
	int& index
)
{
	bool changed = false;

	if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
	{
		for (int n = 0; n < items.size(); n++) {
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n].c_str(), is_selected))
			{
				current_item = items[n];
				index = n;
				changed = true;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return changed;
}

#endif // _DEBUG