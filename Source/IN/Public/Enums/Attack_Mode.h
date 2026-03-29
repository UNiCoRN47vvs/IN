#pragma once

#include "CoreMinimal.h"
//------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EAttack_Mode : uint8
{
	None				UMETA(DisplayName = "None"),
	Melee				UMETA(DisplayName = "Melee"),
	Ranged			UMETA(DisplayName = "Ranged")

};
//------------------------------------------------------------------------------------------------------------