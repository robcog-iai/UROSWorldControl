#include "RemoveModelServer.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSRemoveModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDeleteModelSrv::Request> Request_ =
		MakeShareable(new FROSDeleteModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSRemoveModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSDeleteModelSrv::Request> RemoveModelRequest =
		StaticCastSharedPtr<FROSDeleteModelSrv::Request>(Request);

	AActor* ActorToBeRemoved;

	// get and remove Actor with given UtagID of Controller IDMap

	FString UniqueId = RemoveModelRequest->GetId();

	if (Controller->IdToActorMap.RemoveAndCopyValue(UniqueId, ActorToBeRemoved))
	{
		// Actor was found and will be destroyed on GameThread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			ServiceSuccess = ActorToBeRemoved->Destroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSDeleteModelSrv::Response(ServiceSuccess));
	}
	// the given UtagID was not found.
	UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be removed."), *
		RemoveModelRequest->GetId())
	;
	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSDeleteModelSrv::Response(false));
}
