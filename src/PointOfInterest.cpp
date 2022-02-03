//
// Created by etien on 02/02/2022.
//

#include <cassert>
#include "PointOfInterest.h"

void CreateFromListRecur(hlt::GameMap &rMap, std::vector<hlt::Position> &rPositionList, int treshold, PointOfInterest& rCurrentlyCreatedPoi, const hlt::Position& currentPosition) {

    auto currentCell = rMap.at(currentPosition);

    // Exit the recursion if the treshold is reached
    if(currentCell->halite < treshold) return;

    // Exit the recursion if this cell is already treated
    if(std::find(rPositionList.begin(), rPositionList.end(), currentPosition) == rPositionList.end()) return;

    // Treat the cell
    rCurrentlyCreatedPoi.mPositions.push_back(currentPosition);
    rCurrentlyCreatedPoi.mTotalHalite += currentCell->halite;

    // Remove from the list
    rPositionList.erase(std::find(rPositionList.begin(), rPositionList.end(), currentPosition));

    CreateFromListRecur(rMap, rPositionList, treshold, rCurrentlyCreatedPoi, currentPosition.directional_offset(hlt::Direction::NORTH));
    CreateFromListRecur(rMap, rPositionList, treshold, rCurrentlyCreatedPoi, currentPosition.directional_offset(hlt::Direction::SOUTH));
    CreateFromListRecur(rMap, rPositionList, treshold, rCurrentlyCreatedPoi, currentPosition.directional_offset(hlt::Direction::WEST));
    CreateFromListRecur(rMap, rPositionList, treshold, rCurrentlyCreatedPoi, currentPosition.directional_offset(hlt::Direction::EAST));
}

PointOfInterest PointOfInterest::CreateFromList(hlt::GameMap &rMap, std::vector<hlt::Position> &rPositionList, int treshold) {
    assert(!rPositionList.empty());

    PointOfInterest poi;
    poi.mOrigin = *rPositionList.begin();
    CreateFromListRecur(rMap, rPositionList, treshold, poi, poi.mOrigin);
    return poi;
}

bool PointOfInterest::IsInside(const hlt::Position &position) {
    return std::find(mPositions.begin(), mPositions.end(), position) != mPositions.end();
}
