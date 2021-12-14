// Copyright Epic Games, Inc. All Rights Reserved.

#include "MSweeperCommands.h"

#define LOCTEXT_NAMESPACE "FMSweeperModule"

void FMSweeperCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MSweeper", "Execute MSweeper action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
