#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "UROSWorldControlManager.h"
#include "SpawnPhysicsConstraint.h"
#include "PhysicsEngine/ConstraintInstance.h"


class FROSSpawnPhysicsConstraintServer final : public FROSBridgeSrvServer
{
	
private:
	FROSSpawnPhysicsConstraintServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	UROSWorldControlManager* Controller;

	bool SpawnPhysicsConstraintActor(TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> Request);
	void SetupProfileInstance(FConstraintProfileProperties& ProfileInstance, world_control_msgs::PhysicsConstraintDetails Details);
	void SetupAngularLimits(FConstraintInstance& Instance, world_control_msgs::PhysicsConstraintDetails Details);
	void SetupLinearLimits(FConstraintInstance& Instance, world_control_msgs::PhysicsConstraintDetails Details);


public:
	FROSSpawnPhysicsConstraintServer(FString Namespace, FString Name, UWorld* InWorld,
	                                 UROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnPhysicsConstraint"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
