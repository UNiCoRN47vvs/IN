#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character_State.generated.h"
//------------------------------------------------------------------------------------------------------------
UINTERFACE(MinimalAPI)
class UCharacter_State : public UInterface
{
	GENERATED_BODY()
};
//------------------------------------------------------------------------------------------------------------
class IN_API ICharacter_State
{
	GENERATED_BODY()

public:

	virtual bool Is_Attack_Blocked() const = 0;
	virtual bool Is_Move_Blocked() const = 0;
	virtual bool Is_Inventory_Blocked() const = 0;

};
//------------------------------------------------------------------------------------------------------------