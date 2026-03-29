#pragma once

#include "IN/IN.h"
#include "Engine/LevelScriptActor.h"
#include "Main_Level.generated.h"
//------------------------------------------------------------------------------------------------------------
class ADirectionalLight;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API AMain_Level : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	AMain_Level();
protected:
	virtual void BeginPlay() override;
	void Set_Sun_Light_Rotation();

	uint8 Is_Night : 1;
	uint8 Is_Day : 1;
	float Cycle_Angle;
	FTimerHandle Day_Cycle_Handle;
	UPROPERTY() TObjectPtr<ADirectionalLight> Sun_Light;
	UPROPERTY() float Day_Cycle_Angle;
	UPROPERTY() float Night_Cycle_Angle;
	UPROPERTY() float Day_Cycle_Speed;
};
//------------------------------------------------------------------------------------------------------------