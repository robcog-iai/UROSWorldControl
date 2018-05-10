#include "UnrealWorldControlEdTool.h"
#include "UObject/ConstructorHelpers.h"

UUnrealWorldControlEdTool::UUnrealWorldControlEdTool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UUnrealWorldControlEdTool::ConnectToRosBridge()
{
	World = GEditor->GetEditorWorldContext().World();

	if (Controller)
	{
		Controller->DisconnectFromROSBridge();
	}
	Controller = new ROSWorldControlManager(World, ServerAdress, ServerPort, Namespace);

	// Setup Callbacks
	FWebsocketInfoCallBack ErrorCallback;
	ErrorCallback.AddUObject(this, &UUnrealWorldControlEdTool::ConnectionErrorCallback);

	FWebsocketInfoCallBack ConnectedCallback;
	ConnectedCallback.AddUObject(this, &UUnrealWorldControlEdTool::ConnectedCallback);

	Controller->ConnectToROSBridge(ErrorCallback, ConnectedCallback);
}


void UUnrealWorldControlEdTool::ClearMap()
{
	if(Controller)
	{	
		for (auto Element : Controller->IdToActorMap)
		{
			Element.Value->Destroy();
		}
		Controller->IdToActorMap.Empty();
	}
}


void UUnrealWorldControlEdTool::ConnectionErrorCallback() {
	ConnectionStatus = TEXT("Not connected.");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Connection to RosBridge lost.")));

}

void UUnrealWorldControlEdTool::ConnectedCallback() {
	ConnectionStatus = TEXT("Connected to Rosbridge.");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("You are now connected to RosBridge.")));

}