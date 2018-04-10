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
	Controller->ConnectToROSBridge();
}
