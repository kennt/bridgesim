/*****
 * strategy.cc
 * 
 * Functions for converting the YAML file into the State class.
 * 
 * See LICENSE for details.
 *
 *****/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <utility>
#include <vector>

#include "bridge_sim.h"
#include "config.h"
#include "strategy.h"

namespace BridgeSim {

#if 0
std::vector<Move> generateNextMoves(const Model& model)
{
    std::vector<Move>   nextMoves;
    Move                move;
    BridgeId            from;
    BridgeId            to;

    // Generate all forward moves
    if (model.currentBridge() != BRIDGE_END)
    {
        from = model.currentBridge();
        if (from == model.numBridges()-1)
            to = BRIDGE_END;
        else
            to = from + 1;

        for (int i=0; i<model.numPersons()-1; i++)
        {
            if (model.personAt(i).m_bridge != from)
                continue;
            for (int j=i+1; j<model.numPersons(); j++)
            {
                if (model.personAt(j).m_bridge != from)
                    continue;
                move.m_person1 = i;
                move.m_person2 = j;
                move.m_from = from;
                move.m_to = to;
                nextMoves.emplace_back(move);
            }
        }
        // Edge case, there may only be a single user
        // waiting at this bridge.  So we can move this
        // user foward.
        if (nextMoves.size() == 0)
        {
            for (int i=0; i<model.numPersons(); i++)
            {
                if (model.personAt(i).m_bridge == from)
                {
                    move.m_person1 = i;
                    move.m_person2 = PERSON_NONE;
                    move.m_from = from;
                    move.m_to = to;
                    nextMoves.emplace_back(move);
                    break;
                }
            }
        }

    }

    // Generate all backward moves
    if (model.currentBridge() != 0)
    {
        from = model.currentBridge();
        if (from == BRIDGE_END)
            to = model.numBridges() - 1;
        else
            to = from - 1;

        for (int i=0; i<model.numPersons(); i++)
        {
            if (model.personAt(i).m_bridge != from)
                continue;
            move.m_person1 = i;
            move.m_person2 = PERSON_NONE;
            move.m_from = from;
            move.m_to = to;
            nextMoves.emplace_back(move);
        }
    }

    // We should always be generating moves
    assert(!nextMoves.empty());

    return nextMoves;
}

void runSolveSimulation(const State& initialState, Statistics& results)
{
    std::list<std::pair<Model, Move>> moves;
    Model   model;
    float   shortestTime = std::numeric_limits<float>::max();
    Model   bestModel;

    model.initialize(initialState);

    results.m_totalTimeElapsed = 0.0;

    // Initialize the moves list
    auto nextMoves = generateNextMoves(model);
    for (const auto& move : nextMoves)
    {
        moves.emplace_back(model, move);
    }

    std::cout << "moves size: " << moves.size() << std::endl;

    while (!moves.empty())
    {
        // Take next move off the list
        auto next = moves.front();
        moves.pop_front();

        auto currentModel = next.first;
        auto currentMove = next.second;


        // Possible pruning, can stop if over shortest time
        if (currentModel.currentTime() > shortestTime)
            continue;

        // Run the move
        currentModel.move(currentMove);

        // Update limits
        if (currentModel.is_finished())
        {
            if (currentModel.currentTime() < shortestTime)
            {
                shortestTime = currentModel.currentTime();
                bestModel = currentModel;
            }
            continue;
        }

        // Generate next set of moves
        nextMoves = generateNextMoves(currentModel);
        for (const auto& move : nextMoves)
        {
            moves.emplace_back(currentModel, move);
        }
        std::cout << "moves size: " << moves.size() << std::endl;
    }

    // Generate the statistics for the best model
}
#endif

/*************************************************
 * LocalStrategy
 * 
 * Implementation of the "local" strategy.
 * This is local in that we only look at the
 * current bridge and the before/after bridges.
 * 
 * We also move all of the people from the bridge
 * before moving to the next bridge.
 *************************************************/

void LocalStrategy::initialize(const State& state, BridgeId startBridge)
{
    m_model.initialize(state, startBridge);
}

bool LocalStrategy::is_finished() const
{
    return m_model.is_finished();
}


Move LocalStrategy::generateNextMove()
{
    Move move;
    std::vector<std::vector<Person>> people;
    std::vector<int> bridgeCounts(m_model.numBridges());
    BridgeId        current = m_model.currentBridge();
    std::vector<Person>  personsAtEnd;
    int             fastest;

    // Organize the people into groups based on the bridge
    // (to get counts and to find the slowest/fastest people)
    // The people in the model are already sorted, so these
    // smaller groups will be sorted also.
    people.resize(m_model.numBridges());
    for (int i=0; i<m_model.numPersons(); i++)
    {
        auto &p = m_model.personAt(i);
        if (p.m_bridge != BRIDGE_END)
        {
            people[p.m_bridge].emplace_back(p);
        }
        else
        {
            personsAtEnd.emplace_back(p);
        }
    }

    // if there are people at the previous bridge, move
    // backwards
    if ((current == BRIDGE_END && !people.back().empty()) ||
        (current > 0 && !people[current-1].empty()))
    {
        move.m_from = current;
        move.m_to = (current == BRIDGE_END ? m_model.numBridges()-1 : current-1);

        // get the fastest person
        std::vector<Person>  *vPersons;
        if (current == BRIDGE_END)
            vPersons = &personsAtEnd;
        else
            vPersons = &people[current];

        move.m_person1 = vPersons->back().m_id;
        move.m_person2 = PERSON_NONE;
    }
    else
    {
        if (people[current].size() == 1)
        {
            // if there is only a single person here,
            // move them forward.
            assert(current != BRIDGE_END);
            move.m_from = current;
            move.m_to = (current == m_model.numBridges()-1 ? BRIDGE_END : current+1);
            move.m_person1 = people[current].front().m_id;
            move.m_person2 = PERSON_NONE;
        }
        else
        {
            // If the next bridge has a faster person, move 2 slowest
            // Else move the fastest+slowest people forward
            int fastestNextBridge;
            int next = (current == m_model.numBridges()-1 ? BRIDGE_END : current+1);

            // Find the fastest person at the next bridge
            std::vector<Person>  *vPersons;
            if (next == BRIDGE_END)
                vPersons = &personsAtEnd;
            else
                vPersons = &people[next];

            if (vPersons->size() > 0)
                fastestNextBridge = vPersons->back().m_id;
            else
                fastestNextBridge = PERSON_NONE;

            fastest = people[current].back().m_id;

            move.m_from = current;
            move.m_to = next;

            // Always take the slowest person
            move.m_person1 = people[current][0].m_id;

            // is there a faster person at the next bridge?
            // if so, send the second slowest person (and come
            // back with the faster person)
            // if not, send the fastest person.
            if (fastestNextBridge == PERSON_NONE ||
                (m_model.personAt(fastestNextBridge).m_speed >
                 m_model.personAt(fastest).m_speed))
            {
                move.m_person2 = people[current][1].m_id;
            }
            else
            {
                move.m_person2 = people[current].back().m_id;                
            }
        }
    }

    return move;
}

void LocalStrategy::execute(const Move& move)
{
    m_model.execute(move);
}

float LocalStrategy::elapsedTime() const
{
    return m_model.currentTime();
}

Statistics LocalStrategy::results()
{
    Statistics stats;
    stats.m_totalTimeElapsed = m_model.currentTime();
    stats.m_timeBridgeCompleted = m_model.bridgeCompletionTimes();
    return stats;
}



};  // namespace BridgeSim
