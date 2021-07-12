
#include "UMultiWorldManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelBounds.h"

bool UMultiWorldManager::AddWorld(UObject* InWorldContextObject, FWorldInformation InWorldInfo)
{
  bool bSuccess = false;
  int Index = 1;
  if(InWorldContextObject)
    {
      for(auto& SubWorld : InWorldInfo.WorldComponents)
        {
          bool bSuccessTemp = false;
          bSuccess = bSuccessTemp || bSuccess;

          FTimerHandle MyTimerHandle;
          FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject( this,  &UMultiWorldManager::SpawnSubLevel, InWorldContextObject, InWorldInfo, Index - 1);
          GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, SpawnDelegate , Index*1.0, false);
          Index++;
          }
    }

      FTransform WorldOrigin = Origin.FindOrAdd(InWorldInfo.WorldName);
      WorldOrigin = InWorldInfo.WorldOrigin;

  return true;
};

void UMultiWorldManager::SpawnSubLevel(UObject* InWorldContextObject, FWorldInformation InWorldInfo, int LevelIndex)
{
  bool bSuccessTemp = false;
  ULevelStreamingDynamic* LevelStreaming = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(InWorldContextObject,
                                                                                                    InWorldInfo.WorldComponents[LevelIndex],
                                                                                                    InWorldInfo.WorldOrigin.GetLocation(),
                                                                                                    InWorldInfo.WorldOrigin.Rotator(),
                                                                                                    bSuccessTemp
                                                                                                    );

}

FTransform UMultiWorldManager::GetWorldOrigin(FString InWorldName)
{
  if(Origin.Contains(InWorldName))
    {
      return Origin[InWorldName];
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Scene Component for %s not found"), *InWorldName);
    }

  return FTransform();

}

FString UMultiWorldManager::GetPrefix()
{
  return FString(TEXT("/")); //+ WorldList.Last().WorldName;
}

float UMultiWorldManager::GetLevelSize(UObject* InWorldContextObject, const FName& InPackageName)
{
  // ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(InWorldContextObject, InPackageName);
  // if(StreamingLevel)
  //   {
  //     float testsize = ALevelBounds::CalculateLevelBounds(StreamingLevel->GetLoadedLevel()).Max.Size();
  //     UE_LOG(LogTemp, Error, TEXT("Size %f"), testsize);
  //     return testsize;
  //   }
  // else
  //   {
  //     UE_LOG(LogTemp, Error, TEXT("GetLevelSize: Package %s does not exist"), *InPackageName.ToString());
  //     return -1;
  //   }
  return 0;
}

float UMultiWorldManager::GetLevelSize(ULevelStreamingDynamic* InStreamingLevel)
{
  // if(InStreamingLevel)
  //   {
  //     float testsize = ALevelBounds::CalculateLevelBounds(InStreamingLevel->GetLoadedLevel()).Max.Size();
  //     UE_LOG(LogTemp, Error, TEXT("Size %f"), testsize);
  //     return testsize;
  //   }
  // else
  //   {
  //     UE_LOG(LogTemp, Error, TEXT("does not exist"));
  //     return -1;
  //   }
  return 0;
}
