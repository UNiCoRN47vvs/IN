#pragma once

#include "AI/AI_Tasks.h"
#include "AI_Find_Random_Location.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS(Meta = (DisplayName = "Find Random Location"))
class IN_API UAI_Find_Random_Location : public UAI_Tasks
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Input") float Radius;
	UPROPERTY(EditAnywhere, Category = "Output") FVector Random_Location;
protected:

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
};
//------------------------------------------------------------------------------------------------------------