// Fill out your copyright notice in the Description page of Project Settings.

#include "ROSWorldControlManager.h"
#include "Spawner.h"
#include "Relocator.h"
#include "Remover.h"

// Sets default values
AROSWorldControlManager::AROSWorldControlManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bServicesPulished = false;

	ServerAdress = TEXT("127.0.0.1");
	ServerPort = 9090;
	Namespace = TEXT("unreal");

}

// Called when the game starts or when spawned
void AROSWorldControlManager::BeginPlay()
{
	Super::BeginPlay();

	// This check is needed if the Serices got pulished before the game started (e.g. by the Editor).
	if (!bServicesPulished)
	{
		ConnectToROSBridge();
	}
	
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

void AROSWorldControlManager::ConnectToROSBridge()
{

	UWorld* World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return;
	}

	// Setup IDMap
	IdToActorMap = FTagStatics::GetKeyValuesToActor(GetWorld(), "SemLog", "Id");


	// Spawn Relocator into the world
	if (!Relocator)
	{
		Relocator = World->SpawnActor<ARelocator>();
		Relocator->Controller = this;
	}


	// Spawn Spawner into the world
	if(!Spawner) 
	{
		Spawner = World->SpawnActor<ASpawner>();
		Spawner->Controller = this;
	}

	// Spawn the Remover into the world
	if (!Remover)
	{
		Remover = World->SpawnActor<ARemover>();
		Remover->Controller = this;
	}


	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(ServerAdress, ServerPort));

	// Add servers

	//Add spawn_model service
	TSharedPtr<ASpawner::FROSSpawnMeshServer> SpawnServer = MakeShareable<ASpawner::FROSSpawnMeshServer>(new ASpawner::FROSSpawnMeshServer(Namespace, TEXT("spawn_model"), Spawner));
	Handler->AddServiceServer(SpawnServer);

	//Add spawn_semantic_map service
	TSharedPtr<ASpawner::FROSSpawnSemanticMapServer> SpawnSemanticMapServer = MakeShareable<ASpawner::FROSSpawnSemanticMapServer>(new ASpawner::FROSSpawnSemanticMapServer(Namespace, TEXT("spawn_multiple_models"), Spawner));
	Handler->AddServiceServer(SpawnSemanticMapServer);

	// Add set_model_pose service 
	TSharedPtr<ARelocator::FROSRelocationServer> RelocateServer = MakeShareable<ARelocator::FROSRelocationServer>(new ARelocator::FROSRelocationServer(Namespace, TEXT("set_model_pose"), Relocator));
	Handler->AddServiceServer(RelocateServer);


	// Add delete_model service
	TSharedPtr<ARemover::FROSRemoveModelServer> RemoveServer = MakeShareable<ARemover::FROSRemoveModelServer>(new ARemover::FROSRemoveModelServer(Namespace, TEXT("delete_model"), Remover));
	Handler->AddServiceServer(RemoveServer);


	// Connect to ROSBridge Websocket server.
	Handler->Connect();
	bServicesPulished = true;
}

ARelocator * AROSWorldControlManager::GetRelocator()
{
	return Relocator;
}

ASpawner * AROSWorldControlManager::GetSpawner()
{
	return Spawner;
}

