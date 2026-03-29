#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Permanent.generated.h"
//------------------------------------------------------------------------------------------------------------
class UW_Interact_Info;
class UW_Quick_Slot_Bar;
struct FInteract_Info;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Permanent : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init_Interact_Widget(TMulticastDelegate<void(FInteract_Info*)>* delegate);
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Permanent", meta = (BindWidget)) TObjectPtr<UW_Interact_Info> Interact_Info_Widget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Permanent", meta = (BindWidget)) TObjectPtr<UW_Quick_Slot_Bar> Quick_Slot_Bar;
};
//------------------------------------------------------------------------------------------------------------