#include "LinkedList.hpp"

LinkedList::LinkedList(
    ull addr, 
    int weigth):
    addr(addr), 
    weigth(weigth), 
    next(nullptr) {}