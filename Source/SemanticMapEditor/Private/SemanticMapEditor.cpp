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
	UE_LOG(LogTemp, Warning, TEXT("Hello Friend!"));
}

void FSemanticMapEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Warning, TEXT("Du auch hier ?"));
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
	
	World = GEditor->GetEditorWorldContext().World();
	for (TActorIterator<AROSWorldControlManager> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.s
		UE_LOG(LogTemp, Warning, TEXT("ActorName: %s"), *ActorItr->GetName());
		AROSWorldControlManager* Controller = *ActorItr;

		Controller->ConnectToROSBridge();
		return;
	}
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