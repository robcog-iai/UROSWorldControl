// Fill out your copyright notice in the Description page of Project Settings.

#include "ROSWorldControlManager.h"
#include "SpawnModelsServer.h"
#include "SetModelPoseServer.h"
#include "RemoveModelServer.h"
#include "AttachModelToParentServer.h"
#include "ChangeVisualOfModelServer.h"
#include "SpawnPhysicsConstraintServer.h"
#include "HighlightModelsServer.h"


ROSWorldControlManager::ROSWorldControlManager(UWorld* InWorld, FString InServerAdress, int InServerPort,
                                               FString InNamespace)
{
	World = InWorld;
	ServerAdress = InServerAdress;
	ServerPort = InServerPort;
	Namespace = InNamespace;
}

void ROSWorldControlManager::ConnectToROSBridge(FWebsocketInfoCallBack CustomErrorCallbacks,
                                                FWebsocketInfoCallBack CustomConnectedCallbacks)
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the World."));
		return;
	}

	// Setup IDMap
	IdToActorMap = FTags::GetKeyValuesToActor(World, "SemLog", "Id");

	// Set websocket server address to ws 
	Handler = MakeShareable<FROSBridgeHandler>(
		new FROSBridgeHandler(ServerAdress, ServerPort, CustomErrorCallbacks, CustomConnectedCallbacks));

	// Add servers

	//Add spawn_model service
	TSharedPtr<FROSSpawnModelServer> SpawnModelServer =
		MakeShareable<FROSSpawnModelServer>(new FROSSpawnModelServer(Namespace, TEXT("spawn_model"), World, this));
	Handler->AddServiceServer(SpawnModelServer);

	//Add spawn_semantic_map service
	TSharedPtr<FROSSpawnMultipleModelsServer> SpawnMultipleModelsServer =
		MakeShareable<FROSSpawnMultipleModelsServer>(
			new FROSSpawnMultipleModelsServer(Namespace, TEXT("spawn_multiple_models"), World, this));
	Handler->AddServiceServer(SpawnMultipleModelsServer);

	// Add set_model_pose service 
	TSharedPtr<FROSSetModelPoseServer> SetModelPoseServer =
		MakeShareable<FROSSetModelPoseServer>(new FROSSetModelPoseServer(Namespace, TEXT("set_model_pose"), World, this));
	Handler->AddServiceServer(SetModelPoseServer);


	// Add delete_model service
	TSharedPtr<FROSRemoveModelServer> DeleteModelServer =
		MakeShareable<FROSRemoveModelServer>(new FROSRemoveModelServer(Namespace, TEXT("delete_model"), World, this));
	Handler->AddServiceServer(DeleteModelServer);

	// Add attach_model_to_parent service
	TSharedPtr<FROSAttachModelToParentServer> AttachModelToParentServer =
		MakeShareable<FROSAttachModelToParentServer>(
			new FROSAttachModelToParentServer(Namespace, TEXT("attach_model_to_parent"), World, this));
	Handler->AddServiceServer(AttachModelToParentServer);

	// Add change_visual_of_model service
	TSharedPtr<FROSChangeVisualOfModelServer> ChangeVisualOfModelServer =
		MakeShareable<FROSChangeVisualOfModelServer>(
			new FROSChangeVisualOfModelServer(Namespace, TEXT("change_visual_of_model"), World, this));
	Handler->AddServiceServer(ChangeVisualOfModelServer);

	// Add spawn_physics_constraint service
	TSharedPtr<FROSSpawnPhysicsConstraintServer> SpawnPhysicsConstraintServer =
		MakeShareable<FROSSpawnPhysicsConstraintServer>(
			new FROSSpawnPhysicsConstraintServer(Namespace, TEXT("spawn_physics_constraint"), World, this));
	Handler->AddServiceServer(SpawnPhysicsConstraintServer);

	// Add highlight_models service
	TSharedPtr<FROSHighlightModelsServer> HighlightModelsServer =
		MakeShareable<FROSHighlightModelsServer>(
			new FROSHighlightModelsServer(Namespace, TEXT("highlight_models"), World, this));
	Handler->AddServiceServer(HighlightModelsServer);


	// Connect to ROSBridge Websocket server.
	Handler->Connect();
	bServicesPulished = true;
}

void ROSWorldControlManager::DisconnectFromROSBridge()
{
	if (Handler.IsValid())
	{
		Handler->Disconnect();
	}
}

bool ROSWorldControlManager::IsConnected()
{
	return Handler->IsClientConnected();
}
