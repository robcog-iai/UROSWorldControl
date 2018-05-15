#pragma once

#include "ROSBridgeSrv.h"
#include "Pose.h"
#include "ModelDescription.h"

class UROSBRIDGE_API FROSBridgeSpawnServiceSrv : public FROSBridgeSrv
{
protected :
	FString Type;

public:
	FROSBridgeSpawnServiceSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest
	{
	private:
		unreal_world_control_msgs::ModelDescription ModelDescription;
	public:
		Request()
		{
		}

		unreal_world_control_msgs::ModelDescription GetModelDescription()
		{
			return ModelDescription;
		}


		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ModelDescription.FromJson(JsonObject->GetObjectField("model_description"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		FString ToString() const override
		{
			return TEXT("RosWorldControlSpawnService::Request {%s} "), ModelDescription.ToString();
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField("static_mesh_description", ModelDescription.ToJsonObject());

			return Object;
		}
	};


	class Response : public SrvResponse
	{
	private:
		bool bSuccess;
		unreal_world_control_msgs::InstanceId InstanceId;
	public:
		Response()
		{
		}

		Response(bool Success, unreal_world_control_msgs::InstanceId InInstanceId)
		{
			bSuccess = Success;
			InstanceId = InInstanceId;
		}

		bool GetSuccess() const { return bSuccess; }
		void SetSuccess(bool Success) { bSuccess = Success; }

		unreal_world_control_msgs::InstanceId GetInstanceId()
		{
			return InstanceId;
		}

		void SetInstanceId(unreal_world_control_msgs::InstanceId InInstanceId)
		{
			InstanceId = InInstanceId;
		}

		void FromJson(TSharedPtr<FJsonObject> JSonObject) override
		{
			bSuccess = JSonObject->GetBoolField("success");
			InstanceId.FromJson(JSonObject->GetObjectField("instance_id"));
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JSonObject)
		{
			Response resp;
			resp.FromJson(JSonObject);
			return resp;
		}

		FString ToString() const override
		{
			return TEXT("RosWorldControlSpawnService::Response { Success: %s, InstanceId: %s }"),
				bSuccess ? TEXT("True") : TEXT("False"), InstanceId.ToString();
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			Object->SetObjectField("instance_id", InstanceId.ToJsonObject());
			return Object;
		}
	};
};
