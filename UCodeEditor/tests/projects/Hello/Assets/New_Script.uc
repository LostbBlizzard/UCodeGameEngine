use ULang;
use UCodeGameEngine;

$Player[Component]:
  float Speed = 10;
   
  |Start[this&] -> void;


  |Update[this&] -> void:
   var pos = Entity().LocalPosition2D();
   var deltatime = Time::DeltaTime();
   
   if Input::IsKeyDown(InputKey::W):pos.Y += Speed * deltatime;
   if Input::IsKeyDown(InputKey::S):pos.Y -= Speed * deltatime;
   if Input::IsKeyDown(InputKey::D):pos.X += Speed * deltatime;
   if Input::IsKeyDown(InputKey::A):pos.X -= Speed * deltatime;

   Entity().LocalPosition2D(pos);
