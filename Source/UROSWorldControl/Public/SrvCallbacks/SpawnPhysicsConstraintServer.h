#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "ROSWorldControlManager.h"
#include "SpawnPhysicsConstraint.h"
#include "Runtime/Engine/Classes/PhysicsEngine/ConstraintInstance.h"


class FROSSpawnPhysicsConstraintServer final : public FROSBridgeSrvServer
{
	struct MoveAssetParams
	{
		AActor* Actor;
		FVector Location;
		FRotator Rotator;
	};

private:
	FROSSpawnPhysicsConstraintServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	ROSWorldControlManager* Controller;

	bool SpawnPhysicsConstraintActor(TSharedPtr<FROSBridgeSpawnPhysiscConstraintSrv::Request> Request);
	void SetupProfileInstance(FConstraintProfileProperties* ProfileInstance, PhysicsConstraintDetails Details);
	void SetupAngularLimits(FConstraintInstance Instance, PhysicsConstraintDetails Details);
	void SetupLinearLimits(FConstraintInstance Instance, PhysicsConstraintDetails Details);


public:
	FROSSpawnPhysicsConstraintServer(FString Namespace, FString Name, UWorld* InWorld,
	                                 ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_world_control_msgs/SpawnPhysicsConstraint"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
