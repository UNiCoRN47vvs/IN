#include "Actor_Components/Storage_Component.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Items/Item_Master.h"
#include "Actors/Items/Item_Actor_Master.h"
#include "Structs/Drop_Item.h"
//------------------------------------------------------------------------------------------------------------
UStorage_Component::UStorage_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
	Is_Init = false;
	Is_Widget_Open = false;

	SetIsReplicatedByDefault(true);
}
//------------------------------------------------------------------------------------------------------------
TMulticastDelegate<void(int32, UDA_Item_Master*)>* UStorage_Component::Get_Item_Slot_Delegate(int32 index)
{
	if (!Storage.IsValidIndex(index))
		return nullptr;

	return &Storage[index].On_Update_Slot;
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::BeginPlay()
{
	Super::BeginPlay();

	Storage.SetNum(Storage_Size);

	On_Item_Interacted.AddUObject(this, &UStorage_Component::Item_Interacted);
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStorage_Component, Storage);
}
//------------------------------------------------------------------------------------------------------------
bool UStorage_Component::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool wrote_something = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FItem& slot : Storage)
	{
		if (slot.Item && slot.Item->IsSupportedForNetworking())
			wrote_something |= Channel->ReplicateSubobject(slot.Item, *Bunch, *RepFlags); // Если объект есть, значит этот UObject нужно синхронизировать
	}

	return wrote_something;
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::On_Rep_Update_Storage()
{
	Rep_Update_Storage();
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Rep_Update_Storage()
{
	if (Is_Widget_Open)
		Update_Storage();
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Update_Slot(const int32 index)
{
	if (!Storage.IsValidIndex(index))
		return;
	if (Storage[index].Item)
		C_Update_Slot(index, Storage[index].Item->Item_Count, Storage[index].Item->Get_Item_DA());
	else
		C_Update_Slot(index, 0, nullptr);
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::C_Update_Slot_Implementation(int32 index, int32 count, UDA_Item_Master* da_item)
{
	if (!Storage.IsValidIndex(index))
		return;

	if (Storage[index].Item)
		Storage[index].On_Update_Slot.Broadcast(count, da_item);
	else
		Storage[index].On_Update_Slot.Broadcast(0, nullptr);
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Update_Storage(int32 start_index)
{
	int32 max_index = start_index + 25;
	TWeakObjectPtr<UStorage_Component> weak_this = this;
	for (int32 i = start_index; i < max_index; i++)
	{
		//Метод Update_Slot() сломает логику!

		if (!Storage.IsValidIndex(i))
			return;

		if (Storage[i].Item)
			Storage[i].On_Update_Slot.Broadcast(Storage[i].Item->Item_Count, Storage[i].Item->Get_Item_DA());
		else
			Storage[i].On_Update_Slot.Broadcast(0, nullptr);
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick([weak_this, max_index]()
		{
			if (weak_this.IsValid())
				weak_this->Update_Storage(max_index);
		});
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Add_Items_To_Storage(const TArray<FDrop_Item>& items)
{
	UItem_Master* item_master = nullptr;
	int32 item_count = 0;
	TArray<int32> empty_slots_indexes = {};

	for (const FDrop_Item& item : items)
	{
		CHECK_ALARM(item.Drop_DA_Item->Item_Object_Class, TEXT("Class is empty!"));

		item_count = item.Drop_Count;

		empty_slots_indexes.Empty();

		for (int32 i = 0; i < Storage.Num(); i++)
		{
			if (Storage[i].Item)
			{
				Storage[i].Item_Combine(item.Drop_DA_Item, item_count);
				if (item_count <= 0)
					break;

				continue;
			}

			empty_slots_indexes.Add(i);
		}

		if (item_count <= 0)
			continue;

		for(const int32 item_index : empty_slots_indexes)
		{
			item_master = NewObject<UItem_Master>(this, item.Drop_DA_Item->Item_Object_Class);
			if (!item_master)
			{
				ALARM_LOG;
				continue;
			}

			item_master->Init_Item(0, item.Drop_DA_Item, item_index);

			Storage[item_index].Item = item_master;
			Storage[item_index].Item_Combine(item.Drop_DA_Item, item_count);

			if (item_count > 0)
				continue;

			if (GetNetMode() != ENetMode::NM_DedicatedServer)
				Update_Slot(item_index);

			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Use_Item(const int32 index, const int32 count)
{
	//Storage[index].Combine_Item();
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Quick_Drop_Item(const int32 index, UStorage_Component* item_owner_component)
{
	CHECK_ALARM(item_owner_component, TEXT("Item Owner Component is nullptr"));

	UItem_Master* item_master = nullptr;
	UItem_Master* item = nullptr;
	UDA_Item_Master* da_item = nullptr;
	TArray<int32> empty_slots_indexes = {};
	const TArray<FItem>& item_owner_storage = item_owner_component->Get_Storage();

	if (!item_owner_storage.IsValidIndex(index))
		return;

	item = item_owner_storage[index].Item;
	CHECK_PTR(item);

	da_item = item->Get_Item_DA();
	CHECK_PTR(da_item);

	for (int32 i = 0; i < Storage.Num(); i++)
	{
		if (!Storage[i].Item)
		{
			empty_slots_indexes.Add(i);
			continue;
		}

		Storage[i].Item_Combine(da_item, item->Item_Count);
		if (item->Item_Count <= 0)
			break;
	}

	if (item->Item_Count <= 0)
	{
		item_owner_component->Remove_Item(index);
		if (GetNetMode() != ENetMode::NM_DedicatedServer)
			item_owner_component->Update_Slot(index);
		return;
	}

	for (int32 item_index : empty_slots_indexes)
	{
		CHECK_ALARM(da_item->Item_Object_Class, TEXT("Class is empty!"));

		item_master = NewObject<UItem_Master>(this, da_item->Item_Object_Class);
		if (!item_master)
		{
			ALARM_LOG;
			continue;
		}

		item_master->Init_Item(0, da_item, item_index);

		Storage[item_index].Item = item_master;
		Storage[item_index].Item_Combine(da_item, item->Item_Count);

		if (item->Item_Count <= 0)
		{
			item_owner_component->Remove_Item(index);
			if (GetNetMode() != ENetMode::NM_DedicatedServer)
				item_owner_component->Update_Slot(index);
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Merge_Triggered(FDrag_Drop_Info drag_info, const int32 drop_index)
{
	if (!Storage.IsValidIndex(drop_index) || !drag_info.Storage_Component
		|| !drag_info.Storage_Component->Get_Storage().IsValidIndex(drag_info.Index) || !drag_info.Storage_Component->Get_Storage()[drag_info.Index].Item)
	{
		ALARM_LOG;
		return;
	}

	UItem_Master* drag_item = drag_info.Storage_Component->Get_Storage()[drag_info.Index].Item;

	Storage[drop_index].Item_Combine(drag_item->Get_Item_DA(), drag_item->Item_Count);
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Drop_Triggered(FDrag_Drop_Info drag_info, const int32 drop_index)
{
	if (!Storage.IsValidIndex(drop_index) || !drag_info.Storage_Component
		|| !drag_info.Storage_Component->Get_Storage().IsValidIndex(drag_info.Index))
	{
		ALARM_LOG;
		return;
	}

	UItem_Master* drag_item = drag_info.Storage_Component->Get_Storage()[drag_info.Index].Item;
	if (!drag_item || !drag_item->Get_Item_DA())
	{
		ALARM_LOG;
		return;
	}

	UItem_Master* item = nullptr;
	FItem& drop_slot = Storage[drop_index];

	if (drag_info.Storage_Component == this)
	{
		drop_slot.Item_Add(drag_item);
		drop_slot.Item->Item_Index = drop_index;
	}
	else
	{
		item = NewObject<UItem_Master>(this, drag_item->Get_Item_DA()->Item_Object_Class);
		CHECK_PTR(item);

		item->Init_Item(drag_item->Item_Count, drag_item->Get_Item_DA(), drop_index);

		drop_slot.Item_Add(item);
	}

	drag_info.Storage_Component->Remove_Item(drag_info.Index);

	drag_info.Storage_Component->On_Slot_Drag_Triggered.Broadcast(drag_info.Index);

	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		Update_Slot(drop_index);
		drag_info.Storage_Component->Update_Slot(drag_info.Index);
	}
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Item_Interacted(const EInteract_Item_Type type, const int32 index)
{
	S_Item_Interacted(type, index);
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Remove_Item(const int32 index)
{
	if (!Storage.IsValidIndex(index))
		return;

	Storage[index].Item_Destroy();
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Clear_Storage()
{
	for (FItem& slot : Storage)
	{
		slot.Item_Destroy();
	}
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::S_Item_Interacted_Implementation(const EInteract_Item_Type type, const int32 index)
{
	if (!Storage.IsValidIndex(index))
		return;

	switch (type)
	{
	case EInteract_Item_Type::Use:
		CHECK_PTR(Storage[index].Item);
		Storage[index].Item->Item_Use();

		if (Storage[index].Item->Item_Count <= 0)
			Storage[index].Item_Destroy();

		break;

	case EInteract_Item_Type::Drop:
		Item_Drop(index);
		break;

	case EInteract_Item_Type::Destroy:
		Storage[index].Item_Destroy();
		break;
	}

	//if (GetNetMode() != NM_DedicatedServer)
		//C_Update_Slot(index);
}
//------------------------------------------------------------------------------------------------------------
void UStorage_Component::Item_Drop(const int32 index)
{
	if (!Storage.IsValidIndex(index))
		return;

	CHECK_PTR(Storage[index].Item);
	CHECK_PTR(Storage[index].Item->Get_Item_DA());

	bool is_hit = false;
	UWorld* world = GetWorld();
	AActor* owner = GetOwner();
	AItem_Actor_Master* item_actor = nullptr;
	const float spawn_distance = 120.0f;
	FHitResult hit_result = {};
	FCollisionQueryParams params = {};
	const FVector box_size_fb = { 70.0f, 30.0, 30.0f };
	const FVector box_size_rl = { box_size_fb.Y, box_size_fb.X, box_size_fb.Z };
	//const FQuat box_rotator = owner->GetActorRotation().Quaternion();
	const FVector owner_location = owner->GetActorLocation();
	FCollisionShape box = FCollisionShape::MakeBox(box_size_fb);
	FVector box_location = owner_location + (owner->GetActorForwardVector() * spawn_distance);
	CHECK_PTR(world);

	is_hit = world->SweepSingleByChannel(hit_result, box_location, box_location, FQuat::Identity, ECC_Visibility, box, params); //Поиск достаточно ли места для спавна
	//DrawDebugBox(world, box_location, box_size_fb, box_rotator, FColor::Red, false, 0.5f); //DEBUG

	if (is_hit)
	{
		box_location = owner_location - (owner->GetActorForwardVector() * spawn_distance); //Если спереди нет места, то пытаемся сзади

		is_hit = world->SweepSingleByChannel(hit_result, box_location, box_location, FQuat::Identity, ECC_Visibility, box, params); //Поиск достаточно ли места для спавна
		//DrawDebugBox(world, box_location, box_size_fb, box_rotator, FColor::Red, false, 0.5f); //DEBUG

		if (is_hit)
		{
			box = FCollisionShape::MakeBox(box_size_rl);

			box_location = owner_location + owner->GetActorRightVector() * spawn_distance; //Если сзади нет места, то пытаемся справа

			is_hit = world->SweepSingleByChannel(hit_result, box_location, box_location, FQuat::Identity, ECC_Visibility, box, params); //Поиск достаточно ли места для спавна
			//DrawDebugBox(world, box_location, box_size_rl, box_rotator, FColor::Red, false, 0.5f); //DEBUG

			if (is_hit)
			{
				box_location = owner_location - owner->GetActorRightVector() * spawn_distance; //Если справа нет места, то пытаемся слева

				is_hit = world->SweepSingleByChannel(hit_result, box_location, box_location, FQuat::Identity, ECC_Visibility, box, params); //Поиск достаточно ли места для спавна
				//DrawDebugBox(world, box_location, box_size_rl, box_rotator, FColor::Red, false, 0.5f); //DEBUG

				if (is_hit)
				{
					//TODO :: добавить сообщение игроку о том, что нет места для спавна предмета
					return; //Если нет места для спавна, то выходим
				}
			}
		}
	}

	item_actor = world->SpawnActor<AItem_Actor_Master>(Storage[index].Item->Get_Item_DA()->Item_Actor_Class, box_location, FRotator::ZeroRotator);
	CHECK_PTR(item_actor);

	item_actor->Init_Item(Storage[index].Item->Get_Item_DA(), Storage[index].Item->Item_Count);

	Storage[index].Item_Destroy();
}
//------------------------------------------------------------------------------------------------------------