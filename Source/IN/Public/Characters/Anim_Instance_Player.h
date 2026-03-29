#pragma once

#include "Characters/Anim_Instance_Master.h"
#include "Anim_Instance_Player.generated.h"
//------------------------------------------------------------------------------------------------------------
class UCharacterMovementComponent;
class ACharacter_Player;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UAnim_Instance_Player : public UAnim_Instance_Master
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
};
//------------------------------------------------------------------------------------------------------------