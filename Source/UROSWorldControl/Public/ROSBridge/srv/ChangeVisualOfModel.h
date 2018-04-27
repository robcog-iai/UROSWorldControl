#pragma once

#include "ROSBridgeSrv.h"
#include "MeshDescription.h"
#include "InstanceId.h"
#include "Pose.h"


class UROSBRIDGE_API RosWorldControlChangeVisualOfModelsSrv : public FROSBridgeSrv {
protected:
	FString Type;

public:
	RosWorldControlChangeVisualOfModelsSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest {
	private:
		unreal_world_control_msgs::InstanceId InstanceId;
		unreal_world_control_msgs::MeshDescription MeshDescription;

	public:
		Request() {}
		unreal_world_control_msgs::InstanceId GetInstanceId() { return InstanceId; };

		unreal_world_control_msgs::MeshDescription GetMeshDescription()
		{
			return MeshDescription;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			InstanceId.FromJson(JsonObject->GetObjectField("instance_id"));
			MeshDescription.FromJson(JsonObject->GetObjectField("mesh_description"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		virtual FString ToString() const override
		{
			return TEXT("RosWorldControlChangeVisualOfModel::Request {") + InstanceId.ToString() + MeshDescription.ToString() + TEXT("}");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField("instance_id", InstanceId.ToJsonObject());
			Object->SetObjectField("mesh_description", MeshDescription.ToJsonObject());

			return Object;
		}

	};


	class Response : public SrvResponse {
	private:
		bool bSuccess;

	public:
		Response() {}
		Response(bool succeded_) : bSuccess(succeded_) {}
		bool GetSuccess() const { return bSuccess; }
		void SetSuccess(bool Success) { bSuccess = Success; }

		virtual void FromJson(TSharedPtr<FJsonObject> JSonObject) override
		{
			bSuccess = JSonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JSonObject)
		{
			Response resp;
			resp.FromJson(JSonObject);
			return resp;
		}

		virtual FString ToString() const override
		{
			return TEXT("RosWorldControlChangeVisualOfModel::Response { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};