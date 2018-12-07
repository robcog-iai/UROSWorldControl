// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "SetMaterial.h"
#include "RWCManager.h"
#include "Engine/StaticMesh.h"


class FROSSetMaterialServer : public FROSBridgeSrvServer
{
protected:
	

	FROSSetMaterialServer()
	{
	};

	UWorld* World;

	FThreadSafeBool ServiceSuccess;
	

private:
	UStaticMesh * LoadMesh(FString Name, FString StartDir);
	UMaterialInterface* LoadMaterial(FString Name, FString StartDir);

	bool ChangeVisual(UStaticMeshComponent * MeshComponent, TArray<FString> MaterialNames, TArray<FString> MaterialPaths);

public:
	FRWCManager * Controller;

	FROSSetMaterialServer(FString Namespace, FString Name, UWorld* InWorld, FRWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SetMaterial"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

};
