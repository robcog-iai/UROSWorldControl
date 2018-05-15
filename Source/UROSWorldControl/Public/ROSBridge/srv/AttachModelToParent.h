#pragma once

#include "ROSBridgeSrv.h"
#include "InstanceId.h"

using namespace unreal_world_control_msgs;

class UROSBRIDGE_API FROSBridgeAttachModelToParentSrv : public FROSBridgeSrv
{
protected:
	FString Type;


public:
	FROSBridgeAttachModelToParentSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest
	{
	private:
		InstanceId Parent;
		InstanceId Child;


	public:
		Request()
		{
		}

		InstanceId GetParent() { return Parent; }

		InstanceId GetChild() { return Child; }

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Parent.FromJson(JsonObject->GetObjectField("parent_model"));
			Child.FromJson(JsonObject->GetObjectField("child_model"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		FString ToString() const override
		{
			return TEXT("FROSBridgeAttachModelToParentSrv::Request {parent_model = %s, child_model = %s}"), Parent.ToString(),
				Child.ToString();
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("parent_model"), Parent.ToJsonObject());
			Object->SetObjectField(TEXT("child_mode"), Child.ToJsonObject());
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
			return TEXT("FROSBridgeAttachModelToParentSrv::Request { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};
