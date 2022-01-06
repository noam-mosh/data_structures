#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H
#define INITIAL_LEVEL 0
#define NO_SCORE -1
#define NO_GROUP -1

namespace data_structures {
class Player{
    public:
    Player(int playerID): playerID(playerID), groupID(NO_GROUP),score(NO_SCORE), level(INITIAL_LEVEL){}
    Player(int PlayerID,int GroupID, int Score):playerID(PlayerID),groupID(GroupID), score(Score), level(INITIAL_LEVEL){};
    ~Player()=default;
    Player(const Player& other)=default;
    Player& operator=(const Player& other) = default;
    bool operator==(const Player& other){
        if(playerID==other.playerID ){
            return true;
        }
        return false;
    }
    int key(){//function need for the hashtable to not what is the key
        return playerID;
    }
    //private:
    int playerID;
    int groupID;
    int score;
    int level;
};
}

#endif //WET2_PLAYER_H