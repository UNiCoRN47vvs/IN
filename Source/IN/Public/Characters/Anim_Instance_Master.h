#pragma once

#include "IN/IN.h"
#include "Animation/AnimInstance.h"
#include "Anim_Instance_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
class ACharacter_Master;
class UCharacterMovementComponent;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UAnim_Instance_Master : public UAnimInstance
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY() TObjectPtr<ACharacter_Master> Character_Master;
	UPROPERTY() TObjectPtr<UCharacterMovementComponent> Movement_Component;

	UPROPERTY(BlueprintReadWrite, Category = "Anim_Instance_Master") bool Is_Jump;
	UPROPERTY(BlueprintReadWrite, Category = "Anim_Instance_Master") bool Is_Crouch;
	UPROPERTY(BlueprintReadWrite, Category = "Anim_Instance_Master") float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Anim_Instance_Master") float Direction;
};
//------------------------------------------------------------------------------------------------------------