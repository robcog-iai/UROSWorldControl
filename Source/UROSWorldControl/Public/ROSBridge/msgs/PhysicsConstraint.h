#pragma once
#include "ROSBridgeMsg.h"
#include "InstanceId.h"
#include "LinearLimits.h"
#include "AngularLimits.h"



namespace unreal_world_control_msgs
{
	class PhysicsConstraint : public FROSBridgeMsg
	{
		unreal_world_control_msgs::InstanceId FirstModel;
		unreal_world_control_msgs::InstanceId SecondModel;
		bool bDisabelCollision;
		bool bEnableProjection;
		float ProjectionLinearTolerance;
		float ProjectionAngularTolerance;
		bool bParentDominates;
		unreal_world_control_msgs::LinearLimits LinearLimits;
		unreal_world_control_msgs::AngularLimits AngularLimits;


	public:
		PhysicsConstraint() {}

		PhysicsConstraint(unreal_world_control_msgs::InstanceId InFirstModel, unreal_world_control_msgs::InstanceId InSecondModel,
			bool InDisabelCollision, bool InEnableProjection,
			float InProjectionLinearTolerance, float InProjectionAngularTolerance, bool InParentDominates,
			unreal_world_control_msgs::LinearLimits InLinearLimits, unreal_world_control_msgs::AngularLimits InAngularLimits)
		{
			PhysicsConstraint();
			FirstModel = InFirstModel;
			SecondModel = InSecondModel;
			bDisabelCollision = InDisabelCollision;
			bEnableProjection = InEnableProjection;
			ProjectionLinearTolerance = InProjectionLinearTolerance;
			ProjectionAngularTolerance = InProjectionAngularTolerance;
			bParentDominates = InParentDominates;
			LinearLimits = InLinearLimits;
			AngularLimits = InAngularLimits;
		}

		unreal_world_control_msgs::InstanceId GetFirstModel()
		{
			return FirstModel;
		}

		unreal_world_control_msgs::InstanceId GetSecondModel()
		{
			return SecondModel;
		}

		bool GetDisabelCollision()
		{
			return bDisabelCollision;
		}

		bool GetEnableProjection()
		{
			return bEnableProjection;
		}

		float GetProjectionLinearTolerance()
		{
			return ProjectionLinearTolerance;
		}

		float GetProjectionAngularTolerance()
		{
			return ProjectionAngularTolerance;
		}

		bool GetParentDominates()
		{
			return bParentDominates;
		}

		unreal_world_control_msgs::LinearLimits GetLinearLimits()
		{
			return LinearLimits;
		}

		unreal_world_control_msgs::AngularLimits GetAngularLimits()
		{
			return AngularLimits;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			FirstModel.FromJson(JsonObject->GetObjectField("first_model"));
			SecondModel.FromJson(JsonObject->GetObjectField("second_model"));
			bDisabelCollision = JsonObject->GetBoolField("disabel_collision");
			bEnableProjection = JsonObject->GetBoolField("enable_projection");
			ProjectionLinearTolerance = JsonObject->GetNumberField("projection_linear_tolerance");
			ProjectionAngularTolerance = JsonObject->GetNumberField("projection_angular_tolerance");
			bParentDominates = JsonObject->GetBoolField("parent_dominates");
			LinearLimits.FromJson(JsonObject->GetObjectField("linear_limits"));
			AngularLimits.FromJson(JsonObject->GetObjectField("angular_limits"));
		}

		static PhysicsConstraint GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PhysicsConstraint Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return TEXT("PhysicsConstraint {first_model = %s, second_model = %s, disabel_collision = %s, enable_projection = %s, projection_linear_tolerance = %s, projection_angular_tolerance = %s, parent_dominates = %s, linear_limits = %s, angular_limits = %s"), 
				FirstModel.ToString(), SecondModel.ToString(), bDisabelCollision ? TEXT("True") : TEXT("False"), 
				bEnableProjection ? TEXT("True") : TEXT("False"), FString::SanitizeFloat(ProjectionLinearTolerance),
				FString::SanitizeFloat(ProjectionAngularTolerance), bParentDominates ? TEXT("True") : TEXT("False"),
				LinearLimits.ToString(), AngularLimits.ToString();
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("first_model"), FirstModel.ToJsonObject());
			Object->SetObjectField(TEXT("second_model"), SecondModel.ToJsonObject());
			Object->SetBoolField(TEXT("disabel_collision"), bDisabelCollision);
			Object->SetBoolField(TEXT("enable_projection"), bEnableProjection);
			Object->SetNumberField(TEXT("projection_linear_tolerance"), ProjectionLinearTolerance);
			Object->SetNumberField(TEXT("projection_angular_tolerance"), ProjectionAngularTolerance);
			Object->SetBoolField(TEXT("parent_dominates"), bParentDominates);
			Object->SetObjectField(TEXT("linear_limits"), LinearLimits.ToJsonObject());
			Object->SetObjectField(TEXT("angular_limits"), AngularLimits.ToJsonObject());
			return Object;
		}

		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}