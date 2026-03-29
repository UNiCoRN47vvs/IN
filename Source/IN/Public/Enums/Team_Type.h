#pragma once

#include "CoreMinimal.h"
//------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class ETeam_Type : uint8
{
	Invalid = 0				UMETA(DisplayName = "Invalid"),
	Player					UMETA(DisplayName = "Player"),
	Zombie					UMETA(DisplayName = "Zombie"),
	Count						UMETA(Hidden)
};
//------------------------------------------------------------------------------------------------------------