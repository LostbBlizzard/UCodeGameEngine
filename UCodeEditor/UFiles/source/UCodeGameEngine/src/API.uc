use ULang;


UCodeGameEngine:
  $LogType[byte] enum export:
   /*For Debuging and testing.*/
   Log,
   /*For Somehting risky or could cause an Error. something may be wourng*/
   Warning,
   /*A Recoverable Error may be cause by an exception or a painc.*/
   Error,
   /*Very bad Errors that may cause put the GameRunTime to be an unstable state or cause a crash. */
   Fatal,
   MinValues = 

  $Debug extern "c" export:
   export extern dynamic |Log[imut String& Msg] -> void;
   export |Log[imut StringSpan Msg] => Internal::Debug__Log0(Msg);
   export extern dynamic |LogWarning[imut String& Msg] -> void;
   export |LogWarning[imut StringSpan Msg] => Internal::Debug__LogWarning0(Msg);
   export extern dynamic |LogError[imut String& Msg] -> void;
   export |LogError[imut StringSpan Msg] => Internal::Debug__LogError0(Msg);
   export extern dynamic |LogFatalError[imut String& Msg] -> void;
   export |LogFatalError[imut StringSpan Msg] => Internal::Debug__LogFatalError0(Msg);


  $InputKey[byte] enum export:
   Null,
   /*Or*/
   Space,
   Tab,
   Escapw,
   Enter,
   Backspace,
   Delete,
   /*Other*/
   RightArrow,
   LeftArrow,
   DownArrow,
   UpArrow,
   /*Numbers*/
   Key_0,
   Key_1,
   Key_2,
   Key_3,
   Key_4,
   Key_5,
   Key_6,
   Key_7,
   Key_8,
   Key_9,
   /*Leters*/
   A,
   B,
   C,
   D,
   E,
   F,
   G,
   H,
   I,
   J,
   K,
   L,
   M,
   N,
   O,
   P,
   Q,
   R,
   S,
   T,
   U,
   V,
   W,
   X,
   Y,
   Z,
   MaxSize

  $MouseButtion[byte] enum export:
   Null,
   Left,
   Right,
   Middle,
   Scrollup,
   ScrollDown,
   MaxSize

  $Input extern "c" export:


UCodeGameEngine::Internal:
  extern dynamic |Debug__Log0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogWarning0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogError0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogFatalError0[imut StringSpan Msg] -> void;
