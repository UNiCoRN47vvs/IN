#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Item_Info_Menu.generated.h"
//------------------------------------------------------------------------------------------------------------
class UVerticalBox;
class UW_Item_Info_Button;
enum class EInteract_Item_Type : uint8;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Item_Info_Menu : public UUserWidget
{
	GENERATED_BODY()
public:
	void Initialize_Item_Info_Menu(const TArray<EInteract_Item_Type>& types, const int32 index, TMulticastDelegate<void(const EInteract_Item_Type, const int32)>* delegate);
protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FTimerHandle Hide_Timer_Handle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Item_Info_Menu", meta = (BindWidget)) TObjectPtr<UVerticalBox> Vertical_Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Item_Info_Menu") TMap<EInteract_Item_Type, FText> Text_Values;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Item_Info_Menu") TSubclassOf<UW_Item_Info_Button> W_Item_Info_Button_Class;
};
//------------------------------------------------------------------------------------------------------------