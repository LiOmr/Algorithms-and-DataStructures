/* 1.3 Если в числе содержится только один бит со значением 1, записать в выходной поток OK. Иначе записать FAIL */

#include <iostream>

using namespace std;

bool countSignificantBits(size_t n);

int main()
{
    size_t num = 0;
    cin >> num;
    cout << (countSignificantBits(num) ? "OK" : "FAIL");

    return 0;
}

bool countSignificantBits(size_t n) {
    int count = 0;
    while (n != 0) {
        if (n & 1) {
            count++;
        }
        n >>= 1;
    }
    return count == 1;
}

/* Time Complexity: O(logN)
 * Space Complexity: O(1) */
