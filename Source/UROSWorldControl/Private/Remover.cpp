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
	try
	{
		// get and remove Actor with given UtagID of Controller IDMap
		ActorToBeRemoved = Parent->Controller->IDMap.FindAndRemoveChecked(Request_->GetUtagId());
	}
	catch (const std::exception&)
	{
		// the given UtagID was not found.
		UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist."));
		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeRemoveModelSrv::Response(false));
	}
	
	//Destroy Actor
	// Execute on game thread
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		ActorToBeRemoved->Destroy();
	}
	);


	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeRemoveModelSrv::Response(true));
}
