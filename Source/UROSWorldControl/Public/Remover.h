// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "Engine/StaticMeshActor.h"
#include "UTags.h"
#include "TagStatics.h"
#include "ROSWorldControlManager.h"
#include "Remover.generated.h"


UCLASS()
class UROSWORLDCONTROL_API ARemover : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARemover();
	AROSWorldControlManager* Controller;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
		FString ServerAdress = TEXT("192.168.1.19");

	UPROPERTY(EditAnywhere, Category = "RosBridge Websocket")
		int ServerPort = 9090;

	virtual void EndPlay(const EEndPlayReason::Type Reason);

	TSharedPtr<FROSBridgeHandler> Handler;

private:

	
	class FROSRemoveModelServer final : public FROSBridgeSrvServer
	{
	private:
		ARemover * Parent;
		bool GameThreadDoneFlag;
		bool ServiceSuccess;

	public:
		FROSRemoveModelServer(FString Name, ARemover* Parent_) :
			FROSBridgeSrvServer(Name, TEXT("ue/delete_model"))
		{
			Parent = Parent_;
		}

		TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

		TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

		void SetGameThreadDoneFlag(bool Flag);

		void SetServiceSuccess(bool success);
	};

};
