#include "AssetSpawner.h"
#include "AssetModifier.h"
#include "Tags.h"
#include "Engine/StaticMeshActor.h"

bool FAssetSpawner::SpawnAsset(UWorld* World, const FSpawnAssetParams Params)
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
	UStaticMesh* Mesh = FAssetModifier::LoadMesh(Params.Name, Params.StartDir);
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Mesh: %s."), *Params.Name);
		return false;
	}

	AStaticMeshActor* SpawnedItem;

	//Check if Id is used already
	TArray<AActor*> Actors = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), Params.Id);

	if (!Actors.IsValidIndex(0))
	{
		//Actual Spawning MeshComponent
		SpawnedItem = World->SpawnActor<AStaticMeshActor>(Params.Location, Params.Rotator, SpawnParams);

		// Needs to be movable if the game is running.
		SpawnedItem->SetMobility(EComponentMobility::Movable);
		//Assigning the Mesh and Material to the Component
		SpawnedItem->GetStaticMeshComponent()->SetStaticMesh(Mesh);



		if (Params.MaterialPaths.Num())
		{
			for (int i = 0; i < Params.MaterialPaths.Num(); i++)
			{
				UMaterialInterface* Material = FAssetModifier::LoadMaterial(Params.MaterialNames[i], Params.MaterialPaths[i]);
				if (Material)
				{
					SpawnedItem->GetStaticMeshComponent()->SetMaterial(i, Material);
				}
			}
		}

		Params.ActorLabel.IsEmpty() ? SpawnedItem->SetActorLabel(Params.Name + "_" + Params.Id) : SpawnedItem->SetActorLabel(Params.ActorLabel);

		FPhysicsProperties Properties = Params.PhysicsProperties;
		SpawnedItem->GetStaticMeshComponent()->SetSimulatePhysics(Properties.bSimulatePhysics);
		SpawnedItem->GetStaticMeshComponent()->SetGenerateOverlapEvents(Properties.bGenerateOverlapEvents);
		SpawnedItem->GetStaticMeshComponent()->SetEnableGravity(Properties.bGravity);
		SpawnedItem->GetStaticMeshComponent()->SetMassOverrideInKg(NAME_None, Properties.Mass);

		Properties.bSimulatePhysics ? SpawnedItem->SetMobility(EComponentMobility::Movable) : SpawnedItem->SetMobility(EComponentMobility::Static);
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
			Tag.TagType,
			Tag.Key,
			Tag.Value);
	}

	return true;
}


