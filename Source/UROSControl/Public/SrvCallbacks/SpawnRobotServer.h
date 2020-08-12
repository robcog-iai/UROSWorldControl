// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSrvServer.h"
#include "SpawnRobot.h"
#include "RWCManager.h"

/**
 * This Service should be able to spawn Robot
 */
class FROSSpawnRobotServer: public FROSBridgeSrvServer
{
protected:

    FROSSpawnRobotServer()
    {
    };

    UWorld* World;

    FThreadSafeBool ServiceSuccess;


private:
//    From SpawnModelsServer
//    UStaticMesh* LoadMesh(FString Name, FString StartDir);
//    UMaterialInterface* LoadMaterial(FString Name, FString StartDir);


public:
    URWCManager* Controller;

    FROSSpawnRobotServer(FString Namespace, FString Name, UWorld* InWorld, URWCManager* InController) :
        FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnRobot"))
    {
        Controller = InController;
        World = InWorld;
    }


    TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

    TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
