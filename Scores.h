#ifndef WET2_SCORES_H
#define WET2_SCORES_H
#define NO_PLAYERS 0
#include "RankAVLTree.h"
namespace data_structures {
class Scores{
    public:
    Scores(): players_counter(NO_PLAYERS){}
    ~Scores()=default;
    Scores(const Scores& other)=default;
    Scores& operator=(const Scores& other) = default;
    
    //private:
    int players_counter;
    //RankAVLTree<> tree_levels;
};
}

#endif //WET2_SCORES_H