
#include "RemoveModelServer.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSRemoveModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeRemoveModelSrv::Request> Request_ =
		MakeShareable(new FROSBridgeRemoveModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSRemoveModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSBridgeRemoveModelSrv::Request> RemoveModelRequest =
		StaticCastSharedPtr<FROSBridgeRemoveModelSrv::Request>(Request);

	AActor* ActorToBeRemoved;

	// get and remove Actor with given UtagID of Controller IDMap

	FGuid UniqueId = RemoveModelRequest->GetInstanceId().GetId();

	if (Controller->IdToActorMap.RemoveAndCopyValue(UniqueId, ActorToBeRemoved)) {
		
		// Actor was found and will be destroyed on GameThread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			ServiceSuccess = ActorToBeRemoved->Destroy();

		}, TStatId(), NULL, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeRemoveModelSrv::Response(ServiceSuccess));
	}
	else
	{
		// the given UtagID was not found.
		UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be removed."), *FIds::GuidToBase64(RemoveModelRequest->GetInstanceId().GetId()));
		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeRemoveModelSrv::Response(false));
	}


}
