#include "Characters/Character_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Actor_Components/Stat_Enemy_Component.h"
#include "Actor_Components/Inventory_Component.h"
//------------------------------------------------------------------------------------------------------------
ACharacter_Enemy::ACharacter_Enemy()
{
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::Enemy_Attack(AActor* target_actor)
{
	if (target_actor)
	{
		const FVector target_location = target_actor->GetActorLocation();

		FVector direction = target_location - GetActorLocation();

		direction.Z = 0.0f;


		SetActorRotation(direction.Rotation());
	}

	
	S_Attack();
}
//------------------------------------------------------------------------------------------------------------
UStat_Enemy_Component* ACharacter_Enemy::Get_Stat_Enemy_Component()
{
	return Stat_Enemy_Component ? Stat_Enemy_Component : Stat_Enemy_Component = FindComponentByClass<UStat_Enemy_Component>();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::BeginPlay()
{
	Super::BeginPlay();

	if (Get_Stat_Enemy_Component())
		Stat_Enemy_Component->Init_Stat_Component(DA_Character);

	if (GetCharacterMovement() && DA_Character)
		GetCharacterMovement()->MaxWalkSpeed = DA_Character->Stats.FindRef(EStat_Name::Speed_Walk);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::Set_Speed_Value(EStat_Name stat_name)
{
	CHECK_ALARM(GetCharacterMovement(), TEXT("Component = nullptr!"));
	CHECK_ALARM(DA_Character, TEXT("DA = nullptr!"));

	GetCharacterMovement()->MaxWalkSpeed = DA_Character->Stats.FindRef(stat_name);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::Apply_Damage(float damage_value, ACharacter_Master* damage_owner)
{
	Super::Apply_Damage(damage_value, damage_owner);

	CHECK_PTR(Get_Stat_Enemy_Component());

	UDA_Character_Enemy* da_character_enemy = Cast<UDA_Character_Enemy>(DA_Character);
	if (da_character_enemy)
	{
		for (const FName& name : da_character_enemy->Get_Team_Type_Friend_Names())
		{
			if (damage_owner->Tags.Contains(name))
				return; //Если тот кто наносит урон для нас друг то мы не получаем урон
		}
	}

	Stat_Enemy_Component->Apply_Damage(damage_value);

	On_Was_Attacked.ExecuteIfBound(damage_owner);
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::Apply_Death()
{
	Super::Apply_Death();
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::Client_Apply_Death()
{
	Super::Client_Apply_Death();

	if (DA_Character->Death_Montages.IsEmpty())
	{
		ALARM_LOG;
		return;
	}

	TSoftObjectPtr<UAnimMontage>& soft_anim_montage = DA_Character->Death_Montages[FMath::RandRange(0, DA_Character->Death_Montages.Num() - 1)];

	if (soft_anim_montage.Get())
		PlayAnimMontage(soft_anim_montage.Get());
	else
	{
		TWeakObjectPtr<ACharacter_Enemy> weak_this = this;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(soft_anim_montage.ToSoftObjectPath(), [weak_this, soft_anim_montage]()
			{
				if (weak_this.IsValid() && soft_anim_montage.Get())
					weak_this->PlayAnimMontage(soft_anim_montage.Get());
			});

	}

	GetMesh()->SetCollisionProfileName(TEXT("Dead_Enemy"));
}
//------------------------------------------------------------------------------------------------------------
FInteract_Info& ACharacter_Enemy::Get_Interact_Info()
{
	return Interact_Info;
}
//------------------------------------------------------------------------------------------------------------
void ACharacter_Enemy::Interact(AActor* actor)
{
	ACharacter_Master* character_master = Cast<ACharacter_Master>(actor);
	UDA_Character_Enemy* da_character_enemy = Cast<UDA_Character_Enemy>(DA_Character);
	CHECK_PTR(character_master);
	CHECK_PTR(da_character_enemy);

	character_master->C_Toggle_Inventory(da_character_enemy->DA_Interact_Widget_Class, Get_Inventory_Component());
}
//------------------------------------------------------------------------------------------------------------
