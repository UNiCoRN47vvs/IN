#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Quick_Slot_Bar.generated.h"
//------------------------------------------------------------------------------------------------------------
class UHorizontalBox;
class UQuick_Bar_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Quick_Slot_Bar : public UUserWidget
{
	GENERATED_BODY()
public:
	
protected:
	virtual void NativeConstruct() override;
	void Init();
	UPROPERTY() TObjectPtr<UQuick_Bar_Component> Quick_Bar_Component;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quick_Slot_Bar", meta = (BindWidget)) TObjectPtr<UHorizontalBox> HB_Bar;
};
//------------------------------------------------------------------------------------------------------------