#pragma once

#include "AI/AI_Tasks.h"
#include "AI_Attack.generated.h"
//------------------------------------------------------------------------------------------------------------
class ACharacter_Enemy;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UAI_Attack : public UAI_Tasks
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Input") ACharacter_Enemy* Character_Enemy;
	UPROPERTY(EditAnywhere, Category = "Input") TObjectPtr<AActor> Target_Actor;
protected:

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
//------------------------------------------------------------------------------------------------------------