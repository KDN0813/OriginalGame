#pragma once
#include <string>
#include <vector>
#include <imgui.h>

#ifdef _DEBUG

/**
 * ドロップダウンメニューの表示
 *
 * \param caption コンボボックスのラベル
 * \param current_item 現在開いているコンボボックスのラベル
 * \param items コンボボックスに表示する名前一覧
 * \param index 現在選択されているボックスのインデックス
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