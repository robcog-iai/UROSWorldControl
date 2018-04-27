#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "RosConnectionMode.h"
#include "ROSWorldControlManager.h"
#include "RosConnectionTool.generated.h"

class FRosConnectionMode;

UCLASS()
class URosConnectionTool : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	FRosConnectionMode* ParentMode;

	UPROPERTY(VisibleInstanceOnly, Category = "RosBridge Websocket")
		FString ConnectionStatus = TEXT("Not connected.");
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
		FString ServerAdress = TEXT("127.0.0.1");
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
		int ServerPort = 9090;


	UPROPERTY(EditAnywhere, Category = "ROS")
		FString Namespace = TEXT("unreal");
	
	UFUNCTION(Exec)
	void ConnectToRosBridge();

	UFUNCTION(Exec)
	void ClearMap();

	UFUNCTION(Exec)
	void SetRelations();

	void SetParent(FRosConnectionMode* NewParent)
	{
		ParentMode = NewParent;
	}

	void ConnectionErrorCallback();
	void ConnectedCallback();

private:
	UWorld * World;
	ROSWorldControlManager* Controller;

};