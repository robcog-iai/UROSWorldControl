#pragma once
#include "ROSBridgeMsg.h"
#include "Vector3.h"



namespace world_control_msgs
{
	class AngularLimits : public FROSBridgeMsg
	{
		uint8 Swing1Motion;
		uint8 Swing2Motion;
		uint8 TwistMotion;
		float Swing1LimitAngle;
		float Swing2LimitAngle;
		float TwistLimitAngle;
		geometry_msgs::Vector3 AngularRoationOffset;
		bool UseAdvanced;
		bool SwingSoftConstrained;
		float SwingStiffness;
		float SwingDamping;
		bool TwistSoftConstrained;
		float TwistStiffness;
		float TwistDamping;


	public:
		AngularLimits() {}

		AngularLimits(uint8 InSwing1Motion, uint8 InSwing2Motion, uint8 InTwistMotion, float InSwing1LimitAngle, float InSwing2LimitAngle, float InTwistLimitAngle, geometry_msgs::Vector3 InAngularRoationOffset, bool InUseAdvanced, bool InSwingSoftConstrained, float InSwingStiffness, float InSwingDamping, bool InTwistSoftConstrained, float InTwistStiffness, float InTwistDamping)
		{
			AngularLimits();
			Swing1Motion = InSwing1Motion;
			Swing2Motion = InSwing2Motion;
			TwistMotion = InTwistMotion;
			Swing1LimitAngle = InSwing1LimitAngle;
			Swing2LimitAngle = InSwing2LimitAngle;
			TwistLimitAngle = InTwistLimitAngle;
			AngularRoationOffset = InAngularRoationOffset;
			UseAdvanced = InUseAdvanced;
			SwingSoftConstrained = InSwingSoftConstrained;
			SwingStiffness = InSwingStiffness;
			SwingDamping = InSwingDamping;
			TwistSoftConstrained = InTwistSoftConstrained;
			TwistStiffness = InTwistStiffness;
			TwistDamping = InTwistDamping;
		}

		uint8 GetSwing1Motion()
		{
			return Swing1Motion;
		}

		uint8 GetSwing2Motion()
		{
			return Swing2Motion;
		}

		uint8 GetTwistMotion()
		{
			return TwistMotion;
		}

		float GetSwing1LimitAngle()
		{
			return Swing1LimitAngle;
		}

		float GetSwing2LimitAngle()
		{
			return Swing2LimitAngle;
		}

		float GetTwistLimitAngle()
		{
			return TwistLimitAngle;
		}

		geometry_msgs::Vector3 GetAngularRoationOffset()
		{
			return AngularRoationOffset;
		}

		bool GetUseAdvanced()
		{
			return UseAdvanced;
		}

		bool GetSwingSoftConstrained()
		{
			return SwingSoftConstrained;
		}

		float GetSwingStiffness()
		{
			return SwingStiffness;
		}

		float GetSwingDamping()
		{
			return SwingDamping;
		}

		bool GetTwistSoftConstrained()
		{
			return TwistSoftConstrained;
		}

		float GetTwistStiffness()
		{
			return TwistStiffness;
		}

		float GetTwistDamping()
		{
			return TwistDamping;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Swing1Motion = JsonObject->GetNumberField("swing_1_motion");
			Swing2Motion = JsonObject->GetNumberField("swing_2_motion");
			TwistMotion = JsonObject->GetNumberField("twist_motion");
			Swing1LimitAngle = JsonObject->GetNumberField("swing_1_limit_angle");
			Swing2LimitAngle = JsonObject->GetNumberField("swing_2_limit_angle");
			TwistLimitAngle = JsonObject->GetNumberField("twist_limit_angle");
			AngularRoationOffset.FromJson(JsonObject->GetObjectField("angular_roation_offset"));
			UseAdvanced = JsonObject->GetBoolField("use_advanced");
			SwingSoftConstrained = JsonObject->GetBoolField("swing_soft_constrained");
			SwingStiffness = JsonObject->GetNumberField("swing_stiffness");
			SwingDamping = JsonObject->GetNumberField("swing_damping");
			TwistSoftConstrained = JsonObject->GetBoolField("twist_soft_constrained");
			TwistStiffness = JsonObject->GetNumberField("twist_stiffness");
			TwistDamping = JsonObject->GetNumberField("twist_damping");
		}

		static AngularLimits GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			AngularLimits Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return TEXT("AngularLimits {swing_1_motion = %s, swing_2_motion = %s, twist_motion = %s, swing_1_limit_angle = %s, swing_2_limit_angle = %s, twist_limit_angle = %s, angular_roation_offset = %s, use_advanced = %s, swing_soft_constrained = %s, swing_stiffness = %s, swing_damping = %s, twist_soft_constrained = %s, twist_stiffness = %s, twist_damping = %s"),
				FString::FromInt(Swing1Motion),
				FString::FromInt(Swing2Motion),
				FString::FromInt(TwistMotion),
				FString::SanitizeFloat(Swing1LimitAngle),
				FString::SanitizeFloat(Swing2LimitAngle),
				FString::SanitizeFloat(TwistLimitAngle),
				AngularRoationOffset.ToString(),
				UseAdvanced ? TEXT("True") : TEXT("False"),
				SwingSoftConstrained ? TEXT("True") : TEXT("False"),
				FString::SanitizeFloat(SwingStiffness),
				FString::SanitizeFloat(SwingDamping),
				TwistSoftConstrained ? TEXT("True") : TEXT("False"),
				FString::SanitizeFloat(TwistStiffness),
				FString::SanitizeFloat(TwistDamping);
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetNumberField(TEXT("swing_1_motion"), Swing1Motion);
			Object->SetNumberField(TEXT("swing_2_motion"), Swing2Motion);
			Object->SetNumberField(TEXT("twist_motion"), TwistMotion);
			Object->SetNumberField(TEXT("swing_1_limit_angle"), Swing1LimitAngle);
			Object->SetNumberField(TEXT("swing_2_limit_angle"), Swing2LimitAngle);
			Object->SetNumberField(TEXT("twist_limit_angle"), TwistLimitAngle);
			Object->SetObjectField(TEXT("angular_roation_offset"), AngularRoationOffset.ToJsonObject());
			Object->SetBoolField(TEXT("use_advanced"), UseAdvanced);
			Object->SetBoolField(TEXT("swing_soft_constrained"), SwingSoftConstrained);
			Object->SetNumberField(TEXT("swing_stiffness"), SwingStiffness);
			Object->SetNumberField(TEXT("swing_damping"), SwingDamping);
			Object->SetBoolField(TEXT("twist_soft_constrained"), TwistSoftConstrained);
			Object->SetNumberField(TEXT("twist_stiffness"), TwistStiffness);
			Object->SetNumberField(TEXT("twist_damping"), TwistDamping);
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