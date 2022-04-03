//
// Created by etien on 02/02/2022.
//

#include "ActionNodes.h"

BehaviourTree::State
BehaviourTree::ActionNodes::Spawn::Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const {
    BT_EVALUATE_LOG_TREE

    if(rGame.me->halite < hlt::constants::SHIP_COST) {
        return State::Failure;
    }

    rCommandQueue.push_back(rGame.me->shipyard->spawn());
    return State::Success;
}

BehaviourTree::State BehaviourTree::ActionNodes::MakeDropoff::Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const {
    BT_EVALUATE_LOG_TREE

    auto ship = rGame.me->get_ship(mShipID);
    hlt::Halite shipCargo = ship->halite;
    hlt::Halite inTheSea = rGame.game_map->at(ship)->halite;

    hlt::log::log(" >>>>> " + std::to_string(rGame.me->halite + shipCargo + inTheSea) + " < " + std::to_string(hlt::constants::DROPOFF_COST));
    if((rGame.me->halite + shipCargo + inTheSea) < hlt::constants::DROPOFF_COST) return State::Failure;

    if(rGame.game_map->at(ship)->has_structure()) return State::Failure;

    rCommandQueue.push_back(rGame.me->get_ship(mShipID)->make_dropoff());
    return State::Success;
}

BehaviourTree::ActionNodes::MakeDropoff::MakeDropoff(int mShipId) : ShipActionNode(mShipId) {}

BehaviourTree::ActionNodes::ShipActionNode::ShipActionNode(int mShipId) : mShipID(mShipId) {}

BehaviourTree::State BehaviourTree::ActionNodes::StayStill::Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const {
    BT_EVALUATE_LOG_TREE

    rCommandQueue.push_back(rGame.me->get_ship(mShipID)->stay_still());
    return State::Success;
}

BehaviourTree::ActionNodes::StayStill::StayStill(int mShipId) : ShipActionNode(mShipId) {}

BehaviourTree::ActionNodes::MoveTowardPosition::MoveTowardPosition(int mShipId, const hlt::Position& mpTargetPosition)
        : ShipActionNode(mShipId), mTargetPosition(mpTargetPosition) {}

BehaviourTree::State BehaviourTree::ActionNodes::MoveTowardPosition::Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const {
    BT_EVALUATE_LOG_TREE

    auto ship = rGame.me->get_ship(mShipID);
    if(ship->position == mTargetPosition) return State::Success;

    auto moveCost = rGame.game_map->at(ship)->halite / hlt::constants::MOVE_COST_RATIO;
    if(ship->halite < moveCost) {
        rCommandQueue.push_back(ship->stay_still());
        return State::Running;
    }

    auto dir = rGame.game_map->naive_navigate(ship, mTargetPosition);
    if(dir == hlt::Direction::STILL) return State::Failure;

    rCommandQueue.push_back(ship->move(dir));
    return BehaviourTree::State::Running;
}

BehaviourTree::ActionNodes::MoveTowardPositionPtr::MoveTowardPositionPtr(int mShipId, const hlt::Position *mpTargetPosition)
        : ShipActionNode(mShipId), mpTargetPosition(mpTargetPosition) {}

BehaviourTree::State BehaviourTree::ActionNodes::MoveTowardPositionPtr::Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const {
    BT_EVALUATE_LOG_TREE

    hlt::log::log(" >>> Move to " + mpTargetPosition->to_string());

    auto ship = rGame.me->get_ship(mShipID);
    if(ship->position == *mpTargetPosition) return State::Success;

    auto moveCost = rGame.game_map->at(ship)->halite / hlt::constants::MOVE_COST_RATIO;
    if(ship->halite < moveCost) {
        rCommandQueue.push_back(ship->stay_still());
        return State::Running;
    }

    auto dir = rGame.game_map->naive_navigate(ship, *mpTargetPosition);
    if(dir == hlt::Direction::STILL) return State::Failure;

    rCommandQueue.push_back(ship->move(dir));

    return BehaviourTree::State::Running;
}
