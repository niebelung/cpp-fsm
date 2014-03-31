#ifndef FSM_HPP
#define FSM_HPP

namespace fsm
{

template<typename derived,
         typename state_t,
         state_t  init>
class state_machine
{
protected:
    state_machine() : m_state(init), m_fsm_ptr(static_cast<derived*>(this)) {}

    template<typename event_t>
    class transition_layer
    {
        typedef void (derived::*action_t)(const event_t&);
        typedef bool (derived::*guard_t)(const event_t&);

    protected:
        struct transition_info {
            action_t action;
            guard_t  guard;
            state_t  start;
            state_t  next;

            struct transition_info* link;
        };

        typedef struct transition_info transition_info_t;

    private:
        transition_info_t* m_list;

    protected:
        transition_layer() : m_list(nullptr) {}

        void add(transition_info_t* const ti)
        {
            if(nullptr == m_list) {
                m_list = ti;
                return;
            }

            transition_info_t* entry = m_list;

            while(nullptr != entry->link) {
                entry = entry->link;
            }

            entry->link = ti;
        }

        state_t call(derived& fsm, const event_t& e)                       const
        {
            state_t st = fsm.state();

            const transition_info_t* entry = m_list;

            while(st != entry->start) {
                if(nullptr == entry->link) {
                    return st;
                }

                entry = entry->link;
            }

            bool transition = true;

            if(nullptr != entry->guard) {
                transition = (fsm.*(entry->guard))(e);
            }

            if(transition) {
                if(nullptr != entry->action) {
                    (fsm.*(entry->action))(e);
                }
                return entry->next;
            }

            return st;
        }
    };

    template<state_t start, typename event_t, state_t next,
             void (derived::*action)(const event_t&) = nullptr,
             bool (derived::*guard)(const event_t&) = nullptr>
    class row : public virtual transition_layer<event_t>
    {
    public:
        row() : ti( {action, guard, start, next, nullptr} )
        {
            this->transition_layer<event_t>::add(&ti);
        }

    private:
        typename transition_layer<event_t>::transition_info_t ti;
    };

    template<typename... rows>
    class transition_table : public rows...
    {
    public:
        template<typename event_t>
        state_t transition(derived& fsm, const event_t& e)                 const
        {
            return this->transition_layer<event_t>::call(fsm, e);
        }
    };

public:
    template<typename transition_table_t, typename event_t>
    state_t transition(const event_t& e)
    {
        static transition_table_t tt;

        m_state = tt.transition<event_t>(*m_fsm_ptr, e);

        return m_state;
    }

    state_t state()                                                        const 
    {
        return m_state; 
    }

    void reset()    { m_state = m_s_init_state; }

private:
    state_machine(const state_machine& other)                          = delete;
    state_machine& operator= (const state_machine& other)              = delete;
    state_machine(const state_machine&& other)                         = delete;
    state_machine& operator= (const state_machine&& other)             = delete;    

    state_t m_state;
    derived* m_fsm_ptr;
    constexpr static state_t m_s_init_state { init };
};

}

#endif // FSM_HPP
