use ULang;


UCodeGameEngine:
  $LogType[byte]:
   

  $Debug extern "c" :
   extern dynamic |Log[imut String& Msg] -> void;
   |Log[imut StringSpan Msg] => Internal::Debug__Log0(Msg);
   extern dynamic |LogWarning[imut String& Msg] -> void;
   |LogWarning[imut StringSpan Msg] => Internal::Debug__LogWarning0(Msg);
   extern dynamic |LogError[imut String& Msg] -> void;
   |LogError[imut StringSpan Msg] => Internal::Debug__LogError0(Msg);
   extern dynamic |LogFatalError[imut String& Msg] -> void;
   |LogFatalError[imut StringSpan Msg] => Internal::Debug__LogFatalError0(Msg);


  $InputKey[byte]:
   Null,
   

  $MouseButtion[byte]:
   Null,
   Left,
   Right,
   Middle,
   Scrollup,
   ScrollDown,
   MaxSize

  $Input extern "c" :


UCodeGameEngine::Internal:
  extern dynamic |Debug__Log0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogWarning0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogError0[imut StringSpan Msg] -> void;
  extern dynamic |Debug__LogFatalError0[imut StringSpan Msg] -> void;
