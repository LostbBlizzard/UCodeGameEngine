#include "ImGui.hpp"
#include "ConsoleWindow.hpp"
#include "Helper/ImGuIHelper.hpp"
#include <UCodeLang/Compilation/Helpers/CompilerTypes.hpp>
#include "Helper/StringHelper.hpp"
EditorStart


ConsoleWindow::ConsoleWindow(const NewEditorWindowData& windowdata):EditorWindow(windowdata), _LookingAtLog(nullptr)
{
    
}
ConsoleWindow::~ConsoleWindow()
{
}
 constexpr ImVec4 ConsoleInput_Color = { 1,1,1,1 };
    constexpr ImVec4 Log_Color = {1,1,1,1};
    constexpr ImVec4 Error_Color = { 1,0.7f,0.7,1 };
    constexpr ImVec4 Warning_Color = { 1,0.8f,1,1 };
void ConsoleWindow::UpdateWindow()
{
    if (_FocusWindow)
    {
        _FocusWindow = false;
        ImGui::SetWindowFocus();
   }

    if (ImGui::Button("ClearLogs"))
    {
        ClearLogs();
    }
    ImGui::SameLine();
   

    ImGuIHelper::InputText("Filter", _filter);

    ImGui::Separator();

    ImGui::BeginGroup();
    for (auto& Item : _Items)
    {
        ImVec4 Color;
        switch (Item._Type)
        {
        case LogType::Warning:
            Color = Warning_Color;
            break;
        case LogType::Error:
            Color = Error_Color;
            break;
        case LogType::ConsoleInput:
            Color = ConsoleInput_Color;
            break;
        case LogType::Log:
        default:
            Color = Log_Color;
            break;
        }
        if (StringHelper::Fllter(_filter, Item.Text))
        {
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, Color);
            ImGui::PushID(&Item);
            
            if (ImGui::Selectable(Item.Text.c_str(), _LookingAtLog == &Item))
            {
                _LookingAtLog = &Item;
            }
            if (ImGui::IsMouseDoubleClicked(0))
            {
                if (Item._OnOpen.has_value())
                {
                    Item._OnOpen.value()(*this, Item);
                }
            }

            ImGui::PopStyleColor();
            ImGui::PopID();
        }
    }
    ImGui::EndGroup();

    ImGui::Separator();
    ImGuIHelper::InputText("Input", _ConsoleIn); ImGui::SameLine();
    if (ImGui::Button("Send"))
    {
        Log Data;
        Data.Text =  _ConsoleIn;
        Data._Type = LogType::ConsoleInput;//TODO Add With ULang StringInput
        AddLog(Data);
        _ConsoleIn.clear();
    }
    if (_LookingAtLog != nullptr) 
    {
        ImGui::Separator();

        ImVec4 Color;
        switch (_LookingAtLog->_Type)
        {
        case LogType::Warning:
            Color = Warning_Color;
            break;
        case LogType::Error:
            Color = Error_Color;
            break;
        case LogType::ConsoleInput:
            Color = ConsoleInput_Color;
            break;
        case LogType::Log:
        default:
            Color = Log_Color;
            break;
        }
        ImGui::TextColored(Color, _LookingAtLog->Text.c_str());
        ImGui::TextColored(Color, _LookingAtLog->MoreText.c_str());

       
        if (_LookingAtLog->_OnOpen.has_value())
        {
            bool OnCicked = ImGui::Button("Open", { 0,20 });
            if (OnCicked) {
                _LookingAtLog->_OnOpen.value()(*this, *_LookingAtLog);
            }
        }
           
    }
}

EditorWindowData ConsoleWindow::GetEditorData()
{
    return EditorWindowData("ConsoleWindow", MakeWin);
}

void ConsoleWindow::AddLog(Log Msg)
{
    const char* TypeText;
    switch (Msg._Type)
    {
    case LogType::Log:
        TypeText = "[Log]";
        break;
    case LogType::Warning:
        TypeText = "[Warning]";
        break;
    case LogType::Error:
        TypeText = "[Error]";
        break;
    case LogType::ConsoleInput:
        TypeText = "[Input]";
        break;
    default:
        TypeText = "";
        break;
    }
    Msg.Text = (UCode::String)TypeText + ":" + Msg.Text;
    auto OldCapc = _Items.capacity();
    auto OldPtr = _LookingAtLog;
    size_t PtrIndex = 0;
    if (OldPtr != nullptr)
    {
        PtrIndex = ((uintptr_t)OldPtr - (uintptr_t)&_Items[0]) / sizeof(Log);
    }

    _Items.push_back(Msg);
    if (OldCapc != _Items.capacity() && OldPtr)//if resized
    {
        _LookingAtLog = &_Items[PtrIndex];
    }
}

EditorWindow* ConsoleWindow::MakeWin(const NewEditorWindowData& windowdata)
{
    return new ConsoleWindow(windowdata);
}

EditorEnd

