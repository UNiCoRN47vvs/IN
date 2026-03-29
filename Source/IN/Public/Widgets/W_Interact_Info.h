#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Interact_Info.generated.h"
//------------------------------------------------------------------------------------------------------------
class UTextBlock;
struct FInteract_Info;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Interact_Info : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init_Interact_Widget(TMulticastDelegate<void(FInteract_Info*)>* delegate);
protected:
	virtual void NativeDestruct() override;

	void Execute_Interact(FInteract_Info* info);

	TMulticastDelegate<void(FInteract_Info*)>* On_Interact;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Interact_Info", meta = (BindWidget)) TObjectPtr<UTextBlock> Name_Action;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Interact_Info", meta = (BindWidget)) TObjectPtr<UTextBlock> Name_Owner;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "W_Interact_Info", meta = (BindWidget)) TObjectPtr<UTextBlock> Name_Button;
};
//------------------------------------------------------------------------------------------------------------