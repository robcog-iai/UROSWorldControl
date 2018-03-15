// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "Engine/StaticMeshActor.h"
#include "UTags.h"
#include "TagStatics.h"
#include "ROSWorldControlManager.h"
#include "Relocator.generated.h"

struct MoveAssetParams
{
	AActor* Actor;
	FVector Location;
	FRotator Rotator;
};

UCLASS()
class UROSWORLDCONTROL_API ARelocator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARelocator();
	
	AROSWorldControlManager* Controller;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);
	
	class FROSRelocationServer final : public FROSBridgeSrvServer
	{
	private:
		ARelocator * Parent;
		bool GameThreadDoneFlag;
		bool ServiceSuccess;


		void SetGameThreadDoneFlag(bool Flag);

		void SetServiceSuccess(bool Success);

	public:
		FROSRelocationServer(FString Namespace, FString Name, ARelocator* InParent) :
			FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_msgs/SetModelPose"))
		{
			Parent = InParent;
		}

		TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

		TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

	};
};
