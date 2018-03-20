#include "ContextCoordinator.h"


ContextCoordinator::ContextCoordinator() {
    SPQR::ConfigurationParameters();
}

bool ContextCoordinator::BallFound(float timer) {
    if (theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen) < (timer)) return true;
    for (auto const &teammate : theTeamData.teammates) {
        if (theFrameInfo.getTimeSince(teammate.theBallModel.timeWhenLastSeen) < (timer)) return true;
    }
    return false;
}

bool ContextCoordinator::robotReady4Interchange() {
    for (auto const &teammate : theTeamData.teammates) {
        if (teammate.timeWhenLastPacketReceived == 0) return false;
    }
    return true;
}

float ContextCoordinator::getRobotCalculatedInfo(int playerNumber, Role::RoleType role) {
    if (playerNumber == theRobotInfo.number) {
        return theUtilityShare.getUtilityValueForRole(role);
    } else {
        for (auto const &teammate : theTeamData.teammates) {
            if (teammate.number == playerNumber) {
                return teammate.theUtilityShare.getUtilityValueForRole(role);
            }
        }
        return 0.f;
    }
}

std::vector<int> ContextCoordinator::getPlayersNumbers() {
    std::vector<int> players;
    players.push_back(theRobotInfo.number);
    for (auto const &teammate : theTeamData.teammates) {
        if (teammate.number != 1) {
            players.push_back(teammate.number);
        }
    }
    return players;
}


unsigned int lastPrintTime = 0;
bool timeToPrint;

void ContextCoordinator::update(Role &role) {

    timeToPrint = Time::getTimeSince(lastPrintTime) > 1000;
    if (timeToPrint) {
        lastPrintTime = Time::getCurrentSystemTime();
    }

    if (theRobotInfo.number == 1 && timeToPrint) {

        std::cout << std::endl;
        std::cout << "Player  " << theRobotInfo.number
                  << "\n---------------------------------------------- \n"
                  << " Stricker " << theUtilityShare.striker_utility
                  << "\n---------------------------------------------- \n"
                  << " Jolly " << theUtilityShare.jolly_utility
                  << "\n---------------------------------------------- 1n"
                  << " Supporter " << theUtilityShare.supp_utility
                  << "\n---------------------------------------------- \n"
                  << " Defender " << theUtilityShare.def_utility
                  << "\n---------------------------------------------- \n"
                  << std::endl;
        for (auto const &teammate : theTeamData.teammates) {
            std::cout << "Player " << teammate.number
                      << "\n---------------------------------------------- \n"
                      << " Stricker " << teammate.theUtilityShare.striker_utility
                      << "\n---------------------------------------------- \n"
                      << " Jolly " << teammate.theUtilityShare.jolly_utility
                      << "\n---------------------------------------------- \n"
                      << " Supporter " << teammate.theUtilityShare.supp_utility
                      << "\n---------------------------------------------- \n"
                      << " Defender " << teammate.theUtilityShare.def_utility
                      << "\n---------------------------------------------- \n" 
                      << std::endl;
        }
    }

    // Wait until first exchange of data
    if (!robotReady4Interchange() && timeToPrint) {
        std::cout << "Player " << theRobotInfo.number << " Not ready to attack" << std::endl;
        return;
    }

    // Simply assign goalie role to first robot
    if (theRobotInfo.number == 1) {
        role.role = Role::goalie;
        return;
    }

    Role::RoleType newRole = role.role;
    std::vector<Role::RoleType> roles;
    std::vector<int> players = getPlayersNumbers();

    if (BallFound(3000.f)) { //PLAYING CONTEXT

        roles = {/*Role::RoleType::goalie, */Role::RoleType::striker, Role::RoleType::defender,
                                             Role::RoleType::supporter, Role::RoleType::jolly};


    } else {   //SEARCH CONTEXT
        roles = {/*Role::RoleType::goalie, */Role::RoleType::searcher_1, Role::RoleType::searcher_2,
                                             Role::RoleType::searcher_3, Role::RoleType::searcher_4};
    }

    for (auto const &roleItem : roles) {

        std::sort(players.begin(), players.end());

        float currentBestUtility = ROLE_UTILITY_MAX;
        int currentBestPlayer = 0;

        // Search for best match for current iteration role
        for (auto const &playerNumber : players) {
            float playerUtilityValue = getRobotCalculatedInfo(playerNumber, roleItem);
            if (playerUtilityValue < currentBestUtility) {
                currentBestUtility = playerUtilityValue;
                currentBestPlayer = playerNumber;
            }
        }

        if (currentBestPlayer == theRobotInfo.number) {
            newRole = roleItem;
            break;
        } else {

            //Remove player from list
            for (unsigned int i = 0; i < players.size(); i++) {
                if (players.at(i) == currentBestPlayer) {
                    players.erase(players.begin() + i);
                }
            }
        }
    }

    if (newRole != role.role && Time::getTimeSince(timeSinceRoleChange) > 10000) {
        role.lastRole = role.role;
        role.role = newRole;
        timeSinceRoleChange = Time::getCurrentSystemTime();

        std::cout << "Player # " << theRobotInfo.number
                  << " switched from " << role.lastRole
                  << " to " << newRole
                  << std::endl;
    }
}

MAKE_MODULE(ContextCoordinator, behaviorControl)
