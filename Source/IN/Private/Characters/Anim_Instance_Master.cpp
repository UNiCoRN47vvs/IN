#include "Characters/Anim_Instance_Master.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Character_Master.h"
//------------------------------------------------------------------------------------------------------------
void UAnim_Instance_Master::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CHECK_CAST(Character_Master, ACharacter_Master, TryGetPawnOwner());

	if (!Movement_Component)
	{
		Movement_Component = Character_Master->GetCharacterMovement();
		CHECK_PTR(Movement_Component);
	}

	const FRotator rotation = Character_Master->GetActorRotation();

	Is_Jump = Movement_Component->IsFalling();
	Is_Crouch = Character_Master->IsCrouched();
	Speed = Movement_Component->Velocity.Length();
	Direction = UKismetAnimationLibrary::CalculateDirection(Movement_Component->Velocity, rotation);
}
//------------------------------------------------------------------------------------------------------------
