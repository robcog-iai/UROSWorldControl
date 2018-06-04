#include "SetMaterialServer.h"
#include "FileManagerGeneric.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSetMaterialServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSetMaterialSrv::Request> Request =
		MakeShareable(new FROSSetMaterialSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSetMaterialServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSetMaterialSrv::Request> ChangeVisualRequest =
		StaticCastSharedPtr<FROSSetMaterialSrv::Request>(Request);

	
	if (ChangeVisualRequest->GetMaterialNames().Num() == 0 &&
		ChangeVisualRequest->GetMaterialPaths().Num() == 0)
	{
		// nothing to do here.
		UE_LOG(LogTemp, Error, TEXT("Nothing to do. %s"), *ChangeVisualRequest->ToString());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetMaterialSrv::Response(false));
	}

	// get Actor with given UtagID of Controller IDMap

	AActor* ActorToBeChanged = *Controller->IdToActorMap.Find(ChangeVisualRequest->GetId());

	TArray<UStaticMeshComponent*> Components;
	ActorToBeChanged->GetComponents<UStaticMeshComponent>(Components);

	if (Components.Num() != 1)
	{
		UE_LOG(LogTemp, Error,
			TEXT(
				"Actor %s has to more then one StaticMeshComponent, since it's not clear which one should be changed nothing was done."
			),
			*ActorToBeChanged->GetName());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetMaterialSrv::Response(false));
	}


	// Execute on game thread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = ChangeVisual(Components.Pop(), ChangeVisualRequest->GetMaterialNames(), ChangeVisualRequest->GetMaterialPaths());
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSetMaterialSrv::Response(ServiceSuccess));
}

bool FROSSetMaterialServer::ChangeVisual(UStaticMeshComponent* MeshComponent, TArray<FString> MaterialNames, TArray<FString> MaterialPaths)
{
	if (MaterialPaths.Num() > 0)
	{
		for(int i = 0; i < MaterialPaths.Num(); i++)
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
			// look recursively
			TArray<FString> FileLocations;
			FFileManagerGeneric Fm;
			Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir(), *MaterialNames[i], true, false, true);

			UMaterialInterface* Material = nullptr;

			for (auto Loc : FileLocations)
			{
				//Try all found files until one works.
				if (Material == nullptr)
				{
					int First = Loc.Find("/Models");
					Loc.RemoveAt(0, First);
					int Last;
					Loc.FindLastChar('.', Last);
					Loc.RemoveAt(Last, Loc.Len() - Last);

					FString FoundPath = "StaticMesh'/Game" + Loc + ".M_" + MaterialNames[i] + "'";
					Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *FoundPath));
					if (Material)
					{
						MeshComponent->SetMaterial(i, Material);
					}
				}
			}
		}
	}

	return true;
}