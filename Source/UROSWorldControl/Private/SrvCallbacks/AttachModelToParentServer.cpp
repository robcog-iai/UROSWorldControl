#include "AttachModelToParentServer.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSAttachModelToParentServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeAttachModelToParentSrv::Request> Request_ =
		MakeShareable(new FROSBridgeAttachModelToParentSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSAttachModelToParentServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSBridgeAttachModelToParentSrv::Request> AttachModelToParentRequest =
		StaticCastSharedPtr<FROSBridgeAttachModelToParentSrv::Request>(Request);

	AActor* Child = *Controller->IdToActorMap.Find(AttachModelToParentRequest->GetChild().GetId());
	AActor* Parent = *Controller->IdToActorMap.Find(AttachModelToParentRequest->GetParent().GetId());
	if (Child && Parent)
	{
		//Actors were found and will be attached, in GameThread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			Child->AttachRootComponentToActor(Parent);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeAttachModelToParentSrv::Response(true));
	}
	//at least one of them could not be found.
	if (!Child)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *AttachModelToParentRequest->GetChild().GetId());
	}

	if (!Parent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *AttachModelToParentRequest->GetParent().GetId()
		);
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeAttachModelToParentSrv::Response(false));

	Child->AttachRootComponentToActor(Parent);
}
