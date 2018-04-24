#include "RosConnectionTool.h"
#include "UObject/ConstructorHelpers.h"

URosConnectionTool::URosConnectionTool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void URosConnectionTool::ConnectToRosBridge()
{
	World = GEditor->GetEditorWorldContext().World();

	if (Controller) {
		Controller->DisconnectFromROSBridge();
	}
	Controller = new ROSWorldControlManager(World, ServerAdress, ServerPort, Namespace);
	
	// Setup Callbacks
	FWebsocketInfoCallBack ErrorCallback;
	ErrorCallback.AddUObject(this, &URosConnectionTool::ConnectionErrorCallback);

	FWebsocketInfoCallBack ConnectedCallback;
	ConnectedCallback.AddUObject(this, &URosConnectionTool::ConnectedCallback);

	Controller->ConnectToROSBridge(ErrorCallback, ConnectedCallback);
	Connected = Controller->IsConnected();
}


void URosConnectionTool::ClearMap()
{
	for (auto Element : Controller->IdToActorMap)
	{
		Element.Value->Destroy();
	}
	Controller->IdToActorMap.Empty();
}

void URosConnectionTool::ConnectionErrorCallback() {
	Connected = false;
}

void URosConnectionTool::ConnectedCallback() {
	Connected = true;
}