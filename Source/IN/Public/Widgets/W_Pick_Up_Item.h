#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Pick_Up_Item.generated.h"
//------------------------------------------------------------------------------------------------------------
class UOverlay;
class UDA_Item_Master;
class UW_Pick_Up_Item_Box;
//------------------------------------------------------------------------------------------------------------
USTRUCT()
struct FPop_Up_Info
{
	GENERATED_BODY()

	UPROPERTY() UDA_Item_Master* DA_Item = nullptr;
	UPROPERTY() int32 Item_Count = 0;
};
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Pick_Up_Item : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Pick_Up_Item", meta = (BindWidget)) TObjectPtr<UOverlay> Overlay;
protected:
	virtual void NativeConstruct() override;

	void Init_Self();
	void On_Pick_Up_Item(UDA_Item_Master* da_item, const int32 item_count);
	void Create_Pop_Up();

	FTimerHandle Pop_Up_Handle;
	UPROPERTY() TArray<FPop_Up_Info> Pop_Up_Array;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Pick_Up_Item") TSubclassOf<UW_Pick_Up_Item_Box> W_Pick_Up_Item_Box_Class;
};
//------------------------------------------------------------------------------------------------------------