#include "Data_Assets/DA_Character_Enemy.h"
//------------------------------------------------------------------------------------------------------------
const TArray<FName>& UDA_Character_Enemy::Get_Team_Type_Friend_Names()
{
   if (Team_Type_Friend_Names.IsEmpty())
   {
		const UEnum* enum_ptr = StaticEnum<ETeam_Type>();
		if (!enum_ptr)
		{
			ensureMsgf(enum_ptr, TEXT("Enum_PTR = nullptr!!!"));
			return Team_Type_Friend_Names;
		}

      for (const ETeam_Type type : Team_Type_Friend)
      {
         Team_Type_Friend_Names.Add(enum_ptr->GetNameByValue(static_cast<int64>(type)));
      }
   }

   return Team_Type_Friend_Names;
}
//------------------------------------------------------------------------------------------------------------
const TArray<FName>& UDA_Character_Enemy::Get_Team_Type_Enemies_Names()
{
   if (Team_Type_Enemies_Names.IsEmpty())
   {
      const UEnum* enum_ptr = StaticEnum<ETeam_Type>();
      if (!enum_ptr)
      {
         ensureMsgf(enum_ptr, TEXT("Enum_PTR = nullptr!!!"));
         return Team_Type_Friend_Names;
      }

      for (const ETeam_Type type : Team_Type_Enemy)
      {
         Team_Type_Enemies_Names.Add(enum_ptr->GetNameByValue(static_cast<int64>(type)));
      }
   }

   return Team_Type_Enemies_Names;
}
//------------------------------------------------------------------------------------------------------------