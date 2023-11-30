use ULang;


//InputManger.hppUCodeGameEngine:
  $InputKey[byte] enum export:
   Null,
   /*Mouse*/
   MOUSE_BUTTON_LEFT,
   MOUSE_BUTTON_RIGHT,
   MOUSE_BUTTON_MIDDLE,
   MOUSE_SCROLL_UP,
   MOUSE_SCROLL_Down,
   /*Other*/
   Space,
   ab,
   SCAPE,
   NTER,
   ACKSPACE,
   ELETE,
   /*Other*/
   RIGHTArrow,
   EFTArrow,
   OWNArrow,
   pArrow,
   /*Numbers*/
   Key_0,
   ey_1,
   Key_2,
   Key_3,
   Key_4,
   Key_5,
   Key_6,
   Key_7,
   Key_8,
   ey_9,
   /*Leters*/
   A,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   ,
   MaxSize

  $Input export extern "c":
   export extern dynamic |IsKeyDown[InputKey Key] -> bool;
   export extern dynamic |GetMousePosition[] -> Vec2;


//Loger.hpp  $Debug export extern "c":
   export extern dynamic |Log[imut String& Msg] -> void;
   export |Log[imut StringSpan Msg] => Internal::Debug__Log0(Msg);
   export extern dynamic |LogWarning[imut String& Msg] -> void;
   export |LogWarning[imut StringSpan Msg] => Internal::Debug__LogWarning0(Msg);
   export extern dynamic |LogError[imut String& Msg] -> void;
   export |LogError[imut StringSpan Msg] => Internal::Debug__LogError0(Msg);
   export extern dynamic |LogFatalError[imut String& Msg] -> void;
   export |LogFatalError[imut StringSpan Msg] => Internal::Debug__LogFatalError0(Msg);


UCodeGameEngine::Internal:
  extern dynamic |Debug__Log0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogWarning0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogError0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogFatalError0[imut StringSpan Msg] -> void;
