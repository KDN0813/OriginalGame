#include "ImGuiHelper.h"

bool ImGui::ComboUI(
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

bool ImGui::InputSize_t(
	const std::string& label,
	size_t& value
)
{
	int v = static_cast<int>(value);
	if (ImGui::InputInt(label.c_str(), &v))
	{
		if (v < 0)
		{
			v = static_cast<int>(value);
			return false;
		};
		value = static_cast<size_t>(v);
		return true;
	}
	return false;
}

bool ImGui::InputTextString(
	const std::string& label,
	std::string& text
)
{
	char buffer[1024];
	::strncpy_s(buffer, sizeof(buffer), text.c_str(), sizeof(buffer));
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
	{
		text = std::string(buffer);
		return true;
	}
	return false;
}
;
