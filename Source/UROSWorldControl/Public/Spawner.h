// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "SpawnModelSrv.h"
#include "ROSWorldControlManager.h"
#include "Spawner.generated.h"


struct SpawnAssetParams {
	FString PathOfMesh;
	FString PathOfMaterial;
	FVector Location;
	FRotator Rotator;
	TArray<UTagMsg> Tags;
};



UCLASS()
class UROSWORLDCONTROL_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();	
	TArray<SpawnAssetParams> SpawnAtNextTick;
	AROSWorldControlManager* Controller;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
	FString ServerAdress = TEXT("192.168.1.19");

	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
	int ServerPort = 9090;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type Reason);

	TSharedPtr<FROSBridgeHandler> Handler;

	//Spawns a StaticMeshComponent at the given Location, Rotation and Material.
	//Object pointed to by PathOfMesh has to be a StaticMeshComponent.
	//Object pointed to by PathOfMaterial has to be a UMaterial.
	bool SpawnAsset(const FString PathOfMesh, const FString PathOfMaterial, const FVector Location, const FRotator Rotation, const TArray<UTagMsg> Tags);


private: 

	UStaticMesh * LoadMesh(const FString Path);

	UMaterialInterface * LoadMaterial(const FString Path);

	class FROSSpawnMeshServer final : public FROSBridgeSrvServer
	{
	private: 
		ASpawner* Parent;
		bool GameThreadDoneFlag;
		bool ServiceSuccess;

	public:
		FROSSpawnMeshServer(FString Name, ASpawner* Parent_) :
			FROSBridgeSrvServer(Name, TEXT("ue/spawn_model")) 
		{
			Parent = Parent_;
		}
		

		TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

		TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

		void SetGameThreadDoneFlag(bool Flag);

		void SetServiceSuccess(bool success);
	};

	
};
