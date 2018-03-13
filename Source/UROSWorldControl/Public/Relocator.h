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

	TArray<MoveAssetParams> MoveAtNextTick;
	TSharedPtr<FROSBridgeHandler> Handler;
	AROSWorldControlManager* Controller;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
		FString ServerAdress = TEXT("192.168.1.19");

	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
		int ServerPort = 9090;

	virtual void EndPlay(const EEndPlayReason::Type Reason);


private:

	class FROSRelocationServer final : public FROSBridgeSrvServer
	{
	private:
		ARelocator * Parent;
		bool GameThreadDoneFlag;
		bool ServiceSuccess;

	public:
		FROSRelocationServer(FString Name, ARelocator* Parent_) :
			FROSBridgeSrvServer(Name, TEXT("ue/relocate_model"))
		{
			Parent = Parent_;
		}

		TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

		TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

		void SetGameThreadDoneFlag(bool Flag);

		void SetServiceSuccess(bool success);
	};
};
