// Fill out your copyright notice in the Description page of Project Settings.

#include "ROSWorldControlManager.h"
#include "Spawner.h"
#include "Relocator.h"
#include "Remover.h"

// Sets default values
AROSWorldControlManager::AROSWorldControlManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AROSWorldControlManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return;
	}
	

	// Setup IDMap
	IDMap = FTagStatics::GetKeyValuesToActor(GetWorld(), "SemLog", "id");


	// Spawn Relocator into the world
	Relocator = World->SpawnActor<ARelocator>();
	Relocator->ServerAdress = ServerAdress;
	Relocator->ServerPort = ServerPort;
	Relocator->Controller = this;


	// Spawn Spawner into the world
	Spawner = World->SpawnActor<ASpawner>();
	Spawner->ServerAdress = ServerAdress;
	Spawner->ServerPort = ServerPort;
	Spawner->Controller = this;

	// Spawn the Remover into the world
	Remover = World->SpawnActor<ARemover>();
	Remover->ServerAdress = ServerAdress;
	Remover->ServerPort = ServerPort;
	Remover->Controller = this;

	
}

// Called every frame
void AROSWorldControlManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ARelocator * AROSWorldControlManager::GetRelocator()
{
	return Relocator;
}

ASpawner * AROSWorldControlManager::GetSpawner()
{
	return Spawner;
}

