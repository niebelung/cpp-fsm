#include <stdio.h>
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

class test_fsm : public fsm::state_machine<test_fsm, st, 5, st::s_1>
{
    void on_ev_1(const ev_1& e) { (void)(e); printf("event 1 -> "); }
    void on_ev_2(const ev_2& e) { (void)(e); printf("event 2 -> "); }
    void on_ev_3(const ev_3& e) { (void)(e); printf("event 3 -> "); }
    void on_ev_4(const ev_4& e) { (void)(e); printf("event 4 -> "); }
    void on_ev_5(const ev_5& e) { (void)(e); printf("event 5 -> "); }
    void on_ev_6(const ev_6& e) { (void)(e); printf("event 6 -> "); }

    bool g_ev_1(const ev_1& e) { (void)(e); printf("event 1 guard -> "); return true; }
    bool g_ev_2(const ev_2& e) { (void)(e); printf("event 2 guard -> "); return true; }
    bool g_ev_3(const ev_3& e) { (void)(e); printf("event 3 guard -> "); return true; }
    bool g_ev_4(const ev_4& e) { (void)(e); printf("event 4 guard -> "); return true; }
    bool g_ev_5(const ev_5& e) { (void)(e); printf("event 5 guard -> "); return true; }
    bool g_ev_6(const ev_6& e) { (void)(e); printf("event 6 guard -> "); return true; }

    template<typename ev>
    bool g_false(const ev& e) { (void)(e); printf("false guard -> "); return false; }

    template<typename ev>
    bool g_true(const ev& e) { (void)(e); printf("true guard -> "); return true; }

    typedef test_fsm f;

    typedef struct transition_table<
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

public:
    template<typename event_t>
    st process_event(const event_t& e)
    {
        return transition<transition_table_t, event_t>(e);
    }
};

void out_state(st s)
{
	printf("state: ");
	switch(s) {
		case st::s_1:
			printf("s_1\n\r");
		break;
		case st::s_2:
			printf("s_2\n\r");
		break;
		case st::s_3:
			printf("s_3\n\r");
		break;
		case st::s_4:
			printf("s_4\n\r");
		break;
		case st::s_5:
			printf("s_5\n\r");
		break;
	}
}

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);
    
    test_fsm fsm;

    printf("sizeof(test_fsm): %lu\n\r", sizeof(test_fsm));

    struct ev_1 ev1;
    struct ev_2 ev2;
    struct ev_3 ev3;
    struct ev_4 ev4;
    struct ev_5 ev5;
    struct ev_6 ev6;

    st s;

    printf("TEST 1\n\r");
    printf("initial state: ");
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

    printf("\n\rTEST 2\n\r");
    printf("initial state: ");
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
    printf("\n\rTEST 3\n\r");
    printf("initial state: ");
    out_state(fsm.state());

    // 1 -> 2 -> 3
    s = fsm.process_event(ev1);
    out_state(s);
    s = fsm.process_event(ev2);
    out_state(s);
    s = fsm.process_event(ev6);
    out_state(s);
}