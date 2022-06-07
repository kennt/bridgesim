/*****
 * test_bridge_sim.cc
 * 
 * See LICENSE for details.
 *
 *****/

#include "external/catch.hpp"

#include "bridge_sim.h"
#include "config.h"

TEST_CASE("bridge_sim", "[model]")
{
    BridgeSim::Model model;
    BridgeSim::State state;
    BridgeSim::Person person;
    BridgeSim::Bridge bridge;
    BridgeSim::Move move;

    bridge.m_id = 0;
    bridge.m_length = 100;
    state.m_bridges.push_back(bridge);

    bridge.m_id = 1;
    bridge.m_length = 200;
    state.m_bridges.push_back(bridge);

    person.m_id = 0;
    person.m_speed = 10;
    person.m_name = "A";
    person.m_cost.assign({10, 20, 30});
    person.m_bridge = 0;
    state.m_persons.push_back(person);

    person.m_id = 0;
    person.m_speed = 20;
    person.m_name = "B";
    person.m_cost.assign({5, 10, 15});
    person.m_bridge = 0;
    state.m_persons.push_back(person);

    // test moving 10,20 from 0 -> 1
    model.initialize(state);

    REQUIRE( model.currentBridge() == 0 );
    REQUIRE( model.currentTime() == 0 );
    move.m_from = 0;
    move.m_to = 1;
    move.m_person1 = 0;
    move.m_person2 = 1;
    model.execute(move);
    REQUIRE( model.currentBridge() == 1 );
    REQUIRE( model.currentTime() == 10 );
    REQUIRE( model.personAt(0).m_bridge == 1);
    REQUIRE( model.personAt(1).m_bridge == 1);

    // test moving 10 from 1 -> 0
    REQUIRE( model.currentBridge() == 1 );
    REQUIRE( model.currentTime() == 10 );
    move.m_from = 1;
    move.m_to = 0;
    move.m_person1 = 0;
    move.m_person2 = BridgeSim::PERSON_NONE;
    model.execute(move);
    REQUIRE( model.currentBridge() == 0 );
    REQUIRE( model.currentTime() == 20 );
    REQUIRE( model.personAt(0).m_bridge == 0);
    REQUIRE( model.personAt(1).m_bridge == 1);
}

TEST_CASE("bridge-end", "[model]")
{
    BridgeSim::Model model;
    BridgeSim::State state;
    BridgeSim::Person person;
    BridgeSim::Bridge bridge;
    BridgeSim::Move move;

    bridge.m_id = 0;
    bridge.m_length = 100;
    state.m_bridges.push_back(bridge);

    person.m_id = 0;
    person.m_speed = 10;
    person.m_name = "A";
    person.m_cost.assign({10, 20, 30});
    person.m_bridge = 0;
    state.m_persons.push_back(person);

    person.m_id = 1;
    person.m_speed = 20;
    person.m_name = "B";
    person.m_cost.assign({5, 10, 15});
    person.m_bridge = 0;
    state.m_persons.push_back(person);

    // test moving 10,20 from 0 -> -1
    model.initialize(state);

    REQUIRE( model.currentBridge() == 0 );
    REQUIRE( model.currentTime() == 0 );
    move.m_from = 0;
    move.m_to = BridgeSim::BRIDGE_END;
    move.m_person1 = 0;
    move.m_person2 = 1;
    model.execute(move);
    REQUIRE( model.currentBridge() == BridgeSim::BRIDGE_END );
    REQUIRE( model.currentTime() == 10 );
    REQUIRE( model.personAt(0).m_bridge == BridgeSim::BRIDGE_END);
    REQUIRE( model.personAt(1).m_bridge == BridgeSim::BRIDGE_END);

    // test moving 10 from -1 -> 0
    REQUIRE( model.currentBridge() == BridgeSim::BRIDGE_END );
    REQUIRE( model.currentTime() == 10 );
    move.m_from = BridgeSim::BRIDGE_END;
    move.m_to = 0;
    move.m_person1 = 1;
    move.m_person2 = BridgeSim::PERSON_NONE;
    model.execute(move);
    REQUIRE( model.currentBridge() == 0 );
    REQUIRE( model.currentTime() == 15 );
    REQUIRE( model.personAt(0).m_bridge == BridgeSim::BRIDGE_END);
    REQUIRE( model.personAt(1).m_bridge == 0);

}


//$ TODO: test exceptions
