#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "Engine/StaticMeshActor.h"
#include "UTags.h"
#include "TagStatics.h"
#include "ROSWorldControlManager.h"
#include "ROSBridge/srv/RemoveModel.h"
#include "HelperFunctions.h"

class FROSRemoveModelServer final : public FROSBridgeSrvServer
{
private:
	FROSRemoveModelServer();
	UWorld* World;
	bool GameThreadDoneFlag;
	bool ServiceSuccess;

	ROSWorldControlManager* Controller;

	void SetGameThreadDoneFlag(bool Flag);

	void SetServiceSuccess(bool Success);

public:
	FROSRemoveModelServer(FString Namespace, FString Name, UWorld* InWorld,
		ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_msgs/DeleteModel"))
	{
		World = InWorld;
		Controller = InController; 
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

};