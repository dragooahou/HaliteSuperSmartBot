//
// Created by etien on 02/02/2022.
//

#include <algorithm>
#include <sstream>

#include "InputNode.h"
#include "ActionNodes.h"

#include "MainBehaviour.h"

bool CompareCellByAmount(const hlt::MapCell& lhs, const hlt::MapCell& rhs) {
    return lhs.halite > rhs.halite;
}


MainBehaviour::MainBehaviour(hlt::Game *mpGame) : mpGame(mpGame) {

    // Build tree
    auto treeRoot = bt::Create<bt::OrNode>();
    {
        auto andNode = bt::Create<bt::AndNode>();
        {

            // Check if we have a safe amount of halite
            andNode->AddChild(
                        bt::Create<bt::InputNode>([](hlt::Game& rGame) { return rGame.turn_number < 100 || rGame.me->halite >= 4000; })
                    );

            // Check if the max amount of ship is reached
//            andNode->AddChild(
//                    bt::Create<bt::InputNode>([](hlt::Game& rGame) { return rGame.me->ships.size() < MAX_SHIP_COUNT; })
//            );

            // If it's not the end of the game
            andNode->AddChild(
                    bt::Create<bt::InputNode>([](hlt::Game& rGame) { return rGame.turn_number < 310; })
            );

            // Check if the shipyard is not occupied by a ship
            andNode->AddChild(
                    bt::Create<bt::InputNode>([](hlt::Game& rGame) { return !rGame.game_map->at(rGame.me->shipyard)->is_occupied(); })
            );

            // Spawn new ship
            andNode->AddChild(
                    bt::Create<bt::ActionNodes::Spawn>()
            );

        }
        treeRoot->AddChild(andNode);
    }
    tree = treeRoot;


    // Find all the POI (Point of interest) on the map
    FindPointsOfInterrest();


    // Choose the most interresting poi
    mPoiToVisit = -1;
    SelectMostInterrestingPoi();

}

std::vector<hlt::Command> MainBehaviour::Evaluate() {
    std::vector<hlt::Command> commandQueue;

    // Evaluate the global behaviour (chef d'orchestre)
    tree->Evaluate(*mpGame,commandQueue);

    // Iterate the ships and create a behaviour for the ones which doesn't already have one
    for (const auto & pair : mpGame->me->ships) {
        const hlt::EntityId& shipId = pair.first;
        std::shared_ptr<hlt::Ship> ship = pair.second;

        // If no behaviour
        if(mShipBeahviours.find(shipId) == mShipBeahviours.end()) {

            hlt::Position positionToGo = mpGame->turn_number < 500 ? mpGame->me->shipyard->position : mPoiList[mPoiToVisit].mOrigin;

            mShipBeahviours[shipId] = bt::Create<ExploitPosition>(shipId, positionToGo);

        }
    }

    // region Evaluate or remove behaviour of destroyed ships
    std::vector<hlt::EntityId> destroyedShips;
    for (auto& pair : mShipBeahviours) {
        const hlt::EntityId& shipId = pair.first;
        bt::Ptr<ExploitPosition> shipTree = pair.second;

        if(mpGame->me->has_ship(shipId)) {
            shipTree->Evaluate(*mpGame, commandQueue);
        }
        else {
            destroyedShips.push_back(shipId);
        }

    }
    for(const auto& shipId : destroyedShips) {
        mShipBeahviours.erase(shipId);
    }
    // endregion

    // If a ship's position to exploit is 0, give him an other
    for (const auto & pair : mpGame->me->ships) {
        const hlt::EntityId &shipId = pair.first;
        auto ship= mpGame->me->get_ship(shipId);

        if(mpGame->game_map->at(mShipBeahviours[shipId]->mPositionToExploit)->halite == 0) {
            mShipBeahviours[shipId]->mPositionToExploit = mPoiList[mPoiToVisit].mOrigin;

            ++mShipCountOnCurrPoi;
            if(mShipCountOnCurrPoi >= SHIP_PER_POI) {
                SelectMostInterrestingPoi();
                mShipCountOnCurrPoi = 0;
            }
        }
    }
    
    return commandQueue;
}

void MainBehaviour::FindPointsOfInterrest() {

    // Create a list of cell
    std::vector<hlt::MapCell> cells;
    cells.reserve(64*64);
    for(const auto& cellColumn : mpGame->game_map->cells) {
        for(const hlt::MapCell& cell : cellColumn) {
            cells.push_back(cell);
        }
    }

    // Sort it by halite amount
    std::sort(cells.begin(), cells.end(), CompareCellByAmount);

    // Convert to position list
    std::vector<hlt::Position> positions;
    positions.reserve(cells.size());
    for(const auto& cell : cells) {
        positions.push_back(cell.position);
    }

    // Build the points of interest
    while (mpGame->game_map->at(*positions.begin())->halite > mPoiTreshold) {
        mPoiList.push_back(PointOfInterest::CreateFromList(*mpGame->game_map, positions, mPoiTreshold));
        hlt::log::log("Created POI : " + mPoiList.back().mOrigin.to_string());
    }
}

void MainBehaviour::SelectMostInterrestingPoi() {

    int closestPoiIdx = -1;
    int closestPoiDist = INT_MAX;
    while(closestPoiIdx == -1) {

        for(int i = mPoiToVisit+1; i < mPoiList.size(); i++) {

            int halite = mpGame->game_map->at(mPoiList[i].mOrigin)->halite;
            int dist = mpGame->game_map->calculate_distance(mpGame->me->shipyard->position, mPoiList[i].mOrigin);

            // If this poi is not insterresting skip it
            if(halite < mInterrestingPoiTreshold) continue;

            // Check if this poi is more valuable
            if(dist < closestPoiDist) {
                closestPoiDist = dist;
                closestPoiIdx = i;
            }

        }

        // Lower the poi interresting treshold if none has been found
        if(closestPoiIdx == -1) {
            mInterrestingPoiTreshold = std::max(0, mInterrestingPoiTreshold-100);
            if(mInterrestingPoiTreshold == 0) {
                closestPoiIdx = 0;
                break;
            }
        }

    }
    mPoiToVisit = closestPoiIdx;
    hlt::log::log("choose poi : " + mPoiList[mPoiToVisit].mOrigin.to_string() + ", shipyard : " + mpGame->me->shipyard->position.to_string() + ", dist = " + std::to_string(closestPoiDist));

}
