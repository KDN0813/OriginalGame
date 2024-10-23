#include "Audio.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // DEBUG


Audio::Audio()
    :Singleton(this)
{
}

#ifdef _DEBUG

void Audio::DebugDrawGUI()
{
    if (ImGui::Begin("Audio"))
    {

    }
    ImGui::End();
}

#endif // DEBUG