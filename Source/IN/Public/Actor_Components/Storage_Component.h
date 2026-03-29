#pragma once

#include "IN/IN.h"
#include "Components/ActorComponent.h"
#include "Structs/Item.h"
#include "Widgets/DDO/Drag_Drop_Operation.h"
#include "Storage_Component.generated.h"
//------------------------------------------------------------------------------------------------------------
struct FDrop_Item;
enum class EInteract_Item_Type : uint8;
//------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IN_API UStorage_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStorage_Component();

	TMulticastDelegate<void(int32, UDA_Item_Master*)>* Get_Item_Slot_Delegate(int32 index);
	inline const TArray<FItem>& Get_Storage() const { return Storage; }
	inline bool Is_Component_Init() const { return Is_Init; };

	void Add_Items_To_Storage(const TArray<FDrop_Item>& items);

	void Use_Item(const int32 index, const int32 count = 1);
	void Quick_Drop_Item(const int32 index, UStorage_Component* item_owner_component);

	void Drop_Triggered(FDrag_Drop_Info drag_info, const int32 drop_index);
	void Merge_Triggered(FDrag_Drop_Info drag_info, const int32 drop_index);

	void Clear_Storage();
	UFUNCTION() void On_Rep_Update_Storage();
	UFUNCTION(Client, Unreliable) void C_Update_Slot(int32 index, int32 count, UDA_Item_Master* da_item);

	uint8 Is_Widget_Open : 1;
	TMulticastDelegate<void(const EInteract_Item_Type, const int32)> On_Item_Interacted;
	TMulticastDelegate<void(int32)> On_Slot_Drag_Triggered;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void Rep_Update_Storage();

	void Item_Interacted(const EInteract_Item_Type type, const int32 index);
	void Remove_Item(const int32 index);
	void Item_Drop(const int32 index);

	void Update_Slot(const int32 index);
	void Update_Storage(int32 start_index = 0);

	uint8 Is_Init : 1;
	UFUNCTION(Server, Reliable) void S_Item_Interacted(const EInteract_Item_Type type, const int32 index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage_Component") int32 Storage_Size;

	UPROPERTY(Replicated, ReplicatedUsing = "On_Rep_Update_Storage") TArray<FItem> Storage;
};
//------------------------------------------------------------------------------------------------------------