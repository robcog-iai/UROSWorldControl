#include "SpawnPhysicsConstraintServer.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// SetAngularLimits for Physics Constraints
static FORCEINLINE void SetAngularLimits(
	FConstraintInstance& Constraint,
	const uint8 Swing1Limit, const uint8 Swing2Limit, const uint8 TwistLimit,
	const float Swing1LimitAngle,
	const float Swing2LimitAngle,
	const float TwistLimitAngle,
	bool SoftSwingLimit = true, bool SoftTwistLimit = true,
	const float SwingStiff = 50, const float SwingDamp = 5,
	const float TwistStiff = 50, const float TwistDamp = 5
)
{
	switch (Swing1Limit)
	{
	case 0: Constraint.SetAngularSwing1Motion(ACM_Free);
		break;
	case 1: Constraint.SetAngularSwing1Motion(ACM_Limited);
		break;
	case 2: Constraint.SetAngularSwing1Motion(ACM_Locked);
		break;
	}
	switch (Swing2Limit)
	{
	case 0: Constraint.SetAngularSwing2Motion(ACM_Free);
		break;
	case 1: Constraint.SetAngularSwing2Motion(ACM_Limited);
		break;
	case 2: Constraint.SetAngularSwing2Motion(ACM_Locked);
		break;
	}
	switch (TwistLimit)
	{
	case 0: Constraint.SetAngularTwistMotion(ACM_Free);
		break;
	case 1: Constraint.SetAngularTwistMotion(ACM_Limited);
		break;
	case 2: Constraint.SetAngularTwistMotion(ACM_Locked);
		break;
	}

	// Soft Limit?
	if (SoftSwingLimit) Constraint.ProfileInstance.ConeLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.ConeLimit.bSoftConstraint = 0;

	if (SoftTwistLimit) Constraint.ProfileInstance.TwistLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.TwistLimit.bSoftConstraint = 0;

	// Limit Angles
	Constraint.SetAngularSwing1Limit(ACM_Free, Swing1LimitAngle);
	Constraint.SetAngularSwing2Limit(ACM_Free, Swing2LimitAngle);
	Constraint.SetAngularTwistLimit(ACM_Free, TwistLimitAngle);

	Constraint.ProfileInstance.ConeLimit.Stiffness = SwingStiff;
	Constraint.ProfileInstance.ConeLimit.Damping = SwingDamp;
	Constraint.ProfileInstance.TwistLimit.Stiffness = TwistStiff;
	Constraint.ProfileInstance.TwistLimit.Damping = TwistDamp;
}


// SetLinearLimits for Physics Constraints
static FORCEINLINE void SetLinearLimits(
	FConstraintInstance& Constraint,
	const uint8 XLimit, const uint8 YLimit, const uint8 ZLimit,
	const float Size,
	bool SoftLimit = false,
	const float SoftStiffness = 0,
	const float SoftDampening = 0
)
{
	switch (XLimit)
	{
	case 0: Constraint.SetLinearXMotion(LCM_Free);
		break;
	case 1: Constraint.SetLinearXMotion(LCM_Limited);
		break;
	case 2: Constraint.SetLinearXMotion(LCM_Locked);
		break;
	}
	switch (YLimit)
	{
	case 0: Constraint.SetLinearYMotion(LCM_Free);
		break;
	case 1: Constraint.SetLinearYMotion(LCM_Limited);
		break;
	case 2: Constraint.SetLinearYMotion(LCM_Locked);
		break;
	}
	switch (ZLimit)
	{
	case 0: Constraint.SetLinearZMotion(LCM_Free);
		break;
	case 1: Constraint.SetLinearZMotion(LCM_Limited);
		break;
	case 2: Constraint.SetLinearZMotion(LCM_Locked);
		break;
	}

	Constraint.SetLinearLimitSize(Size);

	// Soft Limit?
	if (SoftLimit) Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 0;

	Constraint.ProfileInstance.LinearLimit.Stiffness = SoftStiffness;
	Constraint.ProfileInstance.LinearLimit.Damping = SoftDampening;
}


bool FROSSpawnPhysicsConstraintServer::SpawnPhysicsConstraintActor(
	TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> Request)
{
	world_control_msgs::PhysicsConstraintDetails Details = Request->GetConstraintDetails();
	AActor** First = Controller->IdToActorMap.Find(Details.GetIdFirstModel());
	AActor** Second = Controller->IdToActorMap.Find(Details.GetIdSecondModel());

	if (!First || !Second)
	{
		//at least one of them could not be found.
		if (!First)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *Details.GetIdFirstModel());
		}

		if (!Second)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *Details.GetIdFirstModel());
		}

		return false;
	}


	//Actors do Exist. Set as ConstraintActors of component. 
	UPhysicsConstraintComponent* ConstraintComponent = NewObject<UPhysicsConstraintComponent>(*First);
	ConstraintComponent->bEditableWhenInherited = true;
	ConstraintComponent->ConstraintActor1 = *First;
	ConstraintComponent->ConstraintActor2 = *Second;

	//Set World Location
	ConstraintComponent->SetWorldLocationAndRotation(
		Request->GetPose().GetPosition().GetVector(),
		FRotator(Request->GetPose().GetOrientation().GetQuat()));

	//set up the constraint instance with all the desired values
	FConstraintInstance ConstraintInstance;


	SetupProfileInstance(ConstraintInstance.ProfileInstance, Details);
	SetupLinearLimits(ConstraintInstance, Details);
	SetupAngularLimits(ConstraintInstance, Details);

	ConstraintComponent->ConstraintInstance = ConstraintInstance;

	ConstraintComponent->AttachToComponent((*First)->GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	ConstraintComponent->RegisterComponent();

	return true;

}

void FROSSpawnPhysicsConstraintServer::SetupProfileInstance(FConstraintProfileProperties& ProfileInstance,
	world_control_msgs::PhysicsConstraintDetails Details)
{
	ProfileInstance.bDisableCollision = Details.GetDisableCollision();
	ProfileInstance.bEnableProjection = Details.GetEnableProjection();
	ProfileInstance.ProjectionLinearTolerance = Details.GetProjectionLinearTolerance();
	ProfileInstance.ProjectionAngularTolerance = Details.GetProjectionAngularTolerance();
	ProfileInstance.bParentDominates = Details.GetParentDominates();
}

void FROSSpawnPhysicsConstraintServer::SetupAngularLimits(FConstraintInstance& Instance,
	world_control_msgs::PhysicsConstraintDetails Details)
{
	world_control_msgs::AngularLimits AngLimit = Details.GetAngularLimits();

	if (AngLimit.GetUseAdvanced())
	{
		//Advanced features will be set as well.
		SetAngularLimits(Instance,
			AngLimit.GetSwing1Motion(), AngLimit.GetSwing2Motion(), AngLimit.GetTwistMotion(),
			AngLimit.GetSwing1LimitAngle(), AngLimit.GetSwing2LimitAngle(), AngLimit.GetTwistLimitAngle(),
			AngLimit.GetSwingSoftConstraint(), AngLimit.GetTwistSoftConstraint(),
			AngLimit.GetSwingStiffness(), AngLimit.GetSwingDamping(),
			AngLimit.GetTwistStiffness(), AngLimit.GetTwistDamping()
		);

		Instance.AngularRotationOffset = AngLimit.GetAngularRoationOffset().GetVector().Rotation();
	}
	else
	{
		//Advanced features will be left default.
		SetAngularLimits(Instance,
			AngLimit.GetSwing1Motion(), AngLimit.GetSwing2Motion(), AngLimit.GetTwistMotion(),
			AngLimit.GetSwing1LimitAngle(), AngLimit.GetSwing2LimitAngle(), AngLimit.GetTwistLimitAngle());
	}
}

void FROSSpawnPhysicsConstraintServer::SetupLinearLimits(FConstraintInstance& Instance, world_control_msgs::PhysicsConstraintDetails Details)
{
	world_control_msgs::LinearLimits LinLimit = Details.GetLinearLimits();

	if (LinLimit.GetUseAdvanced())
	{
		//Advanced features will be set as well.
		SetLinearLimits(Instance,
			LinLimit.GetXMotion(), LinLimit.GetYMotion(), LinLimit.GetZMotion(),
			LinLimit.GetLimit(),
			LinLimit.GetSoftConstraint(), LinLimit.GetStiffness(), LinLimit.GetDamping()
		);
	}
	else
	{
		//Advanced features will be left default.
		SetLinearLimits(Instance,
			LinLimit.GetXMotion(), LinLimit.GetYMotion(), LinLimit.GetZMotion(),
			LinLimit.GetLimit()
		);
	}
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnPhysicsConstraintServer::FromJson(
	TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> Request =
		MakeShareable(new FROSSpawnPhysicsConstraintSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnPhysicsConstraintServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> SpawnPhysicsConstraintRequest =
		StaticCastSharedPtr<FROSSpawnPhysicsConstraintSrv::Request>(Request);

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = SpawnPhysicsConstraintActor(SpawnPhysicsConstraintRequest);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSSpawnPhysicsConstraintSrv::Response(ServiceSuccess));

}
