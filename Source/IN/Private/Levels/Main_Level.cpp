#include "Levels/Main_Level.h"
#include "EngineUtils.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
//------------------------------------------------------------------------------------------------------------
AMain_Level::AMain_Level()
{
	PrimaryActorTick.bCanEverTick = true;

	Is_Day = false;
	Is_Night = false;
	Day_Cycle_Angle = 0.1f;
	Night_Cycle_Angle = Day_Cycle_Angle * 2.0f;
	Day_Cycle_Speed = 1.0f;
}
//------------------------------------------------------------------------------------------------------------
void AMain_Level::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(Day_Cycle_Handle, this, &AMain_Level::Set_Sun_Light_Rotation, Day_Cycle_Speed, true);
}
//------------------------------------------------------------------------------------------------------------
void AMain_Level::Set_Sun_Light_Rotation()
{
	if (!Sun_Light)
	{
		for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
		{
			Sun_Light = *It;
			break;
		}

		if (!Sun_Light)
		{
			ensureMsgf(Sun_Light, TEXT("Sun_Light NULLPTR!!!"));
			return;
		}
	}

	float current_pitch = 0.0f;

	Sun_Light->AddActorLocalRotation(FRotator{Cycle_Angle, 0.0f, 0.0f});
	
	current_pitch = Sun_Light->GetActorRotation().Pitch;

	if (!Is_Night && current_pitch > 0.0f)
	{
		Is_Night = true;
		Is_Day = false;

		Cycle_Angle = Night_Cycle_Angle;
	}
	else if (!Is_Day && current_pitch < 0.0f)
	{
		Is_Night = false;
		Is_Day = true;

		Cycle_Angle = Day_Cycle_Angle;
	}

}
//------------------------------------------------------------------------------------------------------------
