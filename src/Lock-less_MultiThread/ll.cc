#include "ll.h"
#include "Parallel_Processing/HazardPointers/HazardPointer.h"
using namespace std;

// The Hazard pointer used by each thread is stored in the array of each thread
thread_local vector<HPRecType *> hpVec;
// The number of hazard pointers used by each thread
const int hpPerThread = 7; 


list::list_elem::list_elem() : id_(-1), next_(NULL) {};
list::list_elem::list_elem(int _id) : id_(_id), next_(NULL) {};

list::list () {};

const int tail_id = -1;

// Head pointer of linked list
static struct list * p_ll  = NULL;

// address's 0bit is 1?
inline int is_marked_reference(long i) {
    return (int) (i & 0x1L);
}

// return (address + 1) when address is even
inline long get_marked_reference(long w) {
    return w | 0x1L;
}

// return (address - 1) when address is odd
inline long get_unmarked_reference(long w) {
    return w & ~0x1L;
}

void list::delete_all_elems() {
    struct list::list_elem *cur = sentinel_.next_;
    struct list::list_elem *prev;
    int deleted = 0;

    if (cur) { //skip sentinel
        prev = cur;
        cur = (struct list::list_elem *) get_unmarked_reference((long)cur->next_);
    }

    while (cur) {
        delete(prev);
        deleted++;
        prev = cur;
        cur = (struct list::list_elem *) get_unmarked_reference((long)cur->next_);
    }

    // don't delete tailNode
    if (prev && (cur != NULL)) {
        delete(prev);
        deleted++;
    }
}

bool list::push_front(int _id) {
    struct list::list_elem *right, *left;
    right = left = NULL;
    struct list::list_elem * elem = new list::list_elem(_id);
    do {
        right = lookup_harris_haz(_id, &left);
        hpVec[0]->pHazard_ = right;
        if( (right != &tail) && (right->id_ == _id) ) {
            elem->next_ = NULL;
            delete(elem);
            return false;
        }
        elem->next_ = sentinel_.next_;;
        if(__sync_bool_compare_and_swap(&(sentinel_.next_), elem->next_, elem)) {
            return true;
        }
    } while(1);
}

struct list::list_elem* list::lookup_harris_haz(int _id) {
    struct list::list_elem* prev;

    return lookup_harris_haz(_id, &prev);
}

int list::pop_back() {
    // get id before the tail_id
    struct list::list_elem* prev;
    lookup_harris_haz(tail_id, &prev);
    int deleteID = prev->id_;

    // remove the node
    op_linklist(REMOVE, deleteID);
    return deleteID;
}

// left_node is prev.
// return node is _id's node or tail node.
struct list::list_elem* list::lookup_harris_haz(int _id, struct list_elem **left_node) {
    struct list::list_elem *left_node_next, *right_node;
    left_node_next = right_node = NULL;

    do {
        struct list::list_elem *t = &sentinel_;
        struct list::list_elem *t_next = sentinel_.next_;
        struct list::list_elem * refer = t;
        bool is_continue_flag = false;

        hpVec[2]->pHazard_ = (void *)get_unmarked_reference((long)t);
        hpVec[3]->pHazard_ = (void *)get_unmarked_reference((long)t_next);

        // 1: Find left_node and right_node
        do {
            if (!is_marked_reference((long)t_next)) {
                (*left_node) = t;
                hpVec[4]->pHazard_ = (void *)get_unmarked_reference((long) t);  //  (*left_node)
                left_node_next = t_next;
                hpVec[5]->pHazard_ = (void *)get_unmarked_reference((long) left_node_next );
            }
            t = (struct list::list_elem *)get_unmarked_reference((long)t_next);
            hpVec[2]->pHazard_ = t;

            if (t == &tail) break;

            if( (void *)get_unmarked_reference((long)refer->next_ )!=  t || t == NULL) {
                is_continue_flag = true;
                break;
            }

            t_next = t->next_;
            hpVec[3]->pHazard_ = (void *)get_unmarked_reference((long)t_next);
            refer = t;
            hpVec[6]->pHazard_ = refer;
        } while(is_marked_reference((long)t_next) || (t->id_ != _id));

        // t == NULL
        if(is_continue_flag) continue;

        right_node = t;

        // 2: Check node are adjcent
        if (left_node_next == right_node){
            if ( (right_node != &tail) && is_marked_reference((long)right_node->next_))
                continue;
            else    // improve point: this place should set  et_use_point(hp1/2/3/4, 0);
                return right_node;
        }

        // 3: Remove one or more marked nodes
        struct list::list_elem * detenode = (struct list::list_elem *)get_unmarked_reference((long) (*left_node)->next_ );
        if (__sync_bool_compare_and_swap(&((*left_node)->next_), left_node_next, right_node)) {
            // add detele node on freepool
            while(detenode && detenode != right_node) {
                struct list::list_elem * temp = detenode->next_;
                put_point2BeReleased_on_rlist(detenode);
                detenode = (struct list::list_elem *)get_unmarked_reference((long) temp);
            }
            if ( (right_node != &tail) && is_marked_reference((long)right_node->next_))
                continue;
            else
                return right_node;
        }
    } while(1);
}


bool list::remove_node(int _id) {
    struct list::list_elem *prev, *deleteNode, *del_next;
    while(1) {
        deleteNode = lookup_harris_haz(_id, &prev);
        hpVec[0]->pHazard_ = deleteNode;
        hpVec[1]->pHazard_ = prev;
        if (deleteNode == &tail || deleteNode->id_ != _id) {
            return false;
        }
        del_next = deleteNode->next_;
        if(!is_marked_reference((long)del_next)) {
            // logic delete, set the deleteNoed's next's 0bit to 1.
            if(__sync_bool_compare_and_swap(&(deleteNode->next_), del_next, (struct list::list_elem *)get_marked_reference((long)del_next))) {
                // put deleteNode in freepool, not put this, must this node not on list then the node can put on freepool.
                break;
            }
        }
    }

    // physics delete
    if (!__sync_bool_compare_and_swap (&(prev->next_), deleteNode, (struct list::list_elem *)get_unmarked_reference((long) del_next) ))  
    {   // If the physical delete fails, hand it over to the search() for physical delete
        deleteNode = lookup_harris_haz (deleteNode->id_, &prev); 
    } else {
        deleteNode->next_ = NULL;
        put_point2BeReleased_on_rlist(deleteNode);            // physics delete success 
        #ifdef UNIT_TEST_LIST_NODE_NUM
        delete_Node_Num++;
        #endif
    }

    return true;
}



void init_ll_before_startingMultiThread() {
    p_ll = new list();
    p_ll->sentinel_.next_ = &p_ll->tail;
    
    op_linklist(PUSH_FRONT, tail_id);
}

int op_linklist(int choose, int id){
    if(hpVec.size() == 0) {
        init_hplist_after_startingThisThread();
    }

    struct list* ll = p_ll;
    int ret = -1;
    switch (choose) {
        case PUSH_FRONT:
            ll->push_front(id);
            break;
        case REMOVE:
            ll->remove_node(id);
            break;
        case POP_BACK:
            ret = ll->pop_back();
        default:
            ll->lookup_harris_haz(id);
            break;
    }

    return ret;
}

void delete_ll() {
    p_ll->delete_all_elems();
    delete (p_ll);
}

// init Hazard pointer
void init_hplist_after_startingThisThread() {
    hpVec = vector<HPRecType *>(hpPerThread);
    for(int j = 0; j < hpPerThread; j++) {
        hpVec[j] = HPRecType::Acquire();
    }
}