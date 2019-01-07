#include "SpawnModelsServer.h"
#include "Engine/StaticMeshActor.h"
#include "FileManagerGeneric.h"
#include "Ids.h"
#include "Conversions.h"
#include "AssetSpawner.h"

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


	// Move data to spawner Struct
	FAssetSpawner::FSpawnAssetParams Params;
	Params.Id = SpawnMeshRequest->GetId().IsEmpty() ? FIds::NewGuidInBase64() : SpawnMeshRequest->GetId();
	Params.Name = SpawnMeshRequest->GetName();
	Params.Location = FConversions::ROSToU(SpawnMeshRequest->GetPose().GetPosition().GetVector());
	Params.Rotator = FRotator(FConversions::ROSToU(SpawnMeshRequest->GetPose().GetOrientation().GetQuat()));
	for (auto Tag : SpawnMeshRequest->GetTags())
	{
		FAssetSpawner::FTag SpawnerTag;
		SpawnerTag.TagType = Tag.GetTagType();
		SpawnerTag.Key = Tag.GetKey();
		SpawnerTag.Value = Tag.GetValue();
		Params.Tags.Add(SpawnerTag);
	}
	Params.PhysicsProperties.bSimulatePhysics = SpawnMeshRequest->GetPhysicsProperties().GetSimulatePhysics();
	Params.PhysicsProperties.bGravity = SpawnMeshRequest->GetPhysicsProperties().GetGravity();
	Params.PhysicsProperties.bGenerateOverlapEvents = SpawnMeshRequest->GetPhysicsProperties().GetGenerateOverlapEvents();
	Params.PhysicsProperties.Mass = SpawnMeshRequest->GetPhysicsProperties().GetMass();
	Params.StartDir = SpawnMeshRequest->GetPath();
	Params.ActorLabel = SpawnMeshRequest->GetActorLabel();
	Params.MaterialNames = SpawnMeshRequest->GetMaterialNames();
	Params.MaterialPaths = SpawnMeshRequest->GetMaterialPaths();
	Params.ParentId = SpawnMeshRequest->GetParentId();


	// Execute on game thread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = FAssetSpawner::SpawnAsset(World, Params);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSpawnModelSrv::Response(Params.Id, ServiceSuccess));
}

