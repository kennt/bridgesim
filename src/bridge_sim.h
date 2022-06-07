/*****
 * bridge_sim.h
 * Main header file
 * 
 * See LICENSE for details.
 *
 *****/

#ifndef bridgesim_bridge_sim_h
#define bridgesim_bridge_sim_h

namespace BridgeSim
{

/*****
 * The following classes describe the initial state
 * of the objects in the system.
 * 
 * The context (bridge and person objects), are
 * created from the yaml file.
 *****/

typedef int     BridgeId;
typedef int     PersonId;

// This is used to that a person has crossed all bridges
const BridgeId  BRIDGE_END = -1;

const PersonId  PERSON_NONE = -1;


class SimException : public std::exception
{
public:
    SimException(const char *sz) :
        m_msg(sz)
    {}

    SimException(const std::string st) :
        m_msg(st)
    {}

    virtual const char * what() const throw()
    {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};


struct Person
{
    PersonId        m_id;
    std::string     m_name;
    float           m_speed;    // feet/min

    // Initial starting point for this person
    BridgeId        m_bridge;

    // The cost (length/speed) to cross a bridge
    // e.g.  m_cost[bridgeId]
    std::vector<float> m_cost;
};


struct Bridge
{
    BridgeId        m_id;
    int             m_length;   // feet
};


/*****************************
 * Describes the state of the system, the list of bridges
 * and a list of people.
 *****************************/
struct State
{
    // The bridges appear in order
    // bridge[0] is the first bridge to cross
    // bridge[n-1] is the last bridge to cross
    std::vector<Bridge> m_bridges;

    // This array is sorted by speed (ascending)
    std::vector<Person> m_persons;
};


struct Move
{
    BridgeId    m_from;
    BridgeId    m_to;
    PersonId    m_person1;
    PersonId    m_person2;

    bool  is_forwards() const
    {
        return (m_to == BRIDGE_END) || 
               (m_from != BRIDGE_END &&
                m_to > m_from);
    }
};


class Model
{
public:

    void initialize(const State& initialState, BridgeId startBridge=0)
    {
        m_currentTime = 0.0;
        m_currentBridge = startBridge;
        m_state = initialState;
        m_bridgeCompletionTimes.resize(m_state.m_bridges.size(), 0.0);
    }

    int numBridges() const
    {
        return static_cast<int>(m_state.m_bridges.size());
    }

    const Bridge bridgeAt(BridgeId id) const
    {
        return m_state.m_bridges[id];
    }

    int numPersons() const
    {
        return static_cast<int>(m_state.m_persons.size());
    }

    const Person personAt(PersonId id) const
    {
        return m_state.m_persons[id];
    }

    const State currentState() const
    {
        return m_state;
    }

    float       currentTime() const
    {
        return m_currentTime;
    }

    BridgeId    currentBridge() const
    {
        return m_currentBridge;
    }

    std::vector<float> bridgeCompletionTimes() const
    {
        return m_bridgeCompletionTimes;
    }

    void        execute(const Move& move);
    bool        is_finished() const;

private:
    State       m_state;
    float       m_currentTime;
    BridgeId    m_currentBridge;
    std::vector<float> m_bridgeCompletionTimes;
};


struct Statistics
{
    std::vector<float>      m_timeBridgeCompleted;
    float                   m_totalTimeElapsed;
};

}  /* namespace BridgeSim */

#endif /* bridgesim_bridge_sim_h */

