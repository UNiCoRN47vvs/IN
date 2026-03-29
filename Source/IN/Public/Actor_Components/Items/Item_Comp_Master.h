#pragma once

#include "IN/IN.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Item_Representation.h"
#include "Enums/Attack_Mode.h"
#include "Item_Comp_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Master;
class UItem_Master;
//------------------------------------------------------------------------------------------------------------
struct FAttack_Sphere_Info
{
	FVector Local_Top = {};
	FVector Local_Bottom = {};
	TArray<TEnumAsByte<EObjectTypeQuery>> Object_Types = { EObjectTypeQuery::ObjectTypeQuery3 };
};
//------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IN_API UItem_Comp_Master : public UActorComponent, public IItem_Representation
{
	GENERATED_BODY()

public:	
	UItem_Comp_Master();
#pragma region IItem_Representation
	inline virtual UDA_Item_Master* Get_Item_DA() const override { return Item_DA; }
#pragma endregion

	virtual void Init_Item_Comp(UDA_Item_Master* item_da, USkeletalMeshComponent* owner_mesh, UItem_Master* item);

	inline UAnimMontage* Get_Montage_Interactive() const { return Montage_Interactive; }
	inline const EAttack_Mode Get_Attack_Mode() const { return Attack_Mode; }

	void Notify_Attack(const bool is_start);

	bool Remove_And_Check_Same_Item(UItem_Master* item);

	TDelegate<void(bool)> On_Attacking;
	TDelegate<void(const float, AActor*)> On_Apply_Damage;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Set_Bounds(const FBoxSphereBounds& bounds);
	void Attack_Collision_Tick();
	UFUNCTION() void On_Attack_Montage_Ended(UAnimMontage* montage, bool is_interrupted);
	UFUNCTION(Server, Reliable) void S_Apply_Damage(const float damage, AActor* damage_causer);

	uint8 Is_Attacking : 1;
	uint8 Can_Damaged : 1;
	EAttack_Mode Attack_Mode;

	UPROPERTY() TObjectPtr<UItem_Master> Current_Item;
	UPROPERTY(Replicated) TObjectPtr<UAnimMontage> Montage_Interactive;
	UPROPERTY(Replicated) TObjectPtr<UPrimitiveComponent> Current_Hand_Comp;
	UPROPERTY(Replicated) FBoxSphereBounds Current_Bounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Comp_Master") bool Show_Debug_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item_Comp_Master", Replicated) TObjectPtr<UDA_Item_Master> Item_DA;

protected:
	uint8 Skip_Attack_Frame : 1;
	FAttack_Sphere_Info Attack_Sphere_Info;
	UPROPERTY(Replicated) TArray<AActor*> Hit_Actors;
};
//------------------------------------------------------------------------------------------------------------