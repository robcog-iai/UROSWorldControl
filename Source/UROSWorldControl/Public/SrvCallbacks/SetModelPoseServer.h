#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "ROSWorldControlManager.h"
#include "HelperFunctions.h"


class FROSSetModelPoseServer final : public FROSBridgeSrvServer
{
	struct MoveAssetParams
	{
		AActor* Actor;
		FVector Location;
		FRotator Rotator;
	};

private:
	FROSSetModelPoseServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	ROSWorldControlManager* Controller;

	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);

public:
	FROSSetModelPoseServer(FString Namespace, FString Name, UWorld* InWorld,
	                       ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_world_control_msgs/SetModelPose"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
