#pragma once

#include "Modules/ModuleManager.h"
//------------------------------------------------------------------------------------------------------------
class FMACRO_MASTERModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
//------------------------------------------------------------------------------------------------------------

#pragma region MACRO
//-----------------------------------------------------------------------------------------------------------
//проверка на нульпоинтер и оповещение в случае нульпоинтера
#if !UE_BUILD_SHIPPING
#define CHECK_PTR(Ptr)\
       if (!(Ptr)) {\
           UE_LOG(LogTemp, Warning, TEXT("Null pointer in function %s, line %d"),\
               *FString(__FUNCTION__), __LINE__);\
           return;}
#else
#define CHECK_PTR(Ptr)\
         if (!(Ptr)) {\
            return;}
#endif
//-----------------------------------------------------------------------------------------------------------
//#if !UE_BUILD_SHIPPING
#define CHECK_WEAK_PTR(Ptr)\
       if (!Ptr.IsValid()) {\
           UE_LOG(LogTemp, Warning, TEXT("Null pointer in function %s, line %d"),\
               *FString(__FUNCTION__), __LINE__);\
           return;}
//#else
//#define CHECK_PTR(Ptr)\
//         if (!(Ptr)) {\
//            return;}
//#endif
//-----------------------------------------------------------------------------------------------------------
#define ALARM_LOG \
      UE_LOG(LogTemp, Warning, TEXT("Null pointer in function %s, line %d"), \
               *FString(__FUNCTION__), __LINE__);

//-----------------------------------------------------------------------------------------------------------
#define CHECK_ALARM(Ptr, Text) \
   if(!Ptr) { \
      ensureMsgf(Ptr, Text); \
      return;}
//-----------------------------------------------------------------------------------------------------------
#define CHECK_CAST(Variable, Class, Object) \
     if(!(Variable))  {\
         Variable = Cast<Class>(Object);\
         CHECK_PTR(Variable)\
     };
//-----------------------------------------------------------------------------------------------------------
#define PTR(Ptr) \
           if (!(Ptr)) {\
           UE_LOG(LogTemp, Warning, TEXT("Null pointer in function %s, line %d"),\
               *FString(__FUNCTION__), __LINE__);\
           return;}\
           Ptr
//-----------------------------------------------------------------------------------------------------------
#define TIME_SPENT_START \
   double TimeSpentStart = 0.0; \
   double TimeSpentEnd = 0.0; \
   double TimeSpentResult = 0.0; \
   TimeSpentStart = FPlatformTime::Seconds(); \
//-----------------------------------------------------------------------------------------------------------
#define TIME_SPENT_END \
   TimeSpentEnd = FPlatformTime::Seconds(); \
   TimeSpentResult = TimeSpentEnd - TimeSpentStart; \
   UE_LOG(LogTemp, Warning, TEXT(" --\\-- TIME SPENT SECONDS: %f --\\--"), TimeSpentResult);
//-----------------------------------------------------------------------------------------------------------
#define DEBUG_MESSAGE(Time, String) \
      if(GEngine) GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Blue, String);
//-----------------------------------------------------------------------------------------------------------
#define ENUM_TO_TEXT(Value, Class, Text_Value) \
      const UEnum* enumPtr = StaticEnum<Class>(); \
     enumPtr  ? Text_Value = enumPtr->GetDisplayNameTextByValue(static_cast<int64>(Value)) : Text_Value = FText::FromString("Unknown");
#pragma endregion
