#pragma once
#include "CoreMinimal.h"
#include "ROSWorldControlManager.h"
#include "ChangeVisualOfModel.h"
#include "HelperFunctions.h"

class FROSChangeVisualOfModelServer final : public FROSBridgeSrvServer
{
private:
	FROSChangeVisualOfModelServer();
	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	ROSWorldControlManager* Controller;

	bool ChangeVisual(unreal_world_control_msgs::MeshDescription MeshDescription, UStaticMeshComponent* MeshComponent);

	void SetServiceSuccess(bool Success);

public:
	FROSChangeVisualOfModelServer(FString Namespace, FString Name, UWorld* InWorld,
	                              ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_world_control_msgs/ChangeVisualOfModel")),
		World(InWorld),
		Controller(InController)
	{
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
