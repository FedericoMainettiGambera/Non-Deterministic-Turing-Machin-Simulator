#include <stdio.h>
#include <stdlib.h>

struct lista_transizioni {
    int statoIN;
    int statoOUT;
    char carattereLETTO;
    char carattereSCRITTO;
    char movimento;
    struct lista_transizioni *nextPtr;

};
typedef struct lista_transizioni Lista_transizioni;
typedef struct lista_transizioni *Lista_transizioniPtr;
Lista_transizioniPtr testa_lista_transizioni=NULL;
int enumerazione_transizioni=0;

struct lista_stati_di_accettazione {
    int stato_di_accettazione;
    struct lista_stati_di_accettazione *nextPtr;
};
typedef struct lista_stati_di_accettazione Lista_stati_di_accettazione;
typedef struct lista_stati_di_accettazione *Lista_stati_di_accettazionePtr;
Lista_stati_di_accettazionePtr testa_lista_stati_di_accettazione=NULL;

int MAX;
int max_ripetizioni_mosse;

struct coda_stati_livello{
    int stato;
    struct coda_stati_livello *nextPtr;
    struct nastro *testina_curr;
};
typedef struct coda_stati_livello Coda_stati_livello;
typedef struct coda_stati_livello *Coda_stati_livelloPtr;
Coda_stati_livelloPtr stati_livello_primoPtr=NULL;
Coda_stati_livelloPtr stati_livello_ultimoPtr=NULL;

struct nastro{
    char carattere;
    int utilizzatori;
    struct nastro *downPtr_coda;
    struct nastro *nextPtr;
    struct nastro *prevPtr;
};
typedef struct nastro Nastro;
typedef struct nastro *NastroPtr;
NastroPtr nastro_primoPtr=NULL;
NastroPtr nastro_ultimoPtr=NULL;

int stato_in_analisi;
char carattere_in_analisi;
int profondita = 0;
int lista_stati_da_controllare[300];
int lista_testine_stati_da_controllare[300];
int transizione_trovata =0;

void inserisci_transizione (int stato_IN, int stato_OUT, char carattere_LETTO, char carattere_SCRITTO, char Movimento);
void stampa_lista_transizioni ();

void inserisci_stato_di_accettazione (int acc);
void stampa_lista_lista_stati_di_accettazione ();

int leggi_input();
void inserisci_nastro (char c);
void stampa_nastro ();
void svuota_nastro();
void svuota_coda();
void inizializza_coda_stati_livello();
void svuota_coda_stati_livello();
void stampa_coda_stati_livello ();

int simulazione();
void enqueue_nastro(NastroPtr testa_coda, char carattere, int utilizzatore);
void dequeue_nastro(NastroPtr testa_coda);
void ricompatto();
void unisci(NastroPtr Ptr1, NastroPtr Ptr2);
void prolunga_nastro_destra();
void prolunga_nastro_sinistra();
int controllo_elementi_coda_stati_livelli();
int controlla_accettazione();
int salva_stati();
void inizializza_liste_salvati();

int main ()
{
//    freopen("input.txt", "r", stdin); 
    char riga_letta[20];
    int stato_IN;
    int stato_OUT;
    char carattere_LETTO;
    char carattere_SCRITTO;
    char Movimento;
    int acc;
    int fine_input=0;
    int VALOREOUTPUT=33;

    //leggo e ingoro "tr":
//    printf("Riga letta:");
    fgets(riga_letta, 20, stdin);
//    printf("Ignoro:%s\n", riga_letta);

    //leggo tutte le transizioni e le metto in una lista
//    printf("Riga letta:");
    fgets(riga_letta, 20, stdin);
    while(riga_letta[0]!='a')
    {
        sscanf (riga_letta, "%d %c %c %c %d", &stato_IN,&carattere_LETTO,&carattere_SCRITTO,&Movimento,&stato_OUT);
//        printf("Stato_IN: %d\nStato_OUT: %d\nCarattere_LETTO: %c\nCarattere_SCRITTO: %c\nMovimento: %c\n\n",stato_IN,stato_OUT,carattere_LETTO,carattere_SCRITTO,Movimento);
        inserisci_transizione (stato_IN, stato_OUT, carattere_LETTO, carattere_SCRITTO, Movimento);

//        printf("Riga letta:");
        fgets(riga_letta, 20, stdin);
    }
//    printf("Tutte le transizioni sono state lette.\n\n");
//    stampa_lista_transizioni();

    //leggo tutti gli stati di accettazione e li metto in  una lista
//    printf("\nRiga letta:");
    fgets(riga_letta, 20, stdin);
    while(riga_letta[0]!='m')
    {
        sscanf(riga_letta, "%d", &acc);
        inserisci_stato_di_accettazione (acc);
//        printf("Riga letta:");
        fgets(riga_letta, 20, stdin);
    }
//    printf("Tutti gli stati di accettazione sono stati letti.\n\n");
//    stampa_lista_lista_stati_di_accettazione();

    //leggo numero massimo di passi eseguibili
//    printf("\nRiga letta:");
    fgets(riga_letta, 20, stdin);
    sscanf(riga_letta, "%d", &MAX);
//    printf("numero massimo di passi eseguibile: %d\n\n", MAX);


//    printf("\nRiga letta:");
    fgets(riga_letta, 20, stdin);
    if (riga_letta[0]!= 'r')
        {printf("ERRORE: mancata lettura di 'run'.\n");}

//    printf("inizio sezione \"run\"\n");

    inizializza_liste_salvati();

    while (fine_input==0)
    {
        VALOREOUTPUT=33;
        fine_input=leggi_input();
        if (fine_input==1){break;}
//        stampa_nastro();
        inizializza_coda_stati_livello();
//        stampa_coda_stati_livello();

        VALOREOUTPUT=simulazione();
        if (VALOREOUTPUT==0)
        {
            printf("0\n");
        }
        if (VALOREOUTPUT==1)
        {
            printf("1\n");
        }
        if (VALOREOUTPUT==2)
        {
            printf("U\n");
        }
        if (VALOREOUTPUT==33)
        {
            printf("ERRORE: output 33\n");
        }
        if ((VALOREOUTPUT!=33)&&(VALOREOUTPUT!=2)&&(VALOREOUTPUT!=1)&&(VALOREOUTPUT!=0))
        {
            printf("ERRORE: output %d\n", VALOREOUTPUT);
        }

        svuota_nastro();
        nastro_primoPtr=NULL;
//        stampa_nastro();
        svuota_coda_stati_livello();
//        stampa_coda_stati_livello();
    }
    return 0;
}

void inserisci_transizione (int stato_IN, int stato_OUT, char carattere_LETTO, char carattere_SCRITTO, char Movimento)
{
    Lista_transizioniPtr nuovoPtr;
    nuovoPtr = malloc ( sizeof (Lista_transizioni));
    if (nuovoPtr == NULL)
        {printf("ERRORE: memoria non sufficiente per la lista delle transizioni.");}
    else
    {
        nuovoPtr->carattereLETTO=carattere_LETTO;
        nuovoPtr->carattereSCRITTO=carattere_SCRITTO;
        nuovoPtr->statoIN=stato_IN;
        nuovoPtr->statoOUT=stato_OUT;
        nuovoPtr->movimento=Movimento;
        nuovoPtr->nextPtr=testa_lista_transizioni;
        testa_lista_transizioni=nuovoPtr;
    }
    return;
}

void stampa_lista_transizioni()
{
    Lista_transizioniPtr currPtr;
    printf("Lista delle transizioni:\n");
    currPtr = testa_lista_transizioni;
    while (currPtr != NULL)
    {
        printf("%d %c %c %c %d\n", currPtr->statoIN,currPtr->carattereLETTO,currPtr->carattereSCRITTO,currPtr->movimento,currPtr->statoOUT);
        currPtr=currPtr->nextPtr;
    }
    return;
}

void inserisci_stato_di_accettazione (int acc)
{
    Lista_stati_di_accettazionePtr nuovoPtr;
    nuovoPtr = malloc ( sizeof (Lista_stati_di_accettazione));
    if (nuovoPtr == NULL) {printf("ERRORE: memoria non sufficiente per la lista degli stati di accettazione.");}
    else
    {
        nuovoPtr->stato_di_accettazione=acc;
        nuovoPtr->nextPtr=testa_lista_stati_di_accettazione;
        testa_lista_stati_di_accettazione=nuovoPtr;
    }
    return;
}

void stampa_lista_lista_stati_di_accettazione ()
{
    Lista_stati_di_accettazionePtr currPtr;
    printf("Lista degli stati di accettazione: \n");
    currPtr=testa_lista_stati_di_accettazione;
    while (currPtr != NULL)
    {
        printf("%d->", currPtr->stato_di_accettazione);
        currPtr=currPtr->nextPtr;
    }
    printf("NULL\n");
    return;
}

int leggi_input()
{
    char c;
    int fine;

    if ((fine=scanf("%c", &c)) != EOF)
    {
        while((fine!= EOF)&&(c!='\n'))
        {
            inserisci_nastro(c);
//            printf("\nho letto:%c", c);
            fine=scanf("%c", &c);
        }
        return 0;
    }
    else
        {return 1;}
}

void inserisci_nastro (char c)
{
    NastroPtr nuovoPtr;
    if (nastro_primoPtr==NULL)
    {
        nastro_primoPtr=malloc(sizeof(Nastro));
        if (nastro_primoPtr==NULL){printf("ERRORE: malloc.\n");}
        else
        {
            nastro_primoPtr->carattere=c;
            nastro_primoPtr->downPtr_coda=NULL;
            nastro_primoPtr->nextPtr=NULL;
            nastro_primoPtr->prevPtr=NULL;
            nastro_primoPtr->utilizzatori=1;
            nastro_ultimoPtr=nastro_primoPtr;
        }
    }
    else
    {
        nuovoPtr=malloc(sizeof(Nastro));
        if (nuovoPtr==NULL){printf("ERRORE: malloc.\n");}
        else
        {
            nuovoPtr->carattere=c;
            nuovoPtr->downPtr_coda=NULL;
            nuovoPtr->nextPtr=NULL;
            nuovoPtr->prevPtr=nastro_ultimoPtr;
            nuovoPtr->utilizzatori=1;
            nastro_ultimoPtr->nextPtr=nuovoPtr;
            nastro_ultimoPtr=nuovoPtr;
        }
    }
}

void stampa_nastro()
{
    NastroPtr currPtr1;
    NastroPtr currPtr2;

    currPtr1=nastro_primoPtr;
    printf("\nNastro:\n");
    while (currPtr1!=NULL)
    {
        currPtr2=currPtr1->downPtr_coda;
        printf("%c(%d) ", currPtr1->carattere, currPtr1->utilizzatori);
        while(currPtr2!=NULL)
        {
            printf("%c(%d) ", currPtr2->carattere, currPtr2->utilizzatori);
            currPtr2=currPtr2->downPtr_coda;
        }
        printf("\n");
        currPtr1=currPtr1->nextPtr;
    }
    return;
}

void svuota_nastro()
{
    if (nastro_primoPtr==NULL)
    {
        return;
    }
    else
    {
        while (nastro_ultimoPtr!=nastro_primoPtr)
        {
            svuota_coda();
            nastro_ultimoPtr=nastro_ultimoPtr->prevPtr;
            free(nastro_ultimoPtr->nextPtr);
        }
        free(nastro_ultimoPtr);
        nastro_primoPtr=NULL;
        nastro_ultimoPtr=NULL;
    }
    return;
}

void svuota_coda()
{
    NastroPtr tempPtr1;
    NastroPtr tempPtr2;
    if(nastro_ultimoPtr->downPtr_coda==NULL){return;}
    tempPtr1=nastro_ultimoPtr->downPtr_coda;
    if (tempPtr1->downPtr_coda==NULL)
    {
        free(tempPtr1);
        return;
    }
    tempPtr2=tempPtr1->downPtr_coda;
    while (tempPtr1!=NULL)
    {
        free(tempPtr1);
        tempPtr1=tempPtr2;
        if(tempPtr1!=NULL)
        {
            tempPtr2=tempPtr2->downPtr_coda;
        }
    }
    return;
}

void inizializza_coda_stati_livello()
{
    stati_livello_primoPtr=malloc(sizeof (Coda_stati_livello));
    if (stati_livello_primoPtr==NULL){printf("ERRORE: malloc\n");}
    else
    {
        stati_livello_primoPtr->stato=0;
        stati_livello_primoPtr->testina_curr=nastro_primoPtr;
        stati_livello_primoPtr->nextPtr=malloc(sizeof (Coda_stati_livello));
        if (stati_livello_primoPtr->nextPtr==NULL){printf("ERRORE: malloc\n");}
        else
        {
            stati_livello_primoPtr->nextPtr->stato=99999;
            stati_livello_primoPtr->nextPtr->testina_curr=NULL;
            stati_livello_primoPtr->nextPtr->nextPtr=NULL;
            stati_livello_ultimoPtr=stati_livello_primoPtr->nextPtr;
        }
    }
    return;
}

void svuota_coda_stati_livello()
{
    Coda_stati_livelloPtr tempPtr;
    if(stati_livello_primoPtr==NULL)
    {
        return;
    }
    while (stati_livello_primoPtr!=stati_livello_ultimoPtr)
    {
        tempPtr=stati_livello_primoPtr;
        stati_livello_primoPtr=stati_livello_primoPtr->nextPtr;
        free(tempPtr);
    }
    free(stati_livello_primoPtr);
    stati_livello_primoPtr=NULL;
    stati_livello_ultimoPtr=NULL;
    return;
}

void stampa_coda_stati_livello ()
{
    Coda_stati_livelloPtr tempPtr;
    tempPtr=stati_livello_primoPtr;
    printf("\ncoda degli stati: ");
    while (tempPtr!=NULL)
    {
        printf("%d ", tempPtr->stato);
        tempPtr=tempPtr->nextPtr;
    }
    if (tempPtr==NULL)
    {
        printf("NULL");
    }
    return;
}

int simulazione()
{

    Lista_transizioniPtr transizione_curr;
    NastroPtr nastro_curr;
    NastroPtr RLSmovimentoPtr;
    Coda_stati_livelloPtr tempPtr;
    Coda_stati_livelloPtr nuovoPtr;
    int stati_da_controllare;
    int accettare=0;
    int trovata_transizione=0;
    Lista_stati_di_accettazionePtr PUNT;

    max_ripetizioni_mosse=0;
    profondita=0;
    while(profondita<=MAX)
    {
        stati_da_controllare=1;
//        printf("\n\nINIZIO RICERCA IN PROFONDITA': %d\n", profondita);

        stato_in_analisi=stati_livello_primoPtr->stato;
//        printf("\nSTATO IN ANALISI: %d", stato_in_analisi);
        carattere_in_analisi=stati_livello_primoPtr->testina_curr->carattere;
//        printf("\nCARATTERE IN ANALISI: %c", carattere_in_analisi);

        while (stato_in_analisi!=99999)
        {
            //inizio ciclo in cui scorro tutte le transizioni e trovo quelle da fare
            transizione_curr=testa_lista_transizioni;
            trovata_transizione=0;
            while(transizione_curr!=NULL)
            {
                //se trovo una transizione da fare
                if((transizione_curr->carattereLETTO==carattere_in_analisi)&&(transizione_curr->statoIN==stato_in_analisi))
                {
                    trovata_transizione=1;
//                    printf("\neseguo transizione: %d %c %c %c %d",transizione_curr->statoIN,transizione_curr->carattereLETTO,transizione_curr->carattereSCRITTO,transizione_curr->movimento,transizione_curr->statoOUT);
                    enqueue_nastro(stati_livello_primoPtr->testina_curr, transizione_curr->carattereSCRITTO, 1);

                    nastro_curr=nastro_primoPtr;
                    while(nastro_curr!=NULL)
                    {
                        if (nastro_curr!=stati_livello_primoPtr->testina_curr)
                        {
                            enqueue_nastro(nastro_curr, nastro_curr->carattere, 1);
                        }
                        nastro_curr=nastro_curr->nextPtr;

                    }
                    nuovoPtr=malloc(sizeof(Coda_stati_livello));
                    if (nuovoPtr==NULL){printf("ERRORE: malloc");}
                    else
                    {
                        nuovoPtr->stato=transizione_curr->statoOUT;
                        if (transizione_curr->movimento=='R')
                        {
                            if(stati_livello_primoPtr->testina_curr->nextPtr==NULL)
                            {
//                                stampa_nastro();
                                prolunga_nastro_destra();
//                                stampa_nastro();
                            }
                            RLSmovimentoPtr=stati_livello_primoPtr->testina_curr->nextPtr;
                            if (RLSmovimentoPtr->downPtr_coda==NULL)
                            {
                                nuovoPtr->testina_curr=RLSmovimentoPtr;
                            }
                            else
                            {
                                while (RLSmovimentoPtr->downPtr_coda!=NULL)
                                {
                                    RLSmovimentoPtr=RLSmovimentoPtr->downPtr_coda;
                                }
                                nuovoPtr->testina_curr=RLSmovimentoPtr;
                            }
                        }
                        if (transizione_curr->movimento=='L') //DA MODIFICARE COME IL CASO R
                        {
                            if(stati_livello_primoPtr->testina_curr->prevPtr==NULL)
                            {
//                                stampa_nastro();
                                prolunga_nastro_sinistra();
//                                stampa_nastro();

                            }
                            RLSmovimentoPtr=stati_livello_primoPtr->testina_curr->prevPtr;
                            if (RLSmovimentoPtr->downPtr_coda==NULL)
                            {
                                nuovoPtr->testina_curr=RLSmovimentoPtr;
                            }
                            else
                            {
                                while (RLSmovimentoPtr->downPtr_coda!=NULL)
                                {
                                    RLSmovimentoPtr=RLSmovimentoPtr->downPtr_coda;
                                }
                                nuovoPtr->testina_curr=RLSmovimentoPtr;
                            }
                        }
                        if((transizione_curr->movimento!='R')&&(transizione_curr->movimento!='L'))
                        {
                            RLSmovimentoPtr=stati_livello_primoPtr->testina_curr;
                            while (RLSmovimentoPtr->downPtr_coda!=NULL)
                            {
                                RLSmovimentoPtr=RLSmovimentoPtr->downPtr_coda;
                            }
                            nuovoPtr->testina_curr=RLSmovimentoPtr;
                        }
                        nuovoPtr->nextPtr=NULL;
                        stati_livello_ultimoPtr->nextPtr=nuovoPtr;
                        stati_livello_ultimoPtr=nuovoPtr;
                    }
                }
                transizione_curr=transizione_curr->nextPtr;

            }
//            printf("\ntutte le transizioni sono state controllate.");

            //la variabile trovata_transizione mi sta dicendo se ho trovato almento una transizione
            if (trovata_transizione==0)
            {
                //controllo se � in accettazione
                PUNT=testa_lista_stati_di_accettazione;
                while (PUNT!=NULL)
                {
                    if(PUNT->stato_di_accettazione==stati_livello_primoPtr->stato)
                    {
                        //se � in accettazione, accetto e chiudo
                        return 1;
                    }
                    PUNT=PUNT->nextPtr;
                }

                //se non � in accettazione divido in due casi:

                //caso in cui quello dopo � uguale a 99999
                if((stati_livello_primoPtr->nextPtr->stato==99999)&&(stati_livello_primoPtr->nextPtr->nextPtr==NULL))
                {
                    return 0;
                }

                //caso in cui quello dopo non � 99999
                //continuo senza far nulla.
            }


            /*
            if (trovata_transizione==0)
            {
                PUNT=testa_lista_stati_di_accettazione;
                while (PUNT!=NULL)
                {
                    if (stato_in_analisi==PUNT->stato_di_accettazione)
                    {
                        return 1;
                    }
                    PUNT=PUNT->nextPtr;
                }
            }


            stati_da_controllare=controllo_elementi_coda_stati_livelli();
            if (stati_da_controllare==0)//CASO IN CUI CI SIA IN CODA SOLAMENTE (STATO)-->(99999)-->NULL
                                        //smettere di fare ricerca, prendere la prossima stringa
            {
                accettare=controlla_accettazione();
                if (accettare==1)
                {
//                    printf("\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n///////////////////////////////////STRINGA ACCETTATA///////////////////////////////////\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n");
                    return 1;
                }
                else
                {
//                    printf("\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n///////////////////////////////////STRINGA RIFIUTATA///////////////////////////////////\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n");
                    return 0;
                }
            }*/


//            stampa_coda_stati_livello();
            tempPtr=stati_livello_primoPtr; // elimino lo stato dalla lista degli stati, perch� ho guardato tutte le transizioni che dovevo
            stati_livello_primoPtr=stati_livello_primoPtr->nextPtr;
            free(tempPtr);
//            printf("\nho eliminato il primo elemento (stato: %d) dalla lista degli stati.", stato_in_analisi);
//            stampa_coda_stati_livello();

//            stampa_nastro();
            ricompatto();
//            stampa_nastro();

            nastro_curr=nastro_primoPtr;
            while(nastro_curr!=NULL)
            {
//                printf("\ndecremento utilizzatore");
                nastro_curr->utilizzatori--;
                if (nastro_curr->utilizzatori==0)
                {
//                    printf("\nutilizzatore a ZERO");
                    if(nastro_curr->nextPtr!=NULL)
                    {
                        nastro_curr=nastro_curr->nextPtr;//vado avanti di uno per evitare che cancelli quello in cui sono con dequeue e perda il puntatore
                        dequeue_nastro(nastro_curr->prevPtr);//dequeuo quello prima perch� sono andato  avanti di uno
                        nastro_curr=nastro_curr->prevPtr; //ritorno indietro di uno su quello nuovo e continuo il while normalmente
                    }
                    else
                    {
                        nastro_curr->downPtr_coda->nextPtr=NULL;
                        nastro_curr->downPtr_coda->prevPtr=nastro_curr->prevPtr;
                        nastro_curr->prevPtr->nextPtr=nastro_curr->downPtr_coda;
                        nastro_ultimoPtr=nastro_curr->downPtr_coda;
                        free(nastro_curr);
                        nastro_curr=NULL;
//                        printf("->elemento eliminato");
                        break;
                    }
//                    printf("->elemento eliminato");

                }
                nastro_curr=nastro_curr->nextPtr;
            }
//            printf("\nho decrementato tutti gli utilizzatori della prima riga per lo stato: %d", stato_in_analisi);
//            stampa_nastro();
            stato_in_analisi=stati_livello_primoPtr->stato;
//            printf("\nSTATO IN ANALISI: %d", stato_in_analisi);
            if (stato_in_analisi==99999)
            {
//                stampa_coda_stati_livello();
                break;
            }
            carattere_in_analisi=stati_livello_primoPtr->testina_curr->carattere;
//            printf("\nCARATTERE IN ANALISI: %c", carattere_in_analisi);
        }
        if (stato_in_analisi==99999)
        {
            tempPtr=stati_livello_primoPtr; // dequeuo lo stato 99999
            stati_livello_primoPtr=stati_livello_primoPtr->nextPtr;
            free(tempPtr);
            tempPtr=malloc(sizeof(Coda_stati_livello));
            if(tempPtr==NULL){printf("\nERRORE: malloc.\n");}
            else
            {
                tempPtr->stato=99999;
                tempPtr->nextPtr=NULL;
                stati_livello_ultimoPtr->nextPtr=tempPtr;
                stati_livello_ultimoPtr=tempPtr;
            }
//            stampa_coda_stati_livello();
        }
//        printf("\nPROFONDITA' RAGGIUNTA: ");
//        printf("%d\n",profondita);
//        stampa_coda_stati_livello();
        max_ripetizioni_mosse=max_ripetizioni_mosse+(salva_stati());
//        printf("\nmosse ripetute uguali: %d", max_ripetizioni_mosse);
        if(max_ripetizioni_mosse>1000)
        {
            return 2;
        }
//        stampa_nastro();
//        printf("\n\n");

        profondita++;
    }

    if (profondita> MAX)//capire se 0 1 U
    {
        /*
        transizione_trovata=0;
//        printf("PROFONDITA' MAX RAGGIUNTA (siamo a profondita': %d)", profondita-1);
//        stampa_coda_stati_livello();
//        stampa_nastro();

        //inizio ciclo in cui scorro tutte le transizioni e trovo quelle da fare

        tempPtr=stati_livello_primoPtr;
        while (tempPtr!=NULL)
        {
            if(tempPtr->stato==99999)
            {
                printf("ERRORE??"); //??
                break;
            }
            carattere_in_analisi=tempPtr->testina_curr->carattere;
            stato_in_analisi=tempPtr->stato;
            transizione_curr=testa_lista_transizioni;
            while(transizione_curr!=NULL)
            {
                //se trovo una transizione da fare
                if((transizione_curr->carattereLETTO==carattere_in_analisi)&&(transizione_curr->statoIN==stato_in_analisi))
                {
                    transizione_trovata =1;
                    break;
                }
                transizione_curr=transizione_curr->nextPtr;
            }
            if (transizione_trovata==1)
            {
                break;
            }
            tempPtr=tempPtr->nextPtr;
        }
        if (transizione_trovata==0)
        {
            //devo verificare l'accettazione o meno.
            accettare=controlla_accettazione();
            if (accettare==1)
            {
//                printf("\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n///////////////////////////////////STRINGA ACCETTATA///////////////////////////////////\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n");
                return 1;
            }
            else
            {
//                printf("\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n///////////////////////////////////STRINGA RIFIUTATA///////////////////////////////////\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n");
                return 0;
            }

        }
        else
        {
//            printf("\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n///////////////////////////////////STRINGA NON TERMINATA: U///////////////////////////////////\n\n\n\n\n/////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n\n\n");
            */
            return 2;
            /*
        }*/

    }
    return 33;
}

void enqueue_nastro(NastroPtr testa_coda, char carattere, int utilizzatore)
{
    NastroPtr nuovoPtr;
    NastroPtr ricercatorePtr;
    nuovoPtr=malloc(sizeof(Nastro));
    if (nuovoPtr==NULL){printf("\nERRORE: malloc.\n");}
    else
    {
        nuovoPtr->carattere=carattere;
        nuovoPtr->utilizzatori=utilizzatore;
        nuovoPtr->nextPtr=NULL;
        nuovoPtr->prevPtr=NULL;
        nuovoPtr->downPtr_coda=NULL;
        ricercatorePtr=testa_coda;
        while (ricercatorePtr->downPtr_coda!=NULL)
        {
            ricercatorePtr=ricercatorePtr->downPtr_coda;
        }//ricercatorePtr punta all'ultimo elemento della coda
        ricercatorePtr->downPtr_coda=nuovoPtr;
    }
    return;
}

void dequeue_nastro(NastroPtr testa_coda)
{
    if (testa_coda==NULL){printf("NON PENSO SI VERIFICHER� MAI QUESTO CASO, SE MI LEGGI VAI A CONTROLLARE IL CODICE");return;} //ANCHE QUESTO CASO NON SO SE VERIFICHER� MAI
    if(testa_coda->nextPtr!=NULL)
    {
////        if(testa_coda->downPtr_coda!=NULL)//DOVREBBE ESSERLO PER FORZA IMMAGINO
////        {
////            printf("\nESISTOOOOOOOOOOOOOOOOO.111");
            testa_coda->downPtr_coda->nextPtr=testa_coda->nextPtr;
            testa_coda->nextPtr->prevPtr=testa_coda->downPtr_coda;
////            printf("\nESISTOOOOOOOOOOOOOOOOO.111");
////        }
    }
    else
    {
////        printf("\nESISTOOOOOOOOOOOOOOOOO.222");
        testa_coda->downPtr_coda->nextPtr=NULL;
    }
    if (testa_coda->prevPtr!=NULL)
    {
////        if(testa_coda->downPtr_coda!=NULL)//DOVREBBE ESSERLO PER FORZA IMMAGINO
////        {
////            printf("\nESISTOOOOOOOOOOOOOOOOO.333");
            testa_coda->downPtr_coda->prevPtr=testa_coda->prevPtr;
            testa_coda->prevPtr->nextPtr=testa_coda->downPtr_coda;
////            printf("\nESISTOOOOOOOOOOOOOOOOO.333");
////        }
    }
    else
    {
////        printf("\nESISTOOOOOOOOOOOOOOOOO.444");
        testa_coda->downPtr_coda->prevPtr=NULL;
////        printf("\nESISTOOOOOOOOOOOOOOOOO.444");

    }
    if (testa_coda==nastro_primoPtr)
    {
////        printf("\nESISTOOOOOOOOOOOOOOOOO.555.primoelemento");
        nastro_primoPtr=testa_coda->downPtr_coda;
////        printf("\nESISTOOOOOOOOOOOOOOOOO.555.primoelemento");

    }
    if (testa_coda==nastro_ultimoPtr)
    {
////        printf("\nESISTOOOOOOOOOOOOOOOOO.666.ultimoelemento");
        nastro_ultimoPtr=testa_coda->downPtr_coda;
////        printf("\nESISTOOOOOOOOOOOOOOOOO.666.ultimoelemento");
    }

    free(testa_coda);

    return;
}

void ricompatto()
{
    NastroPtr currPtr_prima_riga;
    NastroPtr currPtr_scorri_coda1;
    NastroPtr currPtr_scorri_coda2;
    Coda_stati_livelloPtr tempPtr;
    char carattere1;
    char carattere2;

//    printf("\nRICOMPATTANDO");

    if(nastro_primoPtr==NULL)
    {
//        printf("\nnastro vuoto, non ricompatto.\n");//NON DOVREBBE MAI ESSERLO
        return;
    }
    currPtr_prima_riga=nastro_primoPtr;
    while(currPtr_prima_riga!=NULL)
    {
        currPtr_scorri_coda1=currPtr_prima_riga;
        while (currPtr_scorri_coda1!=NULL)
        {
            carattere1=currPtr_scorri_coda1->carattere;
            if (currPtr_scorri_coda1->downPtr_coda!=NULL)
            {
                currPtr_scorri_coda2=currPtr_scorri_coda1->downPtr_coda;
                carattere2=currPtr_scorri_coda2->carattere;
                while(carattere1==carattere2)
                {

                    //controlla tutti gli stati, se una testina � uguale a currPtr_scorri_coda2 sposta la testina su currPtr_scorri_coda1 e continua
                    tempPtr=stati_livello_primoPtr->nextPtr;
                    while (tempPtr!=NULL)
                    {
                        if(tempPtr->stato!=99999)
                        {
                            if(tempPtr->testina_curr==currPtr_scorri_coda2)
                            {
                                tempPtr->testina_curr=currPtr_scorri_coda1;
//                                printf("\ncorreggo testina degli stati prima di compattare.");
                            }
                        }
                        tempPtr=tempPtr->nextPtr;
                    }

                    unisci(currPtr_scorri_coda1, currPtr_scorri_coda2);
                    if (currPtr_scorri_coda1->downPtr_coda!=NULL)
                    {
                        currPtr_scorri_coda2=currPtr_scorri_coda1->downPtr_coda;
                        carattere2=currPtr_scorri_coda2->carattere;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            currPtr_scorri_coda1=currPtr_scorri_coda1->downPtr_coda;
        }
        currPtr_prima_riga=currPtr_prima_riga->nextPtr;
    }
    return;
}

void unisci(NastroPtr Ptr1, NastroPtr Ptr2)
{
    Ptr1->utilizzatori=Ptr1->utilizzatori+Ptr2->utilizzatori;
    Ptr1->downPtr_coda=Ptr2->downPtr_coda;
    free(Ptr2);
    return;
}

void prolunga_nastro_destra()
{
    NastroPtr nuovoPtr;
    int utilizzatori=0;
    NastroPtr scorritorePtr;

//    printf("\nlimite del nastro raggiunto, prolungo...");
    scorritorePtr=nastro_primoPtr;
    while (scorritorePtr!=NULL)
    {
        utilizzatori=utilizzatori+scorritorePtr->utilizzatori;
        scorritorePtr=scorritorePtr->downPtr_coda;
    }
    nuovoPtr=malloc(sizeof(Nastro));
    if (nuovoPtr==NULL){printf("\nERRORE:malloc");}
    else
    {
        nuovoPtr->utilizzatori=utilizzatori;
        nuovoPtr->carattere='_';
        nuovoPtr->downPtr_coda=NULL;
        nuovoPtr->nextPtr=NULL;
        nuovoPtr->prevPtr=nastro_ultimoPtr;
        nastro_ultimoPtr->nextPtr=nuovoPtr;
        nastro_ultimoPtr=nuovoPtr;
    }
    return;
}

void prolunga_nastro_sinistra()
{
    NastroPtr nuovoPtr;
    int utilizzatori=0;
    NastroPtr scorritorePtr;

//    printf("\nlimite del nastro raggiunto, prolungo...");
    scorritorePtr=nastro_primoPtr;
    while (scorritorePtr!=NULL)
    {
        utilizzatori=utilizzatori+scorritorePtr->utilizzatori;
        scorritorePtr=scorritorePtr->downPtr_coda;
    }
    nuovoPtr=malloc(sizeof(Nastro));
    if (nuovoPtr==NULL){printf("\nERRORE:malloc");}
    else
    {
        nuovoPtr->utilizzatori=utilizzatori;
        nuovoPtr->carattere='_';
        nuovoPtr->downPtr_coda=NULL;
        nuovoPtr->nextPtr=nastro_primoPtr;
        nuovoPtr->prevPtr=NULL;
        nastro_primoPtr->prevPtr=nuovoPtr;
        nastro_primoPtr=nuovoPtr;
    }
    return;
}

int controllo_elementi_coda_stati_livelli()
{
    if(stati_livello_primoPtr->nextPtr==stati_livello_ultimoPtr)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int salva_stati()
{
    Coda_stati_livelloPtr tempPtr;
    int i=0;
    int cambiatostato=0;//parto dal pressupposto che non sia cambiata
    int cambiatocarattere=0;

    tempPtr=stati_livello_primoPtr;

    while(tempPtr!=NULL)
    {
        if(lista_stati_da_controllare[i]!=tempPtr->stato)
        {
            cambiatostato=1;
        }
        lista_stati_da_controllare[i]=tempPtr->stato;
        if (tempPtr->stato!=99999)
        {
            if(lista_testine_stati_da_controllare[i]!=tempPtr->testina_curr->carattere)
            {
                cambiatocarattere=1;
            }
            lista_testine_stati_da_controllare[i]=tempPtr->testina_curr->carattere;
        }
        else {lista_testine_stati_da_controllare[i]='_';}
        tempPtr=tempPtr->nextPtr;
//        printf("\nsalvo gli stati");

        if (i>300)
        {
            printf("\nERRORE: l'array degli stati da copiare � al massimo lungo 300. gli stati da copiare sono di pi�");
            break;
        }
        i++;
    }
//    printf("\n stati salvati");
    if(cambiatostato==0)
    {
        return 1;
    }
    else
    {
        max_ripetizioni_mosse=0;
        return 0;
    }
}

void inizializza_liste_salvati()
{
    int i=0;
    while (i<299)
    {
        lista_stati_da_controllare[i]=0;
        lista_testine_stati_da_controllare[i]='o';
        i++;
    }
}

int controlla_accettazione()
{
    int i=0;
    Lista_stati_di_accettazionePtr tempPtr;

    tempPtr=testa_lista_stati_di_accettazione;
    while (tempPtr!=NULL)
    {
        i=0;
        while (lista_stati_da_controllare[i]!=99999)
        {
            if (tempPtr->stato_di_accettazione==lista_stati_da_controllare[i])
            {
                return 1; //ACCETTO
            }
            i++;
        }
        tempPtr=tempPtr->nextPtr;
    }
    return 0; //NON ACCETTO
}
