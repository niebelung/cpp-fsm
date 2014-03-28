#include <stdio.h>
#include "fsm.hpp"

struct ev_1_2 {};
struct ev_2_3 {};
struct ev_3_4 {};
struct ev_4_5 {};
struct ev_5_1 {};
struct ev1 {};

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
    void on_ev_1(const ev_1_2& e) { (void)(e); printf("on_ev_1_2\n\r"); }
    void on_ev_2(const ev_2_3& e) { (void)(e); printf("on_ev_2_3\n\r"); }
    void on_ev_3(const ev_3_4& e) { (void)(e); printf("on_ev_3_4\n\r"); }
    void on_ev_4(const ev_4_5& e) { (void)(e); printf("on_ev_4_5\n\r"); }
    void on_ev_5(const ev_5_1& e) { (void)(e); printf("on_ev_5_1\n\r"); }
    void on_ev1(const ev1& e) { (void)(e); printf("on_ev1\n\r"); }

    bool g_ev_1(const ev_1_2& e) { (void)(e); printf("g_ev_1_2\n\r"); return true; }
    bool g_ev_2(const ev_2_3& e) { (void)(e); printf("g_ev_2_3\n\r"); return true; }
    bool g_ev_3(const ev_3_4& e) { (void)(e); printf("g_ev_3_4\n\r"); return true; }
    bool g_ev_4(const ev_4_5& e) { (void)(e); printf("g_ev_4_5\n\r"); return true; }
    bool g_ev_5(const ev_5_1& e) { (void)(e); printf("g_ev_5_1\n\r"); return true; }
    bool g_ev1(const ev1& e) { (void)(e); printf("g_ev1\n\r"); return true; }

    template<typename ev>
    bool g_false(const ev& e) { (void)(e); printf("g_false\n\r"); return false; }

    typedef test_fsm f;

    typedef transition_table<
//         | Start  | Event |  Next  |  Callback  |     Guard   |
// --------|--------|-------|--------|------------|-------------|
        row< st::s_1, ev_1_2, st::s_2, &f::on_ev_1, &f::g_ev_1 >,
        row< st::s_2, ev_2_3, st::s_3, &f::on_ev_2, &f::g_ev_2 >,
        row< st::s_3, ev_3_4, st::s_4, &f::on_ev_3, &f::g_ev_3 >,
        row< st::s_4, ev_4_5, st::s_5, &f::on_ev_4, &f::g_ev_4 >,
        row< st::s_5, ev_5_1, st::s_1, &f::on_ev_5, &f::g_ev_5 >,
// --------+--------+-------+--------+------------+-------------|
        row< st::s_2, ev1,    st::s_4, &f::on_ev1,  &f::g_ev1 >,
        row< st::s_3, ev1,    st::s_5, &f::on_ev1,  nullptr   >,
        row< st::s_5, ev1,    st::s_1, &f::on_ev1,  &f::g_false >
    > transition_table_t;

public:

    template<typename event_t>
    st process_event(const event_t& e)
    {
        m_state = tt.transition<event_t>(*this, e);

        return m_state;
    }

private:
    transition_table_t tt;
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
    test_fsm fsm;

    printf("sizeof(test_fsm): %u\n\r", sizeof(test_fsm));

    struct ev_1_2 ev12;
    struct ev_2_3 ev23;
    struct ev_3_4 ev34;
    struct ev_4_5 ev45;
    struct ev_5_1 ev51;

    struct ev1 e1;

    st s = st::s_1;

    printf("TEST 1\n\r");
    printf("initial state: ");
    out_state(fsm.state());

    // 1 -> 2 -> 3 -> 4 -> 5 -> 1
    s = fsm.process_event(ev12);
    out_state(s);
    s = fsm.process_event(ev23);
    out_state(s);
    s = fsm.process_event(ev34);
    out_state(s);
    s = fsm.process_event(ev45);
    out_state(s);
    s = fsm.process_event(ev51);
    out_state(s);

    printf("TEST 2\n\r");
    printf("initial state: ");
    out_state(fsm.state());

    // 1 -> 2 -> 4 -> 5 -> 5
    s = fsm.process_event(ev12);
    out_state(s);
    s = fsm.process_event(e1);
    out_state(s);
    s = fsm.process_event(ev45);
    out_state(s);
    s = fsm.process_event(e1);
    out_state(s);

    fsm.reset();
    printf("TEST 3\n\r");
    printf("initial state: ");
    out_state(fsm.state());

    // 1 -> 2 -> 3
    s = fsm.process_event(ev12);
    out_state(s);
    s = fsm.process_event(ev23);
    out_state(s);
    s = fsm.process_event(e1);
    out_state(s);
}