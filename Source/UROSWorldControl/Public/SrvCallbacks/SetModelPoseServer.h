#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "UROSWorldControlManager.h"


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

	UROSWorldControlManager* Controller;

	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);

public:
	FROSSetModelPoseServer(FString Namespace, FString Name, UWorld* InWorld,
	                       UROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SetModelPose"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
