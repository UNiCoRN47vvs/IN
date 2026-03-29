#include "AI/AI_Attack.h"
#include "Characters/Character_Enemy.h"
//------------------------------------------------------------------------------------------------------------
EStateTreeRunStatus UAI_Attack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (!Character_Enemy)
		return EStateTreeRunStatus::Failed;

	Character_Enemy->Enemy_Attack(Target_Actor);

	return EStateTreeRunStatus::Running;
}
//------------------------------------------------------------------------------------------------------------