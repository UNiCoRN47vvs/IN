#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Container.generated.h"
//------------------------------------------------------------------------------------------------------------
class UW_Storage;
class UStorage_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Container : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init_Container_Widget(UStorage_Component* component_player, UStorage_Component* component_interact);
	void Close_Container_Widget();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Container") TObjectPtr<UW_Storage> W_Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Container") TObjectPtr<UW_Storage> W_Interact;
protected:
	UFUNCTION(BlueprintCallable) void Init_W_Container(UW_Storage* widget_inventory, UW_Storage* widget_interact);
};
//------------------------------------------------------------------------------------------------------------