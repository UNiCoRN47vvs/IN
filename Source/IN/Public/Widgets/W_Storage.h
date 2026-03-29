#pragma once

#include "IN/IN.h"
#include "Blueprint/UserWidget.h"
#include "W_Storage.generated.h"
//------------------------------------------------------------------------------------------------------------
class UUniformGridPanel;
class UButton;
class UStorage_Component;
class UW_Inventory_Slot;
class UW_Item_Info_Menu;
class UW_Equipment;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UW_Storage : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void Init_Storage_Widget(UStorage_Component* component, UStorage_Component* target_drop_component);
	void Close_Widget();
protected:
	virtual void NativeDestruct() override;
	void Item_Info_Menu_Create(const int32 index);
	void On_Quick_Drop(const int32 index);
	UPROPERTY() TObjectPtr<UStorage_Component> Storage_Component;
	UPROPERTY() TObjectPtr<UW_Item_Info_Menu> W_Item_Info_Menu;
	UPROPERTY() TObjectPtr<UStorage_Component> Target_Drop_Component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Storage", meta = (BindWidget)) TObjectPtr<UUniformGridPanel> Uniform_Grid_Panel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Storage") TSubclassOf<UW_Inventory_Slot> W_Inventory_Slot_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Storage") int32 Row_Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "W_Storage") TSubclassOf<UW_Item_Info_Menu> W_Item_Info_Menu_Class;

};
//------------------------------------------------------------------------------------------------------------