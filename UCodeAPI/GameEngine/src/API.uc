use ULang;



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
   ForwardSlash,
   BackSlash,
   Dot,
   Comma,
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


//Audio.hpp
UCodeGameEngine:
  $AudioAsset export;
    



//Core.hpp
UCodeGameEngine:
  $SceneAsset export;
    

  $EntityAsset export;
    



//Rendering.hpp
UCodeGameEngine:
  $Sprite export;
    

  $Texture export;
    



//TileMap.hpp
UCodeGameEngine:
  $TileAsset export;
    



//Entity.hpp
UCodeGameEngine:
  $Component trait export:
      uintptr _Handle = 0;
      |entity[this&]  => ComponentAPI::entity(_Handle);
      |entity[imut this&] => ComponentAPI::ientity(_Handle);
    

  $ComponentAPI export extern "c":
   export extern dynamic |entity[uintptr _Handle] -> Entity&;
   export extern dynamic |ientity[uintptr _Handle] -> imut Entity&;


  $Entity export extern "c":
   export extern dynamic |Name[this&] -> imut StringSpan;
   export |Name[this&,StringSpan Value] => Internal::Entity__Name0(this,Value);
   export extern dynamic |Destroy[this&] -> void;
   export extern dynamic |Enable[this&] -> void;
   export extern dynamic |Disable[this&] -> void;
   export extern dynamic |Active[this&] -> bool;
   export |Active[this&,bool value] => Internal::Entity__Active0(this,value);
   export extern dynamic |LocalPosition[this&] -> Vec3&;
   export |LocalPosition[this&,imut Vec3& value] => Internal::Entity__LocalPosition0(this,value);
   export extern dynamic |LocalPosition2D[this&] -> Vec2;
   export |LocalPosition2D[this&,imut Vec2& value] => Internal::Entity__LocalPosition2D0(this,value);
   export extern dynamic |LocalRotation[this&] -> Vec3;
   export |LocalRotation[this&,imut Vec3& value] => Internal::Entity__LocalRotation0(this,value);
   export extern dynamic |LocalRotation2D[this&] -> Vec2;
   export |LocalRotation2D[this&,imut Vec2& value] => Internal::Entity__LocalRotation2D0(this,value);
   export extern dynamic |LocalScale[this&,imut Vec3& value] -> void;
   export |LocalScale[this&] => Internal::Entity__LocalScale0(this);
   export extern dynamic |LocalScale2D[this&,imut Vec2 value] -> void;
   export |LocalScale2D[this&] => Internal::Entity__LocalScale2D0(this);
   export extern dynamic |WorldPosition[this&] -> Vec3;
   export extern dynamic |WorldPosition2D[this&] -> Vec2;
   export extern dynamic |WorldRotation[this&] -> Vec3;
   export extern dynamic |WorldRotation2D[this&] -> Vec2;
   export extern dynamic |WorldScale[this&] -> Vec3;
   export extern dynamic |WorldScale2D[this&] -> Vec2;
   export |WorldPosition[this&,imut Vec3& Value] => Internal::Entity__WorldPosition0(this,Value);
   export |WorldPosition2D[this&,imut Vec2& Value] => Internal::Entity__WorldPosition2D0(this,Value);
   export |WorldRotation[this&,imut Vec3& Value] => Internal::Entity__WorldRotation0(this,Value);
   export |WorldRotation2D[this&,imut Vec2& Value] => Internal::Entity__WorldRotation2D0(this,Value);
   export |WorldScale[this&,imut Vec3& Value] => Internal::Entity__WorldScale0(this,Value);
   export |WorldScale2D[this&,imut Vec2& Value] => Internal::Entity__WorldScale2D0(this,Value);


UCodeGameEngine::Internal:
  extern dynamic |Entity__Name0[Entity&,StringSpan Value] -> void;
  extern dynamic |Entity__Active0[Entity&,bool value] -> void;
  extern dynamic |Entity__LocalPosition0[Entity&,imut Vec3& value] -> void;
  extern dynamic |Entity__LocalPosition2D0[Entity&,imut Vec2& value] -> void;
  extern dynamic |Entity__LocalRotation0[Entity&,imut Vec3& value] -> void;
  extern dynamic |Entity__LocalRotation2D0[Entity&,imut Vec2& value] -> void;
  extern dynamic |Entity__LocalScale0[Entity&] -> Vec3;
  extern dynamic |Entity__LocalScale2D0[Entity&] -> Vec2;
  extern dynamic |Entity__WorldPosition0[Entity&,imut Vec3& Value] -> void;
  extern dynamic |Entity__WorldPosition2D0[Entity&,imut Vec2& Value] -> void;
  extern dynamic |Entity__WorldRotation0[Entity&,imut Vec3& Value] -> void;
  extern dynamic |Entity__WorldRotation2D0[Entity&,imut Vec2& Value] -> void;
  extern dynamic |Entity__WorldScale0[Entity&,imut Vec3& Value] -> void;
  extern dynamic |Entity__WorldScale2D0[Entity&,imut Vec2& Value] -> void;


//Object.hpp
UCodeGameEngine:
  $Object<T> export:
      private:
        byte[/16] padding;
        |GetHandle[imut this&]:
          uintptr _this = unsafe bitcast<uintptr>(this);
          ret _this;
      public:
        
        |new[this&]: ObjectAPI::New(GetHandle());
        |new[this&,imut this& other]: ObjectAPI::New_Copy(GetHandle(),other.GetHandle());
        |new[this&,moved this other]: ObjectAPI::New_Move(GetHandle(),other.GetHandle());
        |drop[this&]: ObjectAPI::Drop(GetHandle());

        |Get[this&] -> T&:
          eval uintptr typehint = type(T).TypeID();
          uintptr pointer = ObjectAPI::Get(GetHandle(),typehint);
          if pointer == 0:
            //panic("Object is a Null Reference");
          ret unsafe bitcast<T&>(pointer);
        
        |Get[imut this&] -> T&:
          eval uintptr typehint = type(T).TypeID();
          uintptr pointer = ObjectAPI::Get(GetHandle(),typehint);
          if pointer == 0:
            //panic("Object is a Null Reference");
          ret unsafe bitcast<T&>(pointer);

        |~>[this&] -> T&:ret Get();
        |~>[imut this&] -> imut T&:ret Get();
    

  $ObjectAPI export extern "c":
   export extern dynamic |New[uintptr Handle] -> void;
   export extern dynamic |New_Copy[uintptr Handle,uintptr Source] -> void;
   export extern dynamic |New_Move[uintptr Handle,uintptr Source] -> void;
   export extern dynamic |Drop[uintptr Handle] -> void;
   export extern dynamic |Get[uintptr Handle,uintptr typehint] -> uintptr;




//Time.hpp
UCodeGameEngine:
  $Time export extern "c":
   export extern dynamic |DeltaTime[] -> float;




//ScirptableObject.hpp
UCodeGameEngine:
  $Asset trait export:
      uintptr _Handle = 0;
    

  $MenuItem<(Size)> tag export:
      char[/Size] MenuName;
      |new[this&,char[/Size] name]:
       MenuName = name;
    


