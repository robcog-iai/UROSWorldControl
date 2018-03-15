// Fill out your copyright notice in the Description page of Project Settings.

#include "Remover.h"
#include "ROSBridge/srv/RemoveModel.h"

// Sets default values
ARemover::ARemover()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARemover::BeginPlay()
{
	Super::BeginPlay();
	}

// Called every frame
void ARemover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


TSharedPtr<FROSBridgeSrv::SrvRequest> ARemover::FROSRemoveModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeRemoveModelSrv::Request> Request_ =
		MakeShareable(new FROSBridgeRemoveModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> ARemover::FROSRemoveModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSBridgeRemoveModelSrv::Request> RemoveModelRequest =
		StaticCastSharedPtr<FROSBridgeRemoveModelSrv::Request>(Request);

	AActor* ActorToBeRemoved;
	
		// get and remove Actor with given UtagID of Controller IDMap
		if(Parent->Controller->IdToActorMap.RemoveAndCopyValue(RemoveModelRequest->GetUtagId(), ActorToBeRemoved)){
			// Actor was found and will be destroyed on GameThread
			GameThreadDoneFlag = false;
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				bool bSuccess = ActorToBeRemoved->Destroy();
				SetServiceSuccess(bSuccess);
				SetGameThreadDoneFlag(true);
			}
			);

			// Wait for gamethread to be done
			while (!GameThreadDoneFlag) {
				FPlatformProcess::Sleep(0.01);
			}

			return TSharedPtr<FROSBridgeSrv::SrvResponse>
				(new FROSBridgeRemoveModelSrv::Response(ServiceSuccess));
		} 
		else
		{
			// the given UtagID was not found.
			UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be removed."), *RemoveModelRequest->GetUtagId());
			return TSharedPtr<FROSBridgeSrv::SrvResponse>
				(new FROSBridgeRemoveModelSrv::Response(false));
		}

		
}


void  ARemover::FROSRemoveModelServer::SetGameThreadDoneFlag(bool Flag)
{
	GameThreadDoneFlag = Flag;
}

void ARemover::FROSRemoveModelServer::SetServiceSuccess(bool Success)
{
	ServiceSuccess = Success;
}