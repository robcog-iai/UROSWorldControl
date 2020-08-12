// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnRobotServer.h"
//#include "SDFParser.h"
#include "WorldControlGameInstance.h"
//#include "URModelBuilder.h"
//#include "URoboSim/Classes/SDF/SDFDataAsset.h"




TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnRobotServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<FROSRobotModelSrv::Request> Request =
        MakeShareable(new FROSRobotModelSrv::Request());
    Request->FromJson(JsonObject);
    return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}



TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnRobotServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
    UE_LOG(LogTemp, Log, TEXT("SpawnRobotServer recieved a Message. Time to see what it is"));

    TSharedPtr<FROSRobotModelSrv::Request> SpawnRobotRequest=StaticCastSharedPtr<FROSRobotModelSrv::Request>(Request);
    //NO check if we check and it fails the whole system gets shoot down

    //start spawning the robot
    //get USDFModel
    //USDFParser* parser = NewObject<USDFParser>();
    //parser->LoadSDF(TEXT("/home/nleusmann/Documents/Sandbox/UE4_Environment/URoboSimExampleRobots/PR2/model.sdf"));
    //FSDFParser* parser =new FSDFParser(TEXT("/home/nleusmann/Documents/Sandbox/UE4_Environment/URoboSimExampleRobots/PR2/model.sdf")); // NewObject<FSDFParser>(TEXT("/home/nleusmann/Documents/Sandbox/UE4_Environment/URoboSimExampleRobots/PR2/model.sdf"));
    UE_LOG(LogTemp, Log, TEXT("Loaded SDF-File"));
    //EObjectFlags flags = RF_Transactional; // In USDFDataAssetFactory --> Flags |= RF_Transactional;

    //USDFDataAsset* dataAsset = parser->ParseToNewDataAsset(GetTransientPackage(),TEXT("PR2DataAsset"), flags);

    UE_LOG(LogTemp, Log, TEXT("We have an dataAsset File"));
    //Create ARModelActor
    //plug everthing into the URMODELBuilder
    //URModelBuilder ModelBuilder= NewObject<URModelBuilder>();
    return MakeShareable<FROSBridgeSrv::SrvResponse>(new FROSRobotModelSrv::Response(/*Id,FinalActorName,ServiceSuccess*/));
}
