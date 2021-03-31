// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundingBox.h"


// Sets default values
ABoundingBox::ABoundingBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    ProMesh=CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProMesh"));

    ProMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);


//    //Testing Purpose
//    TArray<FVector> Vertices; //Points
//    TArray<int32> Triangles;
//    TArray<FVector>Normals;
//    TArray<FVector2D>UVs;
//    int depth=5;
//    int width=20;
//    int height=10;
//    int d=-depth/2;
//    int w=-width/2;
//    int h=-height/2;

//    //
//    //From Playerstart Camera Postion
//    Vertices.Add(FVector(d,w,h)); //FrontBottomRight 0                                          //From Rotation (0|0|0) FrontButtomLeft
//    Vertices.Add(FVector(d,w+width,h)); //BackBottomRight 1                                       //From Rotation (0|0|0) FrontButtomRight
//    Vertices.Add(FVector(d+depth,w,h)); //FrontBottomLeft 2                                       //From Rotation (0|0|0) BackButtomLeft
//    Vertices.Add(FVector(d+depth,w+width,h)); //BackBottomLeft 3                                    //From Rotation (0|0|0) BackButtonRight

//    Vertices.Add(FVector(d,w,h+height)); //FrontTopRight 4                                        //From Rotation (0|0|0) FrontTopLeft
//    Vertices.Add(FVector(d,w+width,h+height)); //BackTopRight 5                                     //From Rotation (0|0|0) FrontTopRight
//    Vertices.Add(FVector(d+depth,w,h+height)); //FrontTopLeft 6                                     //From Rotation (0|0|0) BackTopLeft
//    Vertices.Add(FVector(d+depth,w+width,h+height)); //BackTopLeft 7                                  //From Rotation (0|0|0) BackTopRight

//    //Bottom
//    Triangles.Add(2);
//    Triangles.Add(3);
//    Triangles.Add(0);

//    Triangles.Add(3);
//    Triangles.Add(1);
//    Triangles.Add(0);

//    //Right
//    Triangles.Add(0);
//    Triangles.Add(1);
//    Triangles.Add(4);

//    Triangles.Add(5);
//    Triangles.Add(4);
//    Triangles.Add(1);

//    //Back
//    Triangles.Add(5);
//    Triangles.Add(1);
//    Triangles.Add(3);

//    Triangles.Add(3);
//    Triangles.Add(7);
//    Triangles.Add(5);

//    //Left
//    Triangles.Add(3);
//    Triangles.Add(2);
//    Triangles.Add(6);

//    Triangles.Add(3);
//    Triangles.Add(6);
//    Triangles.Add(7);

//    //Top
//    Triangles.Add(6);
//    Triangles.Add(4);
//    Triangles.Add(7);

//    Triangles.Add(7);
//    Triangles.Add(4);
//    Triangles.Add(5);

//    //Front
//    Triangles.Add(2);
//    Triangles.Add(0);
//    Triangles.Add(6);

//    Triangles.Add(6);
//    Triangles.Add(0);
//    Triangles.Add(5);


//    UVs.Add(FVector2D(0,0));
//    UVs.Add(FVector2D(0,1));
//    UVs.Add(FVector2D(1,0));
//    UVs.Add(FVector2D(1,1));


//    ProMesh->CreateMeshSection(1,Vertices,Triangles,Normals,UVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true); //create Mesh? needs to be in Game Thread?


}

// Called when the game starts or when spawned
void ABoundingBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoundingBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

