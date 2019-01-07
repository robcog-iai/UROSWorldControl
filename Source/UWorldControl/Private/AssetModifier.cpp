#include "AssetModifier.h"
#include "FileManagerGeneric.h"
#include "Tags.h"


bool FAssetModifier::RemoveAsset(UWorld* World, FString Id)
{

	AActor* ActorToBeRemoved = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), Id).Pop();
	if (ActorToBeRemoved)
		return ActorToBeRemoved->Destroy();

	UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be removed."), *Id);
	return false;
}

bool FAssetModifier::Relocate(AActor* Actor, FVector Location, FRotator Rotator)
{
	if (!Actor->TeleportTo(Location, Rotator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not set %s to locaiton: %s, with Rotation: %s"),
			*Actor->GetName(), *Location.ToString(), *Rotator.ToString());
		return false;
	}

	return true;
}

bool FAssetModifier::ChangePhysics(UStaticMeshComponent* MeshComponent, bool bSimulatedPhysics,
	bool bGereateOverlapEvents, bool bGravity, float Mass)
{
	if (!MeshComponent) return false;

	MeshComponent->SetSimulatePhysics(bSimulatedPhysics);
	MeshComponent->SetGenerateOverlapEvents(bGereateOverlapEvents);
	MeshComponent->SetEnableGravity(bGravity);
	MeshComponent->SetMassOverrideInKg(NAME_None, Mass);
	return true;
}

bool FAssetModifier::ChangeVisual(UStaticMeshComponent* MeshComponent, TArray<FString> MaterialNames, TArray<FString> MaterialPaths)
{
	if (MaterialPaths.Num() > 0)
	{
		for (int i = 0; i < MaterialPaths.Num(); i++)
		{
			//Try to load Material 
			UMaterialInterface* Material = Cast<UMaterialInterface>(
				StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MaterialPaths[i]));
			if (Material)
			{
				MeshComponent->SetMaterial(i, Material);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Path %s does not point to a Material"), *MaterialPaths[i]);
				return false;
			}
		}

	}
	else
	{
		for (int i = 0; i < MaterialNames.Num(); i++)
		{
			UMaterialInterface* Material = nullptr;
			Material = LoadMaterial(*MaterialNames[i], TEXT(""));

			if (!Material)
			{
				return false;
			}

			MeshComponent->SetMaterial(i, Material);
		}
	}

	return true;
}



UStaticMesh* FAssetModifier::LoadMesh(FString Name, FString StartDir)
{
	UStaticMesh* Mesh = nullptr;
	//Look for file Recursively

	FString Filename = Name.StartsWith(TEXT("SM_")) ? TEXT("") : TEXT("SM_");
	Filename += Name;
	Filename += Name.EndsWith(TEXT(".uasset")) ? TEXT("") : TEXT(".uasset");

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

UMaterialInterface* FAssetModifier::LoadMaterial(FString Name, FString StartDir)
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


bool FAssetModifier::AttachModelToParent(AActor* Parent, AActor* Child)
{
	if (Parent && Child)
	{
		Child->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
		return true;
	}
	return false;
}

