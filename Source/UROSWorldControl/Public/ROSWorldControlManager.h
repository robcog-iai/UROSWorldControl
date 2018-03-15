// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TagStatics.h"
#include "ROSBridgeHandler.h"
#include "ROSWorldControlManager.generated.h"

//Forward declariation to avoid recursion
class ASpawner;
class ARelocator;
class ARemover;

UCLASS()
class UROSWORLDCONTROL_API AROSWorldControlManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AROSWorldControlManager();
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
	FString ServerAdress;

	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
	int ServerPort;

	UPROPERTY(EditAnywhere, Category = "ROS")
	FString Namespace;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void EndPlay(const EEndPlayReason::Type Reason);
	ARelocator* GetRelocator();
	ASpawner* GetSpawner();
	TMap<FString, AActor*> IdToActorMap;

private:
	TSharedPtr<FROSBridgeHandler> Handler;

	ARelocator* Relocator;
	ASpawner* Spawner;
	ARemover* Remover;
};
