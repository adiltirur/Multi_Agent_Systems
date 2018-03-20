
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
#include "Representations/BehaviorControl/Role.h"

#include "Platform/SystemCall.h"
#include "Platform/Time.h"
#include <mutex>
#include <Representations/spqr_representations/UtilityShare.h>


MODULE(ContextCoordinator,
       {
           ,
           REQUIRES(GameInfo),
           REQUIRES(OpponentTeamInfo),
           REQUIRES(OwnTeamInfo),
           REQUIRES(RobotInfo),
           REQUIRES(RobotPose),
           REQUIRES(BallModel),
           REQUIRES(FrameInfo),
           REQUIRES(FallDownState),
           REQUIRES(TeamData),
           REQUIRES(UtilityShare),
           PROVIDES(Role),
       });

class ContextCoordinator : public ContextCoordinatorBase {
private:
    unsigned int timeSinceRoleChange = Time::getCurrentSystemTime() - 10000;

    bool BallFound(float timer);

    bool robotReady4Interchange();

    std::vector<int> getPlayersNumbers();

    float getRobotCalculatedInfo(int playerNumber, Role::RoleType role);

public:
    void update(Role &role);

    ContextCoordinator();
};
