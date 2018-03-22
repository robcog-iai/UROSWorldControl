#pragma once

#include "ROSBridgeSrv.h"
#include "StaticMeshDescription.h"

class UROSBRIDGE_API FROSBridgeSpawnSemanticMapSrv : public FROSBridgeSrv {
protected:
	FString Type;

public:
	FROSBridgeSpawnSemanticMapSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest {
	private:
		TArray<UStaticMeshDescription> StaticMeshDescriptions;

	public:
		Request() {}

		TArray<UStaticMeshDescription>* GetStaticMeshDescriptions() { return &StaticMeshDescriptions; }

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			TArray<TSharedPtr<FJsonValue>> MeshesPtrArray = JsonObject->GetArrayField(TEXT("static_mesh_descriptions"));
			for (auto &ptr : MeshesPtrArray)
			{
				UStaticMeshDescription Description;
				Description.FromJson(ptr->AsObject());
				StaticMeshDescriptions.Add(Description);
			}
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		virtual FString ToString() const override
		{
			return TEXT("FROSBridgeSpawnSemanticMapSrv::Request { SemanticMap size: %s }"),
				FString::FromInt(StaticMeshDescriptions.Num());

		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			TArray<TSharedPtr<FJsonValue>> MeshesPtrArray;
			for (auto &Tag : StaticMeshDescriptions)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Tag.ToJsonObject()));
				MeshesPtrArray.Add(Ptr);
			}
			Object->SetArrayField(TEXT("static_mesh_descriptions"), MeshesPtrArray);


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
			return TEXT("FROSBridgeSpawnSemanticMapSrv::Response { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};