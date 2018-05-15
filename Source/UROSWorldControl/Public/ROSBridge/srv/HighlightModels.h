#pragma once

#include "ROSBridgeSrv.h"
#include "InstanceId.h"

using namespace unreal_world_control_msgs;

class UROSBRIDGE_API FROSBridgeHighlightModelsSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSBridgeHighlightModelsSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest
	{
	private:
		TArray<InstanceId> InstanceIds;
		bool bToBeHighlighted;
		int32 ColorIndex;

	public:
		Request()
		{
		}

		TArray<InstanceId>* GetInstanceIds()
		{
			return &InstanceIds;
		}

		int32 GetColorIndex()
		{
			return ColorIndex;
		}

		bool GetToBeHighlighted()
		{
			return bToBeHighlighted;
		}

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			TArray<TSharedPtr<FJsonValue>> PtrArray = JsonObject->GetArrayField(TEXT("instance_ids"));
			for (auto& ptr : PtrArray)
			{
				InstanceId Id;
				Id.FromJson(ptr->AsObject());
				InstanceIds.Add(Id);
			}
			bToBeHighlighted = JsonObject->GetBoolField(TEXT("to_be_highlighted"));
			int ColorIndex = JsonObject->GetNumberField(TEXT("color_index"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		FString ToString() const override
		{
			return TEXT("FROSBridgeHighlightModelsSrv::Request { Highligt %s Models}"),
				FString::FromInt(InstanceIds.Num());
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			TArray<TSharedPtr<FJsonValue>> PtrArray;
			for (auto& Tag : InstanceIds)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Tag.ToJsonObject()));
				PtrArray.Add(Ptr);
			}
			Object->SetArrayField(TEXT("instance_ids"), PtrArray);


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

		Response(bool Success) : bSuccess(Success)
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
			return TEXT("FROSBridgeHighlightModelsSrv::Response { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};
