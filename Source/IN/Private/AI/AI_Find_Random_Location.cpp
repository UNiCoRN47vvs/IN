#include "AI/AI_Find_Random_Location.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "StateTreeExecutionContext.h"
//------------------------------------------------------------------------------------------------------------
EStateTreeRunStatus UAI_Find_Random_Location::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UNavigationSystemV1* nav_sys = nullptr;
	AActor* owner_actor = nullptr;
	UObject* owner = Context.GetOwner();
	FVector origin_location = {};
	FNavLocation result_location = {};

	//Context.SendEvent(FGameplayTag::RequestGameplayTag(TEXT("AI.Chaise"))); //отправка ивента

	if (!owner)
		return EStateTreeRunStatus::Failed;

	owner_actor = Cast<AActor>(owner);
	if (!owner_actor)
		return EStateTreeRunStatus::Failed;

	origin_location = owner_actor->GetActorLocation();

	// Логика поиска точки на навмеше
	nav_sys = UNavigationSystemV1::GetCurrent(owner->GetWorld());
	if (nav_sys && nav_sys->GetRandomReachablePointInRadius(origin_location, Radius, result_location))
	{
		Random_Location = result_location;
		return EStateTreeRunStatus::Running;
		//return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Failed;
}
//------------------------------------------------------------------------------------------------------------