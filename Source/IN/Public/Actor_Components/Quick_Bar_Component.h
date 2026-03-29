#pragma once

#include "Actor_Components/Storage_Component.h"
#include "Quick_Bar_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Interactive;
class UDA_Item_Master;
class UItem_Interactive;
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable)
class IN_API UQuick_Bar_Component : public UStorage_Component
{
	GENERATED_BODY()
	
public:
	UQuick_Bar_Component();
	void Quick_Slot_Item_Use(int32 number);

	TMulticastDelegate<void(int32)> On_Use_Item;
	TMulticastDelegate<void(UDA_Item_Master*, UItem_Interactive*, int32)> On_Hand_Item_Changed;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Rep_Update_Storage() override;

	void Hand_Item_Changed(UDA_Item_Master* item_master, UItem_Interactive* item_interactive, int32 index);
	void Slot_Drag_Triggered(int32 index);
	UFUNCTION(Client, Unreliable) void C_Use_Item_Update(int32 index);

	int32 Current_Hand_Item_Index;
	UPROPERTY() TObjectPtr<UDA_Item_Master> Current_Hand_Item_DA;
	UPROPERTY() TObjectPtr<UItem_Interactive> Current_Hand_Item;
};
//------------------------------------------------------------------------------------------------------------