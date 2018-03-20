
#include "UtilityShare.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Tools/Module/Blackboard.h"

#include <iostream>

//#define PLOT_SINGE_TSL(name) \
//  PLOT("representation:FieldFeatureOverview:TimeSinceLast:" #name, theFrameInfo.getTimeSince(statuses[name].lastSeen));
//

void UtilityShare::operator >> (BHumanMessage& m) const
{ 
  
  //std::cout<<passData<<std::endl;
  //std::cout<<def_utility<<std::endl;	
  m.theBHumanArbitraryMessage.queue.out.bin << def_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << supp_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << jolly_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << striker_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << searcher1_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << searcher2_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << searcher3_utility;
  m.theBHumanArbitraryMessage.queue.out.bin << searcher4_utility;


  
  m.theBHumanArbitraryMessage.queue.out.finishMessage(this->id());
}

bool UtilityShare::handleArbitraryMessage(InMessage& m, const std::function<unsigned(unsigned)>& toLocalTimestamp)
{
  ASSERT(m.getMessageID() == id());


  m.bin >> def_utility;
  m.bin >> supp_utility;
  m.bin >> jolly_utility;
  m.bin >> striker_utility;
  m.bin >> searcher1_utility;
  m.bin >> searcher2_utility;
  m.bin >> searcher3_utility;
  m.bin >> searcher4_utility;

  return true;
}

float UtilityShare::getUtilityValueForRole(Role::RoleType role) const {
  switch (role) {
    case Role::RoleType::defender:
      return def_utility;
    case Role::RoleType::striker:
      return striker_utility;
    case Role::RoleType::jolly:
      return jolly_utility;
    case Role::RoleType::supporter:
      return supp_utility;
    case Role::RoleType::searcher_1:
      return searcher1_utility;
    case Role::RoleType::searcher_2:
      return searcher2_utility;
    case Role::RoleType::searcher_3:
      return searcher3_utility;
    case Role::RoleType::searcher_4:
      return searcher4_utility;
    default:
      return ROLE_UTILITY_MAX;
  }
}
