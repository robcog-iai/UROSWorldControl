#pragma once

#include "ROSBridgeSrv.h"
#include "PhysicsConstraint.h"
#include "Pose.h"

using namespace unreal_world_control_msgs;

class UROSBRIDGE_API FROSBridgeSpawnPhysiscConstraintSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSBridgeSpawnPhysiscConstraintSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest
	{
	private:
		PhysicsConstraintDetails ConstraintDetails;
		geometry_msgs::Pose Pose;
	public:
		Request()
		{
		}

		PhysicsConstraintDetails GetConstraintDetails()
		{
			return ConstraintDetails;
		}

		FVector GetLocation()
		{
			return Pose.GetPosition().GetVector();
		}

		FRotator GetRotator()
		{
			return FRotator(Pose.GetOrientation().GetQuat());
		}

		FQuat GetQuat()
		{
			return Pose.GetOrientation().GetQuat();
		}


		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ConstraintDetails.FromJson(JsonObject->GetObjectField("constraint_details"));
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		FString ToString() const override
		{
			return TEXT("FROSBridgeSpawnPhysiscConstraintSrv::Request {Constraint Details = %s, Pose: %s}"), ConstraintDetails.
				ToString(), Pose.ToString();
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("constraint_details"), ConstraintDetails.ToJsonObject());
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
			return TEXT("FROSBridgeSpawnPhysiscConstraintSrv::Request { %s }"), bSuccess ? TEXT("True") : TEXT("False");
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField("success", bSuccess);
			return Object;
		}
	};
};
