#include "AssetSpawner.h"
#include "AssetModifier.h"
#include "Tags.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/EngineTypes.h"
#include "Editor.h"

bool FAssetSpawner::SpawnAsset(UWorld* World, const FSpawnAssetParams Params, FString &FinalActorName, FString &ErrType)
{
	//Check if World is avialable
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: Couldn't find the World."), *FString(__FUNCTION__));
		return false;
	}

	FString Label = (Params.ActorLabel.IsEmpty() ? Params.Name : Params.ActorLabel);
#if WITH_EDITOR
	GEditor->BeginTransaction(FText::FromString(TEXT("Spawning: ")+ Label));
	World->Modify();
#endif

	FActorSpawnParameters SpawnParams;

	//Load Mesh and check if it succeded.
	UStaticMesh* Mesh = FAssetModifier::LoadMesh(Params.Name, Params.StartDir);
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: Could not find Mesh: %s."), *FString(__FUNCTION__), *Params.Name);
#if WITH_EDITOR
		GEditor->EndTransaction();
#endif
		return false;
	}

	AStaticMeshActor* SpawnedItem;

	//Check if Id is used already
	TArray<AActor*> Actors = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), Params.Id);

	if (!Actors.IsValidIndex(0))
	{
		// SpawnCollission Testing
		TArray<FOverlapResult> Results;
		bool bIsBlocked = World->OverlapMultiByChannel(Results, Params.Location, Params.Rotator.Quaternion(), ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeBox(Mesh->GetBoundingBox().GetExtent()));

		if (bIsBlocked)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]: Spawn Location is obstructed for \"%s\""), *FString(__FUNCTION__), *Params.Id);
			ErrType = "2";
#if WITH_EDITOR
			GEditor->EndTransaction();
#endif
			return false;
		}

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

		SpawnedItem->SetActorLabel(Label);

		FPhysicsProperties Properties = Params.PhysicsProperties;
		SpawnedItem->GetStaticMeshComponent()->SetSimulatePhysics(Properties.bSimulatePhysics);
		SpawnedItem->GetStaticMeshComponent()->SetGenerateOverlapEvents(Properties.bGenerateOverlapEvents);
		SpawnedItem->GetStaticMeshComponent()->SetEnableGravity(Properties.bGravity);
		SpawnedItem->GetStaticMeshComponent()->SetMassOverrideInKg(NAME_None, Properties.Mass);

		SpawnedItem->SetMobility(Properties.Mobility);

	}
	else
	{
		//ID is already taken
		UE_LOG(LogTemp, Error, TEXT("[%s]: Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *FString(__FUNCTION__), *Params.Id);
		ErrType = "1";
	
#if WITH_EDITOR
	GEditor->EndTransaction();
#endif
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
#if WITH_EDITOR
	SpawnedItem->Modify();
	GEditor->EndTransaction();
#endif
	FinalActorName = SpawnedItem->GetName();

	return true;
}


