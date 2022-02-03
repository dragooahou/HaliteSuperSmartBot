//
// Created by etien on 02/02/2022.
//

#include "cassert"

#include "BehaviourTree.h"

void BehaviourTree::MultiChildrenNode::AddChild(const BehaviourTree::NodePtr &node) {
    mChildren.push_back(node);
}

BehaviourTree::State BehaviourTree::Selector::Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const {
    assert(!mChildren.empty());

    BT_EVALUATE_LOG_TREE_CHILDREN

    for (const auto& child : mChildren) {

        auto result = child->Evaluate(rGame, rCommandQueue);
        if(result != State::Failure) {
            return result;
        }

    }

    return State::Failure;
}


BehaviourTree::State BehaviourTree::Sequencer::Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const {
    assert(!mChildren.empty());

    BT_EVALUATE_LOG_TREE_CHILDREN

    for (const auto& child : mChildren) {

        auto result = child->Evaluate(rGame, rCommandQueue);
        if(result != State::Success) {
            return result;
        }

    }

    return BehaviourTree::State::Success;
}

void BehaviourTree::Decorator::SetChild(const BehaviourTree::NodePtr &node) {
    mpChild = node;
}

BehaviourTree::State BehaviourTree::Succeeder::Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const {
    assert(mpChild);
    BT_EVALUATE_LOG_TREE_CHILD
    return mpChild->Evaluate(rGame, rCommandQueue) == State::Running ? State::Running : State::Success;
}

BehaviourTree::State BehaviourTree::Failer::Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const {
    assert(mpChild);
    BT_EVALUATE_LOG_TREE_CHILD
    return mpChild->Evaluate(rGame, rCommandQueue) == State::Running ? State::Running : State::Failure;
}

BehaviourTree::State BehaviourTree::Inverter::Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const {
    assert(mpChild);
    BT_EVALUATE_LOG_TREE_CHILD
    auto result = mpChild->Evaluate(rGame, rCommandQueue);
    if(result == State::Running) return State::Running;
    return result == State::Success ? State::Failure : State::Success;
}