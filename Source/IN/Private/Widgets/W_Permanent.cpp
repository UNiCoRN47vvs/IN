#include "Widgets/W_Permanent.h"
#include "Widgets/W_Interact_Info.h"
//------------------------------------------------------------------------------------------------------------
void UW_Permanent::Init_Interact_Widget(TMulticastDelegate<void(FInteract_Info*)>* delegate)
{
	CHECK_PTR(delegate);
	CHECK_PTR(Interact_Info_Widget);

	Interact_Info_Widget->Init_Interact_Widget(delegate);
}
//------------------------------------------------------------------------------------------------------------