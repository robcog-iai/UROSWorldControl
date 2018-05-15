#pragma once

#include "ROSBridgeSrv.h"
#include "InstanceId.h"
#include "Pose.h"

class UROSBRIDGE_API FROSBridgeSetModelPoseSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSBridgeSetModelPoseSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest
	{
	private:
		unreal_world_control_msgs::InstanceId InstanceId;
		geometry_msgs::Pose Pose;

	public:
		Request()
		{
		}

		unreal_world_control_msgs::InstanceId GetInstanceId() { return InstanceId; };

		FVector GetLocation()
		{
			return Pose.GetPosition().GetVector();
		}

		FRotator GetRotator()
		{
			return FRotator(Pose.GetOrientation().GetQuat());
		}

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			InstanceId.FromJson(JsonObject->GetObjectField("instance_id"));
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		FString ToString() const override
		{
			return TEXT("RosWorldControlSetModelPoseSrv::Request { InstanceID = ") + InstanceId.ToString() +
				TEXT(", Location = ") + Pose.GetPosition().GetVector().ToString() +
				TEXT(" and Rotator  = ") + FRotator(Pose.GetOrientation().GetQuat()).ToString() +
				TEXT("} ");
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField("instance_id", InstanceId.ToJsonObject());
			Object->SetObjectField("pose", Pose.ToJsonObject());

			return Object;
		}
	};


	class Response : public SrvResponse
	{
	private:
		bool bSuccess;

	public:
		Response()
		{
		}

		Response(bool succeded_) : bSuccess(succeded_)
		{
		}

		bool GetSuccess() const { return bSuccess; }
		void SetSuccess(bool Success) { bSuccess = Success; }

		void FromJson(TSharedPtr<FJsonObject> JSonObject) override
		{
			bSuccess = JSonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JSonObject)
		{
			Response resp;
			resp.FromJson(JSonObject);
			return resp;
		}

		FString ToString() const override
		{
			return TEXT("RosWorldControlSetModelPoseSrv::Request { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};
