#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "UROSWorldControlManager.h"
#include "DeleteModel.h"

class FROSRemoveModelServer final : public FROSBridgeSrvServer
{
private:
	FROSRemoveModelServer();
	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	UROSWorldControlManager* Controller;

public:
	FROSRemoveModelServer(FString Namespace, FString Name, UWorld* InWorld,
	                      UROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/DeleteModel"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
