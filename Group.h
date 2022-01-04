#ifndef WET2_GROUP_H
#define WET2_GROUP_H
#include "Scores.h"
#define MAX_SCALE 200
#define NO_PLAYER 0
#define LEVEL int 

namespace data_structures {
class Group{
    public:
    Group(int scale):playerCounter(NO_PLAYERS),array_scores(new Scores[scale]){}
    ~Group(){
        delete[] array_scores;
    }
    Group(const Group& other)=default;//check
    Group& operator=(const Group& node) = default;//check
    //private:
    int playerCounter;
    Scores* array_scores;
    RankAVLTree<LEVEL,LevelRank> tree_levels;
};
}

#endif //WET2_GROUP_H