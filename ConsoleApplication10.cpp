#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

// Функция для генерации матрицы смежности
vector<vector<int>> generateAdjMatrix(int n) {
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    srand(time(0));  // Инициализация генератора случайных чисел

    // Заполняем матрицу случайными весами рёбер (от 1 до 10)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 2) {  // Ребро существует с вероятностью 50%
                int weight = rand() % 10 + 1;  // Вес рёбер от 1 до 10
                adjMatrix[i][j] = weight;
                adjMatrix[j][i] = weight;  // Граф неориентированный
            }
        }
    }
    return adjMatrix;
}
// Функция для поиска расстояний от исходной вершины
vector<int> BFSD(const vector<vector<int>>& adjMatrix, int start) {
    int n = adjMatrix.size();
    vector<int> dist(n, -1);  // Вектор расстояний, инициализируем -1 (не посещено)
    queue<int> q;

    // Исходная вершина
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();  // Текущая вершина
        q.pop();

        for (int i = 0; i < n; i++) {
            // Если существует ребро и вершина ещё не посещена
            if (adjMatrix[v][i] > 0 && dist[i] == -1) {
                dist[i] = dist[v] + adjMatrix[v][i];  // Обновляем расстояние
                q.push(i);  // Добавляем вершину в очередь
            }
        }
    }

    return dist;
}




// Функция для вывода матрицы смежности
void printAdjMatrix(const vector<vector<int>>& adjMatrix) {
    for (const auto& row : adjMatrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}


int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int n, start;

    // Ввод количества вершин графа
    cout << "Введите количество вершин в графе: ";
    cin >> n;

    // Ввод исходной вершины
    cout << "Введите исходную вершину (от 0 до " << n - 1 << "): ";
    cin >> start;

    if (start < 0 || start >= n) {
        cout << "Ошибка: исходная вершина должна быть в пределах от 0 до " << n - 1 << endl;
        return 1;
    }

    // Генерация случайной матрицы смежности
    vector<vector<int>> adjMatrix = generateAdjMatrix(n);

    // Вывод сгенерированной матрицы смежности
    cout << "Сгенерированная матрица смежности:\n";
    printAdjMatrix(adjMatrix);

    // Нахождение расстояний от исходной вершины
    vector<int> dist = BFSD(adjMatrix, start);

    // Вывод результатов
    cout << "Расстояния от вершины " << start << ":\n";
    for (int i = 0; i < dist.size(); i++) {
        if (dist[i] == -1) {
            cout << "Расстояние до вершины " << i << ": недоступно" << endl;
        }
        else {
            cout << "Расстояние до вершины " << i << ": " << dist[i] << endl;
        }
    }

    return 0;
}