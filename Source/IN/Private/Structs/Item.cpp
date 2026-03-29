#include "Structs/Item.h"
//------------------------------------------------------------------------------------------------------------
void FItem::Item_Add(UItem_Master* item)
{
	CHECK_PTR(item);

	Item = item;
	Item->Update_Slot();
}
//------------------------------------------------------------------------------------------------------------
void FItem::Item_Combine(UDA_Item_Master* item_da, int32& item_count)
{
	if (!Item)
		return;

	bool is_combine_success = false;
	is_combine_success = Item->Item_Combine(item_da, item_count);
}
//------------------------------------------------------------------------------------------------------------
void FItem::Item_Destroy()
{
	/*if (Item)
		Item->Update_Slot_Destroy();*/

	Item = nullptr;
}
//------------------------------------------------------------------------------------------------------------
