#include <bits/stdc++.h>

using namespace std;

long double aceleracion_y(bool subiendo, long double g, long double rozamiento, long double paso_tiempo){
    if(subiendo == true){
        return (g+rozamiento);
    }
    else{
        return (g-rozamiento);
    }
}

pair<long double, long double> aceleraciones_rozamiento(long double vel_y, long double vel_x, long double cons_rozamiento, long double masa, long double paso_tiempo, long double g){
    long double vel_total = sqrt(vel_y*vel_y+vel_x*vel_x);
    long double fuerza_rozamiento = cons_rozamiento*vel_total*vel_total;
    long double aceleracion_rozamiento_x = (fuerza_rozamiento*vel_x/vel_total)/masa;
    long double aceleracion_rozamiento_y = (fuerza_rozamiento*abs(vel_y/vel_total))/masa;
    return {aceleracion_rozamiento_x, aceleracion_rozamiento_y};
}

pair<long double, long double> nuevas_variables(long double vel_y, long double vel_x, long double cons_rozamiento, long double masa, long double paso_tiempo, long double g, bool subiendo){
    pair<long double, long double> rozamiento1 = aceleraciones_rozamiento(vel_y, vel_x, cons_rozamiento, masa, paso_tiempo, g);
    long double k_x1 = -rozamiento1.first*paso_tiempo;
    long double k_y1 = -aceleracion_y(subiendo, g, rozamiento1.second, paso_tiempo)*paso_tiempo;
    pair<long double, long double> rozamiento2 = aceleraciones_rozamiento(vel_y+(k_y1/2), vel_x+(k_x1/2), cons_rozamiento, masa, paso_tiempo, g);
    long double k_x2 = -rozamiento2.first*paso_tiempo;
    long double k_y2 = -aceleracion_y(subiendo, g, rozamiento2.second, paso_tiempo)*paso_tiempo;
    pair<long double, long double> rozamiento3 = aceleraciones_rozamiento(vel_y+(k_y2/2), vel_x+(k_x2/2), cons_rozamiento, masa, paso_tiempo, g);
    long double k_x3 = -rozamiento3.first*paso_tiempo;
    long double k_y3 = -aceleracion_y(subiendo, g, rozamiento3.second, paso_tiempo)*paso_tiempo;
    pair<long double, long double> rozamiento4 = aceleraciones_rozamiento(vel_y+k_y3, vel_x+k_x3, cons_rozamiento, masa, paso_tiempo, g);
    long double k_x4 = -rozamiento4.first*paso_tiempo;
    long double k_y4 = -aceleracion_y(subiendo, g, rozamiento4.second, paso_tiempo)*paso_tiempo;
    return{vel_x+(k_x1+2*k_x2+2*k_x3+k_x4)/6, vel_y+(k_y1+2*k_y2+2*k_y3+k_y4)/6};
}

signed main(){
    long double pi = M_PI;
    long double g = 9.80665;
    long double masa = 7.26;
    long double coeficiente = 0.44;
    long double area_frontal = pi*0.06*0.06;
    cout << "Ingrese por orden y separados por espacios: la distancia que recorre, el angulo de lanzamiento en grados, la altura inicial (no negativos y aunque sea 0 se escribe) y la distancia inicial (aunque sea 0 se escribe):" << endl;
    long double distancia_final; 
    while(cin >> distancia_final){
        long double grados; cin >> grados;
        long double altura_inicial; cin >> altura_inicial;
        if(altura_inicial <= 0){
            altura_inicial = 0.000000001;
        }
        long double ang = pi*(grados)/180;
        long double distancia_inicial; cin >> distancia_inicial;
        long double densidad = 1.155;
        long double cons_rozamiento = 0.5*coeficiente*area_frontal*densidad;
        long double mejor_velocidad = -1;
        long double mejor_dif = 10000000.0;
        for(long double vel_salida = 13; vel_salida < 14.5; vel_salida += 0.01){
            long double distancia = distancia_inicial;
            long double paso_tiempo = 1.0/1000.0;
            long double altura = altura_inicial;
            long double vel_x = vel_salida*cos(ang);
            long double vel_y = vel_salida*sin(ang);
            bool subiendo = true;
            bool ultimo_paso = false;
            while(altura > 0){
                if(subiendo == false && abs(altura/vel_y) < paso_tiempo){
                    ultimo_paso = true;
                    paso_tiempo = (vel_y+sqrt(vel_y*vel_y+2*g*altura))/g;
                }
                if(vel_y <=0){
                    subiendo = false;
                }
                pair<long double, long double> velocidades = nuevas_variables(vel_y, vel_x, cons_rozamiento, masa, paso_tiempo, g, subiendo);
                if(ultimo_paso == false){
                    distancia += ((vel_x+velocidades.first)/2)*paso_tiempo;
                    altura += ((vel_y+velocidades.second)/2)*paso_tiempo;
                }
                else{
                    distancia += ((vel_x+velocidades.first)/2)*paso_tiempo;
                    altura = 0;
                }
                vel_x = velocidades.first;
                vel_y = velocidades.second;
            }
            if(abs(distancia-distancia_final) < mejor_dif){
                mejor_velocidad = vel_salida;
                mejor_dif = abs(distancia-distancia_final);
            }
        }
        cout << fixed << setprecision(2) << "Velocidad inicial: " << mejor_velocidad << endl;
    }
}
