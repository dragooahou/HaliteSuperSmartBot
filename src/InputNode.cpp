//
// Created by etien on 02/02/2022.
//

#include <utility>

#include "InputNode.h"

BehaviourTree::InputNode::InputNode(std::function<bool(hlt::Game&)> mFunction) : mFunction(std::move(mFunction)) {

}

BehaviourTree::State
BehaviourTree::InputNode::Evaluate(hlt::Game &rGame, std::vector<hlt::Command> &rCommandQueue) const {
    BT_EVALUATE_LOG_TREE
    return mFunction(rGame) ? State::Success : State::Failure;
}
