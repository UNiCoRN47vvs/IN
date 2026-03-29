#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Pick_Up_Item_Box.generated.h"
//------------------------------------------------------------------------------------------------------------
class UTextBlock;
class UImage;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Pick_Up_Item_Box : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Init_W_Pick_Up_Item_Box(const FText& name, const FText& count, const TSoftObjectPtr<UTexture2D>& soft_texture);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Pick_Up_Item_Box", meta = (BindWidget)) TObjectPtr<UTextBlock> TB_Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Pick_Up_Item_Box", meta = (BindWidget)) TObjectPtr<UTextBlock> TB_Count;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Pick_Up_Item_Box", meta = (BindWidget)) TObjectPtr<UImage> I_Icon;
	UPROPERTY(BlueprintReadOnly, Category = "W_Pick_Up_Item_Box", meta = (BindWidgetAnim), Transient) TObjectPtr<UWidgetAnimation> Init_Anim;
};
//------------------------------------------------------------------------------------------------------------