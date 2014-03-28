#ifndef FSM_HPP
#define FSM_HPP

namespace fsm
{

template<typename derived,
         typename state_t,
         size_t   state_count,
         state_t  init>
class state_machine
{
public:
    state_machine() : m_state(init) {}

protected:
    template<typename event_t>
    class transition_layer
    {
        typedef void (derived::*action_t)(const event_t&);
        typedef bool (derived::*guard_t)(const event_t&);

        struct calls {
            action_t action = nullptr;
            guard_t  guard = nullptr;
            state_t  start;
            state_t  next;
        };

    protected:
        bool set_calls(action_t a, guard_t g, state_t start, state_t next)
        {
            for(auto &c : m_s_calls) {
                if((nullptr == c.action) && (nullptr == c.guard)) {
                    c.action = a;
                    c.guard = g;
                    c.start = start;
                    c.next = next;

                    return true;
                }
            }

            return false;
        }

        state_t call(derived& fsm, const event_t& e)
        {
            state_t st = fsm.state();
            for(auto &c: m_s_calls) {
                if(st == c.start) {
                    bool transition = true;

                    if(nullptr != c.guard) {
                        transition = (fsm.*(c.guard))(e);
                    }

                    if(transition) {
                        if(nullptr != c.action) {
                            (fsm.*(c.action))(e);
                        }

                        return c.next;
                    } else {
                        return st;
                    }
                }
            }

            return st;
        }

    public:
        transition_layer()
        {
            for(auto &c: m_s_calls) {
                c.action = nullptr;
                c.guard = nullptr;
            }
        }

    private:
        struct calls m_s_calls[state_count];
    };

    template<state_t start,
             typename event_t,
             state_t next,
             void (derived::*action)(const event_t&),
             bool (derived::*guard)(const event_t&)>
    class row : public virtual transition_layer<event_t>
    {
    public:
        row()
        {
            this->transition_layer<event_t>::set_calls(action, guard,
                                                       start,  next);
        }
    };

    template<typename... rows>
    class transition_table : public rows...
    {
    public:
        template<typename event_t>
        state_t transition(derived& fsm, const event_t& e)
        {
            return this->transition_layer<event_t>::call(fsm, e);
        }
    };

public:
    state_t state() { return m_state; }
    void reset()    { m_state = m_s_init_state; }

protected:
    state_t m_state;
    constexpr static state_t m_s_init_state { init };
};

}

#endif // FSM_HPP
