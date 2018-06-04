#include "HighlightModelsServer.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSHighlightModelsServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSHighlightModelSrv::Request> Request_ =
		MakeShareable(new FROSHighlightModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSHighlightModelsServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSHighlightModelSrv::Request> HighlightModelRequest =
		StaticCastSharedPtr<FROSHighlightModelSrv::Request>(Request);


	AActor** ActorToBeHighlighted = Controller->IdToActorMap.Find(HighlightModelRequest->GetId());
	// Execute on game thread
	if (ActorToBeHighlighted)
	{
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			TArray<UStaticMeshComponent*> Components;
			(*ActorToBeHighlighted)->GetComponents<UStaticMeshComponent>(Components);
			for (auto Component : Components)
			{
				uint8 Color = HighlightModelRequest->GetColor();
				if (Color == 0)
				{
					Component->SetRenderCustomDepth(false);
				} 
				else
				{					
						Component->SetRenderCustomDepth(true);
						Component->CustomDepthStencilValue = Color;
				}
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSHighlightModelSrv::Response(true));
	}
	else
	{
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSHighlightModelSrv::Response(false));
	}


}
