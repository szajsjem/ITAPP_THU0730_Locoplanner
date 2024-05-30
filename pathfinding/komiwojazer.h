#pragma once

#include <math.h>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <functional>

template<class D>
void swap(D& a, D& b) {
	D t = a;
	a = b;
	b = t;
}

int bes170[] = {
	0 , 1 , 2 , 3 , 4 , 5 , 133 , 169 , 112 , 132 , 111 , 110 , 109 , 107 , 106 , 105 , 98 , 95 , 94 , 92 , 91 , 154 , 89 , 90 , 96 , 97 , 165 , 104 , 114 , 115 , 116 , 117 , 118 , 119 , 120 , 102 , 103 , 163 , 99 , 100 , 101 , 123 , 162 , 122 , 121 , 124 , 146 , 145 , 144 , 143 , 147 , 148 , 149 , 161 , 152 , 142 , 134 , 131 , 113 , 164 , 127 , 126 , 125 , 129 , 128 , 130 , 135 , 136 , 137 , 138 , 139 , 140 , 141 , 6 , 7 , 8 , 9 , 10 , 76 , 74 , 75 , 11 , 12 , 13 , 21 , 29 , 30 , 28 , 27 , 26 , 22 , 23 , 24 , 25 , 150 , 160 , 151 , 14 , 15 , 159 , 16 , 17 , 18 , 19 , 20 , 32 , 158 , 157 , 31 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 156 , 155 , 41 , 42 , 44 , 45 , 46 , 47 , 48 , 51 , 52 , 53 , 43 , 55 , 54 , 58 , 59 , 60 , 68 , 67 , 167 , 70 , 69 , 66 , 63 , 64 , 56 , 57 , 62 , 61 , 65 , 88 , 153 , 87 , 85 , 86 , 93 , 166 , 108 , 83 , 84 , 71 , 50 , 49 , 170 , 168 , 72 , 73 , 77 , 78 , 82 , 79 , 80 , 81
};


class Komiwojazer
{
public:
	Komiwojazer();
	~Komiwojazer();
	void loadfromfile(std::string filename);
	void loadfromatsp(std::string filename);
	int metodazahlanna(int* pokolei, int& siz,int start);
	void generaterandom(int size);
	void print();
	int bruteforce(int* pokolei, int& siz);
	int branchbound(int* pokolei, int& siz);
	int progdyn(int* pokolei, int& siz);

	int nielosowe(int* pokolei,int& siz);
	int losowamindroga(int* pokolei, int& currlow);
	int localsearch(int* pokolei, int currmin, long long endtime);
	int localsearcheq(int* pokolei, int currmin, long long timel);
	int localsearchtabkol(int* pokolei, int& siz, long long endtime);
	int taboolocalsearch(int* pokolei, int& siz, long long endtime);

	int losowadroga(int* pokolei, int& currlow);
	int taboomove(int* pokolei, int droga, int* moves, int counter);
	int taboosearch(int* pokolei, int& siz, long long timeend);

private:
	void bRoad(int& lowest, int* remaining, int ile, int currentnum, int currentsum, int const* start, int const* copyloc, const int siz, const int endnum);
	void BnBRoad(int& lowest, int* remainingx, int* remainingy, int ile, int addmin, int* start, int* copyloc, const int siz, int*** tmptab, int** ff, bool makecopy);
	int pRoad(int* remaining, int ile, int currnum, int endnum);
	int** tab;
	int size;
	int** minroad;
	int** nextpoint;
};

Komiwojazer::Komiwojazer()
{
	tab = NULL;
	size = 0;
	minroad=NULL;
	nextpoint = NULL;
}

Komiwojazer::~Komiwojazer()
{
	if (tab != NULL) {
		for (int i = 0; i < size; i++)
			delete[] tab[i];
		delete[] tab;
	}
	tab = NULL;
	size = 0;
}

void Komiwojazer::loadfromfile(std::string filename) {
	this->~Komiwojazer();
	std::fstream file(filename, std::ios::in);
	file >> size;
	if (size <= 0)return;
	tab = new int* [size];
	for (int ii = 0; ii < size; ii++) {
		tab[ii] = new int[size];
		for (int i = 0; i < size; i++)
			file >> tab[ii][i];
	}
}

void Komiwojazer::generaterandom(int size) {
	this->~Komiwojazer();
	this->size = size;
	if (size <= 0)return;
	tab = new int* [size];
	for (int ii = 0; ii < size; ii++) {
		tab[ii] = new int[size];
		for (int i = 0; i < size; i++)
			if (ii == i)
				tab[ii][i] = 10000;
			else
				tab[ii][i] = 5+rand()%300;
	}
}

void Komiwojazer::print() {
	for (int ii = 0; ii < size; ii++) {
		for (int i = 0; i < size; i++)
			printf("%d ", tab[ii][i]);
		printf("\n");
	}
}

//rozmiar 14 -> 70s     ;z if: 21 -> 50s
int Komiwojazer::bruteforce(int *pokolei,int &siz) {
	int lowest = 100000; siz = size;
	int* tmp = new int[size];//tablica z wierzcholkami do przejœcia
	int* najmn = new int[size]; najmn[0] = 0;
	for (int i = 0; i < size; i++)
		tmp[i] = i;
	bRoad(lowest, tmp+1, size-1,0,0,tmp,najmn,size, 0);//znajdz najmniejsze
	memcpy(pokolei, najmn, sizeof(int) * size);
	delete[] tmp;
	delete[] najmn;
	return lowest;
}

//rozmiar 28 -> 25s     wysz naj zera 27 -> 68s
int Komiwojazer::branchbound(int* pokolei, int& siz) {
	int lowest = 100000; siz = size;
	int* tmpx = new int[size];
	int* tmpy = new int[size];
	int* najmn = new int[size];
	int* ouu = new int[size];
	int** ff = new int*[size];
	int*** tmptab = new int** [size+2];
	for (int i = 0; i < size; i++) {
		tmpx[i] = i;
		tmpy[i] = i;
		najmn[i] = -1;
		ff[i] = new int[size];
		for (int j = 0; j < size; j++)
			ff[i][j] = j;
	}
	for (int i = 0; i <= size+1; i++) {
		tmptab[i] = new int* [size];
		for (int j = 0; j < size; j++) {
			tmptab[i][j] = new int[size];
			if (i == (size+1)) {
				for (int y = 0; y < size; y++)
					tmptab[i][j][y] = tab[j][y];
			}
		}
	}
	BnBRoad(lowest, tmpx, tmpy, size, 0, najmn, ouu, 0, tmptab,ff,1);//znajdz najmniejsze
	delete[] tmpx;
	delete[] tmpy;
	for (int i = 0; i <= size+1; i++) {
		for (int j = 0; j < size; j++) {
			delete[] tmptab[i][j];
		}
		delete[] tmptab[i];
	}
	delete[] tmptab;

	pokolei[0] = 0;
	for (int i = 1; i < size; i++)
		pokolei[i] = ouu[pokolei[i - 1]];

	delete[] najmn;
	delete[] ouu;
	for (int i = 1; i < size; i++)
		delete[] ff[i];
	delete[] ff;
	return lowest;
}

//rozmiar 24 -> 54s
int Komiwojazer::progdyn(int* pokolei, int& siz) {
	siz = size;

	minroad = new int* [size];
	nextpoint = new int* [size];
	for (int i = 0; i < size; i++) {
		minroad[i] = new int[1 << size];
		memset(minroad[i], 0, sizeof(int) << size);
		nextpoint[i] = new int[1 << size];
		memset(nextpoint[i], 0, sizeof(int) << size );
	}
	int* tmp = new int[size];//tablica z wierzcholkami do przejœcia
	//for (int i = size; i > 0; i--)tmp[size - i] = i - 1;
	for (int i = 0; i < size; i++)tmp[i] = i;


	int lnum = *tmp;
	int lowest = pRoad(tmp + 1, size - 1, *tmp,*tmp);//znajdz najmniejsze


	int indx = (1 << size)-1;
	indx ^= 1 << lnum;
	pokolei[0] = lnum;
	for (int i = 1; i < size; i++) {
		int nast = nextpoint[lnum][indx];
		pokolei[i] = nast;
		indx ^= 1 << nast;
		lnum = nast;
	}

	for (int i = 0; i < size; i++) {
		delete[] minroad[i];
		delete[] nextpoint[i];
	}
	delete[] minroad;
	delete[] nextpoint;
	delete[] tmp;

	return lowest;
}

void Komiwojazer::bRoad(int& lowest, int* remaining, int ile,int currentnum,int currentsum,int const*start,int const*copyloc, const int siz, const int endnum) {
	//if (currentsum > lowest)return;
	if (ile == 1) {
		currentsum += tab[currentnum][*remaining];
		currentsum += tab[*remaining][endnum];
		if (currentsum < lowest) {
			lowest = currentsum;
			memcpy((void*)copyloc, start, sizeof(int) * siz);
		}
	}
	else {
		bRoad(lowest, remaining + 1, ile - 1, *remaining, currentsum + tab[currentnum][*remaining], start, copyloc, siz, endnum);
		for (int i = 1; i < ile; i++) {
			int t = remaining[i];
			remaining[i] = remaining[0];
			remaining[0] = t;
			bRoad(lowest, remaining + 1, ile - 1, t, currentsum + tab[currentnum][t], start, copyloc, siz, endnum);
			remaining[0] = remaining[i];
			remaining[i] = t;
		 }
	}
}

void Komiwojazer::BnBRoad(int& lowest, int* remainingx, int* remainingy, const int ile, int addmin, int* start, int* copyloc, const int siz, int*** tmptab, int** ff,bool makecopy) {

	if (lowest <= addmin)return;
	
	int** loctab = tmptab[ile];
	int* locff = ff[siz];

	if (makecopy) {
		//kopiowanie tablicy tymczasowej
		for (int x = 0; x < ile; x++) {
			for (int y = 0; y < ile; y++) {
				loctab[remainingx[x]][remainingy[y]] = tmptab[ile + 1][remainingx[x]][remainingy[y]];
			}
		}

		if (ile == 1) {
			addmin += loctab[*remainingx][*remainingy];
			if (addmin < lowest) {
				memcpy(copyloc, start, size * sizeof(int));
				copyloc[*remainingx] = *remainingy;
				lowest = addmin;
			}
			return;
		}

#ifndef tylko_kolumny
		//po wierszach
		for (int x = 0; x < ile; x++) {
			int min = 10000;
			for (int y = 0; y < ile; y++) {
				if (locff[remainingy[y]] == locff[remainingx[x]])
					continue;
				if (loctab[remainingx[x]][remainingy[y]] < min)
					min = loctab[remainingx[x]][remainingy[y]];
			}
			if (min == 0)continue;
			addmin += min;
			for (int y = 0; y < ile; y++) {
				loctab[remainingx[x]][remainingy[y]] -= min;
			}
		}
#endif

		//wy³¹czone
#ifdef tylko_wiersze
	//po kolumnach
		for (int y = 0; y < ile; y++) {
			double min = 10000;
			for (int x = 0; x < ile; x++) {
				if (locff[remainingy[y]] == locff[remainingx[x]])
					continue;
				if (loctab[remainingx[x]][remainingy[y]] < min)
					min = loctab[remainingx[x]][remainingy[y]];
			}
			addmin += min;
			for (int x = 0; x < ile; x++) {
				loctab[remainingx[x]][remainingy[y]] -= min;
			}
		}
#endif

		if (lowest <= addmin)return;
	}

	//wy³¹czone
#ifdef wyszukiwanie_najlepszego_zera
	int xlow = -1;
	int ylow = -1;
	bool xniey = 0;
	int posg = 0;
	for (int x = 0; x < ile; x++) {
		for (int y = 0; y < ile; y++) {
			if (loctab[remainingx[x]][remainingy[y]] == 0) {
				if (locff[remainingy[y]] == locff[remainingx[x]])
					continue;
				int min = 10000;
				for (int yy = 0; yy < ile; yy++) {
					if (yy == y)continue;
					if (locff[remainingy[yy]] == locff[remainingx[x]])
						continue;
					if (loctab[remainingx[x]][remainingy[yy]] < min) {
						min = loctab[remainingx[x]][remainingy[yy]];
						xniey = 0;
					}
				}
				for (int xx = 0; xx < ile; xx++) {
					if (xx == x)continue;
					if (locff[remainingy[y]] == locff[remainingx[xx]])
						continue;
					if (loctab[remainingx[xx]][remainingy[y]] < min){
						min = loctab[remainingx[xx]][remainingy[y]];
						xniey = 1;
					}
				}
				if (min >= posg) {
					posg = min;
					xlow = x;
					ylow = y;
				}
			}
		}
	}
	swap(remainingx[xlow], remainingx[0]);
	swap(remainingy[ylow], remainingy[0]);

	start[remainingx[0]] = remainingy[0];
	int z = locff[remainingx[0]], d = locff[remainingy[0]];

	for (int i = 0; i < size; i++) {
		if (locff[i] != z)
			ff[siz + 1][i] = locff[i];
		else
			ff[siz + 1][i] = d;
	}

	BnBRoad(lowest, remainingx + 1, remainingy + 1, ile - 1, addmin, start, copyloc, siz + 1, tmptab, ff, 1);

	loctab[remainingx[0]][remainingy[0]] = 10000;
	addmin += posg;
	if (xniey) {
		for (int xx = 0; xx < ile; xx++)
			loctab[remainingx[xx]][remainingy[0]] -= posg;
	}
	else {
		for (int yy = 0; yy < ile; yy++)
			loctab[remainingx[0]][remainingy[yy]] -= posg;
	}

	swap(remainingx[xlow], remainingx[0]);
	swap(remainingy[ylow], remainingy[0]);

	BnBRoad(lowest, remainingx, remainingy, ile, addmin, start, copyloc, siz, tmptab, ff,0);
#else

	for (int x = 0; x < ile; x++) {
		for (int y = 0; y < ile; y++) {
			if (loctab[remainingx[x]][remainingy[y]] == 0) {
				if (locff[remainingy[y]] == locff[remainingx[x]])
					continue;
				swap(remainingx[x], remainingx[0]);
				swap(remainingy[y], remainingy[0]);

				start[remainingx[0]] = remainingy[0];
				int z = locff[remainingx[0]], d = locff[remainingy[0]];

				for (int i = 0; i < size; i++) {
					if (locff[i] != z)
						ff[siz + 1][i] = locff[i];
					else
						ff[siz + 1][i] = d;
				}

				BnBRoad(lowest, remainingx + 1, remainingy + 1, ile - 1, addmin, start, copyloc, siz + 1, tmptab, ff,1);

				loctab[remainingx[0]][remainingy[0]] = 10000;


				/*double min = 10000;
				for (int xx = 0; xx < ile; xx++) {
					if (locff[remainingy[y]] == locff[remainingx[xx]])
						continue;
					if (loctab[remainingx[xx]][remainingy[y]] < min)
						min = loctab[remainingx[xx]][remainingy[y]];
				}
				if (min != 0) {
					addmin += min;
					for (int xx = 0; xx < ile; xx++) {
						loctab[remainingx[xx]][remainingy[y]] -= min;
					}
				}
				for (int yy = 0; yy < ile; yy++) {
					if (locff[remainingy[yy]] == locff[remainingx[x]])
						continue;
					if (loctab[remainingx[x]][remainingy[yy]] < min)
						min = loctab[remainingx[x]][remainingy[yy]];
				}
				if (min != 0) {
					addmin += min;
					for (int yy = 0; yy < ile; yy++) {
						loctab[remainingx[x]][remainingy[yy]] -= min;
					}
				}*/

				swap(remainingx[x], remainingx[0]);
				swap(remainingy[y], remainingy[0]);


				//if (lowest < addmin)return;
			}
		}
	}
#endif



}

int Komiwojazer::pRoad(int* remaining, int ile, int currnum, int endnum) {
	int indx = 0;
	for (int i = 0; i < ile; i++)
		indx |= 1 << (remaining[i]);
	int lowst = minroad[currnum][indx];
	if (lowst != 0) {
		return lowst;
	}
	if (ile == 1) {
		lowst = tab[*remaining][endnum];
		lowst += tab[currnum][*remaining];
		nextpoint[currnum][indx] = *remaining;
		minroad[currnum][indx] = lowst;
	}
	else {
		int lnum = *remaining;
		lowst = pRoad(remaining + 1, ile - 1, lnum,endnum);
		lowst += tab[currnum][lnum];
		for (int i = 1; i < ile; i++) {

			int t = remaining[i];
			remaining[i] = remaining[0];
			remaining[0] = t;
			int tmp = pRoad(remaining + 1, ile - 1, t,endnum);
			remaining[0] = remaining[i];
			remaining[i] = t;

			tmp += tab[currnum][t];
			if (tmp < lowst) {
				lnum = t;
				lowst = tmp;
			}
		}
		nextpoint[currnum][indx] = lnum;
		minroad[currnum][indx] = lowst;
	}
	return lowst;
}

int Komiwojazer::localsearch(int* pokolei, int currmin, long long timel) {
	auto st = std::chrono::high_resolution_clock::now();
	bool zmienione = 0;
	do {
		poczotekdo:

		zmienione = 0;

		//przez wstawianie
		for (int gdzie = 0; gdzie < size; gdzie++){
			for (int skad = 0; skad < size; skad++){
				int drugiegdzie = pokolei[gdzie];
				int wyciagane = pokolei[skad];
				int powyciagniete = pokolei[wyciagane];

				if (skad == gdzie)continue;
				if (wyciagane == gdzie)continue;

				int curr = tab[gdzie][drugiegdzie] + tab[skad][wyciagane] + tab[wyciagane][powyciagniete];
				int pozmianie = tab[gdzie][wyciagane] + tab[wyciagane][drugiegdzie] + tab[skad][powyciagniete];
				if (pozmianie < curr) {
					pokolei[gdzie] = wyciagane;
					pokolei[wyciagane] = drugiegdzie;
					pokolei[skad] = powyciagniete;
					currmin -= curr - pozmianie;
					zmienione = 1;
					goto poczotekdo;
				}
			}
		}

		//przez zamianê
		for (int gdzie = 0; gdzie < size; gdzie++) {
			for (int skad = gdzie + 1; skad < size; skad++) {
				int drugiegdzie = pokolei[gdzie];
				int trzeciegdzie = pokolei[drugiegdzie];
				int wyciagane = pokolei[skad];
				int powyciagniete = pokolei[wyciagane];


				if (skad == gdzie)continue;
				if (wyciagane == gdzie)continue;
				if (skad == drugiegdzie)continue;


				int curr = tab[gdzie][drugiegdzie] + tab[drugiegdzie][trzeciegdzie] + tab[skad][wyciagane] + tab[wyciagane][powyciagniete];
				int pozmianie = tab[gdzie][wyciagane] + tab[wyciagane][trzeciegdzie] + tab[skad][drugiegdzie] + tab[drugiegdzie][powyciagniete];
				if (pozmianie < curr) {
					pokolei[gdzie] = wyciagane;
					pokolei[wyciagane] = trzeciegdzie;
					pokolei[skad] = drugiegdzie;
					pokolei[drugiegdzie] = powyciagniete;
					currmin -= curr - pozmianie;
					zmienione = 1;
					goto poczotekdo;
				}
			}
		}

		//przez wstawianie wielu
		for (int gdzie = 0; gdzie < size; gdzie++)
			for (int start = 0; start < size; start++)
				for (int ile = 2; ile < size; ile++) {

					int curr = 0;
					int odw = 0;

					int nextgdzie = pokolei[gdzie];
					int nstart = pokolei[start];
					int beforeend = nstart;
					int nxt = pokolei[beforeend];

					if (start == gdzie)continue;
					if (nstart == gdzie)continue;
					int i = 0;
					for (; i < ile; i++) {
						if (nxt == gdzie)break;
						beforeend = nxt;
						nxt = pokolei[beforeend];
					}

					if (i != ile)break;

					curr += tab[start][nstart];
					curr += tab[beforeend][nxt];
					curr += tab[gdzie][nextgdzie];

					odw += tab[gdzie][nstart];
					odw += tab[beforeend][nextgdzie];
					odw += tab[start][nxt];

					if (odw < curr) {
						pokolei[start] = nxt;
						pokolei[gdzie] = nstart;
						pokolei[beforeend] = nextgdzie;
						currmin -= curr - odw;
						zmienione = 1;
						goto poczotekdo;
					}
				}

		//przez odwracanie wielu
		for (int start = 0; start < size; start++)
			for (int end = 0; end < size; end++) {
				int curr = 0;
				int odw = 0;
				int size = 0;
				int nstart = pokolei[start];
				if (nstart == end)continue;
				int beforeend = nstart;
				int nxt = pokolei[beforeend];
				if (nxt == end)continue;
				for (; nxt != end;) {
					curr += tab[beforeend][nxt];
					odw += tab[nxt][beforeend];
					beforeend = nxt;
					nxt = pokolei[beforeend];
					size++;
					if (size == this->size) {
						size = 0;
						break;
					}
				}
				if (size < 2)continue;
				curr += tab[start][nstart];
				curr += tab[beforeend][end];
				odw += tab[start][beforeend];
				odw += tab[nstart][end];
				if (odw < curr) {
					int pop = 0;
					int beforeend = nstart;
					int nxt = pokolei[beforeend];
					for (; nxt != end;) {
						pop = beforeend;
						beforeend = nxt;
						nxt = pokolei[beforeend];
						pokolei[beforeend] = pop;
					}
					pokolei[start] = beforeend;
					pokolei[nstart] = end;
					currmin -= curr - odw;
					zmienione = 1;
					goto poczotekdo;
				}
			}

		//przez wk³adanie odwróconego
		for (int gdzie = 0; gdzie < size; gdzie++)
			for (int start = 0; start < size; start++)
				for (int ile = 2; ile < size; ile++) {
					
					int curr = 0;
					int odw = 0;

					int nextgdzie = pokolei[gdzie];
					int nstart = pokolei[start];
					int beforeend = nstart;
					int nxt = pokolei[beforeend];

					if (start == gdzie)continue;
					if (nstart == gdzie)continue;
					int i = 0;
					for (; i<ile;i++) {
						if (nxt == gdzie)break;
						curr += tab[beforeend][nxt];
						odw += tab[nxt][beforeend];
						beforeend = nxt;
						nxt = pokolei[beforeend];
					}

					if (i != ile)break;

					curr += tab[start][nstart];
					curr += tab[beforeend][nxt];
					curr += tab[gdzie][nextgdzie];

					odw += tab[gdzie][beforeend];
					odw += tab[nstart][nextgdzie];
					odw += tab[start][nxt];


					if (odw < curr) {
						int pop = 0;
						int beforeend = nstart;
						int nxt = pokolei[beforeend];
						for (int i = 0; i < ile; i++) {
							pop = beforeend;
							beforeend = nxt;
							nxt = pokolei[beforeend];
							pokolei[beforeend] = pop;
						}
						pokolei[start] = nxt;
						pokolei[gdzie] = beforeend;
						pokolei[nstart] = nextgdzie;
						currmin -= curr - odw;
						zmienione = 1;
						goto poczotekdo;
					}
				}



	} while (zmienione && (std::chrono::high_resolution_clock::now() - st).count() < timel);
	return currmin;
}

int Komiwojazer::localsearcheq(int* pokolei, int currmin, long long timel) {
	auto st = std::chrono::high_resolution_clock::now();
	bool zmienione = 0;

	//przez wstawianie
	for (int gdzie = 0; gdzie < size; gdzie++) {
		for (int skad = 0; skad < size; skad++) {
			int drugiegdzie = pokolei[gdzie];
			int wyciagane = pokolei[skad];
			int powyciagniete = pokolei[wyciagane];

			if (skad == gdzie)continue;
			if (wyciagane == gdzie)continue;

			int curr = tab[gdzie][drugiegdzie] + tab[skad][wyciagane] + tab[wyciagane][powyciagniete];
			int pozmianie = tab[gdzie][wyciagane] + tab[wyciagane][drugiegdzie] + tab[skad][powyciagniete];
			if (pozmianie <= curr) {
				pokolei[gdzie] = wyciagane;
				pokolei[wyciagane] = drugiegdzie;
				pokolei[skad] = powyciagniete;
				currmin -= curr - pozmianie;
				zmienione = 1;
			}
		}
	}

	//przez zamianê
	for (int gdzie = 0; gdzie < size; gdzie++) {
		for (int skad = gdzie + 1; skad < size; skad++) {
			int drugiegdzie = pokolei[gdzie];
			int trzeciegdzie = pokolei[drugiegdzie];
			int wyciagane = pokolei[skad];
			int powyciagniete = pokolei[wyciagane];


			if (skad == gdzie)continue;
			if (wyciagane == gdzie)continue;
			if (skad == drugiegdzie)continue;


			int curr = tab[gdzie][drugiegdzie] + tab[drugiegdzie][trzeciegdzie] + tab[skad][wyciagane] + tab[wyciagane][powyciagniete];
			int pozmianie = tab[gdzie][wyciagane] + tab[wyciagane][trzeciegdzie] + tab[skad][drugiegdzie] + tab[drugiegdzie][powyciagniete];
			if (pozmianie <= curr) {
				pokolei[gdzie] = wyciagane;
				pokolei[wyciagane] = trzeciegdzie;
				pokolei[skad] = drugiegdzie;
				pokolei[drugiegdzie] = powyciagniete;
				currmin -= curr - pozmianie;
				zmienione = 1;
			}
		}
	}

	//przez wstawianie wielu
	for (int gdzie = 0; gdzie < size; gdzie++)
		for (int start = 0; start < size; start++)
			for (int ile = 2; ile < size; ile++) {

				int curr = 0;
				int odw = 0;

				int nextgdzie = pokolei[gdzie];
				int nstart = pokolei[start];
				int beforeend = nstart;
				int nxt = pokolei[beforeend];

				if (start == gdzie)continue;
				if (nstart == gdzie)continue;
				int i = 0;
				for (; i < ile; i++) {
					if (nxt == gdzie)break;
					beforeend = nxt;
					nxt = pokolei[beforeend];
				}

				if (i != ile)break;

				curr += tab[start][nstart];
				curr += tab[beforeend][nxt];
				curr += tab[gdzie][nextgdzie];

				odw += tab[gdzie][nstart];
				odw += tab[beforeend][nextgdzie];
				odw += tab[start][nxt];

				if (odw <= curr) {
					pokolei[start] = nxt;
					pokolei[gdzie] = nstart;
					pokolei[beforeend] = nextgdzie;
					currmin -= curr - odw;
					zmienione = 1;
				}
			}

	//przez odwracanie wielu
	for (int start = 0; start < size; start++)
		for (int end = 0; end < size; end++) {
			int curr = 0;
			int odw = 0;
			int size = 0;
			int nstart = pokolei[start];
			if (nstart == end)continue;
			int beforeend = nstart;
			int nxt = pokolei[beforeend];
			if (nxt == end)continue;
			for (; nxt != end;) {
				curr += tab[beforeend][nxt];
				odw += tab[nxt][beforeend];
				beforeend = nxt;
				nxt = pokolei[beforeend];
				size++;
				if (size == this->size) {
					size = 0;
					break;
				}
			}
			if (size < 2)continue;
			curr += tab[start][nstart];
			curr += tab[beforeend][end];
			odw += tab[start][beforeend];
			odw += tab[nstart][end];
			if (odw <= curr) {
				int pop = 0;
				int beforeend = nstart;
				int nxt = pokolei[beforeend];
				for (; nxt != end;) {
					pop = beforeend;
					beforeend = nxt;
					nxt = pokolei[beforeend];
					pokolei[beforeend] = pop;
				}
				pokolei[start] = beforeend;
				pokolei[nstart] = end;
				currmin -= curr - odw;
				zmienione = 1;
			}
		}

	//przez wk³adanie odwróconego
	for (int gdzie = 0; gdzie < size; gdzie++)
		for (int start = 0; start < size; start++)
			for (int ile = 2; ile < size; ile++) {

				int curr = 0;
				int odw = 0;

				int nextgdzie = pokolei[gdzie];
				int nstart = pokolei[start];
				int beforeend = nstart;
				int nxt = pokolei[beforeend];

				if (start == gdzie)continue;
				if (nstart == gdzie)continue;
				int i = 0;
				for (; i < ile; i++) {
					if (nxt == gdzie)break;
					curr += tab[beforeend][nxt];
					odw += tab[nxt][beforeend];
					beforeend = nxt;
					nxt = pokolei[beforeend];
				}

				if (i != ile)break;

				curr += tab[start][nstart];
				curr += tab[beforeend][nxt];
				curr += tab[gdzie][nextgdzie];

				odw += tab[gdzie][beforeend];
				odw += tab[nstart][nextgdzie];
				odw += tab[start][nxt];


				if (odw <= curr) {
					int pop = 0;
					int beforeend = nstart;
					int nxt = pokolei[beforeend];
					for (int i = 0; i < ile; i++) {
						pop = beforeend;
						beforeend = nxt;
						nxt = pokolei[beforeend];
						pokolei[beforeend] = pop;
					}
					pokolei[start] = nxt;
					pokolei[gdzie] = beforeend;
					pokolei[nstart] = nextgdzie;
					currmin -= curr - odw;
					zmienione = 1;
				}
			}

	return currmin;
}


int Komiwojazer::localsearchtabkol(int* pokolei, int& siz, long long endtime) {
	int* tmptab = new int[size];
	int* tmptab2 = new int[size]; memset(tmptab2, 0, sizeof(int) * size);
	int* tabooblock = new int[size * size * size * 3];
	memset(tabooblock, 0, size * size * size * 3 * sizeof(int));
	int count = 0;
	auto st = std::chrono::high_resolution_clock::now();
	int currmin = losowadroga(tmptab, siz);
	int w = localsearch(tmptab, currmin, endtime);
	w = localsearcheq(tmptab, w, endtime);
	do {
		w = taboomove(tmptab, w, tabooblock, count);
		if (w < currmin)count++;
	} while ((std::chrono::high_resolution_clock::now() - st).count() < endtime);
	int pop = 0;
	int tmpsum = 0;
	for (int i = 0; i < size; i++) {
		tmptab2[pop]++;
		if (tmptab2[pop] > 1)printf("err %d\n", pop);
		pokolei[i] = pop;
		tmpsum += tab[pop][tmptab[pop]];
		pop = tmptab[pop];
	}
	if (tmpsum != w)printf("\ncalculated %d\n", tmpsum);
	//else printf("\n\n");
	delete[] tmptab;
	delete[] tmptab2;
	delete[] tabooblock;
	return w;
}


int Komiwojazer::taboolocalsearch(int* pokolei, int& siz, long long endtime) {
	auto st = std::chrono::high_resolution_clock::now();

	int* tmptab = new int[size];
	int* lowest = new int[size];

	int* block = new int[size + 1];
	for (int i = 0; i <= size; i++)
		block[i] = -1;

	int** orgtable = new int* [size];
	for (int i = 0; i < size; i++) {
		orgtable[i] = new int[size];
		memcpy(orgtable[i], tab[i], size * sizeof(int));
	}


	int w = losowamindroga(lowest, siz); 
	int nw = losowamindroga(tmptab, siz);
	long long timm = (std::chrono::high_resolution_clock::now() - st).count();

	std::function<void(int)> blocker =
		[&](int i) {
		if (block[i] >= 0) tab[block[i]][lowest[block[i]]] = orgtable[block[i]][lowest[block[i]]];
		block[i]++;
		if (block[i] == size) {
			blocker(i + 1);
			block[i] = block[i + 1] + 1;
		}
		tab[block[i]][lowest[block[i]]] = 100000;
	};

	do {
		nw = localsearch(tmptab, nw, endtime - timm);
		blocker(0);
		if (nw < w) {
			for (int i = 0; i <= size; i++) {
				if (block[i] >= 0) tab[block[i]][lowest[block[i]]] = orgtable[block[i]][lowest[block[i]]];
				block[i] = -1;
			}
			memcpy(lowest, tmptab, size * sizeof(int));
			w = nw;
		}
		int pop = 0;
		int tmpsum = 0;
		for (int i = 0; i < size; i++) {
			tmpsum += tab[pop][tmptab[pop]];
			pop = tmptab[pop];
		}
		nw = tmpsum;
		timm = (std::chrono::high_resolution_clock::now() - st).count();
	} while ((timm < endtime) && (block[size] == -1));
	for (int i = 0; i <= size; i++) {
		if (block[i] >= 0) tab[block[i]][lowest[block[i]]] = orgtable[block[i]][lowest[block[i]]];
		block[i] = -1;
	}
	int pop = 0;
	int tmpsum = 0;
	for (int i = 0; i < size; i++) {
		pokolei[i] = pop;
		tmpsum += tab[pop][lowest[pop]];
		pop = lowest[pop];
	}
	if (tmpsum != w)printf("\ncalculated %d\n", tmpsum);
	//else printf("\n\n");
	return w;
}

int Komiwojazer::nielosowe(int* pokolei,int& siz) {
	if (size != 171)return 0;
	siz = size;
	int sum = 0;
	pokolei[0] = bes170[0];
	int pop = 0;
	for (int i = 1; i < 171; i++) {
		int nxt = bes170[i];
		pokolei[pop] = nxt;
		sum += tab[pop][nxt];
		pop = nxt;
	}
	pokolei[pop] = 0;
	sum += tab[pop][0];
	return sum;
}

void Komiwojazer::loadfromatsp(std::string filename) {
	std::string sl;
	std::fstream f(filename, std::ios::in);
	if (f.fail())return;
	do {
		f >> sl;
		if (sl == "DIMENSION:") {
			f >> size;
		}
	} while (sl != "EDGE_WEIGHT_SECTION");
	tab = new int* [size];
	for (int ii = 0; ii < size; ii++) {
		tab[ii] = new int[size];
		for (int i = 0; i < size; i++)
			f >> tab[ii][i];
	}
}

int Komiwojazer::metodazahlanna(int* pokolei, int& siz, int start) {
	siz = size;
	int* doodwiedzenia = new int[size];
	int* dtt = doodwiedzenia;
	for (int i = 0; i < size; i++) {
		doodwiedzenia[i] = i;
	}
	swap(*doodwiedzenia, doodwiedzenia[start]);
	int pop = *doodwiedzenia;
	doodwiedzenia++;
	int sizo = size - 1;
	int dl = 0;
	for (; sizo >= 1; sizo--, doodwiedzenia++) {
		int min = 1000000;
		int gdzie = -1;
		for (int i = 0; i < sizo; i++)
			if (tab[pop][doodwiedzenia[i]] < min) {
				min = tab[pop][doodwiedzenia[i]];
				gdzie = i;
			}
		swap(doodwiedzenia[gdzie], *doodwiedzenia);
		pokolei[pop] = *doodwiedzenia;
		pop = *doodwiedzenia;
		dl += min;
	}
	pokolei[pop] = 0;
	dl += tab[pop][0];
	delete[] dtt;
	return dl;
}

int Komiwojazer::losowamindroga(int* pokolei, int& siz) {
	siz = size;
	int* doodwiedzenia = new int[size];
	int* dtt = doodwiedzenia;
	for (int i = 0; i < size; i++) {
		doodwiedzenia[i] = i;
	}
	int pop = *doodwiedzenia;
	doodwiedzenia++;
	int sizo = size - 1;
	int dl = 0;
	for (; sizo >= 1; sizo--, doodwiedzenia++) {
		int min = 1000000;
		int gdzie = -1;
		for (int i = 0; i < sizo; i++)
			if (tab[pop][doodwiedzenia[i]] < min) {
				min = tab[pop][doodwiedzenia[i]];
				gdzie = i;
			}
		swap(doodwiedzenia[gdzie], *doodwiedzenia);
		pokolei[pop] = *doodwiedzenia;
		pop = *doodwiedzenia;
		dl += min;
	}
	pokolei[pop] = 0;
	dl += tab[pop][0];
	delete[] dtt;
	return dl;
}

int Komiwojazer::losowadroga(int* pokolei, int& siz) {
	siz = size;
	int* doodwiedzenia = new int[size];
	int* dtt = doodwiedzenia;
	for (int i = 0; i < size; i++) {
		doodwiedzenia[i] = i;
	}
	int pop = *doodwiedzenia;
	doodwiedzenia++;
	int sizo = size - 1;
	int dl = 0;
	for (; sizo >= 1; sizo--, doodwiedzenia++) {
		int gdzie = rand() % sizo;
		swap(doodwiedzenia[gdzie], *doodwiedzenia);
		pokolei[pop] = *doodwiedzenia;
		dl += tab[pop][*doodwiedzenia];
		pop = *doodwiedzenia;
	}
	pokolei[pop] = 0;
	dl += tab[pop][0];
	delete[] dtt;
	return dl;
}

int Komiwojazer::taboomove(int* pokolei, int droga, int* moves,int counter) {

	int ndroga = 100000;
	int typ = 0;
	union{
		struct {
			int gdzie;
			int wyciagane;
			int skad;
			int drugiegdzie;
			int powyciagniete;
			int trzeciegdzie;
		} jd;
		struct {
			int start;
			int gdzie;
			int beforeend;
			int nxt;
			int nstart;
			int nextgdzie;
			int end;
			int ile;
		} tcp;
	} nmove;
	int indxmove = 0;

	int indx = 0;

	//przez wstawianie
	for (int gdzie = 0; gdzie < size; gdzie++) {
		for (int skad = 0; skad < size; skad++) {
			if (moves[indx++] > counter)continue;
			int drugiegdzie = pokolei[gdzie];
			int wyciagane = pokolei[skad];
			int powyciagniete = pokolei[wyciagane];

			if (skad == gdzie)continue;
			if (wyciagane == gdzie)continue;

			int curr = tab[gdzie][drugiegdzie] + tab[skad][wyciagane] + tab[wyciagane][powyciagniete];
			int pozmianie = tab[gdzie][wyciagane] + tab[wyciagane][drugiegdzie] + tab[skad][powyciagniete];

			int nd = droga - (curr - pozmianie);
			if (nd<ndroga) {
				typ = 1;
				indxmove = indx - 1;
				nmove.jd.gdzie = gdzie;
				nmove.jd.wyciagane = wyciagane;
				nmove.jd.skad = skad;
				nmove.jd.drugiegdzie = drugiegdzie;
				nmove.jd.powyciagniete = powyciagniete;

				ndroga = nd;
			}
		}
	}

	//przez zamianê
	for (int gdzie = 0; gdzie < size; gdzie++) {
		for (int skad = gdzie + 1; skad < size; skad++) {
			if (moves[indx++] > counter)continue;
			int drugiegdzie = pokolei[gdzie];
			int trzeciegdzie = pokolei[drugiegdzie];
			int wyciagane = pokolei[skad];
			int powyciagniete = pokolei[wyciagane];


			if (skad == gdzie)continue;
			if (wyciagane == gdzie)continue;
			if (skad == drugiegdzie)continue;


			int curr = tab[gdzie][drugiegdzie] + tab[drugiegdzie][trzeciegdzie] + tab[skad][wyciagane] + tab[wyciagane][powyciagniete];
			int pozmianie = tab[gdzie][wyciagane] + tab[wyciagane][trzeciegdzie] + tab[skad][drugiegdzie] + tab[drugiegdzie][powyciagniete];

			int nd = droga - (curr - pozmianie);
			if (nd < ndroga) {
				indxmove = indx - 1;
				typ = 2;
				nmove.jd.gdzie = gdzie;
				nmove.jd.wyciagane = wyciagane;
				nmove.jd.trzeciegdzie = trzeciegdzie;
				nmove.jd.skad = skad;
				nmove.jd.drugiegdzie = drugiegdzie;
				nmove.jd.powyciagniete = powyciagniete;

				ndroga = nd;
			}
		}
	}

	//przez wstawianie wielu
	for (int gdzie = 0; gdzie < size; gdzie++)
		for (int start = 0; start < size; start++) {
			indx += size;
			for (int ile = 2; ile < size; ile++) {
				if (moves[indx + ile] > counter)continue;

				int curr = 0;
				int odw = 0;

				int nextgdzie = pokolei[gdzie];
				int nstart = pokolei[start];
				int beforeend = nstart;
				int nxt = pokolei[beforeend];

				if (start == gdzie)continue;
				if (nstart == gdzie)continue;
				int i = 0;
				for (; i < ile; i++) {
					if (nxt == gdzie)break;
					beforeend = nxt;
					nxt = pokolei[beforeend];
				}

				if (i != ile)break;

				curr += tab[start][nstart];
				curr += tab[beforeend][nxt];
				curr += tab[gdzie][nextgdzie];

				odw += tab[gdzie][nstart];
				odw += tab[beforeend][nextgdzie];
				odw += tab[start][nxt];

				int nd = droga - (curr - odw);
				if (nd < ndroga) {
					typ = 3;
					indxmove = indx +ile;
					nmove.tcp.start = start;
					nmove.tcp.nxt = nxt;
					nmove.tcp.gdzie = gdzie;
					nmove.tcp.nstart = nstart;
					nmove.tcp.beforeend = beforeend;
					nmove.tcp.nextgdzie = nextgdzie;

					ndroga = nd;
				}
			}
		}

	//przez odwracanie wielu
	for (int start = 0; start < size; start++)
		for (int end = 0; end < size; end++) {
			if (moves[indx++] > counter)continue;
			int curr = 0;
			int odw = 0;
			int size = 0;
			int nstart = pokolei[start];
			if (nstart == end)continue;
			int beforeend = nstart;
			int nxt = pokolei[beforeend];
			if (nxt == end)continue;
			for (; nxt != end;) {
				curr += tab[beforeend][nxt];
				odw += tab[nxt][beforeend];
				beforeend = nxt;
				nxt = pokolei[beforeend];
				size++;
				if (size == this->size) {
					size = 0;
					break;
				}
			}
			if (size < 2)continue;
			curr += tab[start][nstart];
			curr += tab[beforeend][end];
			odw += tab[start][beforeend];
			odw += tab[nstart][end];

			int nd = droga - (curr - odw);
			if (nd < ndroga) {
				typ = 4;
				indxmove = indx - 1;
				nmove.tcp.nstart = nstart;
				nmove.tcp.start = start;
				nmove.tcp.end = end;

				ndroga = nd;
			}
		}

	//przez wk³adanie odwróconego
	for (int gdzie = 0; gdzie < size; gdzie++)
		for (int start = 0; start < size; start++) {
			indx += size;
			for (int ile = 2; ile < size; ile++) {
				if (moves[indx + ile] > counter)continue;

				int curr = 0;
				int odw = 0;

				int nextgdzie = pokolei[gdzie];
				int nstart = pokolei[start];
				int beforeend = nstart;
				int nxt = pokolei[beforeend];

				if (start == gdzie)continue;
				if (nstart == gdzie)continue;
				int i = 0;
				for (; i < ile; i++) {
					if (nxt == gdzie)break;
					curr += tab[beforeend][nxt];
					odw += tab[nxt][beforeend];
					beforeend = nxt;
					nxt = pokolei[beforeend];
				}

				if (i != ile)break;

				curr += tab[start][nstart];
				curr += tab[beforeend][nxt];
				curr += tab[gdzie][nextgdzie];

				odw += tab[gdzie][beforeend];
				odw += tab[nstart][nextgdzie];
				odw += tab[start][nxt];


				int nd = droga - (curr - odw);
				if (nd < ndroga) {
					typ = 5;
					indxmove = indx + ile;
					nmove.tcp.nstart = nstart;
					nmove.tcp.start = start;
					nmove.tcp.gdzie = gdzie;
					nmove.tcp.nextgdzie = nextgdzie;
					nmove.tcp.ile = ile;

					ndroga = nd;
				}
			}
		}


	if (typ == 1) {//wk³adanie 'wyciagane' w inne miejsce
		pokolei[nmove.jd.gdzie] = nmove.jd.wyciagane;
		pokolei[nmove.jd.wyciagane] = nmove.jd.drugiegdzie;
		pokolei[nmove.jd.skad] = nmove.jd.powyciagniete;
	}
	else if (typ == 2) {//zamiana 'drugiegdzie' z 'wyciagane'
		pokolei[nmove.jd.gdzie] = nmove.jd.wyciagane;
		pokolei[nmove.jd.wyciagane] = nmove.jd.trzeciegdzie;
		pokolei[nmove.jd.skad] = nmove.jd.drugiegdzie;
		pokolei[nmove.jd.drugiegdzie] = nmove.jd.powyciagniete;
	}
	else if (typ == 3) {//przenoszenie 
		pokolei[nmove.tcp.start] = nmove.tcp.nxt;
		pokolei[nmove.tcp.gdzie] = nmove.tcp.nstart;
		pokolei[nmove.tcp.beforeend] = nmove.tcp.nextgdzie;
	}
	else if (typ == 4) {
		int pop = 0;
		int beforeend = nmove.tcp.nstart;
		int nxt = pokolei[beforeend];
		for (; nxt != nmove.tcp.end;) {
			pop = beforeend;
			beforeend = nxt;
			nxt = pokolei[beforeend];
			pokolei[beforeend] = pop;
		}
		pokolei[nmove.tcp.start] = beforeend;
		pokolei[nmove.tcp.nstart] = nmove.tcp.end;
	}
	else if (typ == 5) {
		int pop = 0;
		int beforeend = nmove.tcp.nstart;
		int nxt = pokolei[beforeend];
		for (int i = 0; i < nmove.tcp.ile; i++) {
			pop = beforeend;
			beforeend = nxt;
			nxt = pokolei[beforeend];
			pokolei[beforeend] = pop;
		}
		pokolei[nmove.tcp.start] = nxt;
		pokolei[nmove.tcp.gdzie] = beforeend;
		pokolei[nmove.tcp.nstart] = nmove.tcp.nextgdzie;
	}

	if (typ > 0) {
		moves[indxmove]=counter+10;//blokada na x ruchów
	}

	return ndroga;
}

int Komiwojazer::taboosearch(int* pokolei, int& siz, long long endtime) {
	auto st = std::chrono::high_resolution_clock::now();//czas start
	long long timm = (std::chrono::high_resolution_clock::now() - st).count();
	int* tmptab = new int[size];//tablica z aktualnie u¿ywan¹ drog¹
	int* lowest = new int[size];//tablica z najmniejsz¹ drog¹
	int w = 100000;//wynik najlepszej drogi
	int nw = metodazahlanna(tmptab, siz, 0);//wynik aktualnej drogi i tworzenie œcie¿ki pocz¹tkowej
	int* tabooblock = new int[size * size * size * 3];//tablica zakazanych ruchów
	memset(tabooblock, 0, size * size * size * 3 * sizeof(int));//wszystkie dozwolone na start
	int count = 0;//licznik do odblokowania zablokowanych ruchów
	//ruch jest dozwolony gdy			tabooblock[i]<count
	//blokowanie jest zrobione poprzez	tabooblock[i]=count+20
	int dyw = 0;//licznik do dywersyfikacji
	do {
		if (dyw > size) {//dywersyfikacja
			nw = losowadroga(tmptab, siz);//nowy start w losowym miejscu
			dyw = 0;
		}
		nw = taboomove(tmptab, nw, tabooblock, count);//wykonanie i zablokowanie ruchu
		if (nw < w) {//jeœli u¿ywany jest lepszy 
			memcpy(lowest, tmptab, size * sizeof(int));
			w = nw;//skopiuj go do najlepszego
			dyw = 0;
		}
		else {
			dyw++;
		}
		count++;
		timm = (std::chrono::high_resolution_clock::now() - st).count();//aktualny czas - start
	} while ((timm < endtime));//rób dopóki nie skoñczy ci siê czas
	int pop = 0;//zamiana tablicy s¹siedztwa na ci¹g 
	for (int i = 0; i < size; i++) {
		pokolei[i] = pop;
		pop = lowest[pop];
	}
	delete[] tmptab;
	delete[] lowest;
	delete[] tabooblock;
	return w;//zwracanie najlepszego wyniku
}



