// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "RWCEdMode.h"
#include "RWCManager.h"
#include "RWCEdTool.generated.h"

class FRWCEdMode;

UCLASS()
class URWCEdTool : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	FRWCEdMode* ParentMode;

	UPROPERTY(VisibleInstanceOnly, Category = "ROSBridge Websocket")
	FString ConnectionStatus = TEXT("Not connected.");

	UPROPERTY(EditAnywhere, Category = "ROSBridge Websocket")
	FString ServerAdress = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, Category = "ROSBridge Websocket", meta=(DisplayName="ROS Bridge Connection"))
	int ServerPort = 9090;

	UPROPERTY(EditAnywhere, Category = "ROS")
	FString Namespace = TEXT("unreal");

	UFUNCTION(Exec)
	void Connect();

	UFUNCTION(Exec)
	void ClearMap();


	void SetParent(FRWCEdMode* NewParent)
	{
		ParentMode = NewParent;
	}

	void ConnectionErrorCallback();
	void ConnectedCallback();

private:
	UWorld* World;
	FRWCManager* Controller;
};
