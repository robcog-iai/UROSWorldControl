#include "SetModelPoseServer.h"
#include "SetModelPose.h"
#include "Conversions.h"


bool FROSSetModelPoseServer::Relocate(AActor* Actor, FVector Location, FRotator Rotator)
{
	if (!Actor->TeleportTo(Location, Rotator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not set %s to locaiton: %s, with Rotation: %s"),
			*Actor->GetName(), *Location.ToString(), *Rotator.ToString());
		return false;
	}

	return true;
}


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSetModelPoseServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSetModelPoseSrv::Request> Request_ =
		MakeShareable(new FROSSetModelPoseSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSetModelPoseServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSetModelPoseSrv::Request> SetModelPoseRequest =
		StaticCastSharedPtr<FROSSetModelPoseSrv::Request>(Request);

	//Get Actor for given ID
	FString UniqueId = SetModelPoseRequest->GetId();

	AActor** Actor = Controller->IdToActorMap.Find(UniqueId);


	if (!Actor)
	{
		// Couldn't find Actor for ID 
		UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be moved."), *UniqueId);
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetModelPoseSrv::Response(false));
	}


	// Setup params
	MoveAssetParams Params;
	Params.Actor = *Actor;
	Params.Location = FConversions::ROSToU(SetModelPoseRequest->GetPose().GetPosition().GetVector());
	Params.Rotator = FRotator::FRotator(FConversions::ROSToU(SetModelPoseRequest->GetPose().GetOrientation().GetQuat()));

	//Actor was found and will be relocated, in GameThread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = Relocate(Params.Actor,
		                          Params.Location,
		                          Params.Rotator);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSetModelPoseSrv::Response(ServiceSuccess));
}
