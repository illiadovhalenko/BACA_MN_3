//Illia Dovhalenko
#include <iostream>
#include <stdio.h>
#include <cmath>
typedef void (*FuncPointer)(const double* x, double* y, double* Df);

void printVector(const double* x, unsigned N){
    for(unsigned i=0;i<N;++i)
        printf("%17.17f ",x[i]);
    printf("\n");
}



int findCurve(FuncPointer f, double* x, unsigned k, double h){
    double y[2];
    double df[6];
    double norm;
    double prev_y[2];
    for(int i=1; i<=k ;i++) {
        x[2]+=h;
        int iter=0;
        do {
            f(x, y, df);
            double det = df[0] * df[4] - df[1] * df[3];
            x[0] -= (df[4] * y[0] - df[1] * y[1]) / det;
            x[1] -= (-df[3] * y[0] + df[0] * y[1]) / det;
            //f(x, y, df);
            norm=sqrtf(y[0]*y[0]+y[1]*y[1]);
            iter++;
            if(iter>100)
                return i;
        } while (fabs(y[0])>10e-14 || fabs(y[1])>10e-14);
        if(isnan(x[0]) || isnan(x[1]))
            return i;
        printVector(x, 3);
    }
    return 0;
}

int findSurface(FuncPointer f, double* x, unsigned k1, unsigned k2, double h1, double h2){
    double* y=new double;
    double df[3];
    double temp_x2=x[2];
    for(int i=1; i<=k1; i++){
        x[1]+=h1;
        x[2]=temp_x2;
        for(int j=1; j<=k2; j++){
            //*y=0;
            x[2]+=h2;
            int iter=0;
            do{
                f(x, y, df);
                x[0]-=(*y)/df[0];
               // std::cout<<*y<<std::endl;
               iter++;
               if(iter>100)
                   return i*k1+j;
            }while(std::fabs(*y)>=10e-14);
            if(isnan(x[0]))
                return i*k1+j;
            printVector(x, 3);
        }
        std::cout<<std::endl;
    }
    delete y;
    return 0;
}

int findFixedPoints(FuncPointer f, double* x, unsigned k1, unsigned k2, double h1, double h2){
    double y[2];
    double df[8];
    double y1[2];
    double temp_b=x[3];
    double old_x0, old_x1;
    for(int i=1; i<=k1; i++){
        x[2]+=h1;
        x[3]=temp_b;
        for(int j=1; j<=k2; j++){
            x[3]+=h2;
            int iter=0;
            do {
                f(x, y, df);
                double det = (df[0]-1) * (df[5]-1) - (df[1]) * (df[4]);
                y1[0]=y[0]-x[0];
                y1[1]=y[1]-x[1];
                x[0] -= (((df[5]-1.) * (y[0]-x[0]) - (df[1]) * (y[1]-x[1])) / det);
                x[1] -= ((-(df[4]) * (y[0]-x[0]) + (df[0]-1.) * (y[1]-x[1])) / det);
                //f(x, y, df);
                iter++;
                if(iter>100)
                    return i*k1+j;
                if(isnan(x[0]) || isnan(x[1]))
                    return i*k1+j;
            } while (std::max(std::fabs(y1[0]), std::fabs(y1[1])) > 1e-15);
            printVector(x, 4);
        }
        std::cout<<std::endl;
    }
    return 0;
}