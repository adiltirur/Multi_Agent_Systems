
#pragma once

#include <iostream>
#include <set>
#include "Tools/Module/Module.h"
#include "Tools/Math/Transformation.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/RobotInfo.h"
#include "Representations/Infrastructure/GameInfo.h"
#include "Representations/Infrastructure/TeamInfo.h"
#include "Representations/Sensing/FallDownState.h"
#include "Representations/Communication/TeamData.h"
#include "Representations/spqr_representations/ConfigurationParameters.h"
#include "Representations/spqr_representations/OurDefinitions.h"
#include "Representations/spqr_representations/UtilityShare.h"
#include <Representations/BehaviorControl/Libraries/LibCodeRelease.h>
#include "Representations/Modeling/ObstacleModel.h"

#include "Platform/SystemCall.h"
#include "Platform/Time.h"
#include <mutex>

MODULE(UtilityShareProvider,
       {
           ,
           REQUIRES(GameInfo),
           REQUIRES(OpponentTeamInfo),
           REQUIRES(OwnTeamInfo),
           REQUIRES(RobotInfo),
           REQUIRES(RobotPose),
           REQUIRES(BallModel),
           REQUIRES(ObstacleModel),
           REQUIRES(FallDownState),
           REQUIRES(TeamData),
           REQUIRES(LibCodeRelease),
           PROVIDES(UtilityShare),
       });

class UtilityShareProvider : public UtilityShareProviderBase {
private:

public:
    void update(UtilityShare &us);

    UtilityShareProvider();

private:

    Vector2f glob2Rel(float x, float y);

    float angleToTarget(float x, float y);

    Vector2f rel2Glob(float x, float y);

    float r2D(float x);

    float stateValue(Vector2f position, float attackAngle);

    float distanceTo(float x, float y);

    bool Obstacle(Vector2f target);
};
