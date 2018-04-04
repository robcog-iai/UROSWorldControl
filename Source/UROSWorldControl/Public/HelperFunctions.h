#pragma once

#include "InstanceId.h"
namespace UROSWorldControlHelper {
	static FString GetUniqueIdOfInstanceID(unreal_msgs::InstanceId* InstanceId)
	{
		return InstanceId->GetModelClassName().Append("_").Append(InstanceId->GetId());
	}
}