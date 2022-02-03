//
// Created by etien on 02/02/2022.
//

#ifndef HALITESUPERSMARTBOT_BEHAVIOURTREE_H
#define HALITESUPERSMARTBOT_BEHAVIOURTREE_H


#include <vector>
#include <memory>
#include "game.hpp"

#define BT_LOG_TREE 1
#if BT_LOG_TREE
#define BT_GETNAME_OVERRIDE std::string GetName() const override { return typeid(this).name(); }
#define BT_EVALUATE_LOG_TREE_CHILDREN                                                           \
    {                                                                                           \
    std::string msg;                                                                            \
    for(int i = 0; i < depth; ++i) msg += "\t";                                                 \
    for(const auto& child : mChildren) child->depth = depth+1;                                  \
    hlt::log::log(msg + GetName());                                                             \
    }
#define BT_EVALUATE_LOG_TREE_CHILD                                                              \
    {                                                                                           \
    std::string msg;                                                                            \
    for(int i = 0; i < depth; ++i) msg += "\t";                                                 \
    mpChild->depth = depth+1;                                                                   \
    hlt::log::log(msg + GetName());                                                             \
    }
#define BT_EVALUATE_LOG_TREE                                                                    \
    {                                                                                           \
    std::string msg;                                                                            \
    for(int i = 0; i < depth; ++i) msg += "\t";                                                 \
    hlt::log::log(msg + GetName());                                                             \
    }
#else
#define BT_GETNAME_OVERRIDE
#define BT_EVALUATE_LOG_TREE_CHILDREN
#define BT_EVALUATE_LOG_TREE_CHILD
BT_EVALUATE_LOG_TREE
#endif

namespace BehaviourTree {

    template<typename T> using Ptr = std::shared_ptr<T>;

    enum class State {
        Success,
        Failure,
        Running
    };




    class Node {
    public:
        virtual State Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const = 0;
#if BT_LOG_TREE
        int depth = 0;
        virtual std::string GetName() const = 0;
#endif
    };

    using NodePtr = Ptr<Node>;


    class MultiChildrenNode : public Node {
    protected:
        std::vector<NodePtr> mChildren;
    public:
        void AddChild(const NodePtr& node);
    };


    class Decorator : public Node {
    protected:
        NodePtr mpChild;
    public:
        void SetChild(const NodePtr& node);
    };




    // Base Nodes //////////////////////////////////////////////

    // OR
    class Selector : public MultiChildrenNode {

    public:
        State Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const override;
        BT_GETNAME_OVERRIDE

    };


    // AND
    class Sequencer : public MultiChildrenNode {

    public:
        State Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const override;
        BT_GETNAME_OVERRIDE

    };


    // Decorators
    class Succeeder : public Decorator {
    public:
        State Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const override;
        BT_GETNAME_OVERRIDE
    };

    class Failer : public Decorator {
    public:
        State Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const override;
        BT_GETNAME_OVERRIDE
    };

    class Inverter : public Decorator {
    public:
        State Evaluate(hlt::Game& rGame, std::vector<hlt::Command>& rCommandQueue) const override;
        BT_GETNAME_OVERRIDE
    };


    // Helpers

    using OrNode = Selector;
    using AndNode = Sequencer;

    template<typename T, typename... Args>
    Ptr<T> Create(Args... args) {
        static_assert(std::is_base_of_v<Node, T>, "This function is meant for nodes");
        return std::make_shared<T>(args...);
    }

}

namespace bt = BehaviourTree;

#endif //HALITESUPERSMARTBOT_BEHAVIOURTREE_H
