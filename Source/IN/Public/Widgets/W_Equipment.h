#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Equipment.generated.h"
//------------------------------------------------------------------------------------------------------------
class UVerticalBox;
class UHorizontalBox;
class UButton;
class UW_Equipment_Slot;
class UStorage_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Equipment : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init_Equipment_Widget(UStorage_Component* component);
	void Toggle_Equipment_Widget();
protected:

	uint8 Is_Opened_VB_Main : 1;
	uint8 Is_Opened_HB_Fast_Slot : 1;
	UPROPERTY() TArray<UW_Equipment_Slot*> Equipment_Slots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Equipment", meta = (BindWidget)) TObjectPtr<UVerticalBox> VB_Main;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Equipment", meta = (BindWidgetAnim), Transient) TObjectPtr<UWidgetAnimation> Toggle_Animation_VB_Main;
};
//------------------------------------------------------------------------------------------------------------