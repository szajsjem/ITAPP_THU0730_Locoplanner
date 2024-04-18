#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <algorithm>
#define MAX(x,y) (((x)>(y))?(y):(x))
using namespace std;

#define _DEBUG

struct krawedz {
	int waga;
	int a;
	int b;
};


class grafmacierzowy {
	int** graf = NULL;
	int allocgraf = 0;
	const int notconnected = 0;
	int ilew = 0;
	int pocztk = 0;

	void mekenewgraph() {
		if (graf != NULL) {
			for (int i = 0; i < allocgraf; i++) {
				delete[] graf[i];
			}
			delete[] graf;
		}
		allocgraf = ilew;
		graf = new int* [allocgraf];
		for (int i = 0; i < allocgraf; i++) {
			graf[i] = new int[allocgraf];
			memset(graf[i], notconnected, allocgraf * sizeof(int));
		}
	}
public:
	~grafmacierzowy() {
		if (graf != NULL) {
			for (int i = 0; i < allocgraf; i++) {
				delete[] graf[i];
			}
			delete[] graf;
		}
	}
	void loaddirected(string FileName) {
		fstream file(FileName, ios::in);
		if (file.is_open()) {
			int krawedzie, wierzcholki, wierzpocz;
			file >> krawedzie;
			file >> wierzcholki;
			file >> wierzpocz;

			pocztk = wierzpocz;

			ilew = wierzcholki;
			mekenewgraph();

			for (int i = 0; i < krawedzie; i++) {
				int pocz, kon, waga;
				file >> pocz;
				file >> kon;
				file >> waga;

				graf[pocz][kon] = waga;

				//graf[kon][pocz] = waga;
			}
		}
	}
	void loadundirected(string FileName) {
		fstream file(FileName, ios::in);
		if (file.is_open()) {
			int krawedzie, wierzcholki, wierzpocz;
			file >> krawedzie;
			file >> wierzcholki;
			//file >> wierzpocz;

			//pocztk = wierzpocz;

			ilew = wierzcholki;
			mekenewgraph();

			for (int i = 0; i < krawedzie; i++) {
				int pocz, kon, waga;
				file >> pocz;
				file >> kon;
				file >> waga;

				graf[pocz][kon] = waga;

				graf[kon][pocz] = waga;
			}
		}
	}
	int liczkrawedzie() {
		int s = 0;
		for (int x = 0; x < ilew; x++)
			for (int y = 0; y < ilew; y++)
				if (graf[x][y] != notconnected)
					s++;
		return s;
	}
	void randomdirectional(int ilewierz, int ilekraw) {
		ilew = ilewierz;
		mekenewgraph();
		int* petla = new int[ilew - 1];
		for (int i = 1; i < ilew; i++)
			petla[i - 1] = i;
		for (int i = 0; i < ilew - 1; i++)
			swap(petla[i], petla[rand() % (ilew - 1)]);

		int pop = 0;
		for (int i = 0; i < ilew - 1; i++) {
			graf[pop][petla[i]] = rand() % 1000 + 1;
			pop = petla[i];
		}
		graf[pop][0] = rand() % 1000 + 1;
		//mamy zrobiony cykl

		int ilekr = liczkrawedzie();
		while (ilekr < ilekraw) {
			int a = rand() % ilew;
			int b = rand() % ilew;
			if (a == b)continue;
			if (graf[a][b] == notconnected) {
				ilekr++;
				graf[a][b] = rand() % 1000 + 1;
			}
		}

		delete[] petla;
	}
	void randomundirectional(int ilewierz, int ilekraw) {
		ilew = ilewierz;
		ilekraw /= 2;
		mekenewgraph();
		int* wki = new int[ilew];
		int* wys = new int[ilew];
		for (int i = 0; i < ilew; i++) {
			wys[i] = 1;
			wki[i] = i;
		}
		int ilewdrzew = 0;
		while (ilewdrzew < (ilew - 1)) {
			krawedz t;
			t.a = rand() % ilew;
			t.b = rand() % (ilew - t.a) + t.a;
			if (t.a == t.b)continue;
			t.waga = rand() % 1000 + 1;
			int ta = t.a;
			int tb = t.b;
			while (wki[ta] != ta)ta = wki[ta];
			while (wki[tb] != tb)tb = wki[tb];
			if (ta != tb) {
				graf[t.a][t.b] = t.waga;//
				ilewdrzew++;
				if (wys[ta] < wys[tb])
					wki[ta] = tb;
				else {
					wki[tb] = ta;
					wys[ta] = MAX(wys[tb] + 1, wys[ta]);
				}

			}
		}
		//mamy zrobione drzewo



		int ilekr = liczkrawedzie();
		while (ilekr < ilekraw) {
			int a = rand() % ilew;
			int b = rand() % (ilew - a) + a;
			if (a == b)continue;
			if (graf[a][b] == notconnected) {
				ilekr++;
				graf[a][b] = rand() % 1000 + 1;
			}
		}

		//mirror krawedzie
		for (int x = 0; x < ilew; x++)
			for (int y = x + 1; y < ilew; y++)
				graf[y][x] = graf[x][y];


		delete[] wki;
		delete[] wys;
	}
	void minsciezkaDjikstry(int starting = 0) {
		if (starting < 0)starting = pocztk;
		if (starting >= ilew)starting = pocztk;
		static int* odleglosc = new int[ilew];
		int* parent = new int[ilew];
		for (int i = 0; i < ilew; i++) {
			parent[i] = -1;
			odleglosc[i] = 0x7fffffff;
		}
		int* kolejka=new int[ilew];
		int kolsize = ilew;

		odleglosc[starting] = 0;

		for (int i = 0; i < kolsize; i++)
			kolejka[i] = i;
		std::qsort(kolejka, kolsize, sizeof(int), [](const void* a, const void* b) {
			int& l = *(int*)a;
			int& r = *(int*)b;
			if (odleglosc[l] < odleglosc[r])
				return 1;
			if (odleglosc[l] > odleglosc[r])
				return -1;
			return 0;
			});

		while (kolsize > 0) {
			kolsize--;
			int u = kolejka[kolsize]; 
			bool sort = 0;
			for (int i = 0; i < ilew; i++) {
				int odlui = graf[u][i];
				if (odlui != notconnected) {
					odlui += odleglosc[u];
					if (odleglosc[i] > odlui) {
						odleglosc[i] = odlui;
						parent[i] = u;
						sort = 1;
					}
				}
			}
			if (sort && kolsize>0) {
				std::qsort(kolejka, kolsize, sizeof(int), [](const void*a,const void*b) {
					int& l = *(int*)a;
					int& r = *(int*)b;
					if(odleglosc[l]<odleglosc[r])
						return 1;
					if (odleglosc[l] > odleglosc[r])
						return -1;
					return 0;
					});
			}
		}


#ifdef _DEBUG
		printsciezka(starting, parent, odleglosc);
#endif

		delete[] odleglosc;
		delete[] parent;
	}
	void minsciezkaFordaBellmana(int start = 0) {
		if (start < 0)start = pocztk;
		if (start >= ilew)start = pocztk;
		int ilekraw = 0;
		krawedz* kr = new krawedz[ilew * ilew];//-ilew, albo trzymac ile jest wierzcholkow
		for (int x = 0; x < ilew; x++) {
			for (int y = 0; y < ilew; y++) {
				int odl = graf[x][y];
				if (odl != notconnected) {
					kr[ilekraw++] = { odl,x,y };
				}
			}
		}
		int* odleglosc = new int[ilew];
		int* parent = new int[ilew];
		for (int i = 0; i < ilew; i++) {
			parent[i] = -1;
			odleglosc[i] = 0x00ffffff;
		}

		odleglosc[start] = 0;


		for (int i = 0; i < ilew - 1; i++) {
			for (int ii = 0; ii < ilekraw; ii++) {
				krawedz t = kr[ii];
				int odl = odleglosc[t.a] + t.waga;
				if (odl < odleglosc[t.b]) {
					odleglosc[t.b] = odl;
					parent[t.b] = t.a;
				}
			}
		}

		for (int ii = 0; ii < ilekraw; ii++) {
			krawedz t = kr[ii];
			int odl = odleglosc[t.a] + t.waga;
			if (odl < odleglosc[t.b]) {
				//petla
			}
		}

#ifdef _DEBUG
		printsciezka(start, parent, odleglosc);
#endif


		delete[] odleglosc;
		delete[] parent;
		delete[] kr;

	}
	void print() {
		printf("\n");
		for (int x = 0; x < ilew; x++) {
			for (int y = 0; y < ilew; y++) {
				printf("%4d ", graf[x][y]);
			}
			printf("\n");
		}
	}

	void printsciezka(int start, int* parent, int* odleglosc) {
		printf("wierzcholek	odleglosc	droga\n");
		for (int i = 0; i < ilew; i++) {
			printf("%d			%d			", i, odleglosc[i]);
			int p = i;
			while (p != -1) {
				printf("%d ", p);
				p = parent[p];
			}
			printf("\n");
		}
	}
};

