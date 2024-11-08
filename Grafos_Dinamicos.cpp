#include "MiniWin.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <cstdio>
#include <vector>
#include <queue>
#include <stack>

using namespace std;
using namespace miniwin;
HWND hwnd;

int xVertice = 100;
int yVertice = 100;
int indice = 0;

void NewVertice(int v) {
    double angulo = 2 * M_PI * indice / v;
    xVertice = 500 + 240 * cos(angulo);
    yVertice = 300 + 240 * sin(angulo);
}

void DibujarFlecha(int xI, int yI, int xF, int yF) {
    double angulo = atan2(yF - yI, xF - xI);
    double longitudFlecha = 15.0;
    double anguloFlecha = M_PI / 6;

    int xM = (xI + xF) / 2;
    int yM = (yI + yF) / 2;

    int x3 = xM - longitudFlecha * cos(angulo + anguloFlecha);
    int y3 = yM - longitudFlecha * sin(angulo + anguloFlecha);

    int x4 = xM - longitudFlecha * cos(angulo - anguloFlecha);
    int y4 = yM - longitudFlecha * sin(angulo - anguloFlecha);

    linea(xM, yM, x3, y3);
    linea(xM, yM, x4, y4);
}

void DibujarVertice(char *n, int v) {
    NewVertice(v);
    color(AZUL);
    circulo_lleno(xVertice, yVertice, 20);
    color(BLANCO);
    char c = *n;
    string s;
    s.push_back(c);
    texto(xVertice - 4, yVertice - 10, s);
}

void DibujarArista(int xI, int yI, int xF, int yF, char*n){
    char c = *n;
    string s;
    s.push_back(c);



    if (xI == xF && yI == yF) {
        int numSegments = 10;
        float segmentLength = 1.0/ numSegments;

        int colors[10][3] = {
                {204, 255, 0},
                {0, 0, 255},
                {0, 0, 255},
                {0, 0, 255},
                {0, 102, 153},
                {0, 204, 51},
                {0, 255, 0},
                {51, 255, 0},
                {102, 255, 0},
                {153, 255, 0}
        };

        float angleStep = 2 * M_PI / numSegments;
        int radio = 20;
        int xc = xI;
        int yc = yI-20;
        for (int i = 0; i < numSegments; ++i) {
            float startAngle = i * angleStep;
            float endAngle = (i + 1) * angleStep;

            int xStart = xc + radio * cos(startAngle);
            int yStart = yc + radio * sin(startAngle);
            int xEnd = xc + radio * cos(endAngle);
            int yEnd = yc + radio * sin(endAngle);

            color_rgb(colors[i][0], colors[i][1], colors[i][2]);
            linea(xStart, yStart, xEnd, yEnd);
        }

        texto(xc-4, yc - radio - 13, s);
    }

    else {
        int numSegments = 10;
        float segmentLength = 1.0/ numSegments;

        int colors[10][3] = {
                {0, 0, 255},
                {0, 51, 204},
                {0, 102, 153},
                {0, 153, 102},
                {0, 204, 51},
                {0, 255, 0},
                {51, 255, 0},
                {102, 255, 0},
                {153, 255, 0},
                {204, 255, 0}
        };

        for (int i = 0; i < numSegments; ++i) {
            float startRatio = i * segmentLength;
            float endRatio = (i + 1) * segmentLength;

            int xStart = xI + (xF - xI) * startRatio;
            int yStart = yI + (yF - yI) * startRatio;
            int xEnd = xI + (xF - xI) * endRatio;
            int yEnd = yI + (yF - yI) * endRatio;

            color_rgb(colors[i][0], colors[i][1], colors[i][2]);
            linea(xStart, yStart + 7, xEnd, yEnd + 7);
            if(i==9){
                DibujarFlecha(xStart, yStart + 7, xEnd, yEnd + 7);
            }
        }

        texto(((xI + xF) / 2), ((yI + yF) / 2)-5, s);
    }
}

class arista;
class vertice {
public:
    vertice *sig;
    char *Etiqueta;
    arista *ini;
    int X_pos;
    int Y_pos;
    vertice(char *eti) {
        ini = NULL;
        sig = NULL;
        X_pos = Y_pos = 0;
        Etiqueta = new char[strlen(eti) + 1];
        strcpy(Etiqueta, eti);
    }
};

class arista {
public:
    arista *sig;
    char *Etiqueta;
    vertice *ultimo;
    arista(vertice *ult, char *eti) {
        sig = NULL;
        ultimo = ult;
        Etiqueta = new char[strlen(eti) + 1];
        strcpy(Etiqueta, eti);
    }
};

class grafo {
public:
    vertice *ini;
    int numVertices;
    vector<vector<int>> matrizAdyacencia;
    vector<vector<int>> listaAdyacencia;

    grafo() {
        ini = NULL;
        numVertices = 0;
        matrizAdyacencia.resize(100, vector<int>(100, 0));
        listaAdyacencia.resize(100);
    }

    void ponVertice(char *vt) {
        if (ini == NULL) {
            ini = new vertice(vt);
        } else {
            vertice *tmp;
            for (tmp = ini; tmp->sig != NULL; tmp = tmp->sig);
            tmp->sig = new vertice(vt);
        }
        numVertices++;
        matrizAdyacencia[numVertices - 1].resize(numVertices, 0);
        listaAdyacencia[numVertices - 1].clear();
    }

    void ponArista(char *v1, char *v2, char *eti) {
        arista *tmp3;
        vertice *tmp = ini, *tmp2 = ini;
        while (tmp->sig != NULL && strcmp(tmp->Etiqueta, v1) != 0)
            tmp = tmp->sig;
        while (tmp2->sig != NULL && strcmp(tmp2->Etiqueta, v2) != 0)
            tmp2 = tmp2->sig;

        if (strcmp(tmp->Etiqueta, v1) == 0 && strcmp(tmp2->Etiqueta, v2) == 0) {
            if (tmp->ini == NULL) {
                tmp->ini = new arista(tmp2, eti);
                tmp3 = tmp->ini;
            } else {
                tmp3 = tmp->ini;
                while (tmp->ini->sig != NULL)
                    tmp->ini = tmp->ini->sig;
                tmp->ini->sig = new arista(tmp2, eti);
            }
            tmp->ini = tmp3;

            int index1 = getIndex(v1);
            int index2 = getIndex(v2);
            if (index1 != -1 && index2 != -1) {
                matrizAdyacencia[index1][index2] = 1;
                listaAdyacencia[index1].push_back(index2);
            }
        } else {
            cout << "No existe nexo" << endl;
            getch();
        }
    }

    int getIndex(char *v) {
        vertice *tmp = ini;
        int index = 0;
        while (tmp != NULL) {
            if (strcmp(tmp->Etiqueta, v) == 0) {
                return index;
            }
            tmp = tmp->sig;
            index++;
        }
        return -1; // No encontrado
    }

    void mostrarMatrizAdyacencia() {
        cout << "Matriz de Adyacencia:" << endl;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                cout << matrizAdyacencia[i][j] << " ";
            }
            cout << endl;
        }
    }

    void mostrarListaAdyacencia() {
        cout << "Lista de Adyacencia:" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << i << ": ";
            for (int j : listaAdyacencia[i]) {
                cout << j << " ";
            }
            cout << endl;
        }
    }

    void BFS(char *start) {
        int startIndex = getIndex(start);
        if (startIndex == -1) return;

        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[startIndex] = true;
        q.push(startIndex);

        cout << "BFS desde " << start << ": ";
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (int i : listaAdyacencia[node]) {
                if (!visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        cout << endl;
    }

    void DFSUtil(int node, vector<bool> &visited) {
        visited[node] = true;
        cout << node << " ";

        for (int i : listaAdyacencia[node]) {
            if (!visited[i]) {
                DFSUtil(i, visited);
            }
        }
    }

    void DFS(char *start) {
        int startIndex = getIndex(start);
        if (startIndex == -1) return;

        vector<bool> visited(numVertices, false);
        cout << "DFS desde " << start << ": ";
        DFSUtil(startIndex, visited);
        cout << endl;
    }

    char* eliminarArista(char* v1, char* v2) {
        vertice* tmp = ini;
        while (tmp != NULL && strcmp(tmp->Etiqueta, v1) != 0) {
            tmp = tmp->sig;
        }

        if (tmp == NULL) {
            cout << "El vértice de origen no existe." << endl;
            return NULL;
        }

        arista* tmp2 = NULL;
        arista* tmp3 = tmp->ini;

        while (tmp3 != NULL && strcmp(tmp3->ultimo->Etiqueta, v2) != 0) {
            tmp2 = tmp3;
            tmp3 = tmp3->sig;
        }

        if (tmp3 == NULL) {
            cout << "La arista no existe." << endl;
            return NULL;
        }

        if (tmp2 == NULL) {
            tmp->ini = tmp3->sig;
        } else {
            tmp2->sig = tmp3->sig;
        }
        char* A = tmp3->Etiqueta;
        delete[] tmp3->Etiqueta;
        delete tmp3;
        return A;
    }

    char* eliminarVertice(char* v1) {
        vertice* tmp = ini;
        vertice* tmp2 = NULL;

        while (tmp != NULL && strcmp(tmp->Etiqueta, v1) != 0) {
            tmp2 = tmp;
            tmp = tmp->sig;
        }

        if (tmp == NULL) {
            cout << "No existe el vertice" << endl;
            return NULL;
        }

        for (vertice* tmp3 = ini; tmp3 != NULL; tmp3 = tmp3->sig) {
            eliminarArista(tmp3->Etiqueta, v1);
        }

        if (tmp2 == NULL) {
            ini = tmp->sig;
        } else {
            tmp2->sig = tmp->sig;
        }

        char* A = tmp->Etiqueta;
        delete[] tmp->Etiqueta;
        delete tmp;
        numVertices--;
        return A;
    }

    void DibujarGrafo() {
        vertice* tmp = ini;
        indice = 0;
        borra();
        for (tmp = ini; tmp != NULL; tmp = tmp->sig) {
            DibujarVertice(tmp->Etiqueta, numVertices);
            tmp->X_pos = xVertice;
            tmp->Y_pos = yVertice;
            indice++;
        }

        for (vertice *tmp2 = ini; tmp2 != NULL; tmp2 = tmp2->sig) {
            for (arista *tmp3 = tmp2->ini; tmp3 != NULL; tmp3 = tmp3->sig) {
                DibujarArista(tmp2->X_pos, tmp2->Y_pos, tmp3->ultimo->X_pos, tmp3->ultimo->Y_pos, tmp3->Etiqueta);
            }
        }
        refresca();
    }
};

void moverConsola(int x, int y, int ancho, int alto) {
    HWND consola = GetConsoleWindow();
    SetWindowPos(consola, HWND_TOP, x, y, ancho, alto, SWP_SHOWWINDOW);
}

void moverVentanaMiniWin(int x, int y, int ancho, int alto) {
    HWND hwnd = GetForegroundWindow(); // Obtiene el identificador de la ventana activa, que debería ser la ventana de MiniWin
    if (hwnd) {
        SetWindowPos(hwnd, HWND_TOP, x, y, ancho, alto, SWP_NOZORDER | SWP_NOSIZE);
    }
}

int main() {
    ///Modificar las posiciones de las ventanas
    moverConsola(100, 550, 1016, 190);
    vredimensiona(1000, 550);
    moverVentanaMiniWin(100, -30, 1000, 600);

    grafo g1;

    int n = 0;
    int opc;
    char C1, C2, C3;
    char A[2], B[2], C[2];

    while (n == 0) {
        g1.mostrarListaAdyacencia();
        g1.mostrarMatrizAdyacencia();
        cout << "Desea realizar alguna operacion?: \n1:\tAgregar Vertice\n2:\tAgregar Arista\n3:\tEliminar Vertice\n4:\tEliminar Arista\n5:\tBFS\n6:\tDFS\n7:\tSalir" << endl;
        cin >> opc;

        switch (opc) {
        case 1:
            cout << "Ingrese el identificador del vertice: ";
            cin >> C1;
            sprintf(A, "%c", C1);
            g1.ponVertice(A);
            g1.DibujarGrafo();
            break;

        case 2:
            cout << "Ingrese el primer vertice: ";
            cin >> C1;
            cout << "Ingrese el segundo vertice: ";
            cin >> C2;
            cout << "Ingrese el peso: ";
            cin >> C3;
            sprintf(A, "%c", C1);
            sprintf(B, "%c", C2);
            sprintf(C, "%c", C3);
            g1.ponArista(A, B, C);
            g1.DibujarGrafo();
            break;

        case 3:
            cout << "Ingrese el identificador del vertice a eliminar: ";
            cin >> C1;
            sprintf(A, "%c", C1);
            g1.eliminarVertice(A);
            g1.DibujarGrafo();
            break;

        case 4:
            cout << "Ingrese el primer vertice: ";
            cin >> C1;
            cout << "Ingrese el segundo vertice: ";
            cin >> C2;
            sprintf(A, "%c", C1);
            sprintf(B, "%c", C2);
            g1.eliminarArista(A, B);
            g1.DibujarGrafo();
            break;

        case 5:
            cout << "Ingrese el vertice de inicio para BFS: ";
            cin >> C1;
            sprintf(A, "%c", C1);
            g1.BFS(A);
            break;

        case 6:
            cout << "Ingrese el vertice de inicio para DFS: ";
            cin >> C1;
            sprintf(A, "%c", C1);
            g1.DFS(A);
            break;

        default:
            n = 1;
            break;
        }
    }
    vcierra();
    return 0;
}
