#pragma
#include "../UDefs.hpp"

#if UCodeGEOpenGL
#include "API_OpenGL/OpenGlRender.hpp"
#endif

RenderAPIStart

#if UCodeGEOpenGL
    using RenderAPI = OpenGlRender;
#endif
RenderAPIEnd