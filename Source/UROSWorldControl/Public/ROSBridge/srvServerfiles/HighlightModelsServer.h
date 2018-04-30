#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "Engine/StaticMeshActor.h"
#include "ROSWorldControlManager.h"
#include "HighlightModels.h"


class FROSHighlightModelsServer final : public FROSBridgeSrvServer
{
	struct MoveAssetParams
	{
		AActor* Actor;
		FVector Location;
		FRotator Rotator;
	};

private:
	FROSHighlightModelsServer();

	UWorld * World;
	FThreadSafeBool ServiceSuccess;

	ROSWorldControlManager* Controller;

	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);

public:
	FROSHighlightModelsServer(FString Namespace, FString Name, UWorld* InWorld,
		ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_world_control_msgs/HighlightModels"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

};