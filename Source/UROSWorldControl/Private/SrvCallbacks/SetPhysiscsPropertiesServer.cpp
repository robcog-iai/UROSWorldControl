#include "SetPhysicsPropertiesServer.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSetPhysicsPropertiesServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSetPhysicsPropertiesSrv::Request> Request =
		MakeShareable(new FROSSetPhysicsPropertiesSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSetPhysicsPropertiesServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSetPhysicsPropertiesSrv::Request> ChangeVisualRequest =
		StaticCastSharedPtr<FROSSetPhysicsPropertiesSrv::Request>(Request);

	// get Actor with given UtagID of Controller IDMap
	AActor** ActorToBeChanged = Controller->IdToActorMap.Find(ChangeVisualRequest->GetId());

	TArray<UStaticMeshComponent*> Components;
	(*ActorToBeChanged)->GetComponents<UStaticMeshComponent>(Components);

	if (Components.Num() != 1)
	{
		UE_LOG(LogTemp, Error,
			TEXT(
				"Actor %s has to more then one StaticMeshComponent, since it's not clear which one should be changed nothing was done."
			),
			*(*ActorToBeChanged)->GetName());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetPhysicsPropertiesSrv::Response(false));
	}
	UStaticMeshComponent* MeshComponent = Components.Pop();

	// Execute on game thread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		MeshComponent->SetSimulatePhysics(ChangeVisualRequest->GetPhysicsProperties().GetSimulatePhysics());
		MeshComponent->bGenerateOverlapEvents = ChangeVisualRequest->GetPhysicsProperties().GetGenerateOverlapEvents();
		MeshComponent->SetEnableGravity(ChangeVisualRequest->GetPhysicsProperties().GetGravity());
		MeshComponent->SetMassOverrideInKg(NAME_None, ChangeVisualRequest->GetPhysicsProperties().GetMass());
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSetPhysicsPropertiesSrv::Response(true));
}