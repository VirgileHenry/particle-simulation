/// Code élémentaire pour le calcul de la trace d'une matrice et la résolution d'équation différentielles.
/// Printemps 2022
/// Objectif : manipuler les éléments de base du C++
///            utiliser des outils de profiling et de débogage
/// ref: l'exercice est inspiré très largement de ressources WWW             

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <functional>


double** initialization(int);
double* fill_vectors(double*, int);
void print_matrix(double** , int);
double* trace (double** , int);
double phi(double, double);
double* eulerExplicite(double, double, std::function<double(double, double)>);
double* eulerImplicite(double, double);
double psi(double, double);
double* eulerExplicite2(double, double, std::function<double(double, double)>);
double* eulerImplicite2(double, double);


 int main()
 {
    int index, n;
    double **matrix;
    double* sum;

    std::cout << std::endl << "Enter the Dimension for a square matrix: " << std::flush;
    std::cin >> n;
    matrix = initialization(n);
    for(index = 0 ; index < n ; index++)
        matrix[index] =  fill_vectors(matrix[index] , n);
    sum = trace(matrix , n);
    print_matrix(matrix , n);
    std::cout << std::endl << "Sum of the diagonal elements are: " << *sum << std::endl;


   std::cout << "Euler explicite" << std::endl ;
   double* approx1 = eulerExplicite(10, 1, &phi);
   int i;
   for (i=0 ; i<10 ; i++){
     std::cout << approx1[i] << std::endl;
   }

   std::cout << "Euler implicite" << std::endl ;
   double* approx2 = eulerImplicite(10, 1);
   int j;
   for (j=0 ; j<10 ; j++){
     std::cout << approx2[j] << std::endl;
   }

   std::cout << "Euler explicite 2" << std::endl ;
   double* approx3 = eulerExplicite2(10, 0, &psi);
   int k;
   for (k=0 ; k<10 ; k++){
     std::cout << approx3[k] << std::endl;
   }

   std::cout << "Euler implicite 2" << std::endl ;
   double* approx4 = eulerImplicite2(100, 0);
   int l;
   for (l=0 ; l<10 ; l++){
     std::cout << approx4[l] << std::endl;
   }

   return 0;

 }


/// @brief Routine d'initialization qui permet d'allouer la mémoire pour une 
///        matrice carrée de taille n et renvoie un pointer vers 
///        la matrice allouée
/// @param[in] n est la taille souhaitée de la matrice
/// @return    Renvoie le pointeur vers la matrice allouée
double** initialization(int n)
{
  int i;
  double** matrix ;
  matrix = (double**) calloc(n , sizeof(double **));
  for(i=0 ; i< n ; ++i)
    matrix[i] = (double*) calloc(n , sizeof(double*));
  return (matrix);
}

/// @brief Intitialise un vecteur avec des valeurs aléatoires comprises
///        entre dans l'intervalle [-10;10]
/// @param[in] vec est le vecteur à initialiser
/// @param[in] n   est la taille du vecteur à initialiser
/// @return    Renvoie le vecteur initialisé.
double* fill_vectors(double* vec , int n)
{
  int i ;
  for(i = 0 ; i < n ; i++)
    vec[i] = rand() % 20 - 10 ;
  return vec;
}

/// @brief Affiche les éléments d'une matrice de taille n
/// @param[in] matrix est la matrice à afficher
/// @param[in] n est la taille de la matrice à afficher
void print_matrix(double** matrix , int n)
{
  int i,j;
  for (i= 0 ; i< n ; i++)
    {
    for(j = 0 ; j < n ; j++)
      std::cout << matrix[i][j] << ", ";
    std::cout << std::endl;
    }
}

/// @brief Calcul la trace d'une matrice
/// @param[in] matrix est la matrice dont on souhaite connaître la trace.
/// @param[in] n est la taille de la matrice.
double* trace (double** matrix , int n)
{
  int i ;
  double* sum = (double*) malloc(sizeof(double));
  for(i=0 ; i<n ; i++)
    *sum+=matrix[i][i];
  return sum;
}

double phi(double x, double u)
{
  double result;
  result = 2*x*u;
  return result;
}

double psi(double x, double u)
{
  double result;
  result = 50*u*cos(x);
  return result;
}

double* eulerExplicite(double nbIter, double init, std::function<double(double, double)> func){
  double* liste ;
  double pas;
  int i;
  pas = 1/nbIter;
  liste = (double*) calloc(nbIter , sizeof(double *));
  liste[0] = init;
  for(i=0 ; i< (nbIter-1) ; ++i){
    liste[i+1] = pas*(func(i*pas, liste[i])) + liste[i];
  }
  return liste;
}

double* eulerImplicite(double nbIter, double init){
  double* liste ;
  double pas;
  int i;
  pas = 1/nbIter;
  liste = (double*) calloc(nbIter , sizeof(double *));
  liste[0] = init;
  for(i=0 ; i< (nbIter-1) ; ++i){
    liste[i+1] = -liste[i]/(pas*2*(i+1)*pas - 1);
  }
  return liste;
}

double* eulerExplicite2(double nbIter, double init, std::function<double(double, double)> func){
  double* liste ;
  double pas;
  int i;
  pas = 2/nbIter;
  liste = (double*) calloc(nbIter , sizeof(double *));
  liste[0] = init;
  for(i=0 ; i< (nbIter-1) ; ++i){
    liste[i+1] = pas*(func(i*pas, liste[i])) + liste[i];
  }
  return liste;
}

double* eulerImplicite2(double nbIter, double init){
  double* liste ;
  double pas;
  int i;
  pas = 2/nbIter;
  liste = (double*) calloc(nbIter , sizeof(double *));
  liste[0] = init;
  for(i=0 ; i< (nbIter-1) ; ++i){
    liste[i+1] = -liste[i]/(50*pas*cos((i+1)*pas) - 1);
  }
  return liste;
}

