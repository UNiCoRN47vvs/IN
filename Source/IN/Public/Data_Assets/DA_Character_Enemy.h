#pragma once

#include "Data_Assets/DA_Character_Master.h"
#include "DA_Character_Enemy.generated.h"
//------------------------------------------------------------------------------------------------------------
class UDA_Interact_Widget_Class;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class IN_API UDA_Character_Enemy : public UDA_Character_Master
{
	GENERATED_BODY()
public:
	const TArray<FName>& Get_Team_Type_Friend_Names();
	const TArray<FName>& Get_Team_Type_Enemies_Names();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Enemy") TObjectPtr<UDA_Interact_Widget_Class> DA_Interact_Widget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Enemy") TArray<ETeam_Type> Team_Type_Friend;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character_Enemy") TArray<ETeam_Type> Team_Type_Enemy;
protected:
	TArray<FName> Team_Type_Friend_Names;
	TArray<FName> Team_Type_Enemies_Names;
};
//------------------------------------------------------------------------------------------------------------