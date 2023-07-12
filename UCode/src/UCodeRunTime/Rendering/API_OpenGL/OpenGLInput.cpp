#include "OpenGLInput.hpp"
#include "OpenGLRender.hpp"
#include  <UCodeRunTime\ULibrarys\Input_Library.hpp>
#include <UCodeRunTime\ULibrarys\Rendering\Camera2d.hpp>
RenderAPIStart
static GLFWmousebuttonfun IMGUICallBack_Mbuttion = nullptr;
static GLFWcursorposfun  IMGUICallBack_MPos = nullptr;
static GLFWscrollfun IMGUICallBack_Mscroll = nullptr;
static GLFWwindowfocusfun IMGUICallBack_focusfun = nullptr;
static GLFWframebuffersizefun Imgui_FrameCallBack = nullptr;
static GLFWwindowsizefun Imgui_WinSizeCallBack = nullptr;
void Input::Set_InputCallBacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window, Input::key_callback);
    
        IMGUICallBack_Mbuttion = glfwSetMouseButtonCallback(window, mouse_callback);
        IMGUICallBack_MPos = glfwSetCursorPosCallback(window, cursor_position_callback);
        IMGUICallBack_Mscroll = glfwSetScrollCallback(window, scroll_callback);
        IMGUICallBack_focusfun = glfwSetWindowFocusCallback(window, focused_callback);
        //Imgui_FrameCallBack = glfwSetFramebufferSizeCallback(window, FramebufferSize_callback);
        Imgui_WinSizeCallBack = glfwSetWindowSizeCallback(window, FramebufferSize_callback);
    
}
InputKey GLFWToUCodeKey(int key)
{
    switch (key)
    {
    case GLFW_KEY_A:return InputKey::A;
    case GLFW_KEY_B:return InputKey::B;
    case GLFW_KEY_C:return InputKey::C;
    case GLFW_KEY_D:return InputKey::D;
    case GLFW_KEY_E:return InputKey::E;
    case GLFW_KEY_F:return InputKey::F;
    case GLFW_KEY_G:return InputKey::G;
    case GLFW_KEY_H:return InputKey::H;
    case GLFW_KEY_I:return InputKey::I;
    case GLFW_KEY_J:return InputKey::J;
    case GLFW_KEY_K:return InputKey::K;
    case GLFW_KEY_L:return InputKey::L;
    case GLFW_KEY_M:return InputKey::M;
    case GLFW_KEY_N:return InputKey::N;
    case GLFW_KEY_O:return InputKey::O;
    case GLFW_KEY_P:return InputKey::P;
    case GLFW_KEY_Q:return InputKey::Q;
    case GLFW_KEY_R:return InputKey::R;
    case GLFW_KEY_S:return InputKey::S;
    case GLFW_KEY_T:return InputKey::T;
    case GLFW_KEY_U:return InputKey::U;
    case GLFW_KEY_V:return InputKey::V;
    case GLFW_KEY_W:return InputKey::W;
    case GLFW_KEY_X:return InputKey::X;
    case GLFW_KEY_Y:return InputKey::Y;
    case GLFW_KEY_Z:return InputKey::Z;
        //Others
    case GLFW_KEY_SPACE:return InputKey::Space;
    case GLFW_KEY_TAB:return InputKey::Tab;
    case GLFW_KEY_ESCAPE:return InputKey::ESCAPE;
    case GLFW_KEY_ENTER:return InputKey::ENTER;
    case GLFW_KEY_BACKSPACE:return InputKey::BACKSPACE;
    case GLFW_KEY_DELETE:return InputKey::DELETE;

    case GLFW_KEY_RIGHT:return InputKey::R;
    case GLFW_KEY_LEFT:return InputKey::LEFTArrow;
    case GLFW_KEY_DOWN:return InputKey::DOWNArrow;
    case GLFW_KEY_UP:return InputKey::UpArrow;

    case GLFW_KEY_0:return InputKey::Key_0;
    case GLFW_KEY_1:return InputKey::Key_1;
    case GLFW_KEY_2:return InputKey::Key_2;
    case GLFW_KEY_3:return InputKey::Key_3;
    case GLFW_KEY_4:return InputKey::Key_4;
    case GLFW_KEY_5:return InputKey::Key_5;
    case GLFW_KEY_6:return InputKey::Key_6;
    case GLFW_KEY_7:return InputKey::Key_7;
    case GLFW_KEY_8:return InputKey::Key_8;
    case GLFW_KEY_9:return InputKey::Key_9;

    default:return InputKey::Null;
    }
}
InputKey GLFWMosuseToUCodeKey(int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:return InputKey::MOUSE_BUTTON_LEFT;  
    case GLFW_MOUSE_BUTTON_RIGHT:return InputKey::MOUSE_BUTTON_RIGHT;
    case GLFW_MOUSE_BUTTON_MIDDLE:return InputKey::MOUSE_BUTTON_MIDDLE;
    default:return InputKey::Null;
    }
}
InputAction GLFWActionToUCodeInputAction(int action)
{
    switch (action)
    {
    case GLFW_RELEASE:return InputAction::Release;
    case GLFW_PRESS:return InputAction::Press;
    case GLFW_REPEAT:return InputAction::Repeat;
    default:return InputAction::Release;
    }
}
CallBackType InputActionToCallBackType(InputAction _action)
{
    switch (_action)
    {
    case InputAction::Press:return CallBackType::Press;
    case InputAction::Release:return CallBackType::Release;
    case InputAction::Repeat:return CallBackType::Repeat; 
    default:return CallBackType::Release;
    }
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (IMGUICallBack_Mscroll) { IMGUICallBack_Mscroll(window, xoffset, yoffset); }
    auto OpenGl = OpenGlRender::GetOpenGlRender(window);
    const auto RuntimeInput = InputManger::GetInput(OpenGl->RunTime);


    RuntimeInput->Input_API_Set_Scroll((float32)xoffset, (float32)yoffset);

}
void Input::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (IMGUICallBack_Mbuttion) { IMGUICallBack_Mbuttion(window,button,action,mods); }

    InputAction _action = GLFWActionToUCodeInputAction(action);
    InputKey InputKey = GLFWMosuseToUCodeKey(button);
    if (InputKey == InputKey::Null) { return; }


    bool value_ = _action == InputAction::Press || _action == InputAction::Repeat;
    auto OpenGl = OpenGlRender::GetOpenGlRender(window);
    const auto RuntimeInput = InputManger::GetInput(OpenGl->RunTime);
    RuntimeInput->Input_API_SetInput(value_, InputKey);

    CallBackType type = InputActionToCallBackType(_action);
    RuntimeInput->Input_API_CallInputCallBack(InputKey, type);
}
void Input::FramebufferSize_callback(GLFWwindow* window, int width, int height)
{
    if (Imgui_FrameCallBack) { Imgui_FrameCallBack(window, width, height); }
    GlCall(glViewport(0, 0, width, height));
    auto OpenGl = OpenGlRender::GetOpenGlRender(window);
    if (Camera2d* Main = Camera2d::FindMainCam(OpenGl->RunTime))
    {
        Main->API_Set_WindowSize(width, height);
        OpenGl->UpdateScreen();
    }
}
void Input::focused_callback(GLFWwindow* window, int focused)
{
    auto OpenGl = OpenGlRender::GetOpenGlRender(window);
    const auto RuntimeInput = InputManger::GetInput(OpenGl->RunTime);
    if (focused)
    {

    }
    else
    {

    }
}
void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{   
    if (IMGUICallBack_MPos) { IMGUICallBack_MPos(window, xpos, ypos); }
    
    auto OpenGl = OpenGlRender::GetOpenGlRender(window);
    auto _Input = InputManger::GetInput(OpenGl->RunTime);
    _Input->Input_API_Set_CursorPosition((float32)xpos, (float32)ypos);

   
}
void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    
    InputAction _action = GLFWActionToUCodeInputAction(action);
    InputKey InputKey = GLFWToUCodeKey(key);
    if (InputKey == InputKey::Null) { return; }

  
   
    
    auto OpenGl =OpenGlRender::GetOpenGlRender(window);
    const auto RuntimeInput =  InputManger::GetInput(OpenGl->RunTime);
    bool value_ = _action == InputAction::Press || _action == InputAction::Repeat;
    RuntimeInput->Input_API_SetInput(value_, InputKey);


    CallBackType type = InputActionToCallBackType(_action);
    RuntimeInput->Input_API_CallInputCallBack(InputKey, type);
}



RenderAPIEnd

