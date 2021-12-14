#include "SMSweeper.h"
#include "SBrick.h"


void SMSweeper::Construct(const FArguments& InArgs)
{
	int size = Width * Height;
	Bricks.Empty();
	Bricks.AddDefaulted(size);

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().Padding(10, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("MineSweeper  Game")))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 24))
		.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FIntPoint(-2, 2))
		]

		]
	+ SScrollBox::Slot().Padding(10, 5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[

			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Width : ")))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
		.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FIntPoint(-2, 2))
		]

	+ SHorizontalBox::Slot()
		[
			SNew(SEditableTextBox)
			.Text(FText::FromString(TEXT("5")))
		.OnTextChanged_Raw(this, &SMSweeper::GetWidth)
		]

	+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Height : ")))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
		.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FIntPoint(-2, 2))
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SEditableTextBox)
			.Text(FText::FromString(TEXT("4")))
		.OnTextChanged_Raw(this, &SMSweeper::GetHeight)
		]

		]
	+ SScrollBox::Slot().Padding(10, 5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[

			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Mine Count: ")))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
		.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FIntPoint(-2, 2))
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SEditableTextBox)
			.Text(FText::FromString(TEXT("6")))
		.OnTextChanged_Raw(this, &SMSweeper::GetMine)
		]
		]
	+ SScrollBox::Slot().Padding(15, 5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().HAlign(HAlign_Center)
		[
			SNew(SButton)
			.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Generate New Grid")))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
		.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
		.ShadowColorAndOpacity(FLinearColor::Black)
		.ShadowOffset(FIntPoint(-2, 2))
		]
	.OnClicked(FOnClicked::CreateRaw(this, &SMSweeper::OnClickedStartButton))
		]
		]
	+ SScrollBox::Slot().Padding(20, 5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1)
		[
			SNew(SScrollBox).Orientation(EOrientation::Orient_Vertical)
			+ SScrollBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox).Orientation(EOrientation::Orient_Horizontal)
			+ SScrollBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(MinesPanel, SGridPanel)
		]
		]
		]
		]
		]
		];
}

FReply SMSweeper::OnClickedStopButton()
{
	return FReply::Handled();
}

FReply SMSweeper::OnClickedStartButton()
{
	if (Width * Height <= MineCount / 2)
	{
		FText DialogText = FText::FromString("Mine count can not be greater than grid size");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return FReply::Handled();
	}
	else if (MineCount == 0)
	{
		FText DialogText = FText::FromString("Mine count can not be 0(zero)");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		return FReply::Handled();
	}

	MinesPanel->ClearChildren();

	for (int i = 0; i < Width * Height; i++)
	{
		SAssignNew(BrickWidget, SBrick);
		BrickWidget->BuildBrick(i);
		BrickWidget->OnBrickUnHide().AddRaw(this, &SMSweeper::OnBrickUnHide);
		ArrayBrick.Add(BrickWidget.Get());
		int x = i % Width;
		int y = i / Width;

		SGridPanel::FSlot& MinesPanelSlot = MinesPanel->AddSlot(x, y);
		MinesPanelSlot.HAlign(HAlign_Fill);
		MinesPanelSlot.VAlign(VAlign_Fill);
		MinesPanelSlot.AttachWidget(BrickWidget.ToSharedRef());
	}
	isGameStarted = true;
	return FReply::Handled();
}

void SMSweeper::OnBrickUnHide(SBrick* ClickedTile)
{
	const int BrickIndex = ClickedTile->GetBrickIndex();

	if (isFirstBrickUnHided)
	{
		LocateMines(BrickIndex);
		isFirstBrickUnHided = false;
		
	}

	UnHideBrick(BrickIndex);
}

void SMSweeper::LocateMines(const int ExcludedBrickIndex)
{	
	for (int i = 0; i < MineCount; i++)
	{
		const int randomBrick = FMath::RandHelper(Bricks.Num());
		if (randomBrick != ExcludedBrickIndex && !Bricks[randomBrick])
		{
			i++;
			Bricks[randomBrick] = -1;
		}
	}
}

void SMSweeper::GetMine(const FText& Text)
{
	FString strMine = Text.ToString();
	MineCount = FCString::Atoi(*strMine);
	MineCount = MineCount * 2;
}

void SMSweeper::GetWidth(const FText& Text)
{
	FString strWidth = Text.ToString();
	Width = FCString::Atoi(*strWidth);
}

void SMSweeper::GetHeight(const FText& Text)
{
	FString strHeight = Text.ToString();
	Height = FCString::Atoi(*strHeight);
}

int SMSweeper::GetMinesAround(const int BrickIndex)
{
	int count = 0;

	for (const int i : GetAroundBricks(BrickIndex))
	{
		count += IsMine(i);
	}

	return count;
}

TArray<int> SMSweeper::GetAroundBricks(const int BrickIndex) const
{
	TArray<int> Adjacents;
	Adjacents.Add(BrickIndex - 4 - 1);	Adjacents.Add(BrickIndex - 4);	Adjacents.Add(BrickIndex - 4 + 1); 	Adjacents.Add(BrickIndex - 1); 	Adjacents.Add(BrickIndex + 1); 	Adjacents.Add(BrickIndex + 4 - 1); 	Adjacents.Add(BrickIndex + 4); 	Adjacents.Add(BrickIndex + 4 + 1);
	return Adjacents;
}

void SMSweeper::UnHideBrick(const int BrickIndex)
{
	if (!isGameStarted) return;

	if (IsMine(BrickIndex))
	{
		StopGame();
	}
	else
	{
		Bricks[BrickIndex] = 1;
		const int CountBrick = GetMinesAround(BrickIndex);
		ArrayBrick[BrickIndex]->UnHide(CountBrick);
	}
}

void SMSweeper::UnHideBricksAround(const int BrickIndex)
{
	for (const int i : GetAroundBricks(BrickIndex))
	{
		if (!IsMine(i))
		{
			UnHideBrick(i);
		}
	}
}

void SMSweeper::StopGame()
{
	isGameStarted = false;

	for (int i = 0; i < Bricks.Num(); i++)
	{
		if (Bricks[i] < 0)
		{
			ArrayBrick[i]->UnHide(-1);
		}
	}



	FText DialogText = FText::FromString("You LOST! :(");
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

bool SMSweeper::IsMine(const int BrickIndex)
{
	if (Bricks.IsValidIndex(BrickIndex))
	{
		return Bricks[BrickIndex] < 0;
	}

	return false;
}
