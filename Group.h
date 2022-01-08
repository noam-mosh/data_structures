#ifndef WET2_GROUP_H
#define WET2_GROUP_H
#include "Scores.h"
#define MAX_SCALE 200
#define LEVEL int

namespace data_structures {
class Group{
    public:
    explicit Group(int scale):playerCounter(NO_PLAYERS),array_scores(new Scores[scale+1]),tree_levels(new RankAVLTree<LEVEL, LevelRank>()){}
    ~Group(){
        delete[] array_scores;
        delete tree_levels;
    }
    Group(const Group& other)=default;//check
    Group& operator=(const Group& node) = default;//check
    //private:
    int playerCounter;
    Scores* array_scores;
    RankAVLTree<LEVEL,LevelRank>* tree_levels;
};
}

#endif //WET2_GROUP_H