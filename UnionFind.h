#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H
#include <cstdio>
#include <cstdlib>
namespace data_structures {
    class UnionFind {
    public:
        int *parent;
        int *size;
        int group_size;

        explicit UnionFind(int k) : group_size(k) {
            parent = new int[group_size];
            size = new int[group_size];
            for (int i = 0; i < group_size; ++i) {
                parent[i] = -1;
                size[i] = 0;
            }
        }
        ~UnionFind(){
            delete [] parent;
            delete [] size;
        }

        int MakeSet(int i) const {
            parent[i - 1] = i - 1;
            size[i - 1] = 1;
            return i;
        }

        int Find(int i) const {
            // if group i does not exists in this struct
            if (parent[i - 1] == -1)
                return -1;

            int j = i - 1;
            while (parent[j] != j) {
                j = parent[j];
            }
            int group_num = j;
            // todo: is it j-1
            int k = i - 1;
            while (parent[k] != k) {
                int temp = parent[k];
                parent[k] = group_num;
                k = temp;
            }
            return group_num + 1;
        }

        int Union(int p, int q) const {
            // p and q are the groups after calling find
            if (size[p - 1] < size[q - 1]) {
                parent[p - 1] = q - 1;
                size[q - 1] += size[p - 1];
                return q;
            } else {
                parent[q - 1] = p - 1;
                size[p - 1] += size[q - 1];
                return p;
            }
        }
    };
}
#endif //WET2_UNIONFIND_H
