#pragma once

#include "Actor_Components/Stat_Master_Component.h"
#include "Enums/Stat.h"
#include "Stat_Player_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Stamina_Change, float, current_value, float, max_value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Hunger_Change, float, current_value, float, max_value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Thirst_Change, float, current_value, float, max_value);
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UStat_Player_Component : public UStat_Master_Component
{
	GENERATED_BODY()
public:
	UStat_Player_Component();

	virtual void Init_Stat_Component(UDA_Character_Master* da_character) override;
	virtual void Update_All_Bars() override;
	virtual void Apply_Damage(float damage_value) override;

	void Add_Stat_Value(const EStat_Name stat_name, const float value);
	bool Sprint();

	inline bool Is_Component_Init() const { return Is_Init; };

	uint8 Is_Sprinting : 1;
	UPROPERTY(BlueprintAssignable) FOn_Stamina_Change On_Stamina_Change;
	UPROPERTY(BlueprintAssignable) FOn_Hunger_Change On_Hunger_Change;
	UPROPERTY(BlueprintAssignable) FOn_Thirst_Change On_Thirst_Change;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION() void On_Rep_Stamina();
	UFUNCTION() void On_Rep_Hunger();
	UFUNCTION() void On_Rep_Thirst();

	uint8 Is_Init : 1;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player", Replicated, ReplicatedUsing = "On_Rep_Stamina") float Stamina_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player") float Stamina_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player") float Stamina_Regen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player", Replicated, ReplicatedUsing = "On_Rep_Hunger") float Hunger_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player") float Hunger_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player") float Hunger_Waste;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player", Replicated, ReplicatedUsing = "On_Rep_Thirst") float Thirst_Current;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player") float Thirst_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat_Player") float Thirst_Waste;

private:
	void Apply_Stamina_Timer();
	void Apply_Hunger_Timer();
	void Apply_Thirst_Timer();
	float Period_Stamina;
	float Period_Hunger;
	float Period_Thirst;
	FTimerHandle TH_Stamina;
	FTimerHandle TH_Hunger;
	FTimerHandle TH_Thirst;
};
//------------------------------------------------------------------------------------------------------------