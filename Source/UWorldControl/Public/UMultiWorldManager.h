#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreamingDynamic.h"
#include "UMultiWorldAsset.h"
#include "UMultiWorldManager.generated.h"


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

  void SpawnSubLevel(UObject* InWorldContextObject, FWorldInformation InWorldInfo, int LevelIndex);

protected:

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UMultiWorldAsset* WorldConfig;

  //WorldName, Origin
  UPROPERTY(BlueprintReadWrite)
    TMap<FString, FTransform> Origin;

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
