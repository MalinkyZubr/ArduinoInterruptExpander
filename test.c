#include <stdio.h>


enum Silly {
    silly = 0,
    fella = 1,
    is = 2,
    silliest = 3
};

int main() {
    enum Silly doof = (enum Silly)2;
    return 0;
}