#pragma once

#include "ROSBridgeSrv.h"

class UROSBRIDGE_API FROSBridgeRemoveModelSrv : public FROSBridgeSrv {
protected:
	FString Type;

public:
	FROSBridgeRemoveModelSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest {
	private:
		FString UTagID;

	public:
		Request() {}
		FString GetUtagId() { return UTagID; };

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			UTagID = JsonObject->GetStringField("UTagID");
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeRemoveModelSrv::Request { UTagID = ") + UTagID + TEXT("} ");

		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField("UTagID", UTagID);

			return Object;
		}

	};

	class Response :public SrvResponse {
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
			return TEXT("FROSBridgeRemoveModelSrv::Request { %s }"), succeded ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("succeded", succeded);
			return Object;
		}
	};
};
