#include "Actors/Items/Item_Actor_Master.h"
#include "Net/UnrealNetwork.h"
#include "Actor_Components/Inventory_Component.h"
//------------------------------------------------------------------------------------------------------------
AItem_Actor_Master::AItem_Actor_Master()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}
//------------------------------------------------------------------------------------------------------------
void AItem_Actor_Master::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
void AItem_Actor_Master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem_Actor_Master, Item_Count);
	DOREPLIFETIME(AItem_Actor_Master, Item_DA);
}
//------------------------------------------------------------------------------------------------------------
void AItem_Actor_Master::Init_Item_Implementation(UDA_Item_Master* item_da, int32 item_count)
{
	Item_DA = item_da;
	Item_Count = item_count;

	//TODO :: Возможно запустить инициализацию на клиенте через мультикаст
}
//------------------------------------------------------------------------------------------------------------
FInteract_Info& AItem_Actor_Master::Get_Interact_Info()
{
	return Interact_Info;
}
//------------------------------------------------------------------------------------------------------------
void AItem_Actor_Master::Interact(AActor* actor)
{
	CHECK_PTR(actor);

	UInventory_Component* inventory_component = actor->FindComponentByClass<UInventory_Component>();

	CHECK_PTR(inventory_component);

	inventory_component->Pick_Up_Item(this);
}
//------------------------------------------------------------------------------------------------------------
