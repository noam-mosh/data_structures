#ifndef WET2_GROUP_H
#define WET2_GROUP_H
#include "Scores.h"
#define MAX_SCALE 200


namespace data_structures {
class Group{
    public:
    Group():playerCounter(NO_PLAYERS),array_scores(new Scores[MAX_SCALE]){}
    ~Group(){
        delete[] array_scores;
    }
    Group(const Group& other)=default;//check
    Group& operator=(const Group& node) = default;//check
    //private:
    int playerCounter;
    Scores* array_scores;
    //RankAVLTree<> tree_levels;
};
}

#endif //WET2_GROUP_H