#include "SpawnModelsServer.h"

bool FROSSpawnModelServer::SpawnAsset(const SpawnAssetParams Params) {
	//Check if World is avialable
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return false;
	}

	//Setup SpawnParameters 
	FActorSpawnParameters SpawnParams;
	//SpawnParams.Instigator = Instigator;
	//SpawnParams.Owner = this;


	//Load Mesh and check if it succeded.
	UStaticMesh* Mesh = LoadMesh(Params.PathOfMesh, Params.InstanceId);
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path does not point to a static mesh"));
		return false;
	}


	//Load Material and check if it succeded
	UMaterialInterface* Material = LoadMaterial(Params.PathOfMaterial, Params.InstanceId);
	if (!Material)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path does not point to a Material"));
		return false;
	}

	AStaticMeshActor* SpawnedItem;

	FString Name = Params.InstanceId->GetModelClassName();
	if (Params.InstanceId->GetId().IsEmpty())
	{
		//ID needs to be generated
		FString Id = GenerateId(4);
		Params.InstanceId->SetId(Id);
	}

	FString UniqueId = UROSWorldControlHelper::GetUniqueIdOfInstanceID(Params.InstanceId);

	if (Controller->IdToActorMap.Find(UniqueId) == nullptr)
	{
		//Actual Spawning MeshComponent
		SpawnedItem = World->SpawnActor<AStaticMeshActor>(Params.Location*100.f, Params.Rotator
			, SpawnParams);

		// Needs to be movable if the game is running.
		SpawnedItem->SetMobility(EComponentMobility::Movable);
		//Assigning the Mesh and Material to the Component
		SpawnedItem->GetStaticMeshComponent()->SetStaticMesh(Mesh);
		SpawnedItem->GetStaticMeshComponent()->SetMaterial(0, Material);

		
		if (Params.bIsStatic) {
			SpawnedItem->GetStaticMeshComponent()->SetSimulatePhysics(false);
			SpawnedItem->GetStaticMeshComponent()->bGenerateOverlapEvents = false;
			SpawnedItem->SetMobility(EComponentMobility::Static);
		} 
		else
		{
			SpawnedItem->GetStaticMeshComponent()->SetSimulatePhysics(true);
			SpawnedItem->GetStaticMeshComponent()->bGenerateOverlapEvents = true;
			SpawnedItem->SetMobility(EComponentMobility::Movable);
		}

		//Add this object to id refrence map
		Controller->IdToActorMap.Add(UniqueId, SpawnedItem);
	}
	else
	{
		//ID is already taken
		UE_LOG(LogTemp, Warning, TEXT("Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *UniqueId);
		return false;
	}

	//Id tag to Actor
	FTagStatics::AddKeyValuePair(
		SpawnedItem,
		TEXT("SemLog"),
		TEXT("id"),
		UniqueId);


	//Add other Tags to Actor
	for (auto Tag : Params.Tags)
	{
		FTagStatics::AddKeyValuePair(
			SpawnedItem,
			Tag.GetTagType(),
			Tag.GetKey(),
			Tag.GetValue());
	}

	return true;
}


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeSpawnServiceSrv::Request> Request_ =
		MakeShareable(new FROSBridgeSpawnServiceSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSBridgeSpawnServiceSrv::Request> SpawnMeshRequest =
		StaticCastSharedPtr<FROSBridgeSpawnServiceSrv::Request>(Request);

	SpawnAssetParams Params;
	Params.PathOfMesh = SpawnMeshRequest->GetModelDescription().GetMeshDescription().GetPathToMesh();
	Params.PathOfMaterial = SpawnMeshRequest->GetModelDescription().GetMeshDescription().GetPathToMaterial();
	Params.Location = SpawnMeshRequest->GetModelDescription().GetPose().GetPosition().GetVector();
	Params.Rotator = FRotator(SpawnMeshRequest->GetModelDescription().GetPose().GetOrientation().GetQuat());
	Params.Tags = SpawnMeshRequest->GetModelDescription().GetTags();
	unreal_msgs::InstanceId Id = SpawnMeshRequest->GetModelDescription().GetInstanceId();
	Params.bIsStatic = SpawnMeshRequest->GetModelDescription().IsStatic();
	Params.InstanceId = &Id;


	GameThreadDoneFlag = false;
	// Execute on game thread
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		bool success = SpawnAsset(Params);
		SetServiceSuccess(success);
		SetGameThreadDoneFlag(true);
	}
	);

	// Wait for gamethread to be done
	while (!GameThreadDoneFlag) {
		FPlatformProcess::Sleep(0.01);
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeSpawnServiceSrv::Response(ServiceSuccess, Id));
}


UStaticMesh * FROSSpawnModelServer::LoadMesh(const FString Path, unreal_msgs::InstanceId* InstanceId)
{

	UStaticMesh* Mesh = nullptr;
	if (Path.IsEmpty())
	{
		//Check default path for Class, since no Path was given
		//StaticMesh'/Game/Models/Sphere/SM_Sphere.SM_Sphere'
		FString ModelClassName = InstanceId->GetModelClassName();
		FString Ns = InstanceId->GetNs();
		FString DefaultPath;
		if (Ns.IsEmpty())
		{
			// without Namespace
			DefaultPath = TEXT("StaticMesh'/Game/Models/") + ModelClassName + TEXT("/SM_") + ModelClassName + TEXT(".SM_") + ModelClassName + TEXT("'");
		}
		else
		{
			// with Namespace
			DefaultPath = TEXT("StaticMesh'/Game/Models/") + FormatNamespace(Ns) + TEXT("/") + ModelClassName + TEXT("/SM_") + ModelClassName + TEXT(".SM_") + ModelClassName + TEXT("'");
		}

		Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *DefaultPath));
	}
	else {
		//Load Mesh From Path
		Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *Path));
	}
	return Mesh;
}

UMaterialInterface * FROSSpawnModelServer::LoadMaterial(const FString Path, unreal_msgs::InstanceId* InstanceId)
{

	UMaterialInterface* Material = nullptr;
	if (Path.IsEmpty())
	{
		//Check default path for Class, since no Path was given
		FString ModelClassName = InstanceId->GetModelClassName();
		FString Ns = InstanceId->GetNs();
		FString DefaultPath;
		if (Ns.IsEmpty())
		{
			// without Namespace
			DefaultPath = TEXT("Material'/Game/Models/") + ModelClassName + TEXT("/M_") + ModelClassName + TEXT(".M_") + ModelClassName + TEXT("'");
		}
		else
		{

			// with Namespace
			DefaultPath = TEXT("Material'/Game/Models/") + FormatNamespace(Ns) + TEXT("/") + ModelClassName + TEXT("/M_") + ModelClassName + TEXT(".M_") + ModelClassName + TEXT("'");
		}

		Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *DefaultPath));
	}
	else {
		Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Path));
	}

	return Material;
}

FString FROSSpawnModelServer::FormatNamespace(FString Ns)
{
	FString NewNs;
	NewNs = Ns.TrimStartAndEnd();
	NewNs.RemoveFromStart(TEXT("/"));
	NewNs.RemoveFromEnd(TEXT("/"));
	return NewNs;
}

FString FORCEINLINE FROSSpawnModelServer::GenerateId(int Length) {
	auto RandChar = []() -> char
	{
		const char CharSet[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t MaxIndex = (sizeof(CharSet) - 1);
		return CharSet[rand() % MaxIndex];
	};
	std::string RandString(Length, 0);
	std::generate_n(RandString.begin(), Length, RandChar);
	// Return as Fstring
	return FString(RandString.c_str());
}


void FROSSpawnModelServer::SetGameThreadDoneFlag(bool Flag)
{
	GameThreadDoneFlag = Flag;
}

void FROSSpawnModelServer::SetServiceSuccess(bool bSuccess)
{
	ServiceSuccess = bSuccess;
}



TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnMultipleModelsServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeSpawnMultipleModelsSrv::Request> Request_ =
		MakeShareable(new FROSBridgeSpawnMultipleModelsSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnMultipleModelsServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSBridgeSpawnMultipleModelsSrv::Request> SpawnSemanticMapRequest =
		StaticCastSharedPtr<FROSBridgeSpawnMultipleModelsSrv::Request>(Request);

	TArray<unreal_msgs::ModelDescription>* Descriptions = SpawnSemanticMapRequest->GetModelDescriptions();

	TArray<bool> SuccessList;
	TArray<unreal_msgs::InstanceId> InstanceIds;

	bool bAllSucceded = true;
	for (auto Descript : *Descriptions)
	{
		SpawnAssetParams Params;
		Params.PathOfMesh = Descript.GetMeshDescription().GetPathToMesh();
		Params.PathOfMaterial = Descript.GetMeshDescription().GetPathToMaterial();
		Params.Location = Descript.GetPose().GetPosition().GetVector();
		Params.Rotator = FRotator(Descript.GetPose().GetOrientation().GetQuat());
		Params.Tags = Descript.GetTags();
		Params.bIsStatic = Descript.IsStatic();
		unreal_msgs::InstanceId Id = Descript.GetInstanceId();
		Params.InstanceId = &Id;

		GameThreadDoneFlag = false;
		// Execute on game thread
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			bool success = SpawnAsset(Params);
			SetServiceSuccess(success);
			SetGameThreadDoneFlag(true);
		}
		);

		// Wait for gamethread to be done
		while (!GameThreadDoneFlag) {
			FPlatformProcess::Sleep(0.01);
		}
		InstanceIds.Add(Id);
		SuccessList.Add(ServiceSuccess);
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeSpawnMultipleModelsSrv::Response(SuccessList, InstanceIds));



}
