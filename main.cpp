#include <cstdlib>
#include <iostream>
#include <libpq-fe.h>
using namespace std;

void mostrarCampos(int campos, PGresult *resultado)
{
     cout << "La informacion se muestra a continuacion:" << endl << endl;
     for (int i=0; i<campos; i++) 
               cout << PQfname(resultado,i) << "          ";                          //Muestra el nombre de los campos
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

int main(int argc, char * argv[])
{
    char *host = "localhost", *port = "5432",*dataBase = "tarea5",*user = "postgres",*passwd = "root";   
    PGconn *cnn = PQsetdbLogin(host,port,NULL,NULL,dataBase,user,passwd);
    if (PQstatus(cnn) != CONNECTION_BAD) 
    {
        cout << "Conexion exitosa!" << endl;
        cout << "host: "<<host<<endl<<"Puerto: "<<port<<endl<<"Base de datos: "<<dataBase<<endl<<"Usuario: "<<user<<endl<<"password: ****"<<endl;
        string op;
        
        do
        {
             cout<<"-s"<<endl;
             cout<<"-9: salir"<<endl;
             cin>>op;
             if(op == "-s")
             {
                  PGresult *result = PQexec(cnn, "select avg(A.nota), stddev(A.nota),C.asignatura from asignaturas_cursadas as A, cursos as C where A.curso_id = C.curso_id group by C.asignatura");
                  int tuplas = PQntuples(result);
                  int campos = PQnfields(result);
                  mostrarCampos(campos,result);
                  mostrarFilas(campos,tuplas,result);
                  PQclear(result);
                 // PQfinish(cnn); 
             }
             
             
        }while(op!="-9");
    } 
    else
    {
        cout << "Error de conexion" << endl;
    }
    PQfinish(cnn);
    system("PAUSE");
    return EXIT_SUCCESS;
}
