#pragma once

#include "Engine/DataAsset.h"
#include "Containers/Map.h"
// clang-format off
#include "UMultiWorldAsset.generated.h"
// clang-format on

USTRUCT()
struct UWORLDCONTROL_API FSubLevelInformation
{
  GENERATED_BODY()
  public:

  UPROPERTY()
  FString FilePath;
};

USTRUCT(BlueprintType)
struct UWORLDCONTROL_API FWorldInformation
{
  GENERATED_BODY()
  public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString WorldName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<TAssetPtr<UWorld>> WorldComponents;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform WorldOrigin;
};

UCLASS(BlueprintType, Category = "Controller")
  class UWORLDCONTROL_API UMultiWorldAsset : public UDataAsset
{
  GENERATED_BODY()

public:
  // Names of the robots for which this configration asset is used
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic Information")
  TArray<FWorldInformation> WorldList;

};
