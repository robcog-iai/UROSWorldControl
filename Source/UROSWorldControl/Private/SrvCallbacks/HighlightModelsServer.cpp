#include "HighlightModelsServer.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Tags.h"
#include "AssetHighlighter.h"

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


	AActor* ActorToBeHighlighted = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), HighlightModelRequest->GetId()).Pop();
	// Execute on game thread
	if (ActorToBeHighlighted)
	{
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			FAssetHighlighter::HighlightAsset(ActorToBeHighlighted, HighlightModelRequest->GetColor());
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
