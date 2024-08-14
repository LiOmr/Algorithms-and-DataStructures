/* Задача 4. «Пятнашки» (8 баллов)
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12] 
[ 13 14 15 0 ]
где 0 задает пустую ячейку.
Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
Формат входных данных
Начальная расстановка.
Формат выходных данных
Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек, которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую последовательность ходов: L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.
*/

#include <iostream>
#include <array>
#include <assert.h>
#include <unordered_map>
#include <queue>
#include <cstring>
#include <algorithm>
#include <set>

using std::string;
using std::unordered_map;

#define sizeOfSize 4
#define numOfCells sizeOfSize * sizeOfSize

const std::array<char, numOfCells> finishField = {1, 2, 3, 4,
                                                 5, 6, 7, 8,
                                                 9, 10, 11, 12,
                                                 13, 14, 15, 0};
#define COUNTER 0
#define ITER_NUMBER 1000000


class GameState {
public:
    GameState(const std::array<char, numOfCells> &field);

    GameState(const GameState &other);
    bool IsSolvable() const;
    bool IsCompleted() const;
    bool CanMoveLeft() const;
    bool CanMoveRight() const;
    bool CanMoveUp() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveRight() const;
    GameState MoveUp() const;
    GameState MoveDown() const;

    size_t ComputeDistance() const;

    size_t GetEmptyIndex() const { return emptyPosition; }

    bool operator==(const GameState &other) const;

    size_t TotalManhattanDistance() const; // расстояние между текущим и финишным состояниями игры

private:
    std::array<char, numOfCells> field;
    char emptyPosition;

    size_t CountInversions() const;
    size_t FindFirstIncorrect() const;
    size_t GetElemPosition(size_t elem) const;
    size_t CalculateManhattanDistance(size_t first, size_t second) const;


    friend struct GSHasher;
};

size_t GameState::TotalManhattanDistance() const {
    size_t dist = 0;
    for (int i = 0; i < numOfCells; ++i) {
        if (field[i] && field[i] != i + 1) {
            dist += CalculateManhattanDistance(i, field[i] - 1);
        }
    }
    return dist;
}

size_t GameState::CountInversions() const {
    size_t invCount = 0;
    for (size_t i = 0; i < numOfCells - 1; i++) {
        for (size_t j = i + 1; j < numOfCells; j++) {
            if (field[i] && field[j] && field[i] > field[j])
                invCount++;
        }
    }
    return invCount;
}

size_t GameState::FindFirstIncorrect() const {
    for (size_t i = 0; i < numOfCells; ++i) {
        if (field[i] != i + 1) {
            return i + 1;
        }
    }
    return 0;
}

size_t GameState::GetElemPosition(size_t elem) const {
    for (size_t i = 0; i < numOfCells; ++i) {
        if (field[i] == elem) {
            return i;
        }
    }
    return 0;
}

size_t GameState::CalculateManhattanDistance(size_t first, size_t second) const {
    size_t x1 = first / sizeOfSize, y1 = first % sizeOfSize;
    size_t x2 = second / sizeOfSize, y2 = second % sizeOfSize;
    return abs(x1 - x2) + abs(y1 - y2);
}

std::ostream& operator<<(std::ostream &out, const std::array<char, numOfCells> &arr) {
    for (int i = 0; i < sizeOfSize; i++) {
        for (int j = 0; j < sizeOfSize; j++) {
            out << static_cast<int>(arr[i * sizeOfSize + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

GameState::GameState(const std::array<char, numOfCells> &field) : field(field){
    emptyPosition = -1;
    for (int i = 0; i < numOfCells; i++) {
        if (field[i] == 0)
            emptyPosition = i;
    }
    assert(emptyPosition != -1);
}

GameState::GameState(const GameState &other) :
    field(other.field), emptyPosition(other.emptyPosition) {}


bool GameState::IsSolvable() const {
    size_t invCount = CountInversions();
    unsigned int numberOfStr = emptyPosition / sizeOfSize + 1;
    if ((invCount + numberOfStr) % 2 == 0) {
        return true;
    } else {
        return false;
    }
}


bool GameState::IsCompleted() const {
    return field == finishField;
}

bool GameState::CanMoveLeft() const {
    return emptyPosition % sizeOfSize != 3;
}

bool GameState::CanMoveRight() const {
    return emptyPosition % sizeOfSize != 0;
}

bool GameState::CanMoveUp() const {
    return emptyPosition < 12;
}

bool GameState::CanMoveDown() const {
    return emptyPosition > 3;
}

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());

    GameState newState(*this);
    std::swap(newState.field[emptyPosition], newState.field[emptyPosition + 1]);
    newState.emptyPosition++;
    return newState;
}

GameState GameState::MoveRight() const {
    GameState newState(*this);
    std::swap(newState.field[emptyPosition], newState.field[emptyPosition - 1]);
    newState.emptyPosition--;
    return newState;
}

GameState GameState::MoveUp() const {
    GameState newState(*this);
    std::swap(newState.field[emptyPosition], newState.field[emptyPosition + 4]);
    newState.emptyPosition += sizeOfSize;
    return newState;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());

    GameState newState(*this);
    std::swap(newState.field[emptyPosition], newState.field[emptyPosition - sizeOfSize]);
    newState.emptyPosition -= sizeOfSize;
    return newState;
}

bool GameState::operator==(const GameState &other) const {
    return field == other.field;
}

size_t GameState::ComputeDistance() const {
    return TotalManhattanDistance();
}

bool operator<(const GameState& left, const GameState& right) {
    return left.ComputeDistance() < right.ComputeDistance();
}

struct GSHasher {
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};


std::string GameSolution(const std::array<char, numOfCells> &initialState) {
    GameState startState(initialState);

    unordered_map<GameState, char, GSHasher> visited;
    visited[startState] = 'S';

    if (!startState.IsSolvable()) {
        return "-1";
    }


    std::set<std::pair<size_t, GameState>> distancesToStates;
    size_t startDistance = startState.ComputeDistance();
    distancesToStates.insert({startDistance, startState});
    int iterCnt = 0;
    while (distancesToStates.size()) {
        std::pair <size_t, GameState> currentVertex = *(distancesToStates.begin());
        GameState currState = currentVertex.second;
        distancesToStates.erase(distancesToStates.begin());
        ++iterCnt;

        if (currState.IsCompleted())
            break;

        if (currState.CanMoveLeft()) {
            GameState newState = currState.MoveLeft();
            size_t newDistance = newState.ComputeDistance();
            if (!visited.count(newState)) {
                visited[newState] = 'L';
                distancesToStates.insert({newDistance, newState});
            }
        }
        if (currState.CanMoveRight()) {
            GameState newState = currState.MoveRight();
            size_t newDistance = newState.ComputeDistance();
            if (!visited.count(newState)) {
                visited[newState] = 'R';
                distancesToStates.insert({newDistance, newState});
            }
        }
        if (currState.CanMoveUp()) {
            GameState newState = currState.MoveUp();
            size_t newDistance = newState.ComputeDistance();
            if (!visited.count(newState)) {
                visited[newState] = 'U';
                distancesToStates.insert({newDistance, newState});
            }
        }
        if (currState.CanMoveDown()) {
            GameState newState = currState.MoveDown();
            size_t newDistance = newState.ComputeDistance();
            if (!visited.count(newState)) {
                visited[newState] = 'D';
                distancesToStates.insert({newDistance, newState});
            }
        }
    }

    std::string solutionPath;
    GameState currState(finishField);
    while (visited[currState] != 'S') {
        char move = visited[currState];
        switch (move) {
            case 'L': {
                currState = currState.MoveRight();
                solutionPath += 'L';
                break;
            }
            case 'R': {
                currState = currState.MoveLeft();
                solutionPath += 'R';
                break;
            }
            case 'U': {
                currState = currState.MoveDown();
                solutionPath += 'U';
                break;
            }
            case 'D': {
                currState = currState.MoveUp();
                solutionPath += 'D';
                break;
            }
        }
    }

    std::reverse(solutionPath.begin(), solutionPath.end());
    return solutionPath;
}

int main() {
    std::array<char, numOfCells> initialState;
    for (int i = 0, val; i < numOfCells; ++i) {
        std::cin >> val;
        initialState[i] = val;
    }

    string solution = GameSolution(initialState);
    if (solution == "-1") std::cout << -1;
    else std::cout << solution.size() << std::endl << solution << std::endl;
    return 0;
}
