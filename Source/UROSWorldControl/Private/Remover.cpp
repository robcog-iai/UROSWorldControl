// Fill out your copyright notice in the Description page of Project Settings.

#include "Remover.h"
#include "ROSBridge/srv/RemoveModelSrv.h"

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


	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(ServerAdress, ServerPort));

	// Add service clients and servers
	TSharedPtr<FROSRemoveModelServer> ServiceServer = MakeShareable<FROSRemoveModelServer>(new FROSRemoveModelServer(TEXT("ue/delete_model"), this));
	Handler->AddServiceServer(ServiceServer);

	// Connect to ROSBridge Websocket server.
	Handler->Connect();

}

// Called every frame
void ARemover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ARemover::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
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
	TSharedPtr<FROSBridgeRemoveModelSrv::Request> Request_ =
		StaticCastSharedPtr<FROSBridgeRemoveModelSrv::Request>(Request);

	AActor* ActorToBeRemoved;
	
		// get and remove Actor with given UtagID of Controller IDMap
		if(Parent->Controller->IDMap.RemoveAndCopyValue(Request_->GetUtagId(), ActorToBeRemoved)){
			// Actor was found and will be destroyed on GameThread
			GameThreadDoneFlag = false;
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				bool success = ActorToBeRemoved->Destroy();
				SetServiceSuccess(success);
				SetGameThreadDoneFlag(true);
			}
			);

			// Wait for gamethread to be done
			while (!GameThreadDoneFlag) {
				FPlatformProcess::Sleep(1);
			}

			return TSharedPtr<FROSBridgeSrv::SrvResponse>
				(new FROSBridgeRemoveModelSrv::Response(ServiceSuccess));
		} 
		else
		{
			// the given UtagID was not found.
			UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be removed."), *Request_->GetUtagId());
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