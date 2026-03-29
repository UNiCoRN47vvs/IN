#pragma once

#include "IN/IN.h"
#include "DetourCrowdAIController.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Controller.generated.h"
//------------------------------------------------------------------------------------------------------------
class UStateTreeAIComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class ACharacter_Enemy;
class UDA_Character_Enemy;
class ACharacter_Master;
struct FAIStimulus;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API AAI_Controller : public ADetourCrowdAIController
{
	GENERATED_BODY()
public:
	AAI_Controller();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Green") float Random_Point_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Green") float Time_To_Wait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Green") FGameplayTag Event_Tag_Wait_End;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Yellow") TObjectPtr<AActor> Target_Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Yellow") FGameplayTag Event_Tag_Chase_Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Yellow") FGameplayTag Event_Tag_Chase_Stop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Red") FGameplayTag Event_Tag_Attack_End;
protected:
	virtual void OnPossess(APawn* InPawn) override;
	void On_Attacking(bool is_attacking);
	void Select_Target_By_Distance(AActor* actor);
	void On_Death(bool is_dead);

	UFUNCTION() void On_Target_Perception_Updated(AActor* actor, FAIStimulus stimulus);

	UPROPERTY() TObjectPtr<ACharacter_Enemy> Character_Enemy;
	UPROPERTY() TObjectPtr<UDA_Character_Enemy> DA_Character_Enemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI_Controller") TObjectPtr<UStateTreeAIComponent> State_Tree_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI_Controller") TObjectPtr<UAIPerceptionComponent> AI_Perception_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI_Controller") TObjectPtr<UAISenseConfig_Sight> Sight_Config;


};
//------------------------------------------------------------------------------------------------------------