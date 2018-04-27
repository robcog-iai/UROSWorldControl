#pragma once

#include "InstanceId.h"
namespace UROSWorldControlHelper {
	static FString GetUniqueNameOfInstanceID(unreal_world_control_msgs::InstanceId* InstanceId)
	{
		return InstanceId->GetModelClassName().Append("_").Append(InstanceId->GetId());
	}
}