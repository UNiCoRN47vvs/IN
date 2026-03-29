#include "Widgets/W_Drop_Slot.h"
#include "Components/Image.h"
//------------------------------------------------------------------------------------------------------------
void UW_Drop_Slot::Init_Drop_Slot_Widget(FSlateBrush& brush)
{
	PTR(Image)->SetBrush(brush);
}
//------------------------------------------------------------------------------------------------------------