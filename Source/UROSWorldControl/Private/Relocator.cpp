// Fill out your copyright notice in the Description page of Project Settings.

#include "Relocator.h"
#include "ROSBridge/srv/RelocateModelSrv.h"

// Sets default values
ARelocator::ARelocator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARelocator::BeginPlay()
{
	Super::BeginPlay();

	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(ServerAdress, ServerPort));

	// Add service clients and servers
	TSharedPtr<FROSRelocationServer> ServiceServer = MakeShareable<FROSRelocationServer>(new FROSRelocationServer(NameSpace, TEXT("relocate_model"), this));
	Handler->AddServiceServer(ServiceServer);


	// Connect to ROSBridge Websocket server.
	Handler->Connect();

}

// Called every frame
void ARelocator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ARelocator::EndPlay(const EEndPlayReason::Type Reason)
{
	Handler->Disconnect();
	// Disconnect the handler before parent ends

	Super::EndPlay(Reason);
}


bool ARelocator::Relocate(AActor* Actor, FVector Location, FRotator Rotator)
{

	if (Actor->TeleportTo(Location, Rotator) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not set %s to locaiton: %s, with Rotation: %s"),
			*Actor->GetName(), *Location.ToString(), *Rotator.ToString());
		return false;
	}

	return true;
}


TSharedPtr<FROSBridgeSrv::SrvRequest> ARelocator::FROSRelocationServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeRelocateModelSrv::Request> Request_ =
		MakeShareable(new FROSBridgeRelocateModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> ARelocator::FROSRelocationServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSBridgeRelocateModelSrv::Request> Request_ =
		StaticCastSharedPtr<FROSBridgeRelocateModelSrv::Request>(Request);

	//Get Actor for given ID
	AActor** Actor = Parent->Controller->IDMap.Find(Request_->GetUtagId());


	if (!Actor) {
		// Couldn't find Actor for ID 
		UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be moved."), *Request_->GetUtagId());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSBridgeRelocateModelSrv::Response(false));
	}


	// Setup params
	MoveAssetParams Params;
	Params.Actor = *Actor;
	Params.Location = Request_->GetLocation();
	Params.Rotator = Request_->GetRotator();

	GameThreadDoneFlag = false;
	//Actor was found and will be relocated, in GameThread
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		bool success = Parent->Relocate(Params.Actor,
			Params.Location,
			Params.Rotator);
		SetServiceSuccess(success);
		SetGameThreadDoneFlag(true);
	}
	);

	// Wait for gamethread to be done
	while (!GameThreadDoneFlag) {
		FPlatformProcess::Sleep(1);
	}
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeRelocateModelSrv::Response(ServiceSuccess));
}

void  ARelocator::FROSRelocationServer::SetGameThreadDoneFlag(bool Flag)
{
	GameThreadDoneFlag = Flag;
}

void ARelocator::FROSRelocationServer::SetServiceSuccess(bool Success)
{
	ServiceSuccess = Success;
}