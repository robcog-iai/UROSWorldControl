
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

	unreal_msgs::InstanceId Id = RemoveModelRequest->GetInstanceId();
	FString UniqueId = UROSWorldControlHelper::GetUniqueIdOfInstanceID(&Id);

	if (Controller->IdToActorMap.RemoveAndCopyValue(UniqueId, ActorToBeRemoved)) {
		// Actor was found and will be destroyed on GameThread
		GameThreadDoneFlag = false;
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			bool bSuccess = ActorToBeRemoved->Destroy();
			SetServiceSuccess(bSuccess);
			SetGameThreadDoneFlag(true);
		}
		);

		// Wait for gamethread to be done
		while (!GameThreadDoneFlag) {
			FPlatformProcess::Sleep(0.01);
		}

		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeRemoveModelSrv::Response(ServiceSuccess));
	}
	else
	{
		// the given UtagID was not found.
		UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be removed."), *RemoveModelRequest->GetInstanceId().GetId());
		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeRemoveModelSrv::Response(false));
	}


}


void  FROSRemoveModelServer::SetGameThreadDoneFlag(bool Flag)
{
	GameThreadDoneFlag = Flag;
}

void FROSRemoveModelServer::SetServiceSuccess(bool Success)
{
	ServiceSuccess = Success;
}