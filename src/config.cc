/*****
 * config.cc
 * 
 * See LICENSE for details.
 *
 *****/

#include <string>
#include <vector>

#define RYML_SINGLE_HDR_DEFINE_NOW
#include "external/ryml_all.hpp"

#include "bridge_sim.h"
#include "config.h"

namespace BridgeSim {

// Reads in the yaml and fills in the BridgeSim::State
//$ TODO: need to update to throw an exception on unexpected output
//$ TODO: versioning?
State loadInitialState(const std::string &contents)
{
    State state;

    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(contents));

    ryml::NodeRef root = tree.rootref();

    //$ TODO: make this throw exceptions if unexpected yaml
    // for now, just assert

    assert( root.is_map() );
    assert( root["bridges"].is_seq() );

    BridgeId current_bridge = 0;
    PersonId current_person = 0;
    for (const auto &child : root["bridges"])
    {
        assert( child.is_map() );
        Bridge bridge;

        bridge.m_id = current_bridge;
        bridge.m_length = std::stoi(child["length"].val().str);
        state.m_bridges.emplace_back(bridge);
        current_bridge++;
    }

    current_bridge = 0;
    current_person = 0;

    for (const auto &child : root["bridges"])
    {
        Person person;
        assert( child["people"].is_seq() );
        for (const auto &member : child["people"])
        {
            person.m_name.assign(member["name"].val().str, member["name"].val().len);
            person.m_speed = std::stof(member["speed"].val().str);
            person.m_bridge = current_bridge;

            person.m_cost.resize(state.m_bridges.size());
            for (size_t i=0; i<state.m_bridges.size(); i++)
            {
                person.m_cost[i] = ((float) state.m_bridges[i].m_length / person.m_speed);
            }

            state.m_persons.emplace_back(person);
            current_person++;
        }

        // Sort the persons array (this is helpful in the strategy classes)
        std::sort(state.m_persons.begin(), state.m_persons.end(),
                [](Person const&lhs, Person const &rhs) {
                        return lhs.m_speed < rhs.m_speed;
                });

        // Set the ids correctly (since they are used as indexes
        // into the persons array).
        for (int i=0; i<static_cast<int>(state.m_persons.size()); i++)
        {
            state.m_persons[i].m_id = i;
        }

        current_bridge++;
    }

    return state;
}

};
