#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "Engine/StaticMeshActor.h"
#include "Tags.h"
#include "ROSWorldControlManager.h"
#include "SetModelPose.h"
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

	UWorld * World;
	bool GameThreadDoneFlag;
	bool ServiceSuccess;

	ROSWorldControlManager* Controller;

	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);

	void SetGameThreadDoneFlag(bool Flag);

	void SetServiceSuccess(bool Success);

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