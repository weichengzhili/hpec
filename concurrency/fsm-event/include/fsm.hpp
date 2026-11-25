// fsm-event —— 状态机与事件驱动
// 对应文档: ../02-cpp/cpp-08-状态机与事件驱动.md
// 职责: 强类型状态机框架（enum class + 转换表 + 进入/退出钩子）。
#pragma once

#include <cstdint>
#include <functional>
#include <array>
#include <type_traits>

namespace fsm {

// ---- 转换动作：返回新状态 ----
template <typename State, typename Event>
using action_t = std::function<State(Event)>;

// ---- 状态机基类（静态/表驱动） ----
template <typename State, typename Event, std::size_t NTrans>
class state_machine {
public:
    static_assert(std::is_enum_v<State> && std::is_enum_v<Event>,
                  "State/Event 必须是 enum class 强类型");

    struct transition {
        State from;
        Event event;
        State to;
        action_t<State, Event> action;
    };

    void init(State initial, std::array<transition, NTrans> table);
    State dispatch(Event ev);          // 非法转换显式上报
    State current() const { return current_; }
    bool can(Event ev) const;

    // 进入/退出钩子
    std::function<void(State)> on_entry;
    std::function<void(State)> on_exit;

private:
    State current_{};
    std::array<transition, NTrans> table_{};
};

// ---- 事件队列（事件驱动解耦） ----
template <typename Event>
class event_queue {
public:
    explicit event_queue(std::size_t capacity);
    void push(Event ev);
    bool pop(Event &ev);
    std::size_t count() const;
private:
    void *handle_ = nullptr;
};

} // namespace fsm
