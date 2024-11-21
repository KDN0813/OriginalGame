#pragma once
#include <string>
#include <vector>
#include <imgui.h>
#include <DirectXMath.h>

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

	static bool InputSize_t(
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

	static bool InputTextString(
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

	/**
     * 角度管理を行う
     * ラジアンで計算を行う
     * 表示状は度数法で行う
     *
     * \param label ラベル
     * \param angle 角度(float3)
     * \return
     */
	static bool DragAngleSlider(
		const std::string& label,
		float& angle
	)
	{
		//　度数法に変換
		float angle_radians = DirectX::XMConvertToDegrees(angle);
		if (ImGui::SliderFloat(label.c_str(), &angle_radians, 0, 360.0f))
		{
			// ラジアン法に変換
			angle = DirectX::XMConvertToRadians(angle_radians);
			return true;
		}
		return false;
	}
	/**
	 * 角度管理を行う
	 * ラジアンで計算を行う
	 * 表示状は度数法で行う
	 * 
	 * \param label ラベル
	 * \param angle 角度(float3)
	 * \return 
	 */
	static bool DragAngle3Slider(
		const std::string& label,
		DirectX::XMFLOAT3& angle
	)
	{
		//　度数法に変換
		DirectX::XMFLOAT3 angle_radians
		{
			DirectX::XMConvertToDegrees(angle.x),
			DirectX::XMConvertToDegrees(angle.y),
			DirectX::XMConvertToDegrees(angle.z),
		};
		if (ImGui::SliderFloat3(label.c_str(), &angle_radians.x, 0, 360.0f))
		{
			// ラジアン法に変換
			angle =
			{
				DirectX::XMConvertToRadians(angle_radians.x),
				DirectX::XMConvertToRadians(angle_radians.y),
				DirectX::XMConvertToRadians(angle_radians.z),
			};
			return true;
		}
		return false;
	}
};

#endif // _DEBUG