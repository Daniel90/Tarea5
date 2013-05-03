#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <libpq-fe.h>
using namespace std;

void mostrarFilas(int campos,int tuplas,PGresult *resultado)
{
     for (int i=0; i<tuplas; i++)                                        //muestra el contenido de las filas
     {
         for (int j=0; j<campos; j++) 
             cout << PQgetvalue(resultado,i,j) << "         ";
         cout << endl;
     }
}

void mostrarFilas2Consultas(int campos,int tuplas,PGresult *resultado,PGresult *resultado2)
{
     for (int i=0; i<tuplas; i++)                                        //muestra el contenido de las filas con 2 consultas
     {
         for (int j=0; j<campos; j++) 
         {
             cout << PQgetvalue(resultado,i,j) << "         " << PQgetvalue(resultado2,i,j);
            
         }
         cout << endl;
     }
     
}

void  mostrarFecha()                   //Muestra Fecha actual
{
      time_t t = time(0);
      struct tm * now = localtime(&t);
      cout << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-"<<  now->tm_mday<< endl;
}

void opcionA(int campos,int tuplas,PGresult *resultado,PGresult *resultado2)  //Guarda consultas en .csv
{
     ofstream fs("estimaciones.csv");
     fs<<"Nota estimada,Nota Real"<<endl; 
     for (int i=0; i<tuplas; i++)                                        
     {
         for (int j=0; j<campos; j++) 
         {
             fs << PQgetvalue(resultado,i,j) <<","<<PQgetvalue(resultado2,i,j)<< endl;
            
         }
         cout << endl;
     }
     fs.close();
}




int main(int argc, char * argv[])
{
    char *host = "localhost", *port = "5432",*dataBase = "tarea5",*user = "postgres",*passwd = "root";   
    PGconn *cnn = PQsetdbLogin(host,port,NULL,NULL,dataBase,user,passwd);
    if (PQstatus(cnn) != CONNECTION_BAD) 
    {
        cout << "Conexion exitosa!" << endl;
        cout << "host: "<<host<<endl<<"Puerto: "<<port<<endl<<"Base de datos: "<<dataBase<<endl<<"Usuario: "<<user<<endl<<"password: ****"<<endl<<endl<<endl;
        string opcion;
        int id;
        cout<<"INSTRUCCIONES"<<endl<<endl;
        cout<<"Se puede ingresar -i id donde id es un numero entero (ejem: -i 4) para saber estimaciones"<<endl;
        cout<<"Se puede ingresar -a para saber estimaciones en general y guardar en .csv"<<endl;
        cout<<"Se puede ingresar -s para saber valores estadisticos de las asignaturas en general"<<endl;
        cout<<"Se puede ingresar -v para saber datos del programa"<<endl<<endl;
        do
        {
             cout<<"-i id"<<endl;  
             cout<<"-a"<<endl;  
             cout<<"-s"<<endl;
             cout<<"-v"<<endl;
             cout<<"-9: salir"<<endl;
             cin>>opcion;
             
             if(opcion == "-i")
             {
                  cin>>id;
                  char query[256];
                  char query2[256];
                  //consultas con parametros
                  sprintf(query,"select avg(A.nota) as Nota_estimada,A.estudiante_id from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id and C.asignatura in('CALCULO I','CALCULO II','CALCULO EN VARIAS VARIABLES') and A.estudiante_id = %d group by A.estudiante_id",id);     
                  sprintf(query2,"select avg(A.nota) as Nota_Real,A.estudiante_id from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id and C.asignatura = 'CALCULO VECTORIAL' and A.estudiante_id = %d group by A.estudiante_id",id);
                  PGresult *result = PQexec(cnn,query); 
                  PGresult *result2 = PQexec(cnn,query2); 
                  cout<<"Nota estimada Calculo IV: "<<PQgetvalue(result,0,0)<<endl;
                  cout <<"Estudiante ID: "<<id<<" "<<"Nota estimada: " <<PQgetvalue(result,0,0)<<" Nota Real: "<<PQgetvalue(result2,0,0)<<endl; 
                  
                  PQclear(result);   
             }
             
             if(opcion == "-a")
             {
                  PGresult *result = PQexec(cnn, "select avg(A.nota) from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id and C.asignatura in('CALCULO I','CALCULO II','CALCULO EN VARIAS VARIABLES') group by A.estudiante_id order by A.estudiante_id");
                  PGresult *result2 = PQexec(cnn, "select avg(A.nota) from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id and C.asignatura = 'CALCULO VECTORIAL' group by A.estudiante_id order by A.estudiante_id");
                  int tuplas = PQntuples(result);
                  int campos = PQnfields(result);
                  cout<<"Nota estimada      "<<"              Nota Real"<<endl;
                  mostrarFilas2Consultas(campos,tuplas,result,result2);
                  opcionA(campos,tuplas,result,result2);
                 
             }
             if(opcion == "-s")
             {
                  PGresult *result = PQexec(cnn, "select avg(A.nota), stddev(A.nota),C.asignatura from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id group by C.asignatura");
                  int tuplas = PQntuples(result);
                  int campos = PQnfields(result);
                  cout<<"Promedio                   Desviacion estandar"<<endl;
                  mostrarFilas(campos,tuplas,result);
                  PQclear(result);
             }
             
             if(opcion == "-v")
             {
                   cout<<"Daniel Abrilot"<<endl<<"John Lopez"<<endl;      
                   cout<<"Fecha actual: ";
                   mostrarFecha();
             }
             
        }while(opcion!="-9");
    } 
    else
    {
        cout << "Error de conexion" << endl;
    }
    PQfinish(cnn);
    system("PAUSE");
    return EXIT_SUCCESS;
}
