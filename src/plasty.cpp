#include <iostream>
#include "simlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define DAYSINYEAR 365
#define YEARINCREASENOTSET -4000
int totalTons= 0;
int reusedTons = 0;
int recycledTons = 0;
int wasteTons = 0;
int incineratedTons = 0;
int decomposedTons = 0;
int Rreused = 0;
int Rincinerated = 0;
int Rwasted = 0;
double delay;
double recChange = 0;
double recSucChange = 0;
double RR,RS;                   // % Plastov recyklovanych a uspesnost recyklacie
int Y;                          // Roky simulacie
double yearlyIncrease = 1.0;

class Plastic : public Process {
    void Behavior() {
        double random = Random();
        bool wasted = false;
        totalTons++;
        if(random <= 0.3) {   // Plast bol znovapozity
            reusedTons++;
        } else if((random > 0.3) && (random <= (0.86 - recChange))) {   // Plast skoncil na skladke
            wasteTons++;
            wasted = true;
        } else if((random > (0.86 - recChange)) && (random <= 0.92)) {    // Plast bol recyklovany
            recycledTons++;
            double recycledRandom = Random();
            if(recycledRandom <= 0.2) {     // Plast bol spaleny
                incineratedTons++;
                Rincinerated++;
            }else if((recycledRandom > 0.2) && (recycledRandom <= (0.4 + recSucChange))) {  // Plast bol znovupouzity
                reusedTons++;
                Rreused++;
            }else if((recycledRandom > (0.4 + recSucChange)) && (recycledRandom <= 1.0)) {  // Plast skoncil na skladke
                wasteTons++;
                Rwasted++;
                wasted = true;
            }
        } else if((random > 0.92) && (random <= 1.0)) {      // Plast bol spaleny
            incineratedTons++;
        }
        if(wasted){
            double decomposeRandom = Random();
            if(decomposeRandom <= 0.05) {           // Ostatny plast
                Wait(Uniform(10*DAYSINYEAR,12*DAYSINYEAR));
            } else if((decomposeRandom > 0.05) && (decomposeRandom <= 0.74)) {  // PET flase,slamky
                Wait(Exponential(450*DAYSINYEAR));
            } else if((decomposeRandom > 0.74) && (decomposeRandom <= 0.9)) {   // Tasky, vrecia ...
                Wait(Uniform(200*DAYSINYEAR,1000*DAYSINYEAR));
            } else if((decomposeRandom > 0.9) && (decomposeRandom <= 1.0)) {    // Obaly na jedlo
                Wait(Uniform(50*DAYSINYEAR,80*DAYSINYEAR));
            }
            decomposedTons++;
        }
    }
};

class Production : public Event {
public:

    void Behavior() {
        (new Plastic)->Activate();
        Activate( Time + delay);
    }
};

void Experiment(){
    double startingProduction = 359.0;      // Produkcia plastov v roku 2018
    for(int i =0; i < Y;i++){               // Y - Pocet rokov simulacie
        Init(0,DAYSINYEAR);
        delay = DAYSINYEAR/startingProduction;
        (new Production)->Activate();
        Run();
        startingProduction *= yearlyIncrease;       // Rocne zvisenie produkcie
    }

    int totalWordWaste = 6300;
    std::cout << "===================================================" << std::endl;
    std::cout << "== Recycling Rate :\t\t" << RR <<"%" <<std::endl;
    std::cout << "== Recycling Succes :\t\t" << RS <<"%"<< std::endl;
    std::cout << "== Years simulated :\t\t" << Y << std::endl;
    std::cout << "== Yearly de/increase in production :\t" << ((yearlyIncrease -1)*100) <<"%"<< std::endl;
    std::cout << "===================================================" << std::endl;

    std::cout << "Total Mtons produced:\t" <<totalTons << "\t" << (double )(totalTons/((double )totalTons/100)) << "%"<< std::endl;
    std::cout << "Reused Mtons\t\t" << reusedTons << "\t" << (double )(reusedTons/((double )totalTons/100)) << "%"<< std::endl;
    std::cout << "Waste Mtons:\t\t" << wasteTons << "\t" << (double )(wasteTons/((double )totalTons/100)) << "%"<<"\tof that:" <<std::endl;
    std::cout << "\tDecomposed:\t" << decomposedTons<< "\t"<< std::endl;

    totalWordWaste += wasteTons;
    totalWordWaste -= decomposedTons;
    std::cout << "Incinerated Mtons:\t" << incineratedTons << "\t" << (double )(incineratedTons/((double )totalTons/100)) << "%" << std::endl;
    std::cout << "Recycled Mtons:\t\t" << recycledTons<< "\t(" << (double )(recycledTons/((double )totalTons/100)) << "%)" << "\tof that:" << std::endl;
    std::cout << "\tReused :\t" << Rreused<< "\t" << std::endl;
    std::cout << "\tIncinerated :\t" << Rincinerated<< "\t" << std::endl;
    std::cout << "\tWasted :\t" << Rwasted<< "\t" << std::endl;
    std::cout << "Total world waste\t"  << totalWordWaste << " milion tons\t" << std::endl;
    std::cout << "===================================================" << std::endl;
}

//  Praca s argumentami
class ArgumentParser{
public:
    char **programArguments;
    long int year = -1, recycleRate = -1, recycleSuccess = -1;
    double yearIncrease = YEARINCREASENOTSET;
    int parseArguments(int argc){
        if(argc == 2 && strcmp(programArguments[1],"--help")==0){
            std::cout << " Priklad spustenia: ./ims-projekt -r 10 -s 20 -y 5 -i 1.3 \n";
            std::cout << " -r\tPercento recyklovanych plastov (cele cislo)\n";
            std::cout << " -s\tPercentualna uspesnost recyklacie(kolko plastu sa skutocne znovupouzije) (cele cislo)\n";
            std::cout << " -y\tPocet rokov simulacie (cele cislo)\n";
            std::cout << " -i\tPercentualny narast produkcie plastov za rok (desatinne cislo)\n";
            return 2;
        }


        opterr = 0;
        int c;
        int index;
        char  *pEnd = NULL;
        while ((c = getopt (argc, programArguments, "r:s:y:i:")) != -1)
            switch (c)
            {
                case 'r':
                    recycleRate = strtol(optarg,NULL,10);
                    if(recycleRate > 62)        // Uvazujeme ze 30% plastu sa znovupouzije a 8 splali
                        recycleRate = 62;       // preto maximalne % recyklacie je 62
                    break;
                case 's':
                    recycleSuccess = strtol(optarg,NULL,10);
                    if(recycleSuccess > 80)     // Uvazujeme ze 20% sa vyuzije ako palivo preto max 80%
                        recycleSuccess = 80;
                    break;
                case 'y':
                    year = strtol(optarg,NULL,10);
                    break;
                case 'i':
                    yearIncrease = strtod(optarg,NULL);
                    break;
                case '?':
                    if (optopt == 'r')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (optopt == 's')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (optopt == 'y')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (optopt == 'i')
                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                    else if (isprint (optopt))
                        fprintf (stderr, "Unknown option `-%c'. use --help\n", optopt);
                    else
                        fprintf (stderr,
                                 "Unknown option character `\\x%x'.\n",
                                 optopt);
                    return 1;
                default:
                    abort ();
            }


        for (index = optind; index < argc; index++){
            std::cout << "Non-option argument " << programArguments[index] << '\n';
        }
        return 0;
    }
};

int main(int argc, char **argv)
{
    ArgumentParser argumentParser;
    argumentParser.programArguments = argv;
    int tmp =argumentParser.parseArguments(argc);
    if(tmp == 1)
        return 1;
    else if(tmp == 2)
        return 0;

    if(argumentParser.recycleRate == -1)
        RR = 6.0;
    else
        RR = (double )argumentParser.recycleRate;

    if(argumentParser.recycleSuccess == -1)
        RS = 20.0;
    else
        RS = (double )argumentParser.recycleSuccess;

    if(argumentParser.year == -1)
        Y = 10;
    else
        Y = argumentParser.year;

    double recyclationPercentage = RR / 100.0;
    recChange = recyclationPercentage - 0.06;

    double recyclationSuccessPercentage = RS /100.0;
    recSucChange = recyclationSuccessPercentage - 0.2;

    if(argumentParser.yearIncrease != YEARINCREASENOTSET)
        yearlyIncrease = 1.0 + (argumentParser.yearIncrease/100);

    Experiment();
}