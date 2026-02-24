// EventManager.cpp
#include "EventManager.h"

// constructor - set up random generator and subsystems
EventManager::EventManager()
    : timeBomb(make_unique<TimeBomb>(5)), seasonEvent(make_unique<SeasonEvent>()), massacreAbility(make_unique<MassacreAbility>()), missionManager(make_unique<MissionManager>()), turnCounter(0)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

// check if we should trigger a random event based on turn count and chance
void EventManager::checkAndTriggerEvent(int currentTurn, Board &board, Color currentPlayer)
{
    // don't trigger if we already have an active event
    if (currentEvent.isActive)
    {
        return;
    }

    turnCounter++;

    // only check every few turns
    if (turnCounter % EVENT_CHECK_INTERVAL != 0)
    {
        return;
    }

    // random chance to trigger
    uniform_int_distribution<int> chanceDist(1, 100);
    if (chanceDist(rng) > EVENT_CHANCE)
    {
        return;
    }

    // pick a random event type
    uniform_int_distribution<int> eventDist(1, 4);
    int eventChoice = eventDist(rng);

    switch (eventChoice)
    {
    case 1:
    {
        // time bomb event - pieces explode if they don't move
        currentEvent.type = EventType::TimeBomb;
        currentEvent.description = "Time Bomb Activated! Pieces explode after 5 moves without moving!";
        currentEvent.isActive = true;
        currentEvent.duration = 10;
        currentEvent.turnActivated = currentTurn;

        // register all pieces on the board as bombs
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Piece *piece = board.getPiece(Position(r, c));
                if (piece)
                {
                    timeBomb->registerPiece(piece);
                }
            }
        }
        break;
    }

    case 2:
    {
        // season change event - weather affects different pieces
        currentEvent.type = EventType::SeasonChange;
        currentEvent.description = "Season Changed! " + seasonEvent->getSeasonName();
        currentEvent.isActive = true;
        currentEvent.duration = 15;
        currentEvent.turnActivated = currentTurn;

        seasonEvent->resetForNewSeason();
        seasonEvent->nextSeason();
        break;
    }

    case 3:
    {
        // massacre event - capture queen gives double move
        currentEvent.type = EventType::Massacre;
        currentEvent.description = "Massacre Mode Activated! Capture queen to get double move!";
        currentEvent.isActive = true;
        currentEvent.duration = 8;
        currentEvent.turnActivated = currentTurn;

        massacreAbility->reset();
        break;
    }

    case 4:
    {
        // mission event - players get special tasks
        currentEvent.type = EventType::NewMission;
        currentEvent.description = "New Mission! Complete special tasks to win!";
        currentEvent.isActive = true;
        currentEvent.duration = 20;
        currentEvent.turnActivated = currentTurn;

        // generate missions for both players
        missionManager->generateMission(Color::White, currentTurn);
        missionManager->generateMission(Color::Black, currentTurn);
        break;
    }
    }

    cout << endl;
    cout << "RANDOM EVENT TRIGGERED!" << endl;
    cout << currentEvent.description << endl;
    cout << endl;
}

// apply the effects of the current event
void EventManager::applyEventEffects(Board &board, Color currentPlayer)
{
    if (!currentEvent.isActive)
    {
        return;
    }

    switch (currentEvent.type)
    {
    case EventType::TimeBomb:
        timeBomb->update(board);
        break;

    case EventType::SeasonChange:
        seasonEvent->applySeasonEffects(board);
        break;

    case EventType::Massacre:
        // no direct effects, triggers when queen is captured
        break;

    case EventType::NewMission:
        cout << "Current Missions:" << endl;
        cout << "White: " << missionManager->getMissionDescription(Color::White) << endl;
        cout << "Black: " << missionManager->getMissionDescription(Color::Black) << endl;
        break;

    default:
        break;
    }
}

// update event state each turn
void EventManager::updateEvents(int currentTurn, Board &board, const Move &lastMove)
{
    if (!currentEvent.isActive)
    {
        return;
    }

    // check if event duration is over
    if (currentTurn - currentEvent.turnActivated >= currentEvent.duration)
    {
        cout << endl;
        cout << "Event Ended: " << currentEvent.description << endl;
        cout << endl;
        clearCurrentEvent();
        return;
    }

    // update based on event type
    switch (currentEvent.type)
    {
    case EventType::SeasonChange:
        seasonEvent->activateNextSeason(currentTurn);
        break;

    case EventType::Massacre:
    {
        // check if a queen was captured in the last move
        Piece *capturedPiece = board.getPiece(lastMove.to);
        if (capturedPiece && capturedPiece->getType() == PieceType::Queen)
        {
            massacreAbility->onQueenCaptured(capturedPiece->getColor());
        }
        break;
    }

    case EventType::NewMission:
        // update mission progress
        if (lastMove.isValid())
        {
            missionManager->updateMission(board, Color::White, lastMove);
            missionManager->updateMission(board, Color::Black, lastMove);
        }
        break;

    default:
        break;
    }
}

// clear the current event
void EventManager::clearCurrentEvent()
{
    currentEvent.isActive = false;
    currentEvent.type = EventType::None;
}

// get event description
string EventManager::getCurrentEventDescription() const
{
    if (!currentEvent.isActive)
    {
        return "No active event";
    }
    return currentEvent.description;
}