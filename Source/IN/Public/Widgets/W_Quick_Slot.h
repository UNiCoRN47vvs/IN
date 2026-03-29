#pragma once

#include "Widgets/W_Item_Slot.h"
#include "W_Quick_Slot.generated.h"
//------------------------------------------------------------------------------------------------------------
class UTextBlock;
class UQuick_Bar_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Quick_Slot : public UW_Item_Slot
{
	GENERATED_BODY()
	
public:
	virtual void Init_Slot(UStorage_Component* storage_component, int32 storage_index) override;
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void Use_Anim_Play(int32 index);

	TMulticastDelegate<void(int32)>* On_Use_Item;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Quick_Slot") int32 Quick_Slot_Index;
	UPROPERTY(BlueprintReadOnly, Category = "W_Quick_Slot", meta = (BindWidget)) TObjectPtr<UTextBlock> Item_Button_Number;
	UPROPERTY(BlueprintReadOnly, Category = "W_Quick_Slot", meta = (BindWidgetAnim), Transient) TObjectPtr<UWidgetAnimation> Use_Animation;
};
//------------------------------------------------------------------------------------------------------------