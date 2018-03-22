#pragma once

#include "ROSBridgeSrv.h"
#include "Pose.h"
#include "ROSBridge/msgs/Tag.h"
#include "StaticMeshDescription.h"

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
		UStaticMeshDescription Description;
	public:
		Request() {}

		FString GetPathToMesh() { return Description.GetPathToMesh(); }
		FString GetPathToMaterial() { return Description.GetPathToMaterial(); }

		TArray<UTagMsg> GetTags() { return Description.GetTags(); }
		
		FVector GetLocation()
		{ 
			return Description.GetLocation();
		}

		FRotator GetRotator()
		{
			return Description.GetRotator();
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override 
		{
			Description.FromJson(JsonObject->GetObjectField("static_mesh_description"));		
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject) 
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeSpawnService::Request {%s} "), Description.ToString();

		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const 
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField("static_mesh_description", Description.ToJsonObject());
			
			return Object;
		}

	};


	class Response : public SrvResponse {
	private:
		bool bSuccess;
			public:
		Response() {}
		Response(bool Success) : bSuccess(Success) {}
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
			return TEXT("FROSBridgeSpawnService::Response { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};