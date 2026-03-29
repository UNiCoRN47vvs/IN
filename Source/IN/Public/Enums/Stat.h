#pragma once

#include "CoreMinimal.h"
//------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EStat_Name : uint8
{
	Invalid = 0				UMETA(DisplayName = "Invalid"),
	Health_Current			UMETA(DisplayName = "Health_Current"),
	Health_Max				UMETA(DisplayName = "Health_Max"),
	Health_Regen			UMETA(DisplayName = "Health_Regen"),
	Health_Regen_Time		UMETA(DisplayName = "Health_Regen_Time"),
	Stamina_Current		UMETA(DisplayName = "Stamina_Current"),
	Stamina_Max				UMETA(DisplayName = "Stamina_Max"),
	Stamina_Regen			UMETA(DisplayName = "Stamina_Regen"),
	Stamina_Regen_Time	UMETA(DisplayName = "Stamina_Regen_Time"),
	Hunger_Current			UMETA(DisplayName = "Hunger_Current"),
	Hunger_Max				UMETA(DisplayName = "Hunger_Max"),
	Hunger_Waste			UMETA(DisplayName = "Hunger_Waste"),
	Thirst_Current			UMETA(DisplayName = "Thirst_Current"),
	Thirst_Max				UMETA(DisplayName = "Thirst_Max"),
	Thirst_Waste			UMETA(DisplayName = "Thirst_Waste"),
	Speed_Walk				UMETA(DisplayName = "Speed_Walk"),
	Speed_Run				UMETA(DisplayName = "Speed_Run"),
	Speed_Sprint			UMETA(DisplayName = "Speed_Sprint"),
	Count						UMETA(Hidden)

};
//------------------------------------------------------------------------------------------------------------