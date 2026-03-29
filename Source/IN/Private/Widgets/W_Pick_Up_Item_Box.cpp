#include "Widgets/W_Pick_Up_Item_Box.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
//------------------------------------------------------------------------------------------------------------
void UW_Pick_Up_Item_Box::Init_W_Pick_Up_Item_Box(const FText& name, const FText& count, const TSoftObjectPtr<UTexture2D>& soft_texture)
{
	PTR(TB_Name)->SetText(name);
	PTR(TB_Count)->SetText(count);
	PTR(I_Icon)->SetBrushFromSoftTexture(soft_texture);

	FWidgetAnimationDynamicEvent event = {};
	event.BindUFunction(this, FName("RemoveFromParent"));

	BindToAnimationFinished(Init_Anim, event);

	PlayAnimation(Init_Anim).GetAnimationState();
}
//------------------------------------------------------------------------------------------------------------
