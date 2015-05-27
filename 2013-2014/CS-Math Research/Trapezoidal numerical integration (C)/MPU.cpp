#include <iostream>
#include<vector>
#include "windows.h"
#include <cmath> 

/* Notes de version :
 * 		Simulation du fonctionnement d'un accéléromètre:
 * On rentre des couples (t,a(t)), et au bout d'un moment, on récupère notre position à un tel instant.
 * 
 * Problèmes:
 * 1) Le code ne prend pas en compte les constantes d'intégration.
*/
 
using namespace std;
 
float integration(vector< vector<float> >, int, int);
float doubleIntegration(vector< vector<float> >, float, float);

// INTEGRATION TRAPEZE
float integration(vector< vector<float> > vecXY, int a, int b) {
	float h;
	if (a==0 && b==0) // Ca marche pas pour ce cas précis, j'ai pas trouvé de solution à part un if tout bête.
		h=0;
	else
		h=vecXY[0][1]-vecXY[0][0]; // Je définis le pas comme étant la distance entre 2 points de mon tableau.
		
	float s=(vecXY[1][a]+vecXY[1][b])*0.5*h; // On commence la somme par [f_a+f_b)]*h*0.5
	
	for(int i=a+1;i<b;i++) {
		s+=vecXY[1][i]*h; // Puis : f_i*h
	}
	
		return s;
}

float doubleIntegration(vector< vector<float> > vecXY, float a, float b) {
	float h=vecXY[0][1]-vecXY[0][0];
	float s=(integration(vecXY,0,a)+integration(vecXY,0,b))*0.5*h; // [F_a+F_b]*h*0.5
	
	for(int i=a+1;i<b;i++) {
		s+=integration(vecXY,0,i)*h; // F_i*h
	}
		
	return s;
}
 
int main() {

//Déclaration de variales
int i=0;
float pas=0.1; // L'intervalle d'actualisation de l'accéléromètre
float t=0;

// Création d'un vecteur à deux colonnes x->f(x)
vector< vector<float> > tab_acc(2,vector<float>(0));

// Simulation de l'accéléromètre

/* Je remplis un tableau avec des accélérations en fonction du temps, puis j'intégre mes "points" deux fois.
 * Puis, je compare mon intégration double à l'intégrale exacte. */

while(i<=100) {
	// Remplissage du tableau avec des couples (t,a(t))
	tab_acc[0].push_back(t); 
	tab_acc[1].push_back(6*t);
	
	// Différents affichages
	cout << "(" << tab_acc[0][i] << "," << tab_acc[1][i] << ") => " << doubleIntegration(tab_acc, 0, i) <<
	" exact= " << pow(t,3) << endl;
	// cout << i << "->" << abs(doubleIntegration(tab_acc, 0, i)-pow(t,4)) << endl; // Affichage de l'erreur

	// On reçoit des valeurs chaque xxxx millisecondes.
	Sleep(200);
	
	t+=pas;
	i++;
}

return 0;
}
