#pragma once

#include "CoreMinimal.h"
//------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EInteract_Item_Type : uint8
{
		Invalid = 0				UMETA(DisplayName = "Invalid"),
		Use						UMETA(DisplayName = "Use"),
		Drop						UMETA(DisplayName = "Drop"),
		Destroy					UMETA(DisplayName = "Destroy")

};
//------------------------------------------------------------------------------------------------------------