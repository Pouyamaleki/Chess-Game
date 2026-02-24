#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "TimeBomb.h"
#include "SeasonEvent.h"
#include "MassacreAbility.h"
#include "Mission.h"
#include <random>
#include <memory>

enum class EventType {
    None,
    TimeBomb,
    SeasonChange,
    Massacre,
    NewMission
};

struct GameEvent {
    EventType type;
    std::string description;
    bool isActive;
    int duration;
    int turnActivated;
    
    GameEvent() : type(EventType::None), isActive(false), duration(0), turnActivated(0) {}
};

class EventManager {
private:
    std::unique_ptr<TimeBomb> timeBomb;
    std::unique_ptr<SeasonEvent> seasonEvent;
    std::unique_ptr<MassacreAbility> massacreAbility;
    std::unique_ptr<MissionManager> missionManager;
    
    GameEvent currentEvent;
    std::mt19937 rng;
    
    int turnCounter;
    const int EVENT_CHECK_INTERVAL = 3;
    const int EVENT_CHANCE = 30;
    
public:
    // constructor
    EventManager();
    
    // check if we should trigger a random event
    void checkAndTriggerEvent(int currentTurn, Board& board, Color currentPlayer);
    
    // apply the effects of current event
    void applyEventEffects(Board& board, Color currentPlayer);
    
    // update event state each turn
    void updateEvents(int currentTurn, Board& board, const Move& lastMove);
    
    // clear the current event
    void clearCurrentEvent();
    
    // check if there's an active event
    bool hasActiveEvent() const { return currentEvent.isActive; }
    
    // get event description
    std::string getCurrentEventDescription() const;
    
    // get access to subsystems
    TimeBomb* getTimeBomb() { return timeBomb.get(); }
    SeasonEvent* getSeasonEvent() { return seasonEvent.get(); }
    MassacreAbility* getMassacreAbility() { return massacreAbility.get(); }
    MissionManager* getMissionManager() { return missionManager.get(); }
};

#endif