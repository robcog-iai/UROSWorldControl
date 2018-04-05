// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SemanticMapEditor.h"
#include "SemanticMapEditorStyle.h"
#include "SemanticMapEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "ROSWorldControlManager.h"
#include "LevelEditor.h"

static const FName SemanticMapEditorTabName("Connect to ROSBridge");

#define LOCTEXT_NAMESPACE "FSemanticMapEditorModule"

FSemanticMapEditorModule::FSemanticMapEditorModule()
{
	//ServerAdress = TEXT("127.0.0.1");
	ServerAdress = TEXT("192.168.1.19");
	ServerPort = 9090;
	Namespace = TEXT("unreal");
	Controller = new ROSWorldControlManager(World, ServerAdress, ServerPort, Namespace);
}

void FSemanticMapEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FSemanticMapEditorStyle::Initialize();
	FSemanticMapEditorStyle::ReloadTextures();

	FSemanticMapEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSemanticMapEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSemanticMapEditorModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSemanticMapEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSemanticMapEditorModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FSemanticMapEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSemanticMapEditorStyle::Shutdown();

	FSemanticMapEditorCommands::Unregister();
}

void FSemanticMapEditorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here

	//delete to ensure disconnecting form Rosbridge if button is pressed multiple times
	World = GEditor->GetEditorWorldContext().World();
	
	if (Controller) {
		Controller->DisconnectFromROSBridge();
	}
	Controller = new ROSWorldControlManager(World, ServerAdress, ServerPort, Namespace);
	Controller->ConnectToROSBridge();

}

void FSemanticMapEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSemanticMapEditorCommands::Get().PluginAction);
}

void FSemanticMapEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSemanticMapEditorCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSemanticMapEditorModule, SemanticMapEditor)