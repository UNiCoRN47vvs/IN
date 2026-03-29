#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damage.generated.h"
//------------------------------------------------------------------------------------------------------------
class ACharacter_Master;
//------------------------------------------------------------------------------------------------------------
UINTERFACE(MinimalAPI)
class UDamage : public UInterface
{
	GENERATED_BODY()
};
//------------------------------------------------------------------------------------------------------------
class IN_API IDamage
{
	GENERATED_BODY()

public:
	virtual void Apply_Damage(float damage_value, ACharacter_Master* damage_owner) = 0;
};
//------------------------------------------------------------------------------------------------------------