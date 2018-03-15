#pragma once

#include "ROSBridgeSrv.h"
#include "Pose.h"
#include "ROSBridge/msgs/Tag.h"

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
		FString PathToMesh;
		FString PathToMaterial;
		geometry_msgs::Pose Pose;
		TArray<UTagMsg> Tags;

	public:
		Request() {}

		FString GetPathToMesh() { return PathToMesh; }
		FString GetPathToMaterial() { return PathToMaterial; }

		TArray<UTagMsg> GetTags() { return Tags; }
		
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
			PathToMesh = JsonObject->GetStringField("PathToMesh");
			PathToMaterial = JsonObject->GetStringField("PathToMaterial");
			Pose = geometry_msgs::Pose::GetFromJson(JsonObject->GetObjectField("Pose"));
	
			TArray<TSharedPtr<FJsonValue>> TagsPtrArray = JsonObject->GetArrayField(TEXT("Tags"));
			for (auto &ptr : TagsPtrArray)
			{
				UTagMsg Tag;
				Tag.FromJson(ptr->AsObject());
				Tags.Add(Tag);
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
			return TEXT("FROSBridgeSpawnService::Request { Mesh = ")
						+ PathToMesh + TEXT(", Material = ") + PathToMaterial + TEXT("} ");

		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const 
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField("PathToMesh", PathToMesh);
			Object->SetStringField("PathToMaterial", PathToMaterial);
			Object->SetObjectField("Pose", Pose.ToJsonObject());
			
			TArray<TSharedPtr<FJsonValue>> TagsPtrArray;
			for (auto &Tag : Tags) 
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Tag.ToJsonObject()));
				TagsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("Tags", TagsPtrArray);
			

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
			bSuccess = JSonObject->GetBoolField("succeded");
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
			Object->SetBoolField("succeded", bSuccess);
			return Object;
		}
	};
};