// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#include "RWCEdTool.h"
#include "UObject/ConstructorHelpers.h"

URWCEdTool::URWCEdTool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URWCEdTool::Connect()
{
	World = GEditor->GetEditorWorldContext().World();

	if (Controller)
	{
		Controller->DisconnectFromROSBridge();
	}
	Controller = new FRWCManager(World, ServerAdress, ServerPort, Namespace);

	// Setup Callbacks
	FROSWebsocketInfoSignature ErrorCallback;
	ErrorCallback.AddUObject(this, &URWCEdTool::ConnectionErrorCallback);

	FROSWebsocketInfoSignature ConnectedCallback;
	ConnectedCallback.AddUObject(this, &URWCEdTool::ConnectedCallback);

	Controller->ConnectToROSBridge(ErrorCallback, ConnectedCallback);
}


void URWCEdTool::ClearMap()
{
	if (Controller)
	{
		for (auto Element : Controller->IdToActorMap)
		{
			Element.Value->Destroy();
		}
		Controller->IdToActorMap.Empty();
	}
}


void URWCEdTool::ConnectionErrorCallback()
{
	ConnectionStatus = TEXT("Not connected.");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Connection to RosBridge lost.")));
}

void URWCEdTool::ConnectedCallback()
{
	ConnectionStatus = TEXT("Connected to Rosbridge.");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		                                 FString::Printf(TEXT("You are now connected to RosBridge.")));
}
