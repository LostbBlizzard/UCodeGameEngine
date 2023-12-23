use ULang;



//Entity.hpp
UCodeGameEngine:
  $Component trait:
      uintptr _Handle = 0;
      |entity[this&]  => ComponentAPI::entity(_Handle);
      |ientity[this&] => ComponentAPI::ientity(_Handle);
    

  $ComponentAPI export extern "c":
   export extern dynamic |entity[uintptr _Handle] -> Entity&;
   export extern dynamic |ientity[uintptr _Handle] -> imut Entity&;


  $Entity export extern "c":
   export extern dynamic |Enable[this&] -> void;
   export extern dynamic |Disable[this&] -> void;
   export extern dynamic |active[this&] -> bool&;
   export extern dynamic |iactive[this&] -> imut bool&;
   export extern dynamic |Destroy[this&] -> void;
   export extern dynamic |name[this&] -> imut StringSpan;
   export |name[this&,StringSpan Value] => Internal::Entity__name0(this,Value);
   export extern dynamic |localposition[this&] -> Vec3&;
   export extern dynamic |ilocalposition[this&] -> imut Vec3&;
   export extern dynamic |localposition2d[this&] -> Vec2&;
   export extern dynamic |ilocalposition2d[this&] -> imut Vec2&;
   export extern dynamic |localrotation[this&] -> Vec3&;
   export extern dynamic |ilocalrotation[this&] -> imut Vec3&;
   export extern dynamic |localrotation2d[this&] -> Vec2&;
   export extern dynamic |ilocalrotation2d[this&] -> imut Vec2&;
   export extern dynamic |localscale[this&] -> Vec3&;
   export extern dynamic |ilocalscale[this&] -> imut Vec3&;
   export extern dynamic |localscale2d[this&] -> Vec2&;
   export extern dynamic |ilocalscale2d[this&] -> imut Vec2&;
   export extern dynamic |worldposition[this&] -> Vec3;
   export extern dynamic |worldposition2d[this&] -> Vec2;
   export extern dynamic |worldrotation[this&] -> Vec3;
   export extern dynamic |worldrotation2d[this&] -> Vec2;
   export extern dynamic |worldscale[this&] -> Vec3;
   export extern dynamic |worldscale2d[this&] -> Vec2;
   export |worldposition[this&,imut Vec3& Value] => Internal::Entity__worldposition0(this,Value);
   export |worldposition[this&,imut Vec2& Value] => Internal::Entity__worldposition1(this,Value);
   export |worldrotation[this&,imut Vec3& Value] => Internal::Entity__worldrotation0(this,Value);
   export |worldrotation[this&,imut Vec2& Value] => Internal::Entity__worldrotation1(this,Value);
   export |worldscale[this&,imut Vec3& Value] => Internal::Entity__worldscale0(this,Value);
   export |worldscale[this&,imut Vec2& Value] => Internal::Entity__worldscale1(this,Value);


UCodeGameEngine::Internal:
  extern dynamic |Entity__name0[Entity&,StringSpan Value] -> void;
  extern dynamic |Entity__worldposition0[Entity&,imut Vec3& Value] -> void;
  extern dynamic |Entity__worldposition1[Entity&,imut Vec2& Value] -> void;
  extern dynamic |Entity__worldrotation0[Entity&,imut Vec3& Value] -> void;
  extern dynamic |Entity__worldrotation1[Entity&,imut Vec2& Value] -> void;
  extern dynamic |Entity__worldscale0[Entity&,imut Vec3& Value] -> void;
  extern dynamic |Entity__worldscale1[Entity&,imut Vec2& Value] -> void;


//InputManger.hpp
UCodeGameEngine:
  $InputKey[byte] enum export:
   Null,
   /*Mouse*/
   LeftMouseButtion,
   RightMouseButtion,
   MiddleMouseButtion,
   MouseScrollUp,
   MouseScrollDown,
   /*Other*/
   Space,
   Tab,
   Escape,
   Enter,
   BackSpace,
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

  $Input export extern "c":
   export extern dynamic |IsKeyDown[InputKey Key] -> bool;
   export extern dynamic |GetMousePosition[] -> Vec2;




//Loger.hpp
UCodeGameEngine:
  $Debug export extern "c":
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


//Time.hpp
UCodeGameEngine:
  $Time export extern "c":
   export extern dynamic |DeltaTime[] -> float;




//ScirptableObject.hpp
UCodeGameEngine:
  $Asset trait:
      uintptr _Handle = 0;
    


