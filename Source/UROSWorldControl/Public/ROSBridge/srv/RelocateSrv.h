#pragma once

#include "ROSBridgeSrv.h"
#include "Pose.h"

class UROSBRIDGE_API FROSBridgeRelocatSrv : public FROSBridgeSrv {
protected:
	FString Type;

public:
	FROSBridgeRelocatSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest {
	private:
		FString UTagID;
		geometry_msgs::Pose Pose;

	public:
		Request() {}
		FString GetUtagId() { return UTagID; };

		FVector GetLocation()
		{
			return Pose.GetPosition().GetVector();
		}

		FRotator GetRotator()
		{
			return FRotator::FRotator(Pose.GetOrientation().GetQuat());
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			UTagID = JsonObject->GetStringField("UTagID");
			Pose = geometry_msgs::Pose::GetFromJson(JsonObject->GetObjectField("Pose"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeRelocatorSrv::Request { UTagID = ") + UTagID +
				TEXT(", Location = ") + Pose.GetPosition().GetVector().ToString() + 
				TEXT(" and Rotator  = ") + FRotator::FRotator(Pose.GetOrientation().GetQuat()).ToString() + 
				TEXT("} ");

		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField("UTagID", UTagID);
			Object->SetObjectField("Pose", Pose.ToJsonObject());

			return Object;
		}

	};


	class Response : public SrvResponse {
	private:
		bool succeded;

	public:
		Response() {}
		Response(bool succeded_) : succeded(succeded_) {}
		bool getWorked() const { return succeded; }
		void setWorked(bool succeded_) { succeded = succeded_; }

		virtual void FromJson(TSharedPtr<FJsonObject> JSonObject) override
		{
			succeded = JSonObject->GetBoolField("succeded");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JSonObject)
		{
			Response resp;
			resp.FromJson(JSonObject);
			return resp;
		}

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeRelocatorSrv::Request { %s }"), succeded ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("succeded", succeded);
			return Object;
		}
	};
};