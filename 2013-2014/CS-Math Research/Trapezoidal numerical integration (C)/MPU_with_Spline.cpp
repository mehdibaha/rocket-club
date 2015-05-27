#include <iostream>
#include<vector>
#include "windows.h"
#include <cmath>

/* Notes de version :
 * 		Simulation du fonctionnement d'un accéléromètre:
 * On rentre des couples (t,a(t)), et au bout d'un moment, on récupère notre position à un tel instant à l'aide de la fonction spline.
 * 
 * Problèmes:
 * 1) Le code ne prend pas en compte les constantes.
 * 2) L'algorithme ne marche que pour un pas=1.
 * 3) Il faut accélerer le programme. Solution possible: Enlever les premières cases du tableau au fur et à mesure du calcul. */
 
using namespace std;
 
float spline(vector< vector<float> >, float);
float integration(vector< vector<float> >, float, float);
float doubleIntegration(vector< vector<float> >, float, float);

// FONCTION SPLINE
float spline(vector< vector<float> > vecXY, float p) {
	// Déclaration de variables
	int i, j, n = vecXY[0].size();
	vector<float> h(n);
	vector<float> s(n,0);
	vector<float> F(n);
	vector< vector<float> > m(n,vector<float>(n,0));
	float a,b,c,d,image,temp;
	
	// Remplissage du tableau F et h
	for(i=n-1;i>0;i--) {
		F[i]=(vecXY[1][i]-vecXY[1][i-1])/(vecXY[0][i]-vecXY[0][i-1]);
		h[i-1]=vecXY[0][i]-vecXY[0][i-1];
	}
	
	// Création de la matrice formée de h, s et f
	for(i=1;i<n-1;i++) {
		m[i][i]=2*(h[i-1]+h[i]);
		if(i!=1) {
			m[i][i-1]=h[i-1];
			m[i-1][i]=h[i-1];
		}
    m[i][n-1]=6*(F[i+1]-F[i]); // Ca serait bien de comprendre la signification mathématique de cette ligne
	}
	
	// Forward elimination
	for(i=1;i<n-2;i++) {
		temp=(m[i+1][i]/m[i][i]);
		for(j=1;j<=n-1;j++)
			m[i+1][j]-=temp*m[i][j];
	}
	
	// Backward substitution
	for(i=n-2;i>0;i--) {
		image=0;
		for(j=i;j<n-1;j++)
			image+=m[i][j]*s[j];
			s[i]=(m[i][n-1]-image)/m[i][i];
	}
 
	for(i=0;i<n-1;i++)
		if(vecXY[0][i]<p+1 && p<=vecXY[0][i+1]) {
			a=(s[i+1]-s[i])/(6*h[i]);
			b=s[i]/2;
			c=(vecXY[1][i+1]-vecXY[1][i])/h[i]-(2*h[i]*s[i]+s[i+1]*h[i])/6;
			d=vecXY[1][i];
			image=a*(p-vecXY[0][i])*(p-vecXY[0][i])*(p-vecXY[0][i])+b*(p-vecXY[0][i])*(p-vecXY[0][i])+c*(p-vecXY[0][i])+d;
    }
		return image;
}

// INTEGRATION TRAPEZE + SPLINE
float integration(vector< vector<float> > vecXY, float a, float b) { // Une intégrale de a à b, à partir d'un tableau de valeurs vecXY
	int n = vecXY[0].size();
	float h=(b-a)/n;
	float s=(spline(vecXY,a)+spline(vecXY,b))*0.5*h; // On commence la somme par [f(a)+f(b)]*h*0.5
	
	for(int i=1;i<n;i++) {
		s+=spline(vecXY,a+i*h)*h; // Puis : f(a+i*h)*h
	}
		
	return s;
}

float doubleIntegration(vector< vector<float> > vecXY, float a, float b) {
	int n = vecXY[0].size();
	float h=(b-a)/n;
	float s=(integration(vecXY,0,a)+integration(vecXY,0,b))*0.5*h;
	
	for(int i=1;i<n;i++) {
		s+=integration(vecXY,0,a+i*h)*h;
	}
		
	return s;
}
 
int main() {

//Déclaration de variales
int i=0, j=0;
float pas=1, t=0; // L'intervalle d'actualisation de l'accéléromètre et l'initialisation du temps.
int iteration=2; // Le nombre de points que le programme doit emmagasiner avant de commencer ses calculs

// Création d'un vecteur à deux colonnes x->f(x)
vector< vector<float> > tab_acc(2,vector<float>(0));

// Simulation de l'accéléromètre

/* Je remplis un tableau avec des accélérations en fonction du temps, puis j'intégre mes "points" deux fois.
 * Puis, je compare mon intégration double à l'intégrale exacte. */

while(j<=100) {
	// Remplissage du tableau avec des couples (t,a(t))
	tab_acc[0].push_back(t); 
	tab_acc[1].push_back(12*t*t);
	
	// Le calcul se fait toutes les "itération" fois.
	if(i%iteration==0) {
		// Différents affichages
		cout << "(" << tab_acc[0][j] << "," << tab_acc[1][j] << ") => " << integration(tab_acc, 0, j) <<
		" exact= " << pow(j,4) << endl;
		//cout << j << "->" << abs(doubleIntegration(tab_acc, 0, j)-pow(j,4)) << endl;  // Affichage de l'erreur
		
		// On reçoit des valeurs chaque xxxx millisecondes.
		Sleep(200);
		
		j++;
	}
	t+=pas;
	i++;
}

return 0;
}
