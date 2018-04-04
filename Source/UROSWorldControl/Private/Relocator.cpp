// Fill out your copyright notice in the Description page of Project Settings.

#include "Relocator.h"
#include "ROSBridge/srv/SetModelPose.h"
#include "HelperFunctions.h"

// Sets default values
ARelocator::ARelocator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARelocator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARelocator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




bool ARelocator::Relocate(AActor* Actor, FVector Location, FRotator Rotator)
{

	if (Actor->TeleportTo(Location, Rotator) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not set %s to locaiton: %s, with Rotation: %s"),
			*Actor->GetName(), *Location.ToString(), *Rotator.ToString());
		return false;
	}

	return true;
}


TSharedPtr<FROSBridgeSrv::SrvRequest> ARelocator::FROSRelocationServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeSetModelPoseSrv::Request> Request_ =
		MakeShareable(new FROSBridgeSetModelPoseSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> ARelocator::FROSRelocationServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSBridgeSetModelPoseSrv::Request> SetModelPoseRequest =
		StaticCastSharedPtr<FROSBridgeSetModelPoseSrv::Request>(Request);

	//Get Actor for given ID
	unreal_msgs::InstanceId Id = SetModelPoseRequest->GetInstanceId();
	FString UniqueId = UROSWorldControlHelper::GetUniqueIdOfInstanceID(&Id);
	
	AActor** Actor = Parent->Controller->IdToActorMap.Find(UniqueId);


	if (!Actor) {
		// Couldn't find Actor for ID 
		UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be moved."), *UniqueId);
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeSetModelPoseSrv::Response(false));
	}


	// Setup params
	MoveAssetParams Params;
	Params.Actor = *Actor;
	Params.Location = SetModelPoseRequest->GetLocation();
	Params.Rotator = SetModelPoseRequest->GetRotator();

	GameThreadDoneFlag = false;
	//Actor was found and will be relocated, in GameThread
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		bool success = Parent->Relocate(Params.Actor,
			Params.Location,
			Params.Rotator);
		SetServiceSuccess(success);
		SetGameThreadDoneFlag(true);
	}
	);

	// Wait for gamethread to be done
	while (!GameThreadDoneFlag) {
		FPlatformProcess::Sleep(0.01);
	}
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeSetModelPoseSrv::Response(ServiceSuccess));
}

void  ARelocator::FROSRelocationServer::SetGameThreadDoneFlag(bool Flag)
{
	GameThreadDoneFlag = Flag;
}

void ARelocator::FROSRelocationServer::SetServiceSuccess(bool Success)
{
	ServiceSuccess = Success;
}