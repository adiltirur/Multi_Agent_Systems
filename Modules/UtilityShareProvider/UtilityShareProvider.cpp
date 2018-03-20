#include "UtilityShareProvider.h"

#include <unistd.h>
#include <iostream>
#include <math.h>
//#define ROTATE_SPEED_DEGREES (float)(0.2f * 180.f / 3.14f) // degree/sec

float UtilityShareProvider::r2D(float x) {
    return (x * 180) / M_PI;
}

Vector2f UtilityShareProvider::rel2Glob(float x, float y) {
    Vector2f result;
    float rho = sqrt(pow(x, 2) + pow(y, 2));

    result.x() = theRobotPose.translation.x() + (rho * cos(theRobotPose.rotation + atan2(y, x)));
    result.y() = theRobotPose.translation.y() + (rho * sin(theRobotPose.rotation + atan2(y, x)));

    return result;
}

float UtilityShareProvider::angleToTarget(float x, float y) {
    //gets the relative position of the point to go for the robot
    Pose2f relativePosition = glob2Rel(x, y);
    //std::cerr << "y relativa: "<< relativePosition.translation.y() << ", x relativa: "<<relativePosition.translation.x() << std::endl;
    return atan2f(relativePosition.translation.y(), relativePosition.translation.x());
    //return glob2Rel(x, y).translation.angle();
}

Vector2f UtilityShareProvider::glob2Rel(float x, float y) {
    Vector2f result;
    float tempX = x - theRobotPose.translation.x();
    float tempY = y - theRobotPose.translation.y();

    result.x() = tempX * cos(theRobotPose.rotation) + tempY * sin(theRobotPose.rotation);
    result.y() = -tempX * sin(theRobotPose.rotation) + tempY * cos(theRobotPose.rotation);

    return result;
}

/**
 * Computes distance from robot to point on field
 * @param x x-coordinate
 * @param y y-coordinate
 * @return distance in mm
 */
float UtilityShareProvider::distanceTo(float x, float y) {
    return sqrt(pow(std::abs(theRobotPose.translation.x() - x), 2) + pow(std::abs(theRobotPose.translation.y() - y), 2));
}

/**
 * Calculates current robot state value relatively to ball distance
 *
 * @return time required to reach position & angle in seconds
 */
float UtilityShareProvider::stateValue(Vector2f position, float attackAngle) {
    //int returnValue = 0;

  //float distance = distanceTo(position.x(), position.y());
  //  float rotationDifference = abs(theRobotPose.rotation.toDegrees() - attackAngle);

    return (distanceTo(position.x(), position.y())) / 220
           + (std::abs(theRobotPose.rotation.toDegrees() - attackAngle)) / ((float)(0.2f * 180.f / M_PI))
           + (Obstacle(position) ? 3.5f : 0.f);
}

bool UtilityShareProvider::Obstacle(Vector2f target) {

    float targetAngle = angleToTarget(target.x(), target.y());
    float targetDistance = distanceTo(target.x(), target.y());


    for (const auto obstacle : theObstacleModel.obstacles) {

        if (obstacle.type == Obstacle::goalpost)
            continue;

        float obstacleDistance = obstacle.center.norm();
        float obstacleAngle = obstacle.center.angle();

        if (targetDistance > obstacleDistance) {
            if (std::abs(targetAngle - obstacleAngle) < 0.5f) {
                return true;
            }
        }
    }

    return false;
}

UtilityShareProvider::UtilityShareProvider() {
    SPQR::ConfigurationParameters();
}


void UtilityShareProvider::update(UtilityShare &representation) {

    float robotAngle = theRobotPose.rotation.toDegrees();
    float extraAdd = 0.0f;
    if (theFallDownState.state != FallDownState::State::upright) {
        extraAdd = 250.0f;
    }

    extraAdd += (std::sqrt((float) theRobotInfo.number)) * 10;

    representation.striker_utility =
            stateValue(theLibCodeRelease.absBallPosition.translation, theLibCodeRelease.angleToGoal) + extraAdd;
    representation.jolly_utility =
            stateValue(theLibCodeRelease.jollyPosition, theLibCodeRelease.angleForJolly) + extraAdd;
    representation.def_utility =
            stateValue(theLibCodeRelease.defenderPosition, theLibCodeRelease.angleForDefender) + extraAdd;
    representation.supp_utility =
            stateValue(theLibCodeRelease.supporterPosition, theLibCodeRelease.angleForSupporter) + extraAdd;
    /*representation.goalieUtilityValue =
            stateValue(theLibCodeRelease.getGoalieCoverPosition.translation, theLibCodeRelease.isGoalieInAngle) +
            extraAdd;*/
    representation.searcher1_utility = stateValue(theLibCodeRelease.searcer_1Position, robotAngle) + extraAdd;
    representation.searcher2_utility = stateValue(theLibCodeRelease.searcer_2Position, robotAngle) + extraAdd;
    representation.searcher3_utility = stateValue(theLibCodeRelease.searcer_3Position, robotAngle) + extraAdd;
    representation.searcher4_utility = stateValue(theLibCodeRelease.searcer_4Position, robotAngle) + extraAdd;;

}

MAKE_MODULE(UtilityShareProvider, modeling)
