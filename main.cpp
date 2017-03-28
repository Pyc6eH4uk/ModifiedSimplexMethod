#include <bits/stdc++.h>

using namespace std;

const int N = 2000;
int n, m, worth_line, better_x;
double sx_tab[N + 1][N + 2];
double A[N][N];
double coeff_func[N];
double B[N];
double delta[N];
int basis[N + 1];

/*Рисуем нашу таблицу*/
void print_simplex_table() {
    for (int i = 0; i < m + 1; i++) {
        for (int j = 0; j < m + 2; j++) {
            cout << sx_tab[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/*Вычисляем оценки(delta)*/
void evaluate_delta() {
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < n; j++) {
            delta[j] = 0;
            for (int k = 0; k < m; k++) {
                delta[j] += sx_tab[m][k + 1] * A[k][j];
            }
            delta[j] -= coeff_func[j];
        }
    }
}

/*Пересчитываем*/
void evaluate_A() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 1; j++) {
            sx_tab[i][m + 1] = 0;
            for (int k = 0; k < m; k++) {
                sx_tab[i][m + 1] += sx_tab[i][k + 1] * A[k][better_x];
            }
            sx_tab[m][m + 1] = delta[better_x];
        }
    }
}

/*Находим минимальную оценку по знаку*/
void find_better_x() {
    better_x = (int) (min_element(delta, delta + n) - delta);
}

/*Находим худшую строку, которую будем изменять преобразованием Гаусса*/
void find_worth_line() {
    worth_line = -1;
    int worth_value = INT_MAX;

    for (int i = 0; i < m; i++) {
        if (sx_tab[i][m + 1] > 0 && worth_value > B[i] / sx_tab[i][m + 1]) {
            worth_value = B[i] / sx_tab[i][m + 1];
            worth_line = i;
        }
    }

    if (worth_line == -1) {
    	cout << "Функция не ограничена сверху, решения не существует" << endl;
    	return;
    }
}

/*Создаём таблицу*/
void create_table() {

    for (int i = 0; i < m; i++) {
        sx_tab[i][0] = B[i];
    }

    for (int i = 0; i < n; i++) {
        delta[i] = -coeff_func[i];
    }

    find_better_x();

    for (int i = 0; i < m; i++) {
        basis[i] = n + m;
    }

    for (int i = 0; i < m; i++) {
        sx_tab[i][i + 1] = 1;
    }

    for (int i = 0; i < m; i++) {
        sx_tab[i][m + 1] = A[i][better_x];
    }
    sx_tab[m][m + 1] = -coeff_func[better_x];

    print_simplex_table();

    find_worth_line();

    basis[worth_line] = better_x;
}

/*Вычисление таблицы*/
bool evaluate_table() {
    double coefficient = sx_tab[worth_line][m + 1];

    for (int i = 0; i < m + 2; i++) {
        sx_tab[worth_line][i] /= coefficient;
    }

    for (int i = 0; i < m + 1; i++) {
        if (i == worth_line) continue;
        coefficient = sx_tab[i][m + 1];
        for (int j = 0; j < m + 2; j++) {
            sx_tab[i][j] -= coefficient * sx_tab[worth_line][j];
        }
    }
    
    evaluate_delta();
    print_simplex_table();
    find_better_x();
    evaluate_A();
    print_simplex_table();
    find_worth_line();
    basis[worth_line] = better_x;
    print_simplex_table();
    for (int i = 0; i < n; i++) {
        if (delta[i] < 0) {
            return false;
        }
    }
    return true;
}
/*Заполняем данные*/
int main() {
    freopen("input", "r", stdin);

    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> coeff_func[i];
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        cin >> B[i];
    }
    /*Создаём таблицу*/
    create_table();
    /*Пока не получим решение производим вычисления*/
    while (!evaluate_table());
    /*Распечатываем таблицу*/
    print_simplex_table();
}
