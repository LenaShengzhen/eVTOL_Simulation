#ifndef LL_H
#define LL_H

#include <cstddef>
#include <iostream>
#include <chrono>
#include <vector>
#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>

enum LIST_STATE {PUSH_FRONT, REMOVE, POP_BACK};

struct list {

    struct list_elem {
        struct list_elem *next_;
        int id_;

        list_elem();
        list_elem(int _id);
    };

    struct list_elem sentinel_;
    struct list_elem tail;      // harris's method  need

    list ();
    void delete_all_elems();

    // use harris + hazardpointer
    struct list_elem* lookup_harris_haz(int _id, struct list_elem **left_node);
    struct list_elem* lookup_harris_haz(int _id);
    bool push_front(int _id);
    bool remove_node(int _id);
    int pop_back();
};

void init_ll_before_startingMultiThread();
int op_linklist(int choose, int id);
void delete_ll();

// init Hazard pointer
void init_hplist_after_startingThisThread();

#endif