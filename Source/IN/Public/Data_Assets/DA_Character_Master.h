#pragma once

#include "IN/IN.h"
#include "Engine/DataAsset.h"
#include "Enums/Team_Type.h"
#include "Enums/Stat.h"
#include "DA_Character_Master.generated.h"
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Character_Master : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") USkeletalMesh* Skeletal_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") UAnimMontage* Montage_Punch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") float Damage_Punch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") FName Hand_Socket_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master") ETeam_Type Team_Type_Self;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master | Stat") TMap<EStat_Name, float> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Master | Death") TArray<TSoftObjectPtr<UAnimMontage>> Death_Montages;
};
//------------------------------------------------------------------------------------------------------------