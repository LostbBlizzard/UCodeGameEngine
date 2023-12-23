use ULang;
use UCodeGameEngine;

$Script[Component]:
  float Speed = 10;
  //Called once when the script is loaded.
  |Start[this&] -> void:
   //


  //Called once every Frame.
  Vec2 newpos = [5,10];
  |Update[this&] -> void:
   var& pos = entity().localposition2d();
   var deltatime = Time::DeltaTime();
   
   if Input::IsKeyDown(InputKey::W):pos.Y += Speed * deltatime;
   if Input::IsKeyDown(InputKey::S):pos.Y -= Speed * deltatime;

   if Input::IsKeyDown(InputKey::D):pos.X += Speed * deltatime;
   if Input::IsKeyDown(InputKey::A):pos.X -= Speed * deltatime;

   //
