#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "SpawnModel.h"
#include "SpawnMultipleModels.h"
#include "ROSWorldControlManager.h"
#include "ModelDescription.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TagStatics.h"
#include "HelperFunctions.h"
#include <string>
#include <algorithm>


class FROSSpawnModelServer : public FROSBridgeSrvServer
{
protected:
	struct SpawnAssetParams {
		FString PathOfMesh;
		FString PathOfMaterial;
		FVector Location;
		FRotator Rotator;
		TArray<unreal_msgs::Tag> Tags;
		unreal_msgs::InstanceId* InstanceId;
	};

	FROSSpawnModelServer() {};
	UWorld * World;
	bool GameThreadDoneFlag;
	bool ServiceSuccess;

	bool SpawnAsset(const FString PathToMesh, const FString PathOfMaterial, const FVector Location, const FRotator Rotation, const TArray<unreal_msgs::Tag> Tags, unreal_msgs::InstanceId * InstanceId);

	void SetGameThreadDoneFlag(bool Flag);
	void SetServiceSuccess(bool Success);
	// This is used by the spawn mutliple models server, otherwise it would have the wrong type
	FROSSpawnModelServer(FString Namespace, FString Name, UWorld * InWorld, ROSWorldControlManager * InController, FString Type) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, Type)
	{
		Controller = InController;
		World = InWorld;
	}

private:
	UStaticMesh * LoadMesh(const FString Path, unreal_msgs::InstanceId * InstanceId);
	UMaterialInterface * LoadMaterial(const FString Path, unreal_msgs::InstanceId * InstanceId);
	FString FormatNamespace(FString Ns);
	FString GenerateId(int Length);


public:
	ROSWorldControlManager * Controller;

	FROSSpawnModelServer(FString Namespace, FString Name, UWorld * InWorld, ROSWorldControlManager * InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_msgs/SpawnModel"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};

class FROSSpawnMultipleModelsServer final : public FROSSpawnModelServer
{
private:
	FROSSpawnMultipleModelsServer() {};

public: 
	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
	
	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	FROSSpawnMultipleModelsServer(FString Namespace, FString Name, UWorld * InWorld, ROSWorldControlManager * InController) :
		FROSSpawnModelServer(Namespace, Name, InWorld, InController, TEXT("unreal_msgs/SpawnMultipleModels"))
	{}


};