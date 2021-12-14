#pragma once

#include "SlateBasics.h"

class SBrick;


class SMSweeper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMSweeper) {}
	SLATE_ARGUMENT(UGameManagerObject*, GameManagerObject)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);

	void OnBrickUnHide(SBrick* ClickedTile);

	void UnHideBrick(const int TileIndex);

	void LocateMines(const int ExcludedTileIndex);

	bool isFirstBrickUnHided = true;

	void GetWidth(const FText& Text);

	void GetHeight(const FText& Text);

	void GetMine(const FText& Text);

	int MineCount = 12;

	int Width = 5;

	int Height = 4;	

	void UnHideBricksAround(const int TileIndex);

	bool IsMine(const int TileIndex);

	void StopGame();

	bool isGameStarted = false;

	int GetMinesAround(const int BrickIndex);

	TArray<int> GetAroundBricks(const int TileIndex) const;

private:
	TSharedPtr<SGridPanel> MinesPanel;

	FReply OnClickedStartButton();

	FReply OnClickedStopButton();
	
	TSharedPtr<SBrick> BrickWidget;

	TArray<SBrick*> ArrayBrick;	

	TArray<int> Bricks;
};
