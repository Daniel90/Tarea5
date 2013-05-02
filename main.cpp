#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <libpq-fe.h>
using namespace std;

void mostrarCampos(int campos, PGresult *resultado)
{
     cout <<"" << endl << endl;
     for (int i=0; i<campos; i++) 
               cout << PQfname(resultado,i) << "          ";   
                            //Muestra el nombre de los campos
     cout<<endl<<endl;
}



  
void mostrarFilas(int campos,int tuplas,PGresult *resultado)
{
     for (int i=0; i<tuplas; i++)                                        //muestra el contenido de las filas
     {
         
         for (int j=0; j<campos; j++) 
             cout << PQgetvalue(resultado,i,j) << "         ";
         cout << endl;
     }
}

void  mostrarFecha()
{
      time_t t = time(0);
      struct tm * now = localtime(&t);
      cout << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-"<<  now->tm_mday<< endl;
}

void opcionA(int campos,int tuplas,PGresult *resultado)
{
     ofstream fs("estimaciones.csv");
     for (int i=0; i<tuplas; i++)                                        //muestra el contenido de las filas
     {
         for (int j=0; j<campos; j++) 
         {
             fs << PQgetvalue(resultado,i,j) << endl;
            
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
        cout << "host: "<<host<<endl<<"Puerto: "<<port<<endl<<"Base de datos: "<<dataBase<<endl<<"Usuario: "<<user<<endl<<"password: ****"<<endl;
        string opcion;
        int id;

        
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
                  sprintf(query,"select avg(A.nota) as Nota_estimada,A.estudiante_id from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id and C.asignatura in('CALCULO I','CALCULO II','CALCULO EN VARIAS VARIABLES') and A.estudiante_id = %d group by A.estudiante_id",id);
                  PGresult *result = PQexec(cnn,query); 
                  //int tuplas = PQntuples(result);
                  //int campos = PQnfields(result);
                  //mostrarCampos(campos,result);
                  //mostrarFilas(campos,tuplas,result);
                  cout <<"Estudiante ID: "<<id<<" "<<PQfname(result,0) << ": "<<PQgetvalue(result,0,0)<<endl; 
                  
                  PQclear(result);   
             }
             
             if(opcion == "-a")
             {
                  
             }
             if(opcion == "-s")
             {
                  PGresult *result = PQexec(cnn, "select avg(A.nota), stddev(A.nota),C.asignatura from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id group by C.asignatura");
                  int tuplas = PQntuples(result);
                  int campos = PQnfields(result);
                  mostrarCampos(campos,result);
                  mostrarFilas(campos,tuplas,result);
                  PQclear(result);
                 // PQfinish(cnn); 
             }
             
             if(opcion == "-v")
             {
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
