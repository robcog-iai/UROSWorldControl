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
			UTagID = JsonObject->GetStringField("id");
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
			Object->SetStringField("id", UTagID);

			return Object;
		}

	};

	class Response :public SrvResponse {
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
			return TEXT("FROSBridgeRemoveModelSrv::Request { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};
