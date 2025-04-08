#include <string>
#include <imgui.h>
#include "GameTimer.h"

#ifdef _DEBUG
void GameTimer::CalculateFrameStats()
{
	this->frames++;

	if ((TimeStamp() - this->time_tlapsed) >= 1.0f)
	{
		this->fps = static_cast<float>(this->frames); // fps = frameCnt / 1
		this->mspf = 1000.0f / this->fps;
		this->frames = 0;
		this->time_tlapsed = TimeStamp();
		std::string str;
		str = "Fps : ";
		str += std::to_string(fps);
		str += "mspf : ";
		str += std::to_string(mspf);
		str += "\n";
		OutputDebugStringA(str.c_str());
	}
}
void GameTimer::DebugDrawGUI()
{
	if (ImGui::Begin("GameTimer"))
	{
		ImGui::InputFloat("FPS", &this->fps);
		ImGui::InputFloat("mspf", &this->mspf);
		ImGui::SliderInt("SyncInterval", &this->sync_interval, 0, 1);
		ImGui::InputFloat("GameSpeed", &this->game_speed);
	}
	ImGui::End();
}
#endif // _DEBUG