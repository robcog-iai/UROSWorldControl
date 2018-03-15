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

	ServerAdress = TEXT("127.0.0.1");
	ServerPort = 9090;
	Namespace = TEXT("unreal");

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
	IdToActorMap = FTagStatics::GetKeyValuesToActor(GetWorld(), "SemLog", "Id");


	// Spawn Relocator into the world
	Relocator = World->SpawnActor<ARelocator>();
	Relocator->Controller = this;


	// Spawn Spawner into the world
	Spawner = World->SpawnActor<ASpawner>();
	Spawner->Controller = this;

	// Spawn the Remover into the world
	Remover = World->SpawnActor<ARemover>();
	Remover->Controller = this;

	
	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(ServerAdress, ServerPort));

	// Add servers

	//Add spawn_model service
	TSharedPtr<ASpawner::FROSSpawnMeshServer> SpawnServer = MakeShareable<ASpawner::FROSSpawnMeshServer>(new ASpawner::FROSSpawnMeshServer(Namespace, TEXT("spawn_model"), Spawner));
	Handler->AddServiceServer(SpawnServer);

	// Add set_model_pose service 
	TSharedPtr<ARelocator::FROSRelocationServer> RelocateServer = MakeShareable<ARelocator::FROSRelocationServer>(new ARelocator::FROSRelocationServer(Namespace, TEXT("set_model_pose"), Relocator));
	Handler->AddServiceServer(RelocateServer);


	// Add delete_model service
	TSharedPtr<ARemover::FROSRemoveModelServer> RemoveServer = MakeShareable<ARemover::FROSRemoveModelServer>(new ARemover::FROSRemoveModelServer(Namespace, TEXT("delete_model"), Remover));
	Handler->AddServiceServer(RemoveServer);


	// Connect to ROSBridge Websocket server.
	Handler->Connect();

}

// Called every frame
void AROSWorldControlManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AROSWorldControlManager::EndPlay(const EEndPlayReason::Type Reason)
{
	// Disconnect the handler before parent ends
	Handler->Disconnect();

	Super::EndPlay(Reason);
}


ARelocator * AROSWorldControlManager::GetRelocator()
{
	return Relocator;
}

ASpawner * AROSWorldControlManager::GetSpawner()
{
	return Spawner;
}

