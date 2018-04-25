#pragma once

#include "InstanceId.h"
namespace UROSWorldControlHelper {
	static FString GetUniqueNameOfInstanceID(unreal_msgs::InstanceId* InstanceId)
	{
		return InstanceId->GetModelClassName().Append("_").Append(InstanceId->GetIdBase64());
	}
}