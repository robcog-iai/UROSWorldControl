#include "HighlightModelsServer.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSHighlightModelsServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeHighlightModelsSrv::Request> Request_ =
		MakeShareable(new FROSBridgeHighlightModelsSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSHighlightModelsServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSBridgeHighlightModelsSrv::Request> SpawnSemanticMapRequest =
		StaticCastSharedPtr<FROSBridgeHighlightModelsSrv::Request>(Request);

	TArray<unreal_world_control_msgs::InstanceId>* InstanceIds = SpawnSemanticMapRequest->GetInstanceIds();

	TArray<bool> SuccessList;

	bool bAllSucceded = true;
	for (auto Instance : *InstanceIds)
	{
		AActor * ActorToBeHighlighted = *Controller->IdToActorMap.Find(Instance.GetId());
		// Execute on game thread
		if (ActorToBeHighlighted)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
			{
				TArray<UStaticMeshComponent*> Components;
				ActorToBeHighlighted->GetComponents<UStaticMeshComponent>(Components);
				for (auto Component : Components)
				{
					Component->SetRenderCustomDepth(true);
				}

			}, TStatId(), NULL, ENamedThreads::GameThread);

			//wait code above to complete
			FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		}
		else
		{
			bAllSucceded = false;
		}
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeHighlightModelsSrv::Response(bAllSucceded));

}
