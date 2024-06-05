use ULang;



//Exporter.hpp
UCodeGameEngineEditor:
  $Exporter trait export:
      uintptr _Handle = 0;
    



//OpenedProject.hpp
UCodeGameEngine:
  $AssetPath export extern "c":
   export extern dynamic |ID[this&] -> uint64;


  $OpenedProject export extern "c":
   export extern dynamic |GetProjectDirPath[] -> Path;




//Window.hpp
UCodeGameEngineEditor:
  $Window trait export:
      uintptr _Handle = 0;
    

  $OverWriteTypeDraw trait export;
    

  $EditorUI export:
     export |Field[imut StringSpan FieldName,uint8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,uint16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,uint32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,uint64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

      
     export |Field[imut StringSpan FieldName,int8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,int16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,int32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,int64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

     export |Field[imut StringSpan FieldName,uintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);
     export |Field[imut StringSpan FieldName,sintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);

     //export |Field<T>[imut StringSpan FieldName,T& Value] => false;
     //Readonly

     export |Field[imut StringSpan FieldName,imut uint8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut uint16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut uint32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut uint64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

      
     export |Field[imut StringSpan FieldName,imut int8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut int16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut int32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut int64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

     export |Field[imut StringSpan FieldName,imut uintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);
     export |Field[imut StringSpan FieldName,imut sintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);

     //export |Field<T>[imut StringSpan FieldName,T& Value] => false;
    

UCodeGameEngineEditorInternal:
  $WindowDrawAPI export extern "c":
   export extern dynamic |Field[imut StringSpan FieldName,uint8& Value] -> bool;
   export |Field[imut StringSpan FieldName,uint16& Value] => Internal::WindowDrawAPI__Field0(FieldName,Value);
   export |Field[imut StringSpan FieldName,uint32& Value] => Internal::WindowDrawAPI__Field1(FieldName,Value);
   export |Field[imut StringSpan FieldName,uint64& Value] => Internal::WindowDrawAPI__Field2(FieldName,Value);
   export |Field[imut StringSpan FieldName,int8& Value] => Internal::WindowDrawAPI__Field3(FieldName,Value);
   export |Field[imut StringSpan FieldName,int16& Value] => Internal::WindowDrawAPI__Field4(FieldName,Value);
   export |Field[imut StringSpan FieldName,int32& Value] => Internal::WindowDrawAPI__Field5(FieldName,Value);
   export |Field[imut StringSpan FieldName,int64& Value] => Internal::WindowDrawAPI__Field6(FieldName,Value);
   export extern dynamic |Fieldptr[imut StringSpan FieldName,uintptr& Value] -> bool;
   export |Fieldptr[imut StringSpan FieldName,sintptr& Value] => Internal::WindowDrawAPI__Fieldptr0(FieldName,Value);
   export |Field[imut StringSpan FieldName,bool& Value] => Internal::WindowDrawAPI__Field7(FieldName,Value);
   export |Field[imut StringSpan FieldName,float32& Value] => Internal::WindowDrawAPI__Field8(FieldName,Value);
   export |Field[imut StringSpan FieldName,float64& Value] => Internal::WindowDrawAPI__Field9(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut uint8& Value] => Internal::WindowDrawAPI__Field10(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut uint16& Value] => Internal::WindowDrawAPI__Field11(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut uint32& Value] => Internal::WindowDrawAPI__Field12(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut uint64& Value] => Internal::WindowDrawAPI__Field13(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut int8& Value] => Internal::WindowDrawAPI__Field14(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut int16& Value] => Internal::WindowDrawAPI__Field15(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut int32& Value] => Internal::WindowDrawAPI__Field16(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut int64& Value] => Internal::WindowDrawAPI__Field17(FieldName,Value);
   export |Fieldptr[imut StringSpan FieldName,imut uintptr& Value] => Internal::WindowDrawAPI__Fieldptr1(FieldName,Value);
   export |Fieldptr[imut StringSpan FieldName,imut sintptr& Value] => Internal::WindowDrawAPI__Fieldptr2(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut bool& Value] => Internal::WindowDrawAPI__Field18(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut float32& Value] => Internal::WindowDrawAPI__Field19(FieldName,Value);
   export |Field[imut StringSpan FieldName,imut float64& Value] => Internal::WindowDrawAPI__Field20(FieldName,Value);
   export extern dynamic |BeginReadOnly[] -> void;
   export extern dynamic |EndReadOnly[] -> void;


UCodeGameEngineEditorInternal::Internal:
  extern dynamic |WindowDrawAPI__Field0[imut StringSpan FieldName,uint16& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field1[imut StringSpan FieldName,uint32& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field2[imut StringSpan FieldName,uint64& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field3[imut StringSpan FieldName,int8& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field4[imut StringSpan FieldName,int16& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field5[imut StringSpan FieldName,int32& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field6[imut StringSpan FieldName,int64& Value] -> bool;
  extern dynamic |WindowDrawAPI__Fieldptr0[imut StringSpan FieldName,sintptr& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field7[imut StringSpan FieldName,bool& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field8[imut StringSpan FieldName,float32& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field9[imut StringSpan FieldName,float64& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field10[imut StringSpan FieldName,imut uint8& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field11[imut StringSpan FieldName,imut uint16& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field12[imut StringSpan FieldName,imut uint32& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field13[imut StringSpan FieldName,imut uint64& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field14[imut StringSpan FieldName,imut int8& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field15[imut StringSpan FieldName,imut int16& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field16[imut StringSpan FieldName,imut int32& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field17[imut StringSpan FieldName,imut int64& Value] -> bool;
  extern dynamic |WindowDrawAPI__Fieldptr1[imut StringSpan FieldName,imut uintptr& Value] -> bool;
  extern dynamic |WindowDrawAPI__Fieldptr2[imut StringSpan FieldName,imut sintptr& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field18[imut StringSpan FieldName,imut bool& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field19[imut StringSpan FieldName,imut float32& Value] -> bool;
  extern dynamic |WindowDrawAPI__Field20[imut StringSpan FieldName,imut float64& Value] -> bool;

