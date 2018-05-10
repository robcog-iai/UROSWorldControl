#include "SpawnPhysicsConstraintServer.h"

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
	case 0: Constraint.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Free); break;
	case 1: Constraint.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Limited); break;
	case 2: Constraint.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Locked); break;
	}
	switch (Swing2Limit)
	{
	case 0: Constraint.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Free); break;
	case 1: Constraint.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Limited); break;
	case 2: Constraint.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Locked); break;
	}
	switch (TwistLimit)
	{
	case 0: Constraint.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Free); break;
	case 1: Constraint.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Limited); break;
	case 2: Constraint.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Locked); break;
	}

	// Soft Limit?
	if (SoftSwingLimit) Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 0;

	if (SoftTwistLimit) Constraint.ProfileInstance.TwistLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.TwistLimit.bSoftConstraint = 0;

	// Limit Angles
	Constraint.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, Swing1LimitAngle);
	Constraint.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, Swing2LimitAngle);
	Constraint.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, TwistLimitAngle);

	Constraint.ProfileInstance.LinearLimit.Stiffness = SwingStiff;
	Constraint.ProfileInstance.LinearLimit.Damping = SwingDamp;
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
	case 0: Constraint.SetLinearXMotion(ELinearConstraintMotion::LCM_Free); break;
	case 1: Constraint.SetLinearXMotion(ELinearConstraintMotion::LCM_Limited); break;
	case 2: Constraint.SetLinearXMotion(ELinearConstraintMotion::LCM_Locked); break;
	}
	switch (YLimit)
	{
	case 0: Constraint.SetLinearYMotion(ELinearConstraintMotion::LCM_Free); break;
	case 1: Constraint.SetLinearYMotion(ELinearConstraintMotion::LCM_Limited); break;
	case 2: Constraint.SetLinearYMotion(ELinearConstraintMotion::LCM_Locked); break;
	}
	switch (ZLimit)
	{
	case 0: Constraint.SetLinearZMotion(ELinearConstraintMotion::LCM_Free); break;
	case 1: Constraint.SetLinearZMotion(ELinearConstraintMotion::LCM_Limited); break;
	case 2: Constraint.SetLinearZMotion(ELinearConstraintMotion::LCM_Locked); break;
	}

	Constraint.SetLinearLimitSize(Size);

	// Soft Limit?
	if (SoftLimit) Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 1;
	else Constraint.ProfileInstance.LinearLimit.bSoftConstraint = 0;

	Constraint.ProfileInstance.LinearLimit.Stiffness = SoftStiffness;
	Constraint.ProfileInstance.LinearLimit.Damping = SoftDampening;
}


bool FROSSpawnPhysicsConstraintServer::SpawnPhysicsConstraintActor(TSharedPtr<FROSBridgeSpawnPhysiscConstraintSrv::Request> Request)
{
	PhysicsConstraintDetails Details = Request->GetConstraintDetails();
	AActor* First = *Controller->IdToActorMap.Find(Details.GetFirstModel().GetId());
	AActor* Second = *Controller->IdToActorMap.Find(Details.GetSecondModel().GetId());

	if (!First || !Second)
	{
		//at least one of them could not be found.
		if (!First)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *Details.GetFirstModel().GetId());
		}

		if (!Second)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor with id:\"%s\" does not exist."), *Details.GetSecondModel().GetId());
		}

		return false;
	}

	//Actors do Exist. Set as ConstraintActors of component.
	UPhysicsConstraintComponent* ConstraintComponent = NewObject<UPhysicsConstraintComponent>(First);
	ConstraintComponent->ConstraintActor1 = First;
	ConstraintComponent->ConstraintActor2 = Second;

	//Set pose of Component
	ConstraintComponent->SetWorldLocationAndRotation(Request->GetLocation(), Request->GetQuat());

	SetupProfileInstance(&ConstraintComponent->ConstraintInstance.ProfileInstance, Details);
	SetupLinearLimits(ConstraintComponent->ConstraintInstance, Details);
	SetupAngularLimits(ConstraintComponent->ConstraintInstance, Details);
	
	//ConstraintComponent->SetConstrainedComponents(First->GetRootComponent(), NAME_None, Second->GetStaticMeshComponent(), NAME_None);
	return true;
}

void FROSSpawnPhysicsConstraintServer::SetupProfileInstance(FConstraintProfileProperties * ProfileInstance, PhysicsConstraintDetails Details)
{
	ProfileInstance->bDisableCollision = Details.GetDisabelCollision();
	ProfileInstance->bEnableProjection = Details.GetEnableProjection();
	ProfileInstance->ProjectionLinearTolerance = Details.GetProjectionLinearTolerance();
	ProfileInstance->ProjectionAngularTolerance = Details.GetProjectionAngularTolerance();
	ProfileInstance->bParentDominates = Details.GetParentDominates();
}

void FROSSpawnPhysicsConstraintServer::SetupAngularLimits(FConstraintInstance Instance, PhysicsConstraintDetails Details)
{
	unreal_world_control_msgs::AngularLimits AngLimit = Details.GetAngularLimits();

	if (AngLimit.GetUseAdvanced()) {
		//Advanced features will be set as well.
		SetAngularLimits(Instance,
			AngLimit.GetSwing1Motion(), AngLimit.GetSwing2Motion(), AngLimit.GetTwistMotion(),
			AngLimit.GetSwing1LimitAngle(), AngLimit.GetSwing2LimitAngle(), AngLimit.GetTwistLimitAngle(),
			AngLimit.GetSwingSoftConstrained(), AngLimit.GetTwistSoftConstrained(),
			AngLimit.GetSwingStiffness(), AngLimit.GetSwingDamping(),
			AngLimit.GetTwistStiffness(), AngLimit.GetTwistDamping()
		);
	}
	else
	{
		//Advanced features will be left default.
		SetAngularLimits(Instance,
			AngLimit.GetSwing1Motion(), AngLimit.GetSwing2Motion(), AngLimit.GetTwistMotion(),
			AngLimit.GetSwing1LimitAngle(), AngLimit.GetSwing2LimitAngle(), AngLimit.GetTwistLimitAngle());
	}

}

void FROSSpawnPhysicsConstraintServer::SetupLinearLimits(FConstraintInstance Instance, PhysicsConstraintDetails Details)
{
	unreal_world_control_msgs::LinearLimits LinLimit = Details.GetLinearLimits();

	if (LinLimit.GetUseAdvanced()) {
		//Advanced features will be set as well.
		SetLinearLimits(Instance,
			LinLimit.GetXMotion(), LinLimit.GetYMotion(), LinLimit.GetZMotion(),
			LinLimit.GetLimit(),
			LinLimit.GetSoftConstrained(), LinLimit.GetStiffness(), LinLimit.GetDamping()
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

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnPhysicsConstraintServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSBridgeSpawnPhysiscConstraintSrv::Request> Request_ =
		MakeShareable(new FROSBridgeSpawnPhysiscConstraintSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnPhysicsConstraintServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSBridgeSpawnPhysiscConstraintSrv::Request> SpawnPhysicsConstraintRequest =
		StaticCastSharedPtr<FROSBridgeSpawnPhysiscConstraintSrv::Request>(Request);

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = SpawnPhysicsConstraintActor(SpawnPhysicsConstraintRequest);

	}, TStatId(), NULL, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSBridgeSpawnPhysiscConstraintSrv::Response(ServiceSuccess));


	return TSharedPtr<FROSBridgeSrv::SrvResponse>();
}
