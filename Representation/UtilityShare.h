
#pragma once


#include "Representations/Communication/BHumanTeamMessageParts/BHumanMessageParticle.h"
#include <Representations/BehaviorControl/Role.h>
#include <limits>

#define ROLE_UTILITY_MAX std::numeric_limits<float>::max()

STREAMABLE(UtilityShare, COMMA
        public PureBHumanArbitraryMessageParticle<idUtilityShare>
{
        float getUtilityValueForRole(Role::RoleType role) const;
        /** BHumanMessageParticle functions */
        void operator>> (BHumanMessage& m) const override;
        bool handleArbitraryMessage(InMessage& m, const std::function<unsigned(unsigned)>& toLocalTimestamp) override;

        float x,//Just ignore this

           (float) def_utility,
           (float) supp_utility,
           (float) jolly_utility,
           (float) striker_utility,
           (float) searcher1_utility,
           (float) searcher2_utility,
           (float) searcher3_utility,
           (float) searcher4_utility,

           });
