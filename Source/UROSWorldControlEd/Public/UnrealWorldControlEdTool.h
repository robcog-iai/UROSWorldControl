#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UnrealWorldControlEdMode.h"
#include "ROSWorldControlManager.h"
#include "UnrealWorldControlEdTool.generated.h"

class FUnrealWorldControlEdMode;

UCLASS()
class UUnrealWorldControlEdTool : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	FUnrealWorldControlEdMode* ParentMode;

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


	void SetParent(FUnrealWorldControlEdMode* NewParent)
	{
		ParentMode = NewParent;
	}

	void ConnectionErrorCallback();
	void ConnectedCallback();

private:
	UWorld * World;
	ROSWorldControlManager* Controller;

};