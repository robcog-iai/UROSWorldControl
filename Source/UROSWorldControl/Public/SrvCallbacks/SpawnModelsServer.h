#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "SpawnModel.h"
#include "PhysicsProperties.h"
#include "UROSWorldControlManager.h"
#include "Engine/StaticMesh.h"
#include "Tags.h"


class FROSSpawnModelServer : public FROSBridgeSrvServer
{
protected:
	struct SpawnAssetParams
	{
		FString Id;
		FString Name;
		FVector Location;
		FRotator Rotator;
		TArray<world_control_msgs::Tag> Tags;
		world_control_msgs::PhysicsProperties PhysicsProperties;
		FString StartDir;
		FString ActorLabel;
		TArray<FString> MaterialNames;
		TArray<FString> MaterialPaths;
		FString ParentId;
	};

	FROSSpawnModelServer()
	{
	};

	UWorld* World;

	FThreadSafeBool ServiceSuccess;

	bool SpawnAsset(SpawnAssetParams Params);


private:
	UStaticMesh* LoadMesh(FString Name, FString StartDir);
	UMaterialInterface* LoadMaterial(FString Name, FString StartDir);


public:
	UROSWorldControlManager* Controller;

	FROSSpawnModelServer(FString Namespace, FString Name, UWorld* InWorld, UROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnModel"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
