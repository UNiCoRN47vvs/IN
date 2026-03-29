#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Item_Info_Button.generated.h"
//------------------------------------------------------------------------------------------------------------
class UTextBlock;
class UButton;
class W_Item_Info_Menu;
enum class EInteract_Item_Type : uint8;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Item_Info_Button : public UUserWidget
{
	GENERATED_BODY()
public:
	void Initialize_Item_Info_Button(const EInteract_Item_Type item_type, const int32 index, TMulticastDelegate<void(const EInteract_Item_Type, const int32)>* delegate, const FText& text);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Item_Info_Button", meta = (BindWidget)) TObjectPtr<UTextBlock> Button_Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Item_Info_Button", meta = (BindWidget)) TObjectPtr<UButton> Button;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION() void Button_Clicked();

	EInteract_Item_Type Item_Type;
	int32 Index;
	TMulticastDelegate<void(const EInteract_Item_Type, const int32)>* On_Item_Interacted;

	
};
//------------------------------------------------------------------------------------------------------------