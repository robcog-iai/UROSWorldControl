#include "AttachModelToParentServer.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSAttachModelToParentServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSAttachModelToParentSrv::Request> Request_ =
		MakeShareable(new FROSAttachModelToParentSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSAttachModelToParentServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSAttachModelToParentSrv::Request> AttachModelToParentRequest =
		StaticCastSharedPtr<FROSAttachModelToParentSrv::Request>(Request);

	AActor* Child = *Controller->IdToActorMap.Find(AttachModelToParentRequest->GetChildId());
	AActor* Parent = *Controller->IdToActorMap.Find(AttachModelToParentRequest->GetParentId());
	if (Child && Parent)
	{
		//Actors were found and will be attached, in GameThread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			Child->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSAttachModelToParentSrv::Response(true));
	}
	//at least one of them could not be found.
	if (!Child)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *AttachModelToParentRequest->GetChildId());
	}

	if (!Parent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *AttachModelToParentRequest->GetParentId());
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSAttachModelToParentSrv::Response(false));
}
