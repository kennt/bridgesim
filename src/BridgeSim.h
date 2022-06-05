/*****
 * BridgeSim.h
 * Main header file
 * 
 * See LICENSE for details.
 *
 *****/

#ifndef bridgesim_BridgeSim_h
#define bridgesim_BridgeSim_h


/*****
 * Classes that describe the current state
 * of the system (the bridges, the people, and
 * the locations of the people).
 *****/

struct Person
{
    std::string m_name;
    int         m_speed;

    // This is bridge that the person is to cross
    int         m_bridge;
};

struct Bridge
{
    std::string m_name;
    int         m_length;

    // This is a min heap
    std::vector<int>    m_persons;
};

class Context
{
public:
    const Bridge&   bridgeAt(int index) const;
    const Person&   personAt(int index) const;

    int             elapsedTime() const;
    void            move(int idBridgeFrom, int idBridgeTo,
                         int idUser1, int idUser2);

private:
    int         m_elapsedTime;
    int         m_currentBridge;

    // The bridges appear in order
    // bridge 0 is the first bridge to cross
    // bridge n-i is the last bridge to cross
    std::vector<Bridge> m_bridges;

    // This is a sorted vector (sorted by speed)
    std::vector<Person> m_persons;
};


/*****
 * The following classes are used by the strategy
 * and describes the changes that are made to a context.
 *****/

class Move
{
public:
    int     m_from;
    int     m_to;
    int     m_person1;
    int     m_person2;
};


class IStrategy
{
public:
    virtual Move getNextMove(const Context &context) = 0;
    virtual void execute(Context &context, const Move &move) = 0;
};


#endif /* bridgesim_BridgeSim_h */

