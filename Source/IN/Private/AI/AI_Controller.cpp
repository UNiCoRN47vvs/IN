#include "AI/AI_Controller.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/StateTreeAIComponent.h"
#include "Characters/Character_Enemy.h"
//------------------------------------------------------------------------------------------------------------
AAI_Controller::AAI_Controller()
{
	State_Tree_Component = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("State Tree Component"));
	AI_Perception_Component = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));

	Sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	State_Tree_Component->SetStartLogicAutomatically(true);

	if (Sight_Config && AI_Perception_Component)
	{
		Sight_Config->SightRadius = 1500.0f;
		Sight_Config->LoseSightRadius = 2000.0f;
		Sight_Config->PeripheralVisionAngleDegrees = 87.5f;

		Sight_Config->DetectionByAffiliation.bDetectEnemies = true;
		Sight_Config->DetectionByAffiliation.bDetectNeutrals = true;
		Sight_Config->DetectionByAffiliation.bDetectFriendlies = true;

		AI_Perception_Component->ConfigureSense(*Sight_Config);
		AI_Perception_Component->SetDominantSense(Sight_Config->GetSenseImplementation());
	}
}
//------------------------------------------------------------------------------------------------------------
void AAI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ensureMsgf(AI_Perception_Component, TEXT("Component = nullptr!"));

	if (AI_Perception_Component)
		AI_Perception_Component->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AAI_Controller::On_Target_Perception_Updated);

	CHECK_CAST(Character_Enemy, ACharacter_Enemy, InPawn);

	Character_Enemy->Get_Flag_Delegate(EState_Flags::Attacking).AddUObject(this, &AAI_Controller::On_Attacking);
	Character_Enemy->Get_Flag_Delegate(EState_Flags::Dead).AddUObject(this, &AAI_Controller::On_Death);
	Character_Enemy->On_Was_Attacked.BindUObject(this, &AAI_Controller::Select_Target_By_Distance);
}
//------------------------------------------------------------------------------------------------------------
void AAI_Controller::On_Target_Perception_Updated(AActor* actor, FAIStimulus stimulus)
{
	CHECK_CAST(Character_Enemy, ACharacter_Enemy, GetPawn());

	if (stimulus.WasSuccessfullySensed())
	{
		if (!DA_Character_Enemy)
		{
			CHECK_CAST(Character_Enemy, ACharacter_Enemy, GetPawn());

			DA_Character_Enemy = Character_Enemy->Get_DA_Character_Enemy();
			CHECK_PTR(DA_Character_Enemy);
		}

		for (const FName& name : DA_Character_Enemy->Get_Team_Type_Enemies_Names())
		{
			if (actor->Tags.Contains(name))
			{
				Select_Target_By_Distance(actor);

				Character_Enemy->Set_Speed_Value(EStat_Name::Speed_Run);

				return;
			}
		}
	}
	else
	{
		if (actor == Target_Actor)
		{
			Target_Actor = nullptr;

			if (State_Tree_Component)
				State_Tree_Component->SendStateTreeEvent(Event_Tag_Chase_Stop);

			Character_Enemy->Set_Speed_Value(EStat_Name::Speed_Walk);
			//TODO :: Реaлизация если потеряли видимость
			return;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AAI_Controller::On_Attacking(bool is_attacking)
{
	if (!is_attacking && State_Tree_Component)
	{
		if (!Target_Actor)
		{
			CHECK_CAST(Character_Enemy, ACharacter_Enemy, GetPawn());

			State_Tree_Component->SendStateTreeEvent(Event_Tag_Attack_End);

			Character_Enemy->AddActorWorldRotation(Character_Enemy->GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f));
			return;
		}

		const double distance = FVector::Distance(GetPawn()->GetActorLocation(), Target_Actor->GetActorLocation());

		if (distance >= 70.0) //TODO :: Брать дистанцию из датаассета!
			State_Tree_Component->SendStateTreeEvent(Event_Tag_Chase_Start);
		else
			State_Tree_Component->SendStateTreeEvent(Event_Tag_Attack_End);

	}
}
//------------------------------------------------------------------------------------------------------------
void AAI_Controller::Select_Target_By_Distance(AActor* actor)
{
	if (!Target_Actor)
	{
		Target_Actor = actor;

		if (State_Tree_Component)
			State_Tree_Component->SendStateTreeEvent(Event_Tag_Chase_Start);
	}
	else
	{
		CHECK_CAST(Character_Enemy, ACharacter_Enemy, GetPawn());

		const FVector pawn_location = Character_Enemy->GetActorLocation();
		const float distance_to_target = FVector::Distance(pawn_location, Target_Actor->GetActorLocation());
		const float distance_to_damage_owner = FVector::Distance(pawn_location, actor->GetActorLocation());

		if (distance_to_damage_owner < distance_to_target)
		{
			Target_Actor = actor;

			if (State_Tree_Component)
				State_Tree_Component->SendStateTreeEvent(Event_Tag_Chase_Start);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AAI_Controller::On_Death(bool is_dead)
{
	if (is_dead)
	{
		if (AI_Perception_Component)
		{
			AI_Perception_Component->Deactivate();
			AI_Perception_Component->DestroyComponent();
		}

		if (State_Tree_Component)
		{
			State_Tree_Component->Deactivate();
			State_Tree_Component->DestroyComponent();
		}

		StopMovement();
	}
}
//------------------------------------------------------------------------------------------------------------
