// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TagStatics.h"
#include "ROSWorldControlManager.generated.h"


//Forward declariation to avoid recursion
class ASpawner;
class ARelocator;

UCLASS()
class UROSWORLDCONTROL_API AROSWorldControlManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AROSWorldControlManager();
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
	FString ServerAdress = TEXT("192.168.1.19");

	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
	int ServerPort = 9090;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ARelocator* GetRelocator();
	ASpawner* GetSpawner();
	TMap<FString, AActor*> IDMap;

private:
	ARelocator* Relocator;
	ASpawner* Spawner;
};
