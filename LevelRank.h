#ifndef WET2_LEVELRANK_H
#define WET2_LEVELRANK_H

namespace data_structures {
    class LevelRank {
    public:
        int level;
        int num_of_players_in_level;
        int num_of_players_mult_with_level;
        LevelRank() : level(0), num_of_players_in_level(0), num_of_players_mult_with_level(0){};
        LevelRank(int level_, int players_in_level) : level(level_), num_of_players_in_level(players_in_level),
                                                              num_of_players_mult_with_level(players_in_level * level_) {};

        LevelRank& operator=(const LevelRank& level_node) = default;

        bool operator<(int num_of_players) const {
            if (num_of_players_in_level < num_of_players)
                return true;
            return false;
        };

        bool operator>(int num_of_players) const {
            if (num_of_players_in_level > num_of_players)
                return true;
            return false;
        };

        bool operator==(int num_of_players) const {
            if (num_of_players_in_level == num_of_players)
                return true;
            return false;
        };

        LevelRank &operator+=(const LevelRank &level_node) {
            num_of_players_in_level += level_node.num_of_players_in_level;
            num_of_players_mult_with_level += level_node.num_of_players_mult_with_level;
            return *this;
        };

        LevelRank &operator+=(int num_of_players) {
            num_of_players_in_level += num_of_players;
            num_of_players_mult_with_level += (num_of_players * level);
            return *this;
        };

        LevelRank &operator-=(int num_of_players) {
            num_of_players_in_level -= num_of_players;
            num_of_players_mult_with_level -= (num_of_players * level);
            return *this;
        };

        LevelRank operator-(const LevelRank &level_node) const {
            LevelRank new_level(level, num_of_players_in_level);
            new_level.num_of_players_in_level += level_node.num_of_players_in_level;
            new_level.num_of_players_mult_with_level += level_node.num_of_players_mult_with_level;
            return new_level;
        };

        int operator-(int num_of_players) const {
            return num_of_players_in_level - num_of_players;
        };

        LevelRank operator+(const LevelRank &level_node) const {
            LevelRank new_level(0, 0);
            new_level.num_of_players_in_level += this->num_of_players_in_level;
            new_level.num_of_players_mult_with_level += this->num_of_players_mult_with_level;
            new_level.num_of_players_in_level += level_node.num_of_players_in_level;
            new_level.num_of_players_mult_with_level += level_node.num_of_players_mult_with_level;
            return new_level;
        };
    };
}
#endif //WET2_LEVELRANK_H
