#include "SBrick.h"

void SBrick::BuildBrick(int BrickIndex)
{
	Index = BrickIndex;
	SetEnabled(true);
	const TAttribute<FText> DisplayText = TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([=]() -> FText
		{
			if (isHided && isFlagged)
			{
				return FText::FromString("?");
			}
			else
			{
				if (MineCount > 0)
				{
					return FText::AsNumber(MineCount);
				}
				else if (MineCount == -1)
				{
					return FText::FromString("*");
				}
			}

			return FText::FromString("");
		}));

	ChildSlot
		[
			SNew(STextBlock).Justification(ETextJustify::Center).Text(DisplayText)
		];

	OnPressed.BindLambda([&]()
		{
			BrickUnHided.Broadcast(this);
		});
}

int SBrick::GetBrickIndex() const
{
	return Index;
}

void SBrick::UnHide(const int NewMineCount)
{
	MineCount = NewMineCount;
	isHided = false;
	SetEnabled(false);
}

FReply SBrick::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FlagBrick();
		return FReply::Handled();
	}

	return SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
}

void SBrick::FlagBrick()
{
	if (isHided)
	{
		isFlagged = !isFlagged;
	}
}