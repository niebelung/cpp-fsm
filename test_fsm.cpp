#include <iostream>
#include "fsm.hpp"

struct ev_1 {};
struct ev_2 {};
struct ev_3 {};
struct ev_4 {};
struct ev_5 {};
struct ev_6 {};

enum class st
{
    s_1,
    s_2,
    s_3,
    s_4,
    s_5
};

class test_fsm : public fsm::state_machine<test_fsm, st, st::s_1>
{
    void on_ev_1(const ev_1& e) { (void)(e); std::cout << "event 1 -> "; }
    void on_ev_2(const ev_2& e) { (void)(e); std::cout << "event 2 -> "; }
    void on_ev_3(const ev_3& e) { (void)(e); std::cout << "event 3 -> "; }
    void on_ev_4(const ev_4& e) { (void)(e); std::cout << "event 4 -> "; }
    void on_ev_5(const ev_5& e) { (void)(e); std::cout << "event 5 -> "; }
    void on_ev_6(const ev_6& e) { (void)(e); std::cout << "event 6 -> "; }

    bool g_ev_1(const ev_1& e) { (void)(e); std::cout << "guard 1 -> "; return true; }
    bool g_ev_2(const ev_2& e) { (void)(e); std::cout << "guard 2 -> "; return true; }
    bool g_ev_3(const ev_3& e) { (void)(e); std::cout << "guard 3 -> "; return true; }
    bool g_ev_4(const ev_4& e) { (void)(e); std::cout << "guard 4 -> "; return true; }
    bool g_ev_5(const ev_5& e) { (void)(e); std::cout << "guard 5 -> "; return true; }
    bool g_ev_6(const ev_6& e) { (void)(e); std::cout << "guard 6 -> "; return true; }

    template<typename ev>
    bool g_false(const ev& e) { (void)(e); std::cout << "false guard -> "; return false; }

    template<typename ev>
    bool g_true(const ev& e) { (void)(e); std::cout << "true guard -> "; return true; }

    void on_s_1_enter() { std::cout << "s_1 enter -> "; }
    void on_s_1_exit()  { std::cout << "s_1 exit -> ";  }
    void on_s_2_enter() { std::cout << "s_2 enter -> "; }
    void on_s_2_exit()  { std::cout << "s_2 exit -> ";  }
    void on_s_3_enter() { std::cout << "s_3 enter -> "; }
    void on_s_3_exit()  { std::cout << "s_3 exit -> ";  }
    void on_s_4_enter() { std::cout << "s_4 enter -> "; }
    void on_s_4_exit()  { std::cout << "s_4 exit -> ";  }
    void on_s_5_enter() { std::cout << "s_5 enter -> "; }
    void on_s_5_exit()  { std::cout << "s_5 exit -> ";  }

    typedef test_fsm f;

    typedef transition_table<
//         | Start  | Event |  Next  |  Callback  |     Guard   |
// --------|--------|-------|--------|------------|-------------|
        row< st::s_1, ev_1, st::s_2, &f::on_ev_1, &f::g_ev_1  >,
        row< st::s_2, ev_2, st::s_3, &f::on_ev_2, &f::g_ev_2  >,
        row< st::s_3, ev_3, st::s_4, &f::on_ev_3, &f::g_ev_3  >,
        row< st::s_4, ev_4, st::s_5, &f::on_ev_4, &f::g_ev_4  >,
        row< st::s_5, ev_5, st::s_1, &f::on_ev_5, &f::g_ev_5  >,
// --------+--------+-------+--------+------------+-------------|
        row< st::s_2, ev_6, st::s_4, &f::on_ev_6, &f::g_ev_6  >,
        row< st::s_3, ev_6, st::s_5, &f::on_ev_6              >,
        row< st::s_5, ev_6, st::s_1, &f::on_ev_6, &f::g_false >
    > transition_table_t;

    typedef callback_table<
        scb< st::s_1, &f::on_s_1_enter, &f::on_s_1_exit >,
        scb< st::s_2, &f::on_s_2_enter                  >,
        scb< st::s_3, nullptr         , &f::on_s_3_exit >,
        scb< st::s_5, &f::on_s_5_enter, &f::on_s_5_exit >
    > callback_table_t;

public:
    template<typename event_t>
    st process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }
};

void out_state(st s)
{
	switch(s) {
		case st::s_1:
			std::cout << "s_1" << std::endl;
		break;
		case st::s_2:
			std::cout << "s_2" << std::endl;
		break;
		case st::s_3:
			std::cout << "s_3" << std::endl;
		break;
		case st::s_4:
			std::cout << "s_4" << std::endl;
		break;
		case st::s_5:
			std::cout << "s_5" << std::endl;
		break;
	}
}

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);
    
    test_fsm fsm;

    std::cout << "sizeof(test_fsm): " << sizeof(test_fsm) << std::endl;

    struct ev_1 ev1;
    struct ev_2 ev2;
    struct ev_3 ev3;
    struct ev_4 ev4;
    struct ev_5 ev5;
    struct ev_6 ev6;

    st s;

    std::cout << "TEST 1" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 3 -> 4 -> 5 -> 1
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev2);
    out_state(s);
    s = fsm.process_event(ev3);
    out_state(s);
    s = fsm.process_event(ev4);
    out_state(s);
    s = fsm.process_event(ev5);
    out_state(s);

    std::cout << std::endl << "TEST 2" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 4 -> 5 -> 5
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);
    s = fsm.process_event(ev4);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);

    fsm.reset();
    std::cout << std::endl << "TEST 3" << std::endl;
    std::cout << "initial state: ";
    out_state(fsm.state());

    // 1 -> 2 -> 3
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev2);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);
}