#include "SpawnModelsServer.h"
#include "Engine/StaticMeshActor.h"
#include "FileManagerGeneric.h"
#include "Ids.h"
#include "Conversions.h"

bool FROSSpawnModelServer::SpawnAsset(const SpawnAssetParams Params)
{
	//Check if World is avialable
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return false;
	}

	//Setup SpawnParameters 
	FActorSpawnParameters SpawnParams;
	//SpawnParams.Instigator = Instigator;
	//SpawnParams.Owner = this;


	//Load Mesh and check if it succeded.
	UStaticMesh* Mesh = LoadMesh(Params.Name, Params.StartDir);
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Mesh: %s."), *Params.Name);
		return false;
	}

	AStaticMeshActor* SpawnedItem;

	if (Controller->IdToActorMap.Find(Params.Id) == nullptr)
	{
		//Actual Spawning MeshComponent
		SpawnedItem = World->SpawnActor<AStaticMeshActor>(Params.Location, Params.Rotator, SpawnParams);

		// Needs to be movable if the game is running.
		SpawnedItem->SetMobility(EComponentMobility::Movable);
		//Assigning the Mesh and Material to the Component
		SpawnedItem->GetStaticMeshComponent()->SetStaticMesh(Mesh);
		
		
		
		if (Params.MaterialPaths.Num())
		{
			for(int i = 0; i < Params.MaterialPaths.Num(); i++)
			{
				UMaterialInterface* Material = LoadMaterial(Params.MaterialNames[i], Params.MaterialPaths[i]);
				if(Material)
				{
					SpawnedItem->GetStaticMeshComponent()->SetMaterial(i, Material);
				}
			}
		}

		Params.ActorLabel.IsEmpty() ? SpawnedItem->SetActorLabel(Params.Name + "_"+ Params.Id) : SpawnedItem->SetActorLabel(Params.ActorLabel);

		world_control_msgs::PhysicsProperties Properties = Params.PhysicsProperties;
		SpawnedItem->GetStaticMeshComponent()->SetSimulatePhysics(Properties.GetSimulatePhysics());
		SpawnedItem->GetStaticMeshComponent()->bGenerateOverlapEvents = Properties.GetGenerateOverlapEvents();
		SpawnedItem->GetStaticMeshComponent()->SetEnableGravity(Properties.GetGravity());
		SpawnedItem->GetStaticMeshComponent()->SetMassOverrideInKg(NAME_None, Properties.GetMass());
		
		Properties.GetSimulatePhysics() ? SpawnedItem->SetMobility(EComponentMobility::Movable) : SpawnedItem->SetMobility(EComponentMobility::Static);
		
		//Add this object to id refrence map
		Controller->IdToActorMap.Add(Params.Id, SpawnedItem);
	}
	else
	{
		//ID is already taken
		UE_LOG(LogTemp, Error, TEXT("Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *Params.Id);
		return false;
	}

	//Id tag to Actor
	FTags::AddKeyValuePair(
		SpawnedItem,
		TEXT("SemLog"),
		TEXT("id"),
		Params.Id);


	//Add other Tags to Actor
	for (auto Tag : Params.Tags)
	{
		FTags::AddKeyValuePair(
			SpawnedItem,
			Tag.GetTagType(),
			Tag.GetKey(),
			Tag.GetValue());
	}

	return true;
}


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSpawnModelSrv::Request> Request =
		MakeShareable(new FROSSpawnModelSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSpawnModelSrv::Request> SpawnMeshRequest =
		StaticCastSharedPtr<FROSSpawnModelSrv::Request>(Request);

	SpawnAssetParams Params;
	Params.Id = SpawnMeshRequest->GetId().IsEmpty() ? FIds::NewGuidInBase64() : SpawnMeshRequest->GetId();
	Params.Name = SpawnMeshRequest->GetName();
	Params.Location = FConversions::ROSToU(SpawnMeshRequest->GetPose().GetPosition().GetVector());
	Params.Rotator = FRotator(FConversions::ROSToU(SpawnMeshRequest->GetPose().GetOrientation().GetQuat()));
	Params.Tags = SpawnMeshRequest->GetTags();
	Params.PhysicsProperties = SpawnMeshRequest->GetPhysicsProperties();
	Params.StartDir = SpawnMeshRequest->GetPath();
	Params.ActorLabel = SpawnMeshRequest->GetActorLabel();
	Params.MaterialNames = SpawnMeshRequest->GetMaterialNames();
	Params.MaterialPaths = SpawnMeshRequest->GetMaterialPaths();
	Params.ParentId = SpawnMeshRequest->GetParentId();


	// Execute on game thread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = SpawnAsset(Params);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSpawnModelSrv::Response(Params.Id, ServiceSuccess));
}


UStaticMesh* FROSSpawnModelServer::LoadMesh(FString Name, FString StartDir)
{
	UStaticMesh* Mesh = nullptr;
	//Look for file Recursively

	FString Filename = Name.StartsWith(TEXT("SM_")) ? TEXT("") : TEXT("SM_");
	Filename += Name;
	Filename += Name.EndsWith(TEXT(".uasset")) ? TEXT("") : TEXT(".uasset");
	
	TArray<FString> FileLocations;
	FFileManagerGeneric Fm;
	Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir().Append(StartDir), *Filename, true, false, true);
	
	if(FileLocations.Num() == 0)
	{
		//Try again with whole ContentDir
		Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir(), *Filename, true, false, true);
	}

	for (auto Loc : FileLocations)
	{
		//Try all found files until one works.
		if (Mesh == nullptr)
		{
			Loc.RemoveFromStart(FPaths::ProjectContentDir());
			int Last;
			Loc.FindLastChar('.', Last);
			Loc.RemoveAt(Last, Loc.Len() - Last);
			
			FString FoundPath = "StaticMesh'/Game/" + Loc + ".SM_" + Name + "'";
			Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *FoundPath));
		}
	}

	return Mesh;
}

UMaterialInterface* FROSSpawnModelServer::LoadMaterial(FString Name, FString StartDir)
{
	UMaterialInterface* Material = nullptr;
	FString Filename;
	if (Name.StartsWith(TEXT("M_")))
		Filename = Name + TEXT(".uasset");
	else
		Filename = TEXT("M_") + Name + TEXT(".uasset");

	TArray<FString> FileLocations;
	FFileManagerGeneric Fm;
	Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir().Append(StartDir), *Filename, true, false, true);

	if (FileLocations.Num() == 0)
	{
		//Try again with whole ContentDir
		Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir(), *Filename, true, false, true);
	}

	for (auto Loc : FileLocations)
	{
		//Try all found files until one works.
		if (Material == nullptr)
		{
			Loc.RemoveFromStart(FPaths::ProjectContentDir());
			int Last;
			Loc.FindLastChar('.', Last);
			Loc.RemoveAt(Last, Loc.Len() - Last);

			FString FoundPath = "StaticMesh'/Game" + Loc + ".M_" + Name + "'";
			Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *FoundPath));
		}
	}
	
	return Material;
	
}