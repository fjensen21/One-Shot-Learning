#include<stdlib.h>
#include<stdio.h>

double **allocatematrix(int n, int m);
double **creatematrix(FILE* f, int n, int m);
void printmatrix(double **matrix, int n, int m);

double **allocatematrix(int n, int m) {
    double **mat = malloc(sizeof(double*)*n);
    int i;
    for (i = 0; i<n; i++) {
        mat[i] = malloc(sizeof(double)*m);
    }
    return mat;
}


void printmatrix(double **matrix, int n, int m) {
    int i;
    int j;


    for(i = 0; i < n; i++) {
        for(j=0; j < m; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void freematrix(double **mat, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

double **transposematrix(double **matrix, int row, int col) {
    double **transposed = allocatematrix(col, row);
    int i;
    int j;

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}


//Takes a file read up to matrix and alters matrixes created for it
void processtrainingdata(FILE *f, double **matrix_x, double **matrix_y, int attributes, int rows) {
    int i;
    int j;
    double val;

    for(i = 0; i < rows; i++){
        for (j = 1; j < attributes + 2; j++) {
            fscanf(f, " %lf", &val);
            if (j == attributes + 1) {
                matrix_y[i][0] = val;
            } else {
                matrix_x[i][j] = val;
            }

        }
    }
    for(i = 0; i < rows; i++) {
        matrix_x[i][0] = 1;
    }
}

double **multiplymatrices(double **mat1, int r1, int c1, double **mat2, int r2, int c2) {
    if (c1 != r2) { return NULL; }
    double **results_mat = allocatematrix(r1, c2);


    int i;
    int j;
    int k;

    // Zero results matrix
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++) {
            results_mat[i][j] = 0;
        }
    }

    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++) {
            for (k = 0; k < c1; k++) {
                results_mat[i][j] = results_mat[i][j] + mat1[i][k]*mat2[k][j];
            }
        }
    }

    return results_mat;

}

double **makeidentitymatrix(int n) {
    double **idmat = allocatematrix(n,n);

    int i; 
    int j;


    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if(j == i) {
                idmat[i][j] = 1;
            } else {
                idmat[i][j] = 0;
            }
        }
    }
    return idmat;
}

double **makeinverse(double **mat, int n) {
    double **idmat = makeidentitymatrix(n);

    int p, i, j;
    double f;
    double neg =  -1.0;

    for (p = 0; p < n; p++) {
        f = mat[p][p];
        //divide mat[p] by f
        //divide idmat[p] by f
        for(j = 0; j < n; j++) {
            mat[p][j] = mat[p][j] * (1.0/f);
            idmat[p][j] = idmat[p][j] * (1.0/f);
        }
        for (i = p+1; i < n; i++) {
            f = mat[i][p];
            // add M[p] * f + M[i]
            // add N[p] * f + N[i]
            for(j = 0; j < n; j++) {
                mat[i][j] = mat[i][j] + (neg * f * (mat[p][j]));
                idmat[i][j] = idmat[i][j] + (neg * f *(idmat[p][j]));
            }
        }
    }
    for (p = n-1; p >= 0; p--) {
        for (i = p-1; i >= 0; i--) {
            f = mat[i][p];
            for(j = 0; j < n; j++) {
                mat[i][j] = mat[i][j] + (neg * f * (mat[p][j]));
                idmat[i][j] = idmat[i][j] + (neg * f *(idmat[p][j]));
            }
        }
    }
    return idmat;
}


double **processdatamatrix(FILE *f, int rows, int cols) {
    double **data = allocatematrix(rows, cols + 1);
    double val;
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols + 1; j++) {
            if(j == 0) {
                data[i][j] = 1;
            } else {
                fscanf(f, " %lf", &val);
                data[i][j] = val;
            }
        }
    }

    return data;
}

// int main(int argc, char **argv) {
    
//     double **mat1;
//     int r1, c1;
//     double val1;

//     FILE *fp;
//     fp = fopen(argv[1], "r");
//     fscanf(fp, " %d", &r1);
//     fscanf(fp, " %d", &c1);
    
//     int i, j;
//     for (i = 0; i < r1; i++) {
//         for (j = 0; j < c1; j++) {
//             fscanf(fp, " %lf", &val1);
//         }
//     }
// }

int main(int argc, char **argv) {
    char type_s[6];
    int attributes;
    int rows;

    double **mat;
    double **price;
    double **transposedmat;
    double **mt;
    double **ty;
    double **invertedmat;

    double **weight;

    // Read in mat, pricemat
    FILE *fp = fp = fopen(argv[1], "r");
    fscanf(fp, " %s", type_s);
    fscanf(fp, " %d", &attributes);
    fscanf(fp, " %d", &rows);

    mat = allocatematrix(rows, attributes + 1);
    price = allocatematrix(rows, 1);

    processtrainingdata(fp, mat, price, attributes, rows);

    fclose(fp);

    // puts("Mat: ");
    // printmatrix(mat, rows, attributes + 1);

    // puts("Prices: ");
    // printmatrix(price, rows, 1);


    transposedmat = transposematrix(mat, rows, attributes + 1);   
    

    // puts("Transposed Mat: ");
    // printmatrix(transposedmat, attributes + 1, rows);


    mt = multiplymatrices(transposedmat, attributes + 1, rows, mat, rows, attributes + 1);
    freematrix(mat, rows);


    // puts("Matrix X Tranposed Matrix: ");
    // printmatrix(mt, attributes + 1, attributes + 1);


    invertedmat = makeinverse(mt, attributes + 1);

    // puts("Inververted Mat: ");
    // printmatrix(invertedmat, attributes + 1, attributes + 1);

    ty = multiplymatrices(transposedmat, attributes + 1, rows, price, rows, 1);

    // puts("Transposed Mat X Price: ");
    // printmatrix(ty, attributes + 1, 1);

    weight = multiplymatrices(invertedmat, attributes + 1, attributes + 1, ty, attributes + 1, 1); 


    // puts("Weights: ");
    // printmatrix(weight, attributes + 1, 1);


    // Data Section

    int dataatts;
    int datarows;
    double **datamat;

    // Read in data matrix
    FILE *datafile;
    datafile = fopen(argv[2], "r");
    fscanf(datafile, " %s", type_s);
    fscanf(datafile, " %d", &dataatts);
    fscanf(datafile, " %d", &datarows);
    datamat = processdatamatrix(datafile, datarows, dataatts);
    fclose(datafile);

    // puts("Data File: ");
    // printmatrix(datamat, datarows, dataatts + 1);
    // puts("Weight File: ");
    // printmatrix(weight, attributes + 1, 1);

    // puts("Price Estimates");

    //Estimate house price
    int i;
    for (i = 0; i < datarows; i++) {
        double house_price = 0;
        int j;
        for (j = 0; j < dataatts + 1; j++) {
            house_price += datamat[i][j] * weight[j][0];
        }
        printf("%.0f\n", house_price);
        house_price = 0;
    }

    freematrix(price, rows);
    freematrix(transposedmat, attributes + 1);
    freematrix(mt, attributes + 1);
    freematrix(ty, attributes + 1);
    freematrix(invertedmat, attributes + 1);
    freematrix(weight, attributes + 1);
    freematrix(datamat, datarows);

    return EXIT_SUCCESS;

}

