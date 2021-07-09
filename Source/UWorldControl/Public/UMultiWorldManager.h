#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreamingDynamic.h"
#include "UMultiWorldManager.generated.h"

USTRUCT(BlueprintType)
struct FWorldInformation
{
  GENERATED_BODY()
  public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString WorldName;

  //World can consist of multiple levels
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<FString> WorldComponents;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform WorldOrigin;

  UPROPERTY()
    USceneComponent* Origin;

  UPROPERTY()
  TArray<FName> WorldPackageNames;
};

USTRUCT()
struct FSubLevelInformation
{
  GENERATED_BODY()
  public:

  UPROPERTY()
  FString FilePath;
};

UCLASS()
class UWORLDCONTROL_API UMultiWorldManager : public UObject
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  virtual bool AddWorld(UObject* InWorldContextObject, FWorldInformation InWorldInfo);

  UFUNCTION(BlueprintCallable)
  virtual FTransform GetWorldOrigin(FString InWorldName);

  UFUNCTION(BlueprintCallable)
  FString GetPrefix();

  virtual float GetLevelSize(UObject* InWorldContextObject, const FName& InPackageName);

  virtual float GetLevelSize(ULevelStreamingDynamic* InStreamingLevel);

protected:

  UPROPERTY()
    TArray<FWorldInformation> WorldList;

};

UCLASS()
class UWORLDCONTROL_API AMultiWorldManagerActor : public AActor
{
  GENERATED_BODY()
    public:

  AMultiWorldManagerActor()
    {
      WorldManager = CreateDefaultSubobject<UMultiWorldManager>(FName(*(GetName() + TEXT("_MultiWorldManager"))));
    };

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn="true"))
    UMultiWorldManager* WorldManager;


};
