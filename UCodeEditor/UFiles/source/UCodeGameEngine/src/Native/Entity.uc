%UCodeGameEngine::Native:
     $EntityHandle = !Handle;

     [API("Entity_New")]
     |New_Entity[] -> EntityHandle;
 
     [API("Entity_Destroy")]
     |Entity_Destroy[EntityHandle Value] -> void;
    
     [API("Entity_Valid")]
     |Entity_Valid[EntityHandle Value] -> bool;

     [API("Entity_invalid")]
     |Invalid_Entity[] -> EntityHandle;