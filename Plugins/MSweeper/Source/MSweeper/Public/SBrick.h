#pragma once

#include "SlateBasics.h"


class SBrick : public SButton
{
	DECLARE_EVENT_OneParam(UManagerObject, FOnBrickUnHided, SBrick*);
	FOnBrickUnHided BrickUnHided;

public:
	void BuildBrick(int BrickIndex);

	int GetBrickIndex() const;

	void UnHide(int NewMineCount);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FOnBrickUnHided& OnBrickUnHide()
	{	
		return BrickUnHided;
	}

protected:
	void FlagBrick();

private:
	int Index;
	bool isHided = true;
	bool isFlagged = false;
	int MineCount = 0;
};
