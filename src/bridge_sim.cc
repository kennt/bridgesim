/*****
 * bridge_sim.cc
 * 
 * Contains the basic classes used by the simulation.
 * 
 * See LICENSE for details.
 *
 *****/

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "bridge_sim.h"
#include "config.h"
#include "strategy.h"

namespace BridgeSim
{

void Model::execute(const Move& move)
{
    PersonId    p1 = move.m_person1;
    PersonId    p2 = move.m_person2;

    // Check that the bridges have valid values
    if (move.m_from < BRIDGE_END ||
        move.m_from >= static_cast<int>(m_state.m_bridges.size()))
    {
        std::ostringstream os;
        os << "Out-of-bounds from-bridge value: " << move.m_from;
        throw SimException(os.str());
    }
    if (move.m_to < BRIDGE_END ||
        move.m_to >= static_cast<int>(m_state.m_bridges.size()))
    {
        std::ostringstream os;
        os << "Out-of-bounds to-bridge value: " << move.m_from;
        throw SimException(os.str());
    }
    if (move.m_from == move.m_to)
    {
        std::ostringstream os;
        os << "Bridge values cannot be the same: " << move.m_from;
        throw SimException(os.str());
    }
    // Check that the bridges are next to each other
    if (move.m_from == BRIDGE_END &&
        (move.m_to != static_cast<int>(m_state.m_bridges.size())-1))
    {
        std::ostringstream os;
        os << "The bridges must be next to each other: "
           << move.m_from << " to " << move.m_to;
        throw SimException(os.str());
    }
    if (move.m_to == BRIDGE_END &&
        (move.m_from != static_cast<int>(m_state.m_bridges.size())-1))
    {
        std::ostringstream os;
        os << "The bridges must be next to each other: "
           << move.m_from << " to " << move.m_to;
        throw SimException(os.str());
    }
    if ( (move.m_from != BRIDGE_END) &&
         (move.m_to != BRIDGE_END) &&
         std::abs(move.m_from - move.m_to) != 1)
    {
        std::ostringstream os;
        os << "The bridges must be next to each other: "
           << move.m_from << " to " << move.m_to;
        throw SimException(os.str());
    }

    // Check that the people have valid values
    if (p1 < PERSON_NONE || p1 >= static_cast<int>(m_state.m_persons.size()))
    {
        std::ostringstream os;
        os << "Invalid value for person1: " << p1;
        throw SimException(os.str());
    }
    if (p2 < PERSON_NONE || p2 >= static_cast<int>(m_state.m_persons.size()))
    {
        std::ostringstream os;
        os << "Invalid value for person2: " << p2;
        throw SimException(os.str());
    }
    // Check that the people are at the bridge
    if (p1 != PERSON_NONE && m_state.m_persons[p1].m_bridge != move.m_from)
    {
        std::ostringstream os;
        os << "person1:" << p1 << " is not at bridge:" << move.m_from;
        throw SimException(os.str());
    }
    if (p2 != PERSON_NONE && m_state.m_persons[p2].m_bridge != move.m_from)
    {
        std::ostringstream os;
        os << "person2:" << p1 << " is not at bridge:" << move.m_from;
        throw SimException(os.str());
    }
    if (p1 == p2)
    {
        std::ostringstream os;
        os << "cannot move the same person twice:" << p1;
        throw SimException(os.str());
    }
    // Check that moving backward uses only a single person
    if (!move.is_forwards() && (p1 != PERSON_NONE && p2 != PERSON_NONE))
    {
        std::ostringstream os;
        os << "when moving backwards, must move only a single person:"
           << p1 << "," << p2 << "  from:" << move.m_from << " to:" << move.m_to;
        throw SimException(os.str());
    }

    // The move must match the current state
    if (m_currentBridge != move.m_from)
    {
        std::ostringstream os;
        os << "the move does not agree with the model:"
           << "move-bridge:" << move.m_from << " model-bridge:" << m_currentBridge;
        throw SimException(os.str());        
    }

    // Actually move the person
    float   cost1 = 0.0;
    float   cost2 = 0.0;
    BridgeId bridgeToCross;

    bridgeToCross = move.is_forwards() ? move.m_from : move.m_to;

    if (p1 != PERSON_NONE)
    {
        cost1 = m_state.m_persons[p1].m_cost[bridgeToCross];
        m_state.m_persons[p1].m_bridge = move.m_to;
    }

    if (p2 != PERSON_NONE)
    {
        cost2 = m_state.m_persons[p2].m_cost[bridgeToCross];
        m_state.m_persons[p2].m_bridge = move.m_to;
    }

    // Move at the slower speed (higher cost)
    m_currentTime += std::max(cost1, cost2);
    m_currentBridge = move.m_to;

    if (move.m_from != BRIDGE_END)
    {
        // Always update the time of transit, so that
        // it holds the time the bridge was last used.
        m_bridgeCompletionTimes[move.m_from] = currentTime();
    }
}


bool Model::is_finished() const
{
    // We are finished when all of the people are at the last bridge
    for (const auto& p : m_state.m_persons)
    {
        if (p.m_bridge != BRIDGE_END)
            return false;
    }
    return true;
}

} // namespace: BridgeSim

