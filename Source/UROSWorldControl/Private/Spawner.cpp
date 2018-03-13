// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TagStatics.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();


	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(ServerAdress, ServerPort));

	// Add service clients and servers
	TSharedPtr<FROSSpawnMeshServer> ServiceServer = MakeShareable<FROSSpawnMeshServer>(new FROSSpawnMeshServer(TEXT("ue/spawn_model"), this));
	Handler->AddServiceServer(ServiceServer);

	// Connect to ROSBridge Websocket server.
	Handler->Connect();

	// DEBUG
	/*SpawnAsset(
		TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"),
		TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"),
		FVector(130, 200, 300),
		FRotator::ZeroRotator,
		TArray<UTagMsg>::TArray()
	);*/
		
}




// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	Handler->Process();
}

void ASpawner::EndPlay(const EEndPlayReason::Type Reason)
{
	Handler->Disconnect();
	// Disconnect the handler before parent ends

	Super::EndPlay(Reason);
}


bool ASpawner::SpawnAsset(const FString PathToMesh, const FString PathOfMaterial, const FVector Location, const FRotator Rotation, const TArray<UTagMsg> Tags)
{
	//Check if Path is empty
	if (PathToMesh.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("Path to the spawning asset, was empty."));
		return false;
	}

	UWorld* World = GetWorld();
	//Check if World is avialable
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return false;
	}

	//Setup SpawnParameters 
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Instigator;
	SpawnParams.Owner = this;


	//Load Mesh and check if it succeded.
	UStaticMesh* Mesh = LoadMesh(PathToMesh);
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path does not point to a static mesh"));
		return false;
	}


	//Load Material and check if it succeded
	UMaterialInterface* Material = LoadMaterial(PathOfMaterial);
	if (!Material)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path does not point to a Material"));
		return false;
	}

	//Actual Spawning MeshComponent
	AStaticMeshActor* SpawnedItem = World->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnParams);

	
	for (auto Tag : Tags)
	{
		FTagStatics::AddKeyValuePair(
			SpawnedItem,
			Tag.GetTagType(),
			Tag.GetKey(),
			Tag.GetValue());

		//Check if current Tag ist SemLog;id
		if (Tag.GetTagType().Equals(TEXT("SemLog")) && Tag.GetKey().Equals(TEXT("id"))) {
			// check if ID was already taken
			if (Controller->IDMap.Find(Tag.GetValue()) == nullptr) 
			{
				//Assigning the Mesh and Material to the Component
				SpawnedItem->SetMobility(EComponentMobility::Movable);
				SpawnedItem->GetStaticMeshComponent()->SetStaticMesh(Mesh);
				SpawnedItem->GetStaticMeshComponent()->SetMaterial(0, Material);

				//Add this object to id refrence map
				Controller->IDMap.Add(Tag.GetValue(), SpawnedItem);
			}
			else
			{
				//ID is already taken
				//Spawing and then destroying object avoids iterating over tags twice.
				//since this code should rarely trigger this should be faster compared to extra iterations on every spawn 
				SpawnedItem->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *Tag.GetValue());
				return false;
			}
			
			
		}
	}

	return true;
}

UStaticMesh * ASpawner::LoadMesh(const FString Path)
{
	UStaticMesh* Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *Path));
	return Mesh;
}

UMaterialInterface * ASpawner::LoadMaterial(const FString Path)
{
	UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Path));
	return Material;
}



TSharedPtr<FROSBridgeSrv::SrvRequest> ASpawner::FROSSpawnMeshServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeSpawnServiceSrv::Request> Request_ =
		MakeShareable(new FROSBridgeSpawnServiceSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> ASpawner::FROSSpawnMeshServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) 
{
	
	TSharedPtr<FROSBridgeSpawnServiceSrv::Request> Request_ =
		StaticCastSharedPtr<FROSBridgeSpawnServiceSrv::Request>(Request);

	SpawnAssetParams Params;
	Params.PathOfMesh = Request_->GetPathToMesh();
	Params.PathOfMaterial = Request_->GetPathToMaterial();
	Params.Location = Request_->GetLocation();
	Params.Rotator = Request_->GetRotator();
	Params.Tags = Request_->GetTags();
	
	GameThreadDoneFlag = false;
	// Execute on game thread
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		bool success = Parent->SpawnAsset(Params.PathOfMesh,
			Params.PathOfMaterial,
			Params.Location,
			Params.Rotator,
			Params.Tags);
		SetServiceSuccess(success);
		SetGameThreadDoneFlag(true);
	}
	);

	// Wait for gamethread to be done
	while (!GameThreadDoneFlag) {
		FPlatformProcess::Sleep(1);
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeSpawnServiceSrv::Response(ServiceSuccess));
}

void ASpawner::FROSSpawnMeshServer::SetGameThreadDoneFlag(bool Flag)
{
	GameThreadDoneFlag = Flag;
}

void ASpawner::FROSSpawnMeshServer::SetServiceSuccess(bool Success)
{
	ServiceSuccess = Success;
}
