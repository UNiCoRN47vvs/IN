#include "AI/AI_Wait.h"
#include "Components/StateTreeAIComponent.h"
//------------------------------------------------------------------------------------------------------------
EStateTreeRunStatus UAI_Wait::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UWorld* world = Context.GetWorld();
	Owner_Actor = Cast<AActor>(Context.GetOwner());

	if (!world || !Owner_Actor.IsValid())
	{
		ALARM_LOG;
		return EStateTreeRunStatus::Failed;
	}

	world->GetTimerManager().SetTimer(Wait_Timer_Handle, this, &UAI_Wait::On_Wait_Finished, Time_To_Wait, false);

	return EStateTreeRunStatus::Running;
}
//------------------------------------------------------------------------------------------------------------
void UAI_Wait::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UWorld* world = Context.GetWorld();
	if (world)
		world->GetTimerManager().ClearTimer(Wait_Timer_Handle);
}
//------------------------------------------------------------------------------------------------------------
void UAI_Wait::On_Wait_Finished()
{
	if (Owner_Actor.IsValid())
	{
		UStateTreeAIComponent* component = Owner_Actor->FindComponentByClass<UStateTreeAIComponent>();
		if (component)
			component->SendStateTreeEvent(Event_Tag);
	}

	FinishTask(true);
}
//------------------------------------------------------------------------------------------------------------
