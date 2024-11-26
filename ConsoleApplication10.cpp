#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <iomanip>
#include <climits>
#include <cstring>  // Для использования strcmp

using namespace std;

// Функция для генерации матрицы смежности
vector<vector<int>> generateAdjMatrix(int n, bool weighted, bool directed) {
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    srand(time(0));  // Инициализация генератора случайных чисел

    // Заполняем матрицу смежности в зависимости от типа графа
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && rand() % 2) {  // Ребро существует с вероятностью 50%, не самосоединение
                int weight = (weighted) ? rand() % 10 + 1 : 1;  // Если взвешенный граф, вес от 1 до 10, если не взвешенный — 1
                adjMatrix[i][j] = weight;

                // Если граф неориентированный, добавляем обратное ребро
                if (!directed) {
                    adjMatrix[j][i] = weight;
                }
            }
        }
    }

    return adjMatrix;
}

// Функция для вывода матрицы смежности
void printAdjMatrix(const vector<vector<int>>& adjMatrix) {
    for (const auto& row : adjMatrix) {
        for (int val : row) {
            cout << std::setw(2) << val << " ";
        }
        cout << endl;
    }
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

// Функция для нахождения радиуса, диаметра, центральных и периферийных вершин
void findGraphProperties(const vector<vector<int>>& adjMatrix, int& radius, int& diameter, vector<int>& centralVertices, vector<int>& peripheralVertices) {
    int n = adjMatrix.size();
    vector<int> dist(n);

    int maxDist = 0;  // Для вычисления диаметра
    int minDist = INT_MAX;  // Для вычисления радиуса

    // Перебираем все вершины
    for (int i = 0; i < n; i++) {
        dist = BFSD(adjMatrix, i);  // Получаем расстояния от вершины i
        int maxDistance = 0;
        for (int d : dist) {
            if (d != -1) {  // Если вершина достижима
                maxDistance = max(maxDistance, d);  // Находим максимальное расстояние от вершины i
            }
        }

        // Находим диаметр
        maxDist = max(maxDist, maxDistance);

        // Находим радиус
        minDist = min(minDist, maxDistance);

        // Добавляем центральные и периферийные вершины
        if (maxDistance == minDist) {
            centralVertices.push_back(i);
        }
        if (maxDistance == maxDist) {
            peripheralVertices.push_back(i);
        }
    }

    radius = minDist;  // Присваиваем радиус
    diameter = maxDist;  // Присваиваем диаметр
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    bool weighted = false;   // По умолчанию граф не взвешенный
    bool directed = false;   // По умолчанию граф не ориентированный
    int n = 0;               // Количество вершин
    int start = -1;          // Исходная вершина

    // Обработка параметров командной строки
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-weighted") == 0) {
            weighted = true;  // Взвешенный граф
        }
        else if (strcmp(argv[i], "-directed") == 0) {
            directed = true;  // Ориентированный граф
        }
        else if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                n = atoi(argv[i + 1]);  // Количество вершин
                i++;  // Пропускаем следующий аргумент
            }
        }
        else if (strcmp(argv[i], "-start") == 0) {
            if (i + 1 < argc) {
                start = atoi(argv[i + 1]);  // Исходная вершина
                i++;  // Пропускаем следующий аргумент
            }
        }
    }

    // Проверка, что все необходимые параметры указаны
    if (n == 0 || start == -1) {
        cout << "Ошибка: необходимо указать количество вершин и исходную вершину.\n";
        return 1;
    }

    if (start < 0 || start >= n) {
        cout << "Ошибка: исходная вершина должна быть в пределах от 0 до " << n - 1 << endl;
        return 1;
    }

    // Генерация матрицы смежности в зависимости от параметров
    vector<vector<int>> adjMatrix = generateAdjMatrix(n, weighted, directed);

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

    // Нахождение радиуса, диаметра и центральных/периферийных вершин
    int radius, diameter;
    vector<int> centralVertices, peripheralVertices;
    findGraphProperties(adjMatrix, radius, diameter, centralVertices, peripheralVertices);

    cout << "\nРадиус графа: " << radius << endl;
    cout << "Диаметр графа: " << diameter << endl;

    cout << "\nЦентральные вершины: ";
    for (int v : centralVertices) {
        cout << v << " ";
    }
    cout << endl;

    cout << "\nПериферийные вершины: ";
    for (int v : peripheralVertices) {
        cout << v << " ";
    }
    cout << endl;
    system("pause");
    return 0;
}
