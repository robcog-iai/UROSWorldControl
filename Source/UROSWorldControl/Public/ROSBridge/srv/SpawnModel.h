#pragma once

#include "ROSBridgeSrv.h"
#include "Pose.h"
#include "ModelDescription.h"

class UROSBRIDGE_API FROSBridgeSpawnServiceSrv : public FROSBridgeSrv {
protected :
	FString Type;

public:
	FROSBridgeSpawnServiceSrv(FString Type_) 
	{
		Type = Type_;
	}

	class Request : public SrvRequest{
	private:
		unreal_msgs::ModelDescription ModelDescription;
	public:
		Request() {}

		unreal_msgs::ModelDescription GetModelDescription()
		{
			return ModelDescription;
		}


		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override 
		{
			ModelDescription.FromJson(JsonObject->GetObjectField("model_description"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject) 
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeSpawnService::Request {%s} "), ModelDescription.ToString();

		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const 
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField("static_mesh_description", ModelDescription.ToJsonObject());
			
			return Object;
		}

	};


	class Response : public SrvResponse {
	private:
		bool bSuccess;
		unreal_msgs::InstanceId InstanceId;
			public:
		Response() {}
		Response(bool Success, unreal_msgs::InstanceId InInstanceId) 
		{
			bSuccess = Success;
			InstanceId = InInstanceId;
		}
		bool GetSuccess() const { return bSuccess; }
		void SetSuccess(bool Success) { bSuccess = Success; }

		unreal_msgs::InstanceId GetInstanceId() {
			return InstanceId;
		}

		void SetInstanceId(unreal_msgs::InstanceId InInstanceId)
		{
			InstanceId = InInstanceId;
		}
		
		virtual void FromJson(TSharedPtr<FJsonObject> JSonObject) override
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

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeSpawnService::Response { Success: %s, InstanceId: %s }"), bSuccess ? TEXT("True") : TEXT("False"), InstanceId.ToString();
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			Object->SetObjectField("instance_id", InstanceId.ToJsonObject());
			return Object;
		}
	};
};