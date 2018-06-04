#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "ROSWorldControlManager.h"
#include "HighlightModel.h"


class FROSHighlightModelsServer final : public FROSBridgeSrvServer
{
		
private:
	FROSHighlightModelsServer();

	UWorld* World;

	ROSWorldControlManager* Controller;
public:
	FROSHighlightModelsServer(FString Namespace, FString Name, UWorld* InWorld,
		ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/HighlightModel")),
		World(InWorld), Controller(InController)
	{
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
