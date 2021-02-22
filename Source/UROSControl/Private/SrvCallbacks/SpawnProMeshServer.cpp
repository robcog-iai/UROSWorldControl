// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnProMeshServer.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "CustomMeshComponent.h"
#include "Tags.h"
#include "Tag.h"
#include "Conversions.h"
#include "PhysicsProperties.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnProMeshServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
    TSharedPtr<FROSSpawnProMeshSrv::Request> Request =
        MakeShareable(new FROSSpawnProMeshSrv::Request());
    Request->FromJson(JsonObject);
    return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnProMeshServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
    TSharedPtr<FROSSpawnProMeshSrv::Request> SpawnProMeshRequest =
        StaticCastSharedPtr<FROSSpawnProMeshSrv::Request>(Request);

    UProceduralMeshComponent* ProMesh = NewObject<UProceduralMeshComponent>();
    FActorSpawnParameters SpawnParams;
    AActor* SpawnedActor;
    //Code what to do :D
    TArray<FVector> Vertices; //Points
    TArray<int32> Triangles;
    TArray<FVector>Normals;
    TArray<FVector2D>UVs;

    Vertices.Add(FVector(0,0,0));
    Vertices.Add(FVector(0,10,0));
    Vertices.Add(FVector(10,0,0));
    Vertices.Add(FVector(10,10,0));

    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);
    Triangles.Add(3);
    Triangles.Add(2);
    Triangles.Add(1);

    UVs.Add(FVector2D(0,0));
    UVs.Add(FVector2D(0,1));
    UVs.Add(FVector2D(1,0));
    UVs.Add(FVector2D(1,1));

    ProMesh->CreateMeshSection(1,Vertices,Triangles,Normals,UVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true); //create Mesh?


    //now Spawn it

    double start=FPlatformTime::Seconds();
    FGraphEventRef Task=FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
    {
        SpawnedActor=this->SpawnProMesh(SpawnProMeshRequest,ProMesh);
        if(SpawnedActor)
        {ServiceSuccess=true;}
        else
        {ServiceSuccess=false;}
    },TStatId(),nullptr,ENamedThreads::GameThread);

    //wait for code above to complete (Spawning in GameThread)
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
    double end= FPlatformTime::Seconds();
    UE_LOG(LogTemp, Display, TEXT("SpawnRobot executed in %f seconds."), end-start);
    UE_LOG(LogTemp, Display, TEXT("SpawnRobot has the Name %s"), *SpawnedActor->GetName());
    UE_LOG(LogTemp,Display,TEXT("Spawned Robot has the ID %s"),*FString::FromInt(SpawnedActor->GetUniqueID()));



    return MakeShareable<FROSBridgeSrv::SrvResponse>
        (new FROSSpawnProMeshSrv::Response(/*Params.Id, FinalActorName, ServiceSuccess*/));
}

AActor* FROSSpawnProMeshServer::SpawnProMesh(TSharedPtr<FROSSpawnProMeshSrv::Request> SpawnProMeshRequest, UProceduralMeshComponent* ProMesh)
{
    //Check if World is avialable
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s]: Couldn't find the World."), *FString(__FUNCTION__));
        return nullptr;
    }
    FVector SpawnLocation=FConversions::ROSToU(SpawnProMeshRequest->GetPose().GetPosition().GetVector());
    FRotator SpawnRotation= FRotator(FConversions::ROSToU(SpawnProMeshRequest->GetPose().GetOrientation().GetQuat()));
    FActorSpawnParameters SpawnParams;

    FString Label = SpawnProMeshRequest->GetActorLabel();

//    AStaticMeshActor* SpawnedItem;
    AActor* SpawnedActor;

    //Check if Id is used already
    TArray<AActor*> Actors = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), SpawnProMeshRequest->GetId());

    if (!Actors.IsValidIndex(0))
    {
        //Actual Spawning MeshComponent
        SpawnedActor = World->SpawnActor<AActor>(SpawnLocation, SpawnRotation, SpawnParams);


        // Needs to be movable if the game is running.
        ProMesh->SetMobility(EComponentMobility::Movable);
        //Assigning the Mesh and Material to the Component
        ProMesh->SetupAttachment(SpawnedActor->GetRootComponent());

        ProMesh->SetSimulatePhysics(SpawnProMeshRequest->GetPhysicsProperties().IsSimulatePhysics());
        ProMesh->SetGenerateOverlapEvents(SpawnProMeshRequest->GetPhysicsProperties().GetGenerateOverlapEvents());
        ProMesh->SetEnableGravity(SpawnProMeshRequest->GetPhysicsProperties().GetGravity());
        ProMesh->SetMassOverrideInKg(NAME_None,SpawnProMeshRequest->GetPhysicsProperties().GetMass());

        ProMesh->SetMobility(SpawnProMeshRequest->GetPhysicsProperties().GetMobility());
    }
    else
    {
        //ID is already taken
        UE_LOG(LogTemp, Error, TEXT("[%s]: Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *FString(__FUNCTION__), *SpawnProMeshRequest->GetId());
        return nullptr;
    }

    //Id tag to Actor
    FTags::AddKeyValuePair(
        SpawnedActor,
        TEXT("SemLog"),
        TEXT("id"),
        SpawnProMeshRequest->GetId());


    //Add other Tags to Actor
    for (auto Tag : SpawnProMeshRequest->GetTags())
    {
        FTags::AddKeyValuePair(
            SpawnedActor,
            Tag.GetType(),
            Tag.GetKey(),
            Tag.GetValue());
    }
    //SpawnedItem->Modify();

    FString FinalActorName = SpawnedActor->GetName();

    return SpawnedActor;

}
