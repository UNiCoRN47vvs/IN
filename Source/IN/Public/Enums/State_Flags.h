#pragma once

#include "CoreMinimal.h"
//------------------------------------------------------------------------------------------------------------
UENUM(meta = (Bitflags))
enum class EState_Flags : uint32
{
	// Remember Update All Flags In Character_Master All_State_Flags!!!!
	None = 0,

	Walking = 1 << 0,
	Sprinting = 1 << 1,
	Inventory_Open = 1 << 2,

	Crouched = 1 << 3,

	Melee = 1 << 4,
	Ranged = 1 << 5,

	Attacking = 1 << 6,

	Dead = 1 << 7,

	Last = 1 << 8,
};
//------------------------------------------------------------------------------------------------------------
ENUM_CLASS_FLAGS(EState_Flags)