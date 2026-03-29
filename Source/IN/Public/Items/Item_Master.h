#pragma once

#include "IN/IN.h"
#include "UObject/Object.h"
#include "Interfaces/Item_Representation.h"
#include "Item_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Item_Master;
//------------------------------------------------------------------------------------------------------------
UCLASS(Blueprintable, BlueprintType)
class IN_API UItem_Master : public UObject, public IItem_Representation
{
	GENERATED_BODY()

public:
#pragma region Interface
   inline virtual UDA_Item_Master* Get_Item_DA() const override { return Item_DA; }
#pragma endregion

   virtual bool IsSupportedForNetworking() const override { return true; }
   virtual UWorld* GetWorld() const override;
   virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

   virtual void Init_Item(const int32 item_count, UDA_Item_Master* item_da, const int32 item_index);
   virtual void Item_Use();

   bool Item_Combine(UDA_Item_Master* item_da, int32& item_count);
   void Update_Slot();
   void Update_Slot_Destroy();

   UPROPERTY(Replicated) int32 Item_Index;
   UPROPERTY(Replicated, ReplicatedUsing = "On_Rep_Item_Count") int32 Item_Count;
protected:
   UFUNCTION() void On_Rep_Item_Count();
   UPROPERTY(Replicated) TObjectPtr<UDA_Item_Master> Item_DA;
};
//------------------------------------------------------------------------------------------------------------