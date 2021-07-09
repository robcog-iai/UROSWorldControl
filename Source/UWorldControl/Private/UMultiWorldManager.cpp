
#include "UMultiWorldManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelBounds.h"

bool UMultiWorldManager::AddWorld(UObject* InWorldContextObject, FWorldInformation InWorldInfo)
{
  bool bSuccess = false;
  if(InWorldContextObject)
    {
      for(auto& WorldName : InWorldInfo.WorldComponents)
        {
          bool bSuccessTemp = false;
          ULevelStreamingDynamic* LevelStreaming = ULevelStreamingDynamic::LoadLevelInstance(InWorldContextObject,
                                                    WorldName,
                                                    InWorldInfo.WorldOrigin.GetLocation(),
                                                    InWorldInfo.WorldOrigin.Rotator(),
                                                    bSuccessTemp
                                                    );
          // true if at least one level loaded
          UE_LOG(LogTemp, Error, TEXT("Name to load %s"), *LevelStreaming->PackageNameToLoad.ToString());
          bSuccess = bSuccessTemp || bSuccess;
          if(bSuccessTemp)
            {
              InWorldInfo.WorldPackageNames.Add(LevelStreaming->GetWorldAssetPackageFName());
              float testsize = GetLevelSize(LevelStreaming);
              UE_LOG(LogTemp, Error, TEXT("Size %f"), testsize);
            }
        }

    }

  if(bSuccess)
    {
      FActorSpawnParameters SpawnInfo;
      SpawnInfo.Name = FName(*(InWorldInfo.WorldName + TEXT("Origin")));
      AActor* OriginActor = InWorldContextObject->GetWorld()->SpawnActor<AActor>(InWorldInfo.WorldOrigin.GetLocation(), InWorldInfo.WorldOrigin.Rotator(), SpawnInfo);
      FName UniqueName = MakeUniqueObjectName(OriginActor, USceneComponent::StaticClass(), SpawnInfo.Name);
      USceneComponent *SceneComponent = NewObject<USceneComponent>(OriginActor, UniqueName);
      OriginActor->SetRootComponent(SceneComponent);
      InWorldInfo.Origin = SceneComponent;

      WorldList.Add(InWorldInfo);
    }

  for(auto& WorldName : InWorldInfo.WorldPackageNames)
    {
      float testsize = GetLevelSize(WorldList[0].Origin->GetOwner(), WorldName);
      UE_LOG(LogTemp, Error, TEXT("Size %f"), testsize);
    }

  return bSuccess;
};

FTransform UMultiWorldManager::GetWorldOrigin(FString InWorldName)
{
  for(auto& World : WorldList)
    {
      if(World.WorldName.Equals(InWorldName))
        {
          return World.Origin->GetComponentTransform();
        }
    }
  return FTransform();

}

FString UMultiWorldManager::GetPrefix()
{
  return FString(TEXT("/")) + WorldList.Last().WorldName;
}

float UMultiWorldManager::GetLevelSize(UObject* InWorldContextObject, const FName& InPackageName)
{
  ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(InWorldContextObject, InPackageName);
  if(StreamingLevel)
    {
      float testsize = ALevelBounds::CalculateLevelBounds(StreamingLevel->GetLoadedLevel()).Max.Size();
      UE_LOG(LogTemp, Error, TEXT("Size %f"), testsize);
      return testsize;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("GetLevelSize: Package %s does not exist"), *InPackageName.ToString());
      return -1;
    }
}

float UMultiWorldManager::GetLevelSize(ULevelStreamingDynamic* InStreamingLevel)
{
  if(InStreamingLevel)
    {
      float testsize = ALevelBounds::CalculateLevelBounds(InStreamingLevel->GetLoadedLevel()).Max.Size();
      UE_LOG(LogTemp, Error, TEXT("Size %f"), testsize);
      return testsize;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("does not exist"));
      return -1;
    }
}
