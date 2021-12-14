// Copyright Epic Games, Inc. All Rights Reserved.

#include "MSweeper.h"
#include "MSweeperStyle.h"
#include "MSweeperCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include <Runtime\EngineSettings\Classes\GameMapsSettings.h>
#include <Runtime\Engine\Classes\GameFramework\InputSettings.h>
#include <EngineSharedPCH.h>
#include "SlateBasics.h"
#include "SMSweeper.h"

static const FName MSweeperTabName("MSweeper");
int ClickCount = 0;
FText TextForString;

#define LOCTEXT_NAMESPACE "FMSweeperModule"

void FMSweeperModule::StartupModule()
{
	FMSweeperStyle::Initialize();
	FMSweeperStyle::ReloadTextures();

	FMSweeperCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMSweeperCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMSweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMSweeperModule::RegisterMenus));

	TSharedRef<class FGlobalTabmanager> tabManager = FGlobalTabmanager::Get();
	tabManager->RegisterNomadTabSpawner(MSweeperTabName, FOnSpawnTab::CreateRaw(this, &FMSweeperModule::SpawnTab))
		.SetDisplayName(FText::FromString(TEXT("Umit Unal")));
}

TSharedRef<SDockTab> FMSweeperModule::SpawnTab(const FSpawnTabArgs& TabSpawnArgs)
{
	TSharedPtr<SDockTab> DockTab;
	SAssignNew(DockTab, SDockTab).TabRole(ETabRole::NomadTab);
	DockTab->SetContent(SNew(SMSweeper).GameManagerObject(MineSweeperManager));
	return DockTab.ToSharedRef();
}

void FMSweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMSweeperStyle::Shutdown();

	FMSweeperCommands::Unregister();
}

void FMSweeperModule::PluginButtonClicked()
{
	TSharedRef<class FGlobalTabmanager> tabManager = FGlobalTabmanager::Get();
	tabManager->InvokeTab(MSweeperTabName);
}

void FMSweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMSweeperCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMSweeperCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMSweeperModule, MSweeper)