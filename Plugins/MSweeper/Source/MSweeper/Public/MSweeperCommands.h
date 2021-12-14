// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MSweeperStyle.h"

class FMSweeperCommands : public TCommands<FMSweeperCommands>
{
public:

	FMSweeperCommands()
		: TCommands<FMSweeperCommands>(TEXT("MSweeper"), NSLOCTEXT("Contexts", "MSweeper", "MSweeper Plugin"), NAME_None, FMSweeperStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
