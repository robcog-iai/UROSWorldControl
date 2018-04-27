#include "ChangeVisualOfModelServer.h"

void FROSChangeVisualOfModelServer::SetServiceSuccess(bool Success)
{
		ServiceSuccess = Success;
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSChangeVisualOfModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<RosWorldControlChangeVisualOfModelsSrv::Request> Request_ =
		MakeShareable(new RosWorldControlChangeVisualOfModelsSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSChangeVisualOfModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<RosWorldControlChangeVisualOfModelsSrv::Request> ChangeVisualRequest =
		StaticCastSharedPtr<RosWorldControlChangeVisualOfModelsSrv::Request>(Request);
	
	unreal_world_control_msgs::MeshDescription MeshDescription = ChangeVisualRequest->GetMeshDescription();
	//Check if at least one Path was set.
	if (MeshDescription.GetPathToMaterial().IsEmpty() && MeshDescription.GetPathToMesh().IsEmpty())
	{
		// nothing to do here.
		UE_LOG(LogTemp, Error, TEXT("Nothing to do. %s has not Paths set. "), *ChangeVisualRequest->ToString());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new RosWorldControlChangeVisualOfModelsSrv::Response(false));
	}

	// get Actor with given UtagID of Controller IDMap

	AActor* ActorToBeChanged = *Controller->IdToActorMap.Find(ChangeVisualRequest->GetInstanceId().GetId());

	TArray<UStaticMeshComponent*> Components;
	ActorToBeChanged->GetComponents<UStaticMeshComponent>(Components);

	if (Components.Num() != 1) {
		// TODO: Handle this some other way
		UE_LOG(LogTemp, Error, 
			TEXT("Actor %s has to more then one StaticMeshComponent, since it's not clear which one should be changed nothing was done."), 
			*ActorToBeChanged->GetName());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new RosWorldControlChangeVisualOfModelsSrv::Response(false));
	}


	// Execute on game thread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = ChangeVisual(MeshDescription, Components.Pop());

	}, TStatId(), NULL, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new RosWorldControlChangeVisualOfModelsSrv::Response(ServiceSuccess));

}

bool FROSChangeVisualOfModelServer::ChangeVisual(unreal_world_control_msgs::MeshDescription MeshDescription, UStaticMeshComponent* MeshComponent)
{

	if (!MeshDescription.GetPathToMesh().IsEmpty())
	{
		//Try to load mesh 
		UStaticMesh* Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *MeshDescription.GetPathToMesh()));
		if (Mesh)
		{
			MeshComponent->SetStaticMesh(Mesh);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Path %s does not point to a StaticMesh"), *MeshDescription.GetPathToMesh());
			return false;
		}
	}

	if (!MeshDescription.GetPathToMaterial().IsEmpty())
	{
		//Try to load Material 
		UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MeshDescription.GetPathToMaterial()));
		if (Material)
		{
			MeshComponent->SetMaterial(0, Material);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Path %s does not point to a Material"), *MeshDescription.GetPathToMaterial());
			return false;
		}
	}

	return true;
}
