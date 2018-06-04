#pragma once
#include "ROSBridgeSrv.h"
#include "PhysicsConstraintDetails.h"
#include "Pose.h"




class UROSBRIDGE_API FROSSpawnPhysicsConstraintSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSSpawnPhysicsConstraintSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
		world_control_msgs::PhysicsConstraintDetails ConstraintDetails;
		geometry_msgs::Pose Pose;


	public:
		Request() {}

		Request(world_control_msgs::PhysicsConstraintDetails InConstraintDetails, geometry_msgs::Pose InPose)
		{
			ConstraintDetails = InConstraintDetails;
			Pose = InPose;
		}

		world_control_msgs::PhysicsConstraintDetails GetConstraintDetails()
		{
			return ConstraintDetails;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ConstraintDetails.FromJson(JsonObject->GetObjectField("constraint_details"));
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
			return TEXT("FROSSpawnPhysicsConstraintSrv:Request {constraint_details = %s, pose = %s"),
				ConstraintDetails.ToString(),
				Pose.ToString();
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("constraint_details"), ConstraintDetails.ToJsonObject());
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;


	public:
		Response() {}

		Response(bool InSuccess)
		{
			Success = InSuccess;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return TEXT("FROSSpawnPhysicsConstraintSrv:Response {success = %s"),
				Success ? TEXT("True") : TEXT("False");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};