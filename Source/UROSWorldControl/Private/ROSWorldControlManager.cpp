// Fill out your copyright notice in the Description page of Project Settings.

#include "ROSWorldControlManager.h"
#include "SpawnModelsServer.h"
#include "SetModelPoseServer.h"
#include "RemoveModelServer.h"
#include "AttachModelToParentServer.h"
#include "SpawnPhysicsConstraintServer.h"
#include "HighlightModelsServer.h"
#include "SetPhysicsPropertiesServer.h"
#include "SetMaterialServer.h"


ROSWorldControlManager::ROSWorldControlManager(UWorld* InWorld, FString InServerAdress, int InServerPort,
                                               FString InNamespace)
{
	World = InWorld;
	ServerAdress = InServerAdress;
	ServerPort = InServerPort;
	Namespace = InNamespace;
}

void ROSWorldControlManager::ConnectToROSBridge(FROSWebsocketInfoSignature CustomErrorCallbacks,
	FROSWebsocketInfoSignature CustomConnectedCallbacks)
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


	// Add set_physics_properties service
	TSharedPtr<FROSSetPhysicsPropertiesServer> SetPhysicsPropertiesServer =
		MakeShareable<FROSSetPhysicsPropertiesServer>(
			new FROSSetPhysicsPropertiesServer(Namespace, TEXT("set_physics_properties"), World, this));
	Handler->AddServiceServer(SetPhysicsPropertiesServer);

	// Add set_material service
	TSharedPtr<FROSSetMaterialServer> SetMaterialServer =
		MakeShareable<FROSSetMaterialServer>(
			new FROSSetMaterialServer(Namespace, TEXT("change_material"), World, this));
	Handler->AddServiceServer(SetMaterialServer);


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
	return Handler->IsConnected();
}
