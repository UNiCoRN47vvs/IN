#include "Actors/Loot_Container.h"
#include "Net/UnrealNetwork.h"
#include "Characters/Character_Master.h"
#include "Actor_Components/Storage_Component.h"
#include "Actor_Components/Drop_Component.h"
#include "Data_Assets/DA_Loot_Container.h"
//------------------------------------------------------------------------------------------------------------
ALoot_Container::ALoot_Container()
{
	Drop_Component = CreateDefaultSubobject<UDrop_Component>(TEXT("Drop_Component"));
	Storage_Component = CreateDefaultSubobject<UStorage_Component>(TEXT("Storage_Component"));
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}
//------------------------------------------------------------------------------------------------------------
FInteract_Info& ALoot_Container::Get_Interact_Info()
{
	return Interact_Info;
}
//------------------------------------------------------------------------------------------------------------
void ALoot_Container::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!Loot_Container_ID.IsValid())
			Loot_Container_ID = FGuid::NewGuid();
	}

	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		if (!Loot_Container_ID.IsValid())
			Loot_Container_ID = FGuid::NewGuid();
	}

	CHECK_ALARM(Drop_Component, TEXT("Drop_Component = nullptr!"));
	Drop_Component->On_Send_Drop.BindUObject(this, &ALoot_Container::On_Init_Drop);
}
//------------------------------------------------------------------------------------------------------------
void ALoot_Container::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Drop_Component)
		Drop_Component->On_Send_Drop.Unbind();
}
//------------------------------------------------------------------------------------------------------------
void ALoot_Container::On_Init_Drop(const TArray<FDrop_Item>& items)
{
	CHECK_ALARM(Storage_Component, TEXT("Storage_Component = nullptr!"));
	
	Storage_Component->Add_Items_To_Storage(items);
}
//------------------------------------------------------------------------------------------------------------
void ALoot_Container::Interact(AActor* actor)
{
	CHECK_ALARM(DA_Loot_Container, TEXT("DA_Loot_Container = nullptr!"));

	const FDateTime current_time = FDateTime::UtcNow();

	if (Last_Opened_Time != FDateTime::MinValue())
	{
		const double time_diff = (current_time - Last_Opened_Time).GetTotalSeconds();

		if (time_diff <= DA_Loot_Container->Drop_Time_Refresh)
		{
			Open_Container_W(actor);
			return;
		}
	}

	Last_Opened_Time = current_time;

	CHECK_ALARM(Drop_Component, TEXT("Drop_Component = nullptr!"));
	CHECK_ALARM(Storage_Component, TEXT("Storage_Component = nullptr!"));

	Storage_Component->Clear_Storage();

	Drop_Component->Generate_Drop(DA_Loot_Container, Storage_Component->Get_Storage().Num());

	Open_Container_W(actor);
}
//------------------------------------------------------------------------------------------------------------
void ALoot_Container::Open_Container_W(AActor* actor)
{
	CHECK_ALARM(DA_Loot_Container, TEXT("DA_Loot_Container = nullptr!"));
	CHECK_ALARM(Storage_Component, TEXT("Storage_Component = nullptr!"));

	ACharacter_Master* character_master = Cast<ACharacter_Master>(actor);
	CHECK_PTR(character_master);

	character_master->C_Toggle_Inventory(DA_Loot_Container->DA_Interact_Widget_Class, Storage_Component);
}
//------------------------------------------------------------------------------------------------------------
