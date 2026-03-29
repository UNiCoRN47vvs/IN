#pragma once

#include "AI/AI_Tasks.h"
#include "GameplayTagContainer.h"
#include "AI_Wait.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Wait"))
class IN_API UAI_Wait : public UAI_Tasks
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Input") float Time_To_Wait;
	UPROPERTY(EditAnywhere, Category = "Input") FGameplayTag Event_Tag;
protected:

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void On_Wait_Finished();

	TWeakObjectPtr<AActor> Owner_Actor;
	FTimerHandle Wait_Timer_Handle;


};
//------------------------------------------------------------------------------------------------------------