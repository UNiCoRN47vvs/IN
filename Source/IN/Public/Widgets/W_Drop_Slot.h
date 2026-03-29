#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Drop_Slot.generated.h"
//------------------------------------------------------------------------------------------------------------
class UImage;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Drop_Slot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init_Drop_Slot_Widget(FSlateBrush& brush);
	UPROPERTY(BlueprintReadOnly, Category = "Cell Drop", meta = (BindWidget)) TObjectPtr<UImage> Image;
};
//------------------------------------------------------------------------------------------------------------