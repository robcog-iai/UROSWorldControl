// Fill out your copyright notice in the Description page of Project Settings.

#include "ROSWorldControlManager.h"
#include "SpawnModelsServer.h"
#include "SetModelPoseServer.h"
#include "RemoveModelServer.h"

ROSWorldControlManager::ROSWorldControlManager(UWorld * InWorld, FString InServerAdress, int InServerPort, FString InNamespace)
{
	World = InWorld;
	ServerAdress = InServerAdress;
	ServerPort = InServerPort;
	Namespace = InNamespace;

}

void ROSWorldControlManager::ConnectToROSBridge()
{
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return;
	}

	// Setup IDMap
	IdToActorMap = FTagStatics::GetKeyValuesToActor(World, "SemLog", "Id");

	
	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(ServerAdress, ServerPort));

	// Add servers

	//Add spawn_model service
	TSharedPtr<FROSSpawnModelServer> SpawnServer = 
		MakeShareable<FROSSpawnModelServer>(new FROSSpawnModelServer(Namespace, TEXT("spawn_model"), World, this));
	Handler->AddServiceServer(SpawnServer);

	//Add spawn_semantic_map service
	TSharedPtr<FROSSpawnMultipleModelsServer> SpawnSemanticMapServer = 
		MakeShareable<FROSSpawnMultipleModelsServer>(new FROSSpawnMultipleModelsServer(Namespace, TEXT("spawn_multiple_models"), World, this));
	Handler->AddServiceServer(SpawnSemanticMapServer);

	// Add set_model_pose service 
	TSharedPtr<FROSSetModelPoseServer> RelocateServer = 
		MakeShareable<FROSSetModelPoseServer>(new FROSSetModelPoseServer(Namespace, TEXT("set_model_pose"), World, this));
	Handler->AddServiceServer(RelocateServer);


	// Add delete_model service
	TSharedPtr<FROSRemoveModelServer> RemoveServer = 
		MakeShareable<FROSRemoveModelServer>(new FROSRemoveModelServer(Namespace, TEXT("delete_model"), World, this));
	Handler->AddServiceServer(RemoveServer);


	// Connect to ROSBridge Websocket server.
	Handler->Connect();
	bServicesPulished = true;
}

void ROSWorldControlManager::DisconnectFromROSBridge()
{
	if (Handler.IsValid()) {
		Handler->Disconnect();
	}
}

bool ROSWorldControlManager::isConnected()
{
	return Handler->IsClientConnected();
}
