#pragma once

#include "IN/IN.h"
#include "Components/ActorComponent.h"
#include "Stat_Master_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Character_Master;
enum class EStat_Name : uint8;
//------------------------------------------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Health_Change, float, current_value, float, max_value);
//------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IN_API UStat_Master_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStat_Master_Component();
	virtual void Apply_Damage(float damage_value);

	virtual void Init_Stat_Component(UDA_Character_Master* da_character);

	TMulticastDelegate<void()> On_Death;

	UFUNCTION(BlueprintCallable) virtual void Update_All_Bars();

	UPROPERTY(BlueprintAssignable) FOn_Health_Change On_Health_Change;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Apply_Health_Timer();

	UFUNCTION() void On_Rep_Health();

	float Period_Heath;
	FTimerHandle TH_Health;
	TMap<EStat_Name, float*> Stat_Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Master", Replicated, ReplicatedUsing = "On_Rep_Health") float Health_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Master") float Health_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Master") float Health_Regen;
		
};
//------------------------------------------------------------------------------------------------------------