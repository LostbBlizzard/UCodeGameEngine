#pragma once
#include "UCodeRunTime/CoreRunTime.hpp"
#include "Helper/AppFiles.hpp"
EditorStart

#define CustomMenuName "Custom"

class MakeableCompoents
{
public:
    typedef UCode::Compoent* (*Make)(UCode::Entity* entity);
    
    
    struct Lib
    {
        const String Name;
        UCode::UComponentsID Id;
        Make MakeCompoent;
        
    };
    const static Lib* Compoents;
    const static size_t Compoents_Size;

    
    inline static  const char* NewMenuName = "/Menu";
    inline static  const char* EndMenuName = "/End";

    template<typename T>
    static UCode::Compoent* Make_Compoent(UCode::Entity* entity)
    {
        return entity->AddCompoent<T>();
    }
private:
   
};
EditorEnd
