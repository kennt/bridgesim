/*****
 * test_strategy.cc
 * 
 * See LICENSE for details.
 *
 *****/

#include "external/catch.hpp"

#include "bridge_sim.h"
#include "config.h"
#include "strategy.h"

TEST_CASE("LocalStrategy-is_finished", "[local_strategy]")
{
    BridgeSim::State state;
    BridgeSim::Person person;
    BridgeSim::Bridge bridge;

    BridgeSim::LocalStrategy local;

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
    person.m_bridge = BridgeSim::BRIDGE_END;
    state.m_persons.push_back(person);

    local.initialize(state);

    REQUIRE( local.is_finished() );

    // Reset the state
    state.m_persons[0].m_bridge = 0;
    local.initialize(state);


    REQUIRE( !local.is_finished() );
}

TEST_CASE("LocalStrategy-generateNextMove", "[local_strategy]")
{
    BridgeSim::State state;
    BridgeSim::Person person1, person2, person3, person4;
    BridgeSim::Bridge bridge;

    bridge.m_id = 0;
    bridge.m_length = 100;
    state.m_bridges.push_back(bridge);

    bridge.m_id = 1;
    bridge.m_length = 200;
    state.m_bridges.push_back(bridge);

    person1.m_id = 0;
    person1.m_speed = 5;
    person1.m_name = "A";
    person1.m_cost.assign({20, 30});
    person1.m_bridge = 0;

    person2.m_id = 1;
    person2.m_speed = 10;
    person2.m_name = "B";
    person2.m_cost.assign({10, 20});
    person2.m_bridge = 1;

    person3.m_id = 2;
    person3.m_speed = 20;
    person3.m_name = "C";
    person3.m_cost.assign({5, 10});
    person3.m_bridge = 0;

    person4.m_id = 3;
    person4.m_speed = 30;
    person4.m_name = "D";
    person4.m_cost.assign({3.333, 6.66});
    person4.m_bridge = 1;

    // backwards - people at previous bridge
    SECTION("Moving backwards")
    {
        BridgeSim::LocalStrategy local;
        state.m_persons.push_back(person1);
        state.m_persons.push_back(person2);
    
        local.initialize(state, 1);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == 1 );
        REQUIRE( move.m_to == 0 );
        REQUIRE( move.m_person1 == 1 );
        REQUIRE( move.m_person2 == BridgeSim::PERSON_NONE );
        REQUIRE( !move.is_forwards() );
    }

    SECTION("Moving backwards at the end")
    {
        BridgeSim::LocalStrategy local;
        person1.m_bridge = BridgeSim::BRIDGE_END;
        state.m_persons.push_back(person1);
        state.m_persons.push_back(person2);
    
        local.initialize(state, BridgeSim::BRIDGE_END);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == BridgeSim::BRIDGE_END );
        REQUIRE( move.m_to == 1 );
        REQUIRE( move.m_person1 == 0 );
        REQUIRE( move.m_person2 == BridgeSim::PERSON_NONE );
        REQUIRE( !move.is_forwards() );
    }

    SECTION("Moving forward - one person waiting at a bridge")
    {
        BridgeSim::LocalStrategy local;
        state.m_persons.push_back(person1);
        state.m_persons.push_back(person2);
    
        local.initialize(state, 0);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == 0 );
        REQUIRE( move.m_to == 1 );
        REQUIRE( move.m_person1 == 0 );
        REQUIRE( move.m_person2 == BridgeSim::PERSON_NONE );
        REQUIRE( move.is_forwards() );
    }

    SECTION("Moving forward - next bridge is empty")
    {
        BridgeSim::LocalStrategy local;
        state.m_persons.push_back(person1);
    
        local.initialize(state, 0);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == 0 );
        REQUIRE( move.m_to == 1 );
        REQUIRE( move.m_person1 == 0 );
        REQUIRE( move.m_person2 == BridgeSim::PERSON_NONE );
        REQUIRE( move.is_forwards() );
    }

    SECTION("Moving forward - next bridge (at the end) is empty")
    {
        BridgeSim::LocalStrategy local;
        state.m_persons.push_back(person2);
    
        local.initialize(state, 1);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == 1 );
        REQUIRE( move.m_to == BridgeSim::BRIDGE_END );
        REQUIRE( move.m_person1 == 1 );
        REQUIRE( move.m_person2 == BridgeSim::PERSON_NONE );
        REQUIRE( move.is_forwards() );
    }

    SECTION("Moving forward - next bridge has faster person")
    {
        BridgeSim::LocalStrategy local;
        state.m_persons.push_back(person1);
        person2.m_bridge = 0;
        state.m_persons.push_back(person2);
        state.m_persons.push_back(person3);
        state.m_persons.push_back(person4);
    
        local.initialize(state, 0);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == 0 );
        REQUIRE( move.m_to == 1 );
        REQUIRE( move.m_person1 == 0 );
        REQUIRE( move.m_person2 == 1 );
        REQUIRE( move.is_forwards() );
    }

    SECTION("Moving forward - this bridge has faster person")
    {
        BridgeSim::LocalStrategy local;
        state.m_persons.push_back(person1);
        person2.m_bridge = 0;
        state.m_persons.push_back(person2);
        state.m_persons.push_back(person3);
        person4.m_speed = 1;
        person4.m_cost.assign({100, 200});
        state.m_persons.push_back(person4);
    
        local.initialize(state, 0);
        auto move = local.generateNextMove();
        REQUIRE( move.m_from == 0 );
        REQUIRE( move.m_to == 1 );
        REQUIRE( move.m_person1 == 0 );
        REQUIRE( move.m_person2 == 2 );
        REQUIRE( move.is_forwards() );
    }

}


//$ TODO: test exceptions
