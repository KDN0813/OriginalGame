#pragma once
#include <string>
#include <vector>
#include <imgui.h>

#ifdef _DEBUG

namespace ImGui
{
	/**
	 * ドロップダウンメニューの表示
	 *
	 * \param caption コンボボックスのラベル
	 * \param current_item 現在開いているコンボボックスのラベル
	 * \param items コンボボックスに表示する名前一覧
	 * \param index 現在選択されているボックスのインデックス
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