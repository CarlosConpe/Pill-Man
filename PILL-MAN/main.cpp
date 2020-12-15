#include <iostream>
#include <winbgim.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <strings.h>

#define NBOTONESM 4
#define TAMBOTON 50

#define OPCIONES 2
#define INC 40
#define N 5

typedef struct
{
    int X1;
    int Y1;
    int X2;
    int Y2;
    int Color;
}DatosPaleta;

typedef struct nod{
    int x,y,x2,y2,bandPared, pac;
    struct nod *izq,*der;
    struct nod *arr,*aba;
    DatosPaleta dibujo[20][20];
}TNodo;

typedef struct{
  char nombre[50];
  int pts;
  int vidas;
}TReg;

TNodo *correCasper1, *correCasper2, *casperAnterior;

TNodo *crea_nodo(int x, int y);
void malla(TNodo **cab, int xi,int yi, int nc, int nr);
void imprime_maya(TNodo *cab);
//void ayuda(char nombre[], int x1, int y1, int x2, int y2);
void guarda_Record(char *nom_arch, TReg j);
void imprime_mapa(int x, int y);
void imprime_mapa(int x, int y, int nc, int nr);
void islaModule(int x, int y, int nc, int nr);
void inicializar_mapa(int x, int y, int nc, int nr, TNodo **nodo);
void AbrirArchivo(char nom[],DatosPaleta CuadroGrande[20][20]);
void AbrirCuadriculaGrande(int x1,int y1,DatosPaleta CuadroGrande[20][20]);
void copiarArchivo(DatosPaleta CuadroGrande1[20][20],DatosPaleta CuadroGrande[20][20]);


using namespace std;

typedef struct{
    int X1; //Variable para capturar la posición inicial en coordenada en X
    int Y1; //Variable para capturar la posición inicial en coordenada en Y
    int X2; //Variable para capturar la posición final en coordenada en X
    int Y2; //Variable para capturar la posición final en coordenada en Y
    int iColorF; //Variable para capturar el color de fondo de los objetos
    int iColorL; //Variable para capturar el color del contorno de los objetos
    int iCualBoton;
}BOTONIN;

typedef struct{
    char nombreJugador[50];
    int puntuacion;
}PuntJugador;

typedef struct sig{
    PuntJugador puntaje;
    struct sig *ptr;
}PuntJugadorPointer;

typedef struct{
    char intro[10][300], Intrucciones[100], Reglas1[200], Reglas2[200], Reglas3[200], Reglas4[200];
}Reglas;


int MenuPrincipal();
void crearventana(int xventana,int yventana);
void boton(int x1,int y1,int color,BOTONIN Botones[5]);
int seleccionaBotonMain(BOTONIN bBot[10],int iXx,int iYy, PuntJugador puntaje[10], Reglas reglas);
int Score(PuntJugador puntaje[10]);
void readArchivoReglas(FILE *fp, Reglas *reglas);
int Help(Reglas reglas);
void AbrirCuadriculaGrande(int x1,int y1,DatosPaleta CuadroGrande[20][20]);
void CopyR();
void readArchivoPuntaje(FILE *fp, PuntJugador puntaje[10]);
void agregarArchivoPuntaje(FILE *fp, PuntJugador puntaje[10], TReg j);
void animacionMenu();
void Jugar();
void moverFantasma(TNodo **Casper, TNodo **PillMan);
void moverFantasma1(TNodo **Casper, TNodo **PillMan);
void moverFantasmaReverso(TNodo **Casper, TNodo **PillMan);
void moverFantasmaReverso1(TNodo **Casper, TNodo **PillMan);

int main()
{
    int opc;
    int xventana=1100,yventana=800;
    crearventana(xventana,yventana);
    opc=MenuPrincipal();

}

int MenuPrincipal()
{
    BOTONIN Botones[10];//estructura de los botones
    PuntJugador puntaje[10];
    FILE *fp, *reglas;
    Reglas reglasText;

    readArchivoPuntaje(fp, puntaje);
    readArchivoReglas(reglas,&reglasText);
    int iXraton,iYraton;
    int xboton = 100, yboton = 600, option = 0;
    //cleardevice();
    //setbkcolor(10);
    animacionMenu();
    settextstyle(0, HORIZ_DIR, 5);
    outtextxy(380, 100, "PILL-MAN");
    CopyR();
    boton(xboton,yboton,11,Botones);
    do
        {
            delay(50);
            if(ismouseclick(WM_LBUTTONDOWN)) //Si el mouse es presionado
            {
                delay(50);
                getmouseclick(WM_LBUTTONDOWN,iXraton,iYraton);
                option = seleccionaBotonMain(Botones,iXraton,iYraton, puntaje,reglasText);//Seleciona el boton de las coordenadas
                delay(10);

            }
        }
    while(option == 0);
    closegraph();
    return(0);
}

void crearventana(int xventana,int yventana)
{
    initwindow(xventana,yventana);
}

void boton(int x1,int y1,int color,BOTONIN Botones[5])
{
    int i,ancho=140,largo=50,aux=ancho/10;
    for(i=0;i<NBOTONESM;i++)
    {
    setcolor(color);
    setfillstyle(1,color);
    bar(x1,y1,x1+ancho,y1+largo);
    Botones[i].X1=x1;
    Botones[i].X2=x1+ancho;
    Botones[i].Y1=y1;
    Botones[i].Y2=y1+largo;
    Botones[i].iCualBoton=i;

    setcolor(15);
    settextstyle(3,0,3);//tipografia,orientacion,tamaño, la tipografia inicia en 1
    switch(i)
    {
    case 0:
        outtextxy(x1+aux+24,y1+aux-3,"EXIT");
        break;
    case 1:
        outtextxy(x1+aux+23,y1+aux-3,"PLAY");
        break;
    case 2:
        outtextxy(x1+aux+13,y1+aux-3,"SCORE");
        break;
    case 3:
        outtextxy(x1+aux+23,y1+aux-3,"HELP");
        break;
    }
    x1=x1+ancho+100;
    }
}

int seleccionaBotonMain(BOTONIN bBot[10],int iXx,int iYy, PuntJugador puntaje[10], Reglas reglas)
{

     int iCont = 0, res = 0;
     for(iCont = 0; iCont < NBOTONESM; iCont++)
        {
            //printf("%d %d %d %d - %d %d\n",bBot[iCont].X1,bBot[iCont].Y1,bBot[iCont].X2,bBot[iCont].Y2, iXx,iYy );
         if((iXx>=bBot[iCont].X1)&&(iYy>=bBot[iCont].Y1)&&(iXx<=bBot[iCont].X2)&&(iYy<=bBot[iCont].Y2))
         {
             setcolor(15);
             switch(bBot[iCont].iCualBoton)
             {
                 case 0:
                        res = 1000; //Salir ... closegraph()
                        break;
                 case 1:
                        res = 0; //Jugar
                        cleardevice();
                        Jugar();
                        break;
                 case 2:
                        cleardevice();
                        res=Score(puntaje);
                        break;
                case 3:
                    res = 3; //Help
                    cleardevice();
                    res = Help(reglas);
                    break;
             }
         }

     }
     //printf("%d", res);
     return (res);
}

void Jugar(){
    closegraph();
    TNodo *m,*corre,*spawn, *spawnCasper1, *spawnCasper2;
    FILE *fp;
    PuntJugador puntaje[10];
    DatosPaleta pillmander[20][20],pillmanizq[20][20],pillmanarr[20][20],pillmanaba[20][20],pillmanaba2[20][20],pillmanarr2[20][20],pillmanizq2[20][20],pillmander2[20][20];
    DatosPaleta pillmanderPOWER[20][20],pillmanizqPOWER[20][20],pillmanarrPOWER[20][20],pillmanabaPOWER[20][20],pillmanaba2POWER[20][20],pillmanarr2POWER[20][20],pillmanizq2POWER[20][20],pillmander2POWER[20][20];
    DatosPaleta lucas[20][20], willy[20][20];
    int tecla,band=0,tiempo=0,bandpoder=0;
    char cadena[25];
    char cadena2[25];
    TReg j;
    j.pts=0;
    j.vidas=3;
    fflush(stdin);
    initwindow(1250,800);

    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(50, 50, "Introduce tu nombre en la terminal :D");
    delay(10);
    //printf("%d", strlen(j.nombre));
    do{
        fflush(stdin);
        printf("Introduce tu nombre (3 Letras. Ejemplo: AAA): ");
        scanf("%s", j.nombre);
    }while(strlen(j.nombre)!=3);
    cleardevice();
    outtextxy(50, 50, "Bienvenido ");
    outtextxy(250, 50, j.nombre);
    delay(2000);
    cleardevice();
    outtextxy(50, 50, "Tu juego se está inicializando");
    delay(1000);
    outtextxy(550, 50, ".");
    delay(1000);
    outtextxy(580, 50, ".");
    delay(1000);
    outtextxy(610, 50, ".");
    delay(1000);
    cleardevice();
    malla(&m,100,100,20,15);
    itoa(j.pts,cadena,10);
    itoa(j.vidas,cadena2,10);
    settextstyle(0, HORIZ_DIR, 2);
    outtextxy(925, 100, "Puntos:");
    outtextxy(1050, 100, cadena);
    outtextxy(928, 130, "Vidas:");
    outtextxy(1050, 130, cadena2);

    imprime_mapa(100,100,20,15);
    inicializar_mapa(100,100,20,15,&m);
    imprime_maya(m);


    corre=m;

    corre = corre->der->aba;//Punto inicial
    corre->pac=1;
    spawn=corre;
    AbrirArchivo("PILL DERECHA.txt", pillmander);
    AbrirArchivo("PILL ARRIBA.txt", pillmanarr);
    AbrirArchivo("PILL ABAJO.txt", pillmanaba);
    AbrirArchivo("PILL IZQUIERDA.txt", pillmanizq);
    AbrirArchivo("PILL DERECHA CERRADO.txt", pillmander2);
    AbrirArchivo("PILL ARRIBA CERRADO.txt", pillmanarr2);
    AbrirArchivo("PILL ABAJO CERRADO.txt", pillmanaba2);
    AbrirArchivo("PILL IZQUIERDA CERRADO.txt", pillmanizq2);

    AbrirArchivo("PILL DERECHA PODER.txt", pillmanderPOWER);
    AbrirArchivo("PILL ARRIBA PODER.txt", pillmanarrPOWER);
    AbrirArchivo("PILL ABAJO PODER.txt", pillmanabaPOWER);
    AbrirArchivo("PILL IZQUIERDA PODER.txt", pillmanizqPOWER);
    AbrirArchivo("PILL DERECHA PODER CERRADO.txt", pillmander2POWER);
    AbrirArchivo("PILL ARRIBA PODER CERRADO.txt", pillmanarr2POWER);
    AbrirArchivo("PILL ABAJO PODER CERRADO.txt", pillmanaba2POWER);
    AbrirArchivo("PILL IZQUIERDA PODER CERRADO.txt", pillmanizq2POWER);

    //printf("Coordenadas X:%d Coordenadas Y:%d",corre->x,corre->y);
    copiarArchivo(corre->dibujo,pillmander);

   do
   {
    if(bandpoder==1)
    {
        if(tiempo==17)
        {
            bandpoder=0;
        }
        else
        {
         tiempo=tiempo+1;
        }
    }
    setcolor(15);
    itoa(j.pts,cadena,10);
    itoa(j.vidas,cadena2,10);
    outtextxy(1050, 100, cadena);
    outtextxy(1050, 130, cadena2);
    AbrirCuadriculaGrande(corre->x,corre->y,corre->dibujo);
    delay(30);
    tecla=getch();
    //printf("%d %d\n", corre->x, corre->y);

    if(!tecla)
        tecla=getch();
    setcolor(15);
    bar(corre->x,corre->y,corre->x2+2,corre->y2);
    corre->pac=0;
    if(bandpoder==0){
    moverFantasma(&correCasper2, &corre);
    moverFantasma1(&correCasper1, &corre);
    } else if(bandpoder==1){
    moverFantasmaReverso(&correCasper2, &corre);
    moverFantasmaReverso1(&correCasper1, &corre);
    }
    if((correCasper1==corre)&&bandpoder==0){
        corre=spawn;
        bar((correCasper1)->x,(correCasper1)->y, (correCasper1)->x+INC+2, (correCasper1)->y+INC);
            bar((correCasper2)->x,(correCasper2)->y, (correCasper2)->x+INC+2, (correCasper2)->y+INC);
            correCasper1->bandPared=0;
            correCasper2->bandPared=0;
            correCasper1=m->der->der->der->der->der->der->der->der->der->der->der->der->der->der->aba->aba->aba->aba;
            correCasper2=m->der->der->der->der->der->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba;
            correCasper1->bandPared=4;
            correCasper2->bandPared=4;
        j.vidas--;
    }

    if((correCasper2==corre)&&bandpoder==0){
        corre=spawn;
        bar((correCasper1)->x,(correCasper1)->y, (correCasper1)->x+INC+2, (correCasper1)->y+INC);
            bar((correCasper2)->x,(correCasper2)->y, (correCasper2)->x+INC+2, (correCasper2)->y+INC);
            correCasper1->bandPared=0;
            correCasper2->bandPared=0;
            correCasper1=m->der->der->der->der->der->der->der->der->der->der->der->der->der->der->aba->aba->aba->aba;
            correCasper2=m->der->der->der->der->der->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba;
            correCasper1->bandPared=4;
            correCasper2->bandPared=4;
        j.vidas--;
    }
    setcolor(15);
    //rectangle(corre->x,corre->y,corre->x2,corre->y2);
     switch(tecla)
     {
         case KEY_UP:
        if(corre->arr && corre->arr->bandPared!=9)
         {
             corre=corre->arr;
             corre->pac=1;
             switch(corre->bandPared)
             {
             case 0:
            if(bandpoder==1)
            {
              if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarrPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2POWER);
                band=0;
             }
            }
            else
            {
             if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarr);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2);
                band=0;
             }
            }
                break;
             case 2:
            j.pts=j.pts+10;
            bandpoder=1;
            tiempo=0;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarrPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2POWER);
                band=0;
             }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 3:
            j.pts=j.pts+1;
            if(bandpoder==1)
            {
              if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarrPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2POWER);
                band=0;
             }
            }
            else
            {
             if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarr);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2);
                band=0;
             }
            }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 4:
            if(bandpoder==1)
            {
            j.pts=j.pts+100;
            setcolor(15);
            bar((correCasper1)->x,(correCasper1)->y, (correCasper1)->x+INC+2, (correCasper1)->y+INC);
            bar((correCasper2)->x,(correCasper2)->y, (correCasper2)->x+INC+2, (correCasper2)->y+INC);
            correCasper1->bandPared=0;
            correCasper2->bandPared=0;
            correCasper1=m->der->der->der->der->der->der->der->der->der->der->der->der->der->der->aba->aba->aba->aba;
            correCasper2=m->der->der->der->der->der->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba;
            correCasper1->bandPared=4;
            correCasper2->bandPared=4;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarrPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2POWER);
                band=0;
             }
            setcolor(0);
            bar(1040,95,1200,125);
            corre->bandPared=0;
            }
            else
            {
             corre=spawn;
            j.vidas=j.vidas-1;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanarr);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanarr2);
                band=0;
             }
             setcolor(0);
             bar(1040,120,1200,125);
            }
                break;
             }
         }
        break;
         case KEY_DOWN:
        if(corre->aba && corre->aba->bandPared!=9)
        {
            corre=corre->aba;
            corre->pac=1;
            switch(corre->bandPared)
             {
            case 0:
            if(bandpoder==1)
            {
                if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanabaPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2POWER);
                band=0;
             }
            }
            else
            {
             if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanaba);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2);
                band=0;
             }
            }
                break;
             case 2:
            j.pts=j.pts+10;
            bandpoder=1;
            tiempo=0;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanabaPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2POWER);
                band=0;
             }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 3:
            j.pts=j.pts+1;
            if(bandpoder==1)
            {
                if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanabaPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2POWER);
                band=0;
             }
            }
            else
            {
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanaba);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2);
                band=0;
             }
            }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 4:
            if(bandpoder==1)
            {
            j.pts=j.pts+100;
            setcolor(15);
            bar((correCasper1)->x,(correCasper1)->y, (correCasper1)->x+INC+2, (correCasper1)->y+INC);
            bar((correCasper2)->x,(correCasper2)->y, (correCasper2)->x+INC+2, (correCasper2)->y+INC);
            correCasper1->bandPared=0;
            correCasper2->bandPared=0;
            correCasper1=m->der->der->der->der->der->der->der->der->der->der->der->der->der->der->aba->aba->aba->aba;
            correCasper2=m->der->der->der->der->der->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba;
            correCasper1->bandPared=4;
            correCasper2->bandPared=4;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanabaPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2POWER);
                band=0;
             }
            setcolor(0);
            bar(1040,95,1200,125);
            corre->bandPared=0;
            }
            else
            {
             corre=spawn;
            j.vidas=j.vidas-1;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanaba);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanaba2);
                band=0;
             }
             setcolor(0);
             bar(1040,120,1200,125);
            }
                break;
             }
        }
        break;
         case KEY_LEFT:
        if(corre->izq && corre->izq->bandPared!=9)
        {
            corre=corre->izq;
            corre->pac=1;
            switch(corre->bandPared)
             {
            case 0:
            if(bandpoder==1)
            {
              if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizqPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2POWER);
                band=0;
             }
            }
            else
            {
             if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizq);
              band=corre->pac=1;1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2);
                band=0;
             }
            }
                break;
             case 2:
            j.pts=j.pts+10;
            bandpoder=1;
            tiempo=0;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizqPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2POWER);
                band=0;
             }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 3:
            j.pts=j.pts+1;
            if(bandpoder==1)
            {
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizqPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2POWER);
                band=0;
             }
            }
            else
            {
             if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizq);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2);
                band=0;
             }
            }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 4:
            if(bandpoder==1)
            {
            j.pts=j.pts+100;
            setcolor(15);
            bar((correCasper1)->x,(correCasper1)->y, (correCasper1)->x+INC+2, (correCasper1)->y+INC);
            bar((correCasper2)->x,(correCasper2)->y, (correCasper2)->x+INC+2, (correCasper2)->y+INC);
            correCasper1->bandPared=0;
            correCasper2->bandPared=0;
            correCasper1=m->der->der->der->der->der->der->der->der->der->der->der->der->der->der->aba->aba->aba->aba;
            correCasper2=m->der->der->der->der->der->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba;
            correCasper1->bandPared=4;
            correCasper2->bandPared=4;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizqPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2POWER);
                band=0;
             }
            setcolor(0);
            bar(1040,95,1200,125);
            corre->bandPared=0;
            }
            else
            {
            corre=spawn;
            j.vidas=j.vidas-1;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanizq);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmanizq2);
                band=0;
             }
             setcolor(0);
             bar(1040,120,1200,125);
            }
                break;
             }
        }
        break;
         case KEY_RIGHT:
        if(corre->der && corre->der->bandPared!=9)
        {
            corre=corre->der;
            corre->pac=1;
            switch(corre->bandPared)
             {
            case 0:
            if(bandpoder==1)
            {
                if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanderPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2POWER);
                band=0;
             }
            }
            else
            {
             if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmander);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2);
                band=0;
             }
            }
                break;
             case 2:
            j.pts=j.pts+10;
            bandpoder=1;
            tiempo=0;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanderPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2POWER);
                band=0;
             }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 3:
            j.pts=j.pts+1;
            if(bandpoder==1)
            {
              if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmanderPOWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2POWER);
                band=0;
             }
            }
            else
            {
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmander);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2);
                band=0;
             }
            }
             setcolor(0);
             bar(1040,95,1200,125);
             corre->bandPared=0;
                break;


             case 4:
            if(bandpoder==1)
            {
            j.pts=j.pts+100;
            setcolor(15);
            bar((correCasper1)->x,(correCasper1)->y, (correCasper1)->x+INC+2, (correCasper1)->y+INC);
            bar((correCasper2)->x,(correCasper2)->y, (correCasper2)->x+INC+2, (correCasper2)->y+INC);
            correCasper1->bandPared=0;
            correCasper2->bandPared=0;
            correCasper1=m->der->der->der->der->der->der->der->der->der->der->der->der->der->der->aba->aba->aba->aba;
            correCasper2=m->der->der->der->der->der->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba->aba;
            correCasper1->bandPared=4;
            correCasper2->bandPared=4;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmander2POWER);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2POWER);
                band=0;
             }
            setcolor(0);
            bar(1040,95,1200,125);
            corre->bandPared=0;
            }
            else
            {
             corre=spawn;
            j.vidas=j.vidas-1;
            if(band==0)
             {
              copiarArchivo(corre->dibujo,pillmander);
              band=1;
             }
             else
             {
                copiarArchivo(corre->dibujo,pillmander2);
                band=0;
             }
             setcolor(0);
             bar(1040,120,1200,125);
            }
                break;
             }
        }
        break;
    }
   }while(j.vidas>0);
   /*TReg j={"Carlos",30};
   guarda_Record("Record.txt",j);*/
   agregarArchivoPuntaje(fp,puntaje,j);
   closegraph();
   crearventana(1100,800);
   MenuPrincipal();
}


int seleccionaBotonAtras(BOTONIN bBot,int iXx,int iYy)
{

         if((iXx>=bBot.X1)&&(iYy>=bBot.Y1)&&(iXx<=bBot.X2)&&(iYy<=bBot.Y2))
         {
             setcolor(15);
             cleardevice();
             MenuPrincipal();
             return 1;
         }
}


int Score(PuntJugador puntaje[10]){
    char snum[5];
    BOTONIN botonAtras;

    int i,ancho=35,aux=ancho/10, anchoBotonX = 125, anchoBotonY = 40, iXraton, iYraton, option =0;
    botonAtras.X1 = 465;
    botonAtras.Y1 = 710;
    botonAtras.X2 = botonAtras.X1+anchoBotonX;
    botonAtras.Y2 = botonAtras.Y1+anchoBotonY;
    bar(botonAtras.X1, botonAtras.Y1, botonAtras.X2, botonAtras.Y2);
    outtextxy(botonAtras.X1+aux+25, botonAtras.Y1+aux+3, "Atras");
    int x1 = 260;
    int y1 = 220;
    setcolor(3);
    settextstyle(0, HORIZ_DIR, 5);
    outtextxy(370, 60, "PILL-MAN");
    outtextxy(310, 110,"TOP PLAYERS");

    setcolor(15);
    settextstyle(10,0,3);//tipografia,orientacion,tamaño, la tipografia inicia en 1


    for(i=0;i<10;i++)
    {
    outtextxy(x1+aux,y1+aux,puntaje[i].nombreJugador);
    outtextxy(x1+aux+500,y1+aux,itoa(puntaje[i].puntuacion,snum, 10));
    y1=y1+ancho+10;
    }
    do
        {
            delay(50);
            if(ismouseclick(WM_LBUTTONDOWN)) //Si el mouse es presionado
            {
                delay(50);
                getmouseclick(WM_LBUTTONDOWN,iXraton,iYraton);
                option = seleccionaBotonAtras(botonAtras, iXraton, iYraton);  //Seleciona el boton de las coordenadas
            }
        }
    while(option == 0);
    return 0;
}

int Help(Reglas reglas){

    BOTONIN botonAtras;
    int ancho=35,aux=ancho/10, anchoBotonX = 125, anchoBotonY = 40, iXraton, iYraton, option =0;
    botonAtras.X1 = 465;
    botonAtras.Y1 = 710;
    botonAtras.X2 = botonAtras.X1+anchoBotonX;
    botonAtras.Y2 = botonAtras.Y1+anchoBotonY;
    bar(botonAtras.X1, botonAtras.Y1, botonAtras.X2, botonAtras.Y2);
    outtextxy(botonAtras.X1+aux+25, botonAtras.Y1+aux+3, "Atras");
    int x1 = 10;
    int y1 = 150;
    setcolor(3);
    settextstyle(0, HORIZ_DIR, 5);
    outtextxy(300, 60, "PILL-MAN HELP");


    setcolor(15);
    settextstyle(10,0,1);//tipografia,orientacion,tamaño, la tipografia inicia en 1

    for(int i =0; i<10;i++){
    outtextxy(x1+aux,y1+aux,reglas.intro[i]);
    y1=y1+ancho-2;
    }
    outtextxy(x1+aux,y1+aux+30,reglas.Intrucciones);
    outtextxy(x1+aux,y1+aux+60,reglas.Reglas1);
    outtextxy(x1+aux,y1+aux+90,reglas.Reglas2);
    outtextxy(x1+aux,y1+aux+120,reglas.Reglas3);
    outtextxy(x1+aux,y1+aux+150,reglas.Reglas4);

    do
        {
            delay(50);
            if(ismouseclick(WM_LBUTTONDOWN)) //Si el mouse es presionado
            {
                delay(50);
                getmouseclick(WM_LBUTTONDOWN,iXraton,iYraton);
                option = seleccionaBotonAtras(botonAtras, iXraton, iYraton);  //Seleciona el boton de las coordenadas
            }
        }
    while(option == 0);
    return 0;
}


void CopyR()
{
    settextstyle(5, HORIZ_DIR, 2);
    outtextxy(100, 725, "Carlos Daniel Contreras Perez");
    settextstyle(5, HORIZ_DIR, 2);
    outtextxy(100, 750, "Alan Edgardo Hernandez Sandoval");
}

void readArchivoPuntaje(FILE *fp, PuntJugador puntaje[10]){
    int i =0, temp, itemp;
    bool swapped = false;
    PuntJugador puntaje1[100];
    char tempName[50];
    fp=fopen("puntaje.txt","r");

    if (fp==NULL) {
        printf("Error al abrir el archivo");
    exit(0);
    }

    do{
    fscanf(fp,"%s %d\n",puntaje1[i].nombreJugador, &puntaje1[i].puntuacion);
    //printf("%s %d\n",puntaje[i].nombreJugador, puntaje[i].puntuacion);
    i++;
    }while(!feof(fp));

    fclose(fp);

    for(int j =0; j<i;j++){
        swapped = false;
        for(int k=0;k<i-j;k++){
            if(puntaje1[k].puntuacion<puntaje1[k+1].puntuacion){
                temp=puntaje1[k].puntuacion;
                strcpy(tempName, puntaje1[k].nombreJugador);

                puntaje1[k].puntuacion=puntaje1[k+1].puntuacion;
                strcpy(puntaje1[k].nombreJugador, puntaje1[k+1].nombreJugador);

                puntaje1[k+1].puntuacion=temp;
                strcpy(puntaje1[k+1].nombreJugador, tempName);


            }
        }

    }

    for(int i =0;i<11;i++){
       // printf("%d",puntaje1[i].puntuacion);
        //printf("%s\n",puntaje1[i].nombreJugador);
    }

    for(int i =0;i<10;i++){
        puntaje[i].puntuacion=puntaje1[i].puntuacion;
        strcpy(puntaje[i].nombreJugador, puntaje1[i].nombreJugador);
    }
}

void agregarArchivoPuntaje(FILE *fp, PuntJugador puntaje[10], TReg j){
    PuntJugador puntAnt;
    int i =0;
    fp=fopen("puntaje.txt","a");

    if (fp==NULL) {
        printf("Error al abrir el archivo");
    exit(0);
    }

    fprintf(fp,"%s %d\n",j.nombre, j.pts);

    fclose(fp);
}

void readArchivoReglas(FILE *fp, Reglas *reglas){
    fp=fopen("reglas.txt","r");

    if (fp==NULL) {
        printf("Error al abrir el archivo");
    exit(0);
    }

    for(int i = 0; i<10 ; i++){
    fgets(reglas->intro[i],300,fp);
    //printf("%s\n",reglas.intro);
    }

    fgets(reglas->Intrucciones, 100, fp );
    //printf("%s\n",reglas->Intrucciones);
    fgets(reglas->Reglas1, 200, fp);
    //printf("%s\n",reglas->Reglas1);
    fgets(reglas->Reglas2, 200, fp);
    //printf("%s\n",reglas->Reglas2);
    fgets(reglas->Reglas3, 200, fp);
    //printf("%s\n",reglas->Reglas3);
    fgets(reglas->Reglas4, 200, fp);

    //printf("%s\n",reglas->Reglas4);

    fclose(fp);
}


void animacionMenu()
{

    int x,y,r;
    x=550;
    y=350;
    r=90;

    setcolor(14);
    setfillstyle(1,14);
    pieslice(x,y,30,330,r);//Grafica circular

}


TNodo *crea_nodo(int x, int y)
{
  TNodo *aux;

  aux=(TNodo*)malloc(sizeof(TNodo));
  aux->x=x;
  aux->y=y;
  aux->y2=aux->y+INC;
  aux->x2=aux->x+INC;
  aux->aba=aux->arr=aux->der=aux->izq=NULL;
  return(aux);
}

void malla(TNodo **cab, int xi,int yi, int nc, int nr)
{
   int r,c;
   TNodo *aux,*ant;

   for(r=0;r<nr;r++)
     for(c=0;c<nc;c++)
        if(r==0)
        {
           if(c==0)
           {
               aux=crea_nodo(xi+(INC*c), yi+(INC*r));
               *cab=aux;
               ant=aux;

           }
           else
           {
              aux->der= crea_nodo(xi+(INC*c), yi+(INC*r));
              aux->der->izq=aux;
              aux=aux->der;
           }

        }
        else
        {
           if(c==0)
           {
              aux=crea_nodo(xi+(INC*c), yi+(INC*r));
              aux->arr=ant;
              ant->aba=aux;
              ant=aux;
           }
           else
           {
               aux->der= crea_nodo(xi+(INC*c), yi+(INC*r));
               aux->der->izq=aux;
               aux=aux->der;
               aux->izq->arr->der->aba=aux;
               aux->arr=aux->izq->arr->der;

           }
        }

}

void imprime_maya(TNodo *cab)
{
  TNodo *aux;

  for(;cab;cab=cab->aba)
    for(aux=cab;aux;aux=aux->der)
    {
       setcolor(15);
       //rectangle(aux->x,aux->y, aux->x+INC, aux->y+INC);
       delay(1);
    }

}

void imprime_mapa(int x, int y, int nc, int nr)
{
    int x_init=x;
    int y_init=y;

       setcolor(15);
       setfillstyle(1,9);
       //CREA PRIMERA LINEA HORIZONTAL
       for(int i = 0; i<nc;i++){
            bar(x,y,x+INC,y+INC);
            x=x+INC;
       }

       x=x_init;
       y=y_init;

       //CREA LINEA IZQUIERDA VERTICAL
       for(int i = 0; i<nr;i++){
            bar(x,y,x+INC,y+INC);
            y=y+INC;
       }
       x=x_init;
       y=y_init;

        //CREA LINEA DERECHA VERTICAL
       for(int i = 0; i<nr;i++){
            bar(x+(INC*(nc-1)),y,x+(INC*nc),y+INC);
            y=y+INC;
       }
       x=x_init;
       y=y_init;
        //CREA ULTIMA LINEA HORIZONTAL
       for(int i = 0; i<nc;i++){
            bar(x,y+(INC*(nr-1)),x+INC,y+(INC*(nr)));
            x=x+INC;
       }

       x=x_init;
       y=y_init;
        //CREA PRIMER MODULO DE IZLAS

       x=x_init;
       y=y_init;
       islaModule(x+(INC*2), y+(INC*2), 1, 4);

       x=x_init;
       y=y_init;
       islaModule(x+(INC*4), y+(INC*2), 1, 4);

        x=x_init;
       y=y_init;
       islaModule(x+(INC*6), y+(INC*2), 2, 4);

        x=x_init;
       y=y_init;
       islaModule(x+(INC*9), y+(INC), 2, 2);

       //CREA SEGUNDO MODULO

        x=x_init;
       y=y_init;
       islaModule(x+(INC*12), y+(INC*2), 2, 4);

       x=x_init;
       y=y_init;
       islaModule(x+(INC*15), y+(INC*2), 1, 4);

       x=x_init;
       y=y_init;
       islaModule(x+(INC*17), y+(INC*2), 1, 4);

        //CREA MODULOS CENTRALES
        x=x_init;
       y=y_init;
       islaModule(x+(INC), y+(INC*7), 7, 1);

        x=x_init;
       y=y_init;
       islaModule(x+(INC*12), y+(INC*7), 7, 1);

        x=x_init;
       y=y_init;
       islaModule(x+(INC*9), y+(INC*5), 2, 5);

       //CREA TERCER MODULO
        x=x_init;
       y=y_init;
       islaModule(x+(INC*2), y+(INC*9), 1, 4);

       x=x_init;
       y=y_init;
       islaModule(x+(INC*4), y+(INC*9), 1, 4);

        x=x_init;
       y=y_init;
       islaModule(x+(INC*6), y+(INC*9), 2, 4);

        x=x_init;
       y=y_init;
       islaModule(x+(INC*9), y+(INC*12), 2, 2);

       //CREA CUARTO MODULO

        x=x_init;
       y=y_init;
       islaModule(x+(INC*12), y+(INC*9), 2, 4);

       x=x_init;
       y=y_init;
       islaModule(x+(INC*15), y+(INC*9), 1, 4);

       x=x_init;
       y=y_init;
       islaModule(x+(INC*17), y+(INC*9), 1, 4);



       delay(1);

}

void islaModule(int x, int y, int nc, int nr){

        int y_init = y;
        setfillstyle(1,9);
        for(int i =0; i<nc;i++){
            for(int j =0; j<nr;j++){
                bar(x,y,x+INC,y+INC);
                y=y+INC;
            }
            x=x+INC;
            y=y_init;


        }


}


/*void ayuda(char nombre[], int x1, int y1, int x2, int y2)
{
  FILE *a;
  char texto[100];
  int x,y;


  cleardevice();
  setcolor(15);
  rectangle(x1,y1,x2, y2);
  a=fopen(nombre, "r");
  if(a!=NULL)
  {
    y=y1+20;
    settextstyle(8,0,2);
    while(!feof(a))
    {
        fgets(texto,100,a);
        x=x1+((x2-x1)-textwidth(texto))/2;
        outtextxy(x,y,texto);
        y+=textheight(texto);
        if(y>y2-20)
        {
            outtextxy(x,y,"ENTER PARA CONTINUAR");
            getch();
            y=y1+20;
            cleardevice();
            setcolor(15);
            rectangle(x1,y1,x2, y2);
        }


    }
     outtextxy(x,y,"ENTER PARA CONTINUAR");
     getch();
    fclose(a);


  }
  else
  {
      outtextxy(x1+100,y1+100,"Archivo de Ayuda no encontrado");
      getch();
  }



}*/

void guarda_Record(char *nom_arch, TReg j)
{
    FILE *f;
    TReg *A;
    int pos,nr;

    f=fopen(nom_arch,"rb+");
    if(f==NULL)
    {
       f=fopen(nom_arch,"wb+");
       fwrite(&j,sizeof(TReg),1,f);
       fclose(f);
    }
    else
    {
        fseek(f,0,SEEK_END);
        pos=ftell(f);
        nr=pos/sizeof(TReg);
        fseek(f,0,SEEK_SET);
        if(nr+1<N)
         A=(TReg*)malloc(sizeof(TReg)*(nr+1));
        else
         A=(TReg*)malloc(sizeof(TReg)*nr);
        fread(A,sizeof(TReg),nr,f);
        while(A[nr-1].pts<j.pts &&nr>0)
        {
            if(nr<N)
                A[nr]=A[nr-1];
            nr--;
        }
        if(nr<N)
            A[nr]=j;
        fseek(f,0,SEEK_SET);
        fwrite(A,sizeof(TReg),nr+1,f);
        fclose(f);
    }

}


void inicializar_mapa(int x, int y, int nc, int nr, TNodo **nodo){

        TNodo *corre = *nodo,*corre2;
        int arrayBanderas[nr][nc];
        int color, x_init=x;
        DatosPaleta Orbegrande[20][20],Orbechico[20][20],Willy[20][20],Lucas[20][20];
        AbrirArchivo("ORBE ENANA.txt",Orbechico);
        AbrirArchivo("ORBE GRANDE.txt",Orbegrande);
        AbrirArchivo("CASPER WILLY ABAJO 1.txt",Willy);
        AbrirArchivo("CASPER LUCAS ABAJO 1.txt",Lucas);
        for(int i =0; i<nr;i++){
            for(int j =0; j<nc;j++){
                arrayBanderas[i][j] = getpixel(x+25, y+25);
                //printf("%d ", arrayBanderas[i][j]);
                x = x +INC;
            }
            y = y + INC;
            //printf("\n");
            x = x_init;
        }
        //printf("\n\n");
        for(int i =0; i<(nr);i++){

            for(int j =0; j<nc;j++){
                corre->bandPared= arrayBanderas[i][j] ;
                //printf("%d ", corre->bandPared);
                if(j!=(nc-1)){
                corre= corre->der;
                }
            }
            if(i!=(nr)-1){
            corre= corre->aba;
            } else {
            break;
            }
            //printf("\n");
            for(int j =nc; j>0;j--){
                corre->bandPared= arrayBanderas[i+1][j-1] ;
                //printf("%d ", corre->bandPared);
                if(j!=1){
                corre= corre->izq;
                }
            }
            corre= corre->aba;
            y = y + INC;
            //printf("\n");
            i++;
        }
        for(int i=0;i<nr-1;i++){
            corre = corre->arr;
        }
        for(int i=0;i<nc-1;i++){
            corre = corre->izq;
        }
        corre=*nodo;
        corre2=corre;

        for(int i=0;i<nr;i++)
        {
            for(int j=0;j<nc;j++)
            {
                if((i==6&&j==1)||(i==6&&j==18)||(i==8&&j==1)||(i==8&&j==18))
                {
                    corre->bandPared=2;//2 para super orbes

                    copiarArchivo(corre->dibujo,Orbegrande);
                    AbrirCuadriculaGrande(corre->x,corre->y,corre->dibujo);

                }
                else if(i==4&&j==14)
                {
                    corre->bandPared=4;//4 para fantasmas
                    copiarArchivo(corre->dibujo,Willy);
                    AbrirCuadriculaGrande(corre->x,corre->y,corre->dibujo);
                    correCasper2=corre;
                }
                else if(i==11&&j==5)
                {
                    corre->bandPared=4;//4 para fantasmas
                    copiarArchivo(corre->dibujo,Lucas);
                    AbrirCuadriculaGrande(corre->x,corre->y,corre->dibujo);
                    correCasper1=corre;
                }
                else if(corre->bandPared==0)
                {
                    corre->bandPared=3;//3 para orbes chicas

                    copiarArchivo(corre->dibujo,Orbechico);
                    AbrirCuadriculaGrande(corre->x,corre->y,corre->dibujo);
                }
                corre=corre->der;
            }
            corre2=corre2->aba;
            corre=corre2;
        }


}


void AbrirArchivo(char nom[],DatosPaleta CuadroGrande[20][20])
{
    int i,j,x;
    FILE *fp;
    fp=fopen(nom,"r");
     for(i=0;i<13;i++)
    {
        for(j=0;j<16;j++)
        {
        fscanf(fp,"%d",&x);
        if(x==15){
            x=0;
        }
        CuadroGrande[i][j].Color=x;
        //printf("Cuadro[%d][%d]=%d\n",i,j,CuadroGrande[i][j].Color);
        }
    }
    fclose(fp);
}

void copiarArchivo(DatosPaleta CuadroGrande1[20][20],DatosPaleta CuadroGrande[20][20])
{
    int i,j,x;
     for(i=0;i<13;i++)
    {
        for(j=0;j<16;j++)
        {
        CuadroGrande1[i][j].Color=CuadroGrande[i][j].Color;
        }
    }
}

void AbrirCuadriculaGrande(int x1,int y1,DatosPaleta CuadroGrande[20][20])
{

    int x11=x1,i,j,ancho=3,largo=3;
    for(i=0;i<13;i++)//Ciclo for para las tramas
    {
        for(j=1;j<15;j++)//Ciclo for para los colores
        {
        setcolor(CuadroGrande[i][j].Color);
        setfillstyle(1,CuadroGrande[i][j].Color);
        bar(x1,y1+1,x1+ancho,y1+largo);
        x1=x1+ancho;
        }
        x1=x11;
        y1=y1+3;
    }
}

void moverFantasma(TNodo **Casper, TNodo **PillMan){

    int xPill, yPill, xCasp, yCasp, bandMov, diffX, diffY, bandOrbe;
    DatosPaleta Orbegrande[20][20],Orbechico[20][20];
    AbrirArchivo("ORBE ENANA.txt",Orbechico);
    AbrirArchivo("ORBE GRANDE.txt",Orbegrande);

    xPill=(*PillMan)->x;
    yPill=(*PillMan)->y;
    xCasp=(*Casper)->x;
    yCasp=(*Casper)->y;

    diffX = xPill-xCasp;
    diffY = yPill-yCasp;

    if(diffX<0){
        diffX = diffX*(-1);
    }

    if(diffY<0){
        diffY = diffY*(-1);
    }

    if((xPill>=xCasp) && (yPill>=yCasp)){
        bandMov=1; //Casp esta arriba izquierda
    } else if((xPill>=xCasp) && (yPill<=yCasp)){
        bandMov=2; // Casp esta abajo izquierda
    } else if((xPill<=xCasp) && (yPill<=yCasp)){
        bandMov=3; // Casp esta abajo derecha
    } else if((xPill<=xCasp) && (yPill>=yCasp)){
        bandMov=4; // Casp esta arriba derecha
    } else if((diffX==40) && (diffY==40)){
        bandMov=5;
    } else {
        bandMov=0;
    }

    //printf("bandera corre: %d, bandera posicion: %d\n", (*PillMan)->bandPared, bandMov);

    switch(bandMov){
    case 1:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);

        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){

            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }


            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 2:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 3:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }

        break;
    case 4:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 5:
        if((*Casper)->der->pac==1){
                if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->arr->pac==1){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;

        } else if((*Casper)->izq->pac==1){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->aba->pac==1){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    }

    DatosPaleta Willy[20][20],Lucas[20][20];

    AbrirArchivo("CASPER WILLY ABAJO 1.txt",Willy);
    AbrirArchivo("CASPER LUCAS ABAJO 1.txt",Lucas);

    //(*Casper)= (*Casper)->arr;
    copiarArchivo((*Casper)->dibujo,Willy);

    AbrirCuadriculaGrande((*Casper)->x,(*Casper)->y,(*Casper)->dibujo);


}



void moverFantasma1(TNodo **Casper, TNodo **PillMan){

    int xPill, yPill, xCasp, yCasp, bandMov, diffX, diffY, bandOrbe;
    DatosPaleta Orbegrande[20][20],Orbechico[20][20];
    AbrirArchivo("ORBE ENANA.txt",Orbechico);
    AbrirArchivo("ORBE GRANDE.txt",Orbegrande);

    xPill=(*PillMan)->x;
    yPill=(*PillMan)->y;
    xCasp=(*Casper)->x;
    yCasp=(*Casper)->y;

    diffX = xPill-xCasp;
    diffY = yPill-yCasp;

    if(diffX<0){
        diffX = diffX*(-1);
    }

    if(diffY<0){
        diffY = diffY*(-1);
    }

    if((xPill>=xCasp) && (yPill>=yCasp)){
        bandMov=1; //Casp esta arriba izquierda
    } else if((xPill>=xCasp) && (yPill<=yCasp)){
        bandMov=2; // Casp esta abajo izquierda
    } else if((xPill<=xCasp) && (yPill<=yCasp)){
        bandMov=3; // Casp esta abajo derecha
    } else if((xPill<=xCasp) && (yPill>=yCasp)){
        bandMov=4; // Casp esta arriba derecha
    } else if((diffX==40) && (diffY==40)){
        bandMov=5;
    } else{
        bandMov=0;
    }

    //printf("bandera corre->arriba: %d, bandera posicion: %d\n", (*PillMan)->arr->bandPared, bandMov);

    switch(bandMov){
    case 1:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);

        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){

            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }


            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 2:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 3:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }

        break;
    case 4:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 5:
        if((*Casper)->der->pac==1){
                if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->arr->pac==1){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;

        } else if((*Casper)->izq->pac==1){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->aba->pac==1){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    }

    if(bandMov!=0){
    DatosPaleta Willy[20][20],Lucas[20][20];

    AbrirArchivo("CASPER WILLY ABAJO 1.txt",Willy);
    AbrirArchivo("CASPER LUCAS ABAJO 1.txt",Lucas);

    //(*Casper)= (*Casper)->arr;
    copiarArchivo((*Casper)->dibujo,Lucas);

    AbrirCuadriculaGrande((*Casper)->x,(*Casper)->y,(*Casper)->dibujo);
    }

}


void moverFantasmaReverso(TNodo **Casper, TNodo **PillMan){

    int xPill, yPill, xCasp, yCasp, bandMov, diffX, diffY, bandOrbe;
    DatosPaleta Orbegrande[20][20],Orbechico[20][20];
    AbrirArchivo("ORBE ENANA.txt",Orbechico);
    AbrirArchivo("ORBE GRANDE.txt",Orbegrande);

    xPill=(*PillMan)->x;
    yPill=(*PillMan)->y;
    xCasp=(*Casper)->x;
    yCasp=(*Casper)->y;

    diffX = xPill-xCasp;
    diffY = yPill-yCasp;

    if(diffX<0){
        diffX = diffX*(-1);
    }

    if(diffY<0){
        diffY = diffY*(-1);
    }

    if((xPill>=xCasp) && (yPill>=yCasp)){
        bandMov=3; //Casp esta arriba izquierda 3
    } else if((xPill>=xCasp) && (yPill<=yCasp)){
        bandMov=4; // Casp esta abajo izquierda  4
    } else if((xPill<=xCasp) && (yPill<=yCasp)){
        bandMov=1; // Casp esta abajo derecha    1
    } else if((xPill<=xCasp) && (yPill>=yCasp)){
        bandMov=2; // Casp esta arriba derecha   2
    } else {
        bandMov=0;
    }

    //printf("bandera corre: %d, bandera posicion: %d\n", (*PillMan)->bandPared, bandMov);

    switch(bandMov){
    case 1:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);

        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){

            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }


            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 2:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 3:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }

        break;
    case 4:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 5:
        if((*Casper)->der->pac==1){
                if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->arr->pac==1){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;

        } else if((*Casper)->izq->pac==1){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->aba->pac==1){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    }

    DatosPaleta Willy[20][20],Lucas[20][20];

    AbrirArchivo("CASPER WILLY ABAJO 1.txt",Willy);
    AbrirArchivo("CASPER LUCAS ABAJO 1.txt",Lucas);

    //(*Casper)= (*Casper)->arr;
    copiarArchivo((*Casper)->dibujo,Willy);

    AbrirCuadriculaGrande((*Casper)->x,(*Casper)->y,(*Casper)->dibujo);


}



void moverFantasmaReverso1(TNodo **Casper, TNodo **PillMan){

    int xPill, yPill, xCasp, yCasp, bandMov, diffX, diffY, bandOrbe;
    DatosPaleta Orbegrande[20][20],Orbechico[20][20];
    AbrirArchivo("ORBE ENANA.txt",Orbechico);
    AbrirArchivo("ORBE GRANDE.txt",Orbegrande);

    xPill=(*PillMan)->x;
    yPill=(*PillMan)->y;
    xCasp=(*Casper)->x;
    yCasp=(*Casper)->y;

    diffX = xPill-xCasp;
    diffY = yPill-yCasp;

    if(diffX<0){
        diffX = diffX*(-1);
    }

    if(diffY<0){
        diffY = diffY*(-1);
    }

    if((xPill>=xCasp) && (yPill>=yCasp)){
        bandMov=3; //Casp esta arriba izquierda
    } else if((xPill>=xCasp) && (yPill<=yCasp)){
        bandMov=4; // Casp esta abajo izquierda
    } else if((xPill<=xCasp) && (yPill<=yCasp)){
        bandMov=1; // Casp esta abajo derecha
    } else if((xPill<=xCasp) && (yPill>=yCasp)){
        bandMov=2; // Casp esta arriba derecha
    } else{
        bandMov=0;
    }

    //printf("bandera corre->arriba: %d, bandera posicion: %d\n", (*PillMan)->arr->bandPared, bandMov);

    switch(bandMov){
    case 1:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);

        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){

            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }


            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 2:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 3:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }

        break;
    case 4:
        setcolor(15);
        bar((*Casper)->x,(*Casper)->y, (*Casper)->x+INC+2, (*Casper)->y+INC);
        if((diffX>=diffY) && ((*Casper)->izq->bandPared!=9) && ((*Casper)->izq->bandPared!=4)){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->aba->bandPared!=9) && ((*Casper)->aba->bandPared!=4)){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->der->bandPared!=9) && ((*Casper)->der->bandPared!=4)){
            if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if(((*Casper)->arr->bandPared!=9) && ((*Casper)->arr->bandPared!=4)){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    case 5:
        if((*Casper)->der->pac==1){
                if(((*Casper)->der->bandPared==3)||((*Casper)->der->bandPared==2)){
                if((*Casper)->der->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->der;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->izq->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                } else{
                    copiarArchivo((*Casper)->izq->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->izq->x,(*Casper)->izq->y,(*Casper)->izq->dibujo);
                }
            }
            if(((*Casper)->izq->bandPared=!3)||((*Casper)->izq->bandPared=!2)||((*Casper)->izq->bandPared=!9)){
            (*Casper)->izq->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->arr->pac==1){
            if(((*Casper)->arr->bandPared==3)||((*Casper)->arr->bandPared==2)){
                if((*Casper)->arr->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->arr;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->aba->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                } else{
                    copiarArchivo((*Casper)->aba->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->aba->x,(*Casper)->aba->y,(*Casper)->aba->dibujo);
                }
            }
            if(((*Casper)->aba->bandPared=!3)||((*Casper)->aba->bandPared=!2)||((*Casper)->aba->bandPared=!9)){
            (*Casper)->aba->bandPared=0;
            }
            (*Casper)->bandPared=4;

        } else if((*Casper)->izq->pac==1){
            if(((*Casper)->izq->bandPared==3)||((*Casper)->izq->bandPared==2)){
                if((*Casper)->izq->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->izq;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->der->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                } else{
                    copiarArchivo((*Casper)->der->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->der->x,(*Casper)->der->y,(*Casper)->der->dibujo);
                }
            }
            if(((*Casper)->der->bandPared=!3)||((*Casper)->der->bandPared=!2)||((*Casper)->der->bandPared=!9)){
            (*Casper)->der->bandPared=0;
            }
            (*Casper)->bandPared=4;
        } else if((*Casper)->aba->pac==1){
            if(((*Casper)->aba->bandPared==3)||((*Casper)->aba->bandPared==2)){
                if((*Casper)->aba->bandPared==3){
                        bandOrbe=3;
                } else{
                        bandOrbe=2;
                }
            }



            (*Casper)=(*Casper)->aba;

            if((bandOrbe==3)||(bandOrbe==2)){
                if(bandOrbe==3){
                    copiarArchivo((*Casper)->arr->dibujo, Orbechico);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                } else{
                    copiarArchivo((*Casper)->arr->dibujo, Orbegrande);
                    AbrirCuadriculaGrande((*Casper)->arr->x,(*Casper)->arr->y,(*Casper)->arr->dibujo);
                }
            }
            if(((*Casper)->arr->bandPared=!3)||((*Casper)->arr->bandPared=!2)||((*Casper)->arr->bandPared=!9)){
            (*Casper)->arr->bandPared=0;
            }
            (*Casper)->bandPared=4;
        }
        break;
    }

    if(bandMov!=0){
    DatosPaleta Willy[20][20],Lucas[20][20];

    AbrirArchivo("CASPER WILLY ABAJO 1.txt",Willy);
    AbrirArchivo("CASPER LUCAS ABAJO 1.txt",Lucas);

    //(*Casper)= (*Casper)->arr;
    copiarArchivo((*Casper)->dibujo,Lucas);

    AbrirCuadriculaGrande((*Casper)->x,(*Casper)->y,(*Casper)->dibujo);
    }

}


