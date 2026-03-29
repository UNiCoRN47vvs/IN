#pragma once

#include "Widgets/W_Storage.h"
#include "W_Inventory.generated.h"
//------------------------------------------------------------------------------------------------------------
class UButton;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Inventory : public UW_Storage
{
	GENERATED_BODY()
public:
	virtual void Init_Storage_Widget(UStorage_Component* component, UStorage_Component* target_drop_component) override;
protected:

	UFUNCTION() void Toggle_Equipment_Widget();
	UFUNCTION(BlueprintCallable) void Init_Equipment_W_Ptr(UW_Equipment* equipment);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Inventory", meta = (BindWidget)) TObjectPtr<UButton> Button_Equipment;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Inventory") TObjectPtr<UW_Equipment> W_Equipment;


};
//------------------------------------------------------------------------------------------------------------