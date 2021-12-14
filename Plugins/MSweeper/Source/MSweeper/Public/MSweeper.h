// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UGameManagerObject;
class FToolBarBuilder;
class FMenuBuilder;

class FMSweeperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TabSpawnArgs);

	TSharedPtr<SVerticalBox> Widget;	

private:

	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	UGameManagerObject* MineSweeperManager;
};
