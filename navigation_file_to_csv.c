#include <stdint.h>
#include <stdio.h>
#include <string.h>
//defining the struct containing the data in one epoch 
typedef struct {
    double numsat;
    double day;
    double month;
    double hour;
    double minute;
    double sec;
    double mlsec;
    double a0;
    double a1;
    double a2;
    double iod;
    double crs;
    double delta_n;
    double M0;
    double Cuc;
    double e;
    double cus;
    double sqrtA;
    double toe;
    double cic;
    double OMEGA;
    double cis;
    double I;
    double Crc;
    double omega;
    double OMEGADOT;
    double isec;
    double L2;
    double GPSweek;
    double L2pCODE;
    double precision_sat;
    double state;
    double TGD;
    double IODC;
    double trans_time;
    double time_spare;
} __attribute__((__packed__)) navobs;
// data printing for checkups it was done during debugging but useful as a check up of how well the data is being parsed 
void print_data(navobs nav) {
    printf("numsat: %i\n", nav.numsat);
    printf("day: %i\n", nav.day);
    printf("month: %i\n", nav.month);
    printf("hour: %i\n", nav.hour);
    printf("minute: %i\n", nav.minute);
    printf("seconds: %i\n", nav.sec);
    printf("milsec: %lf\n", nav.mlsec);
    printf("A0: %.20lf\n", nav.a0);
    printf("A1: %.20lf\n", nav.a1);
    printf("A2: %.20lf\n", nav.a2);
    printf("iod: %.20lf\n", nav.iod);
    printf("crs: %.20lf\n", nav.crs);
    printf("delta_n: %.20lf\n", nav.delta_n);
    printf("M0: %.20lf\n", nav.M0);
    printf("Cuc: %.20lf\n", nav.Cuc);
    printf("e: %.20lf\n", nav.e);
    printf("cus: %.20lf\n", nav.cus);
    printf("sqrtA: %.20lf\n", nav.sqrtA);
    printf("toe: %.20lf\n", nav.toe);
    printf("cic: %.20lf\n", nav.cic);
    printf("OMEGA: %.20lf\n", nav.OMEGA);
    printf("cis: %.20lf\n", nav.cis);
    printf("I: %.20lf\n", nav.I);
    printf("Crc: %.20lf\n", nav.Crc);
    printf("omega: %.20lf\n", nav.omega);
    printf("OMEGADOT: %.20lf\n", nav.OMEGADOT);
    printf("isec: %.20lf\n", nav.isec);
    printf("L2: %.20lf\n", nav.L2);
    printf("GPSweek: %.20lf\n", nav.GPSweek);
    printf("L2pCODE: %.20lf\n", nav.L2pCODE);
    printf("precision_sat: %.20lf\n", nav.precision_sat);
    printf("state: %.20lf\n", nav.state);
    printf("TGD: %.20lf\n", nav.TGD);
    printf("IODC: %.20lf\n", nav.IODC);
    printf("trans_time: %.20lf\n", nav.trans_time);
    printf("time_spare: %.20lf\n", nav.time_spare);
}
int main() {
    FILE *input;
    input = fopen("rabt3500.23n", "rb"); //rinex navigation file 
    FILE* output ;
    output = fopen("output.txt" , "wb"); //modified rinex navfile , easy to parse to csv 
    if (input == NULL) {
        printf("could not open input file");
        return 1;
    }
      if (output == NULL) {
        printf("could not open output file ");
        fclose(output);
        return 1;
    }
    char buffer[100];
    //converting the rinex file to a readable format  , aka modified rinex navfile ,
    //extracting header file 
    while (fgets(buffer, 100, input)  != NULL) 
    {
        //printf("%s", buffer); printing the header  for debugging reasons 
        fwrite(buffer , strlen(buffer), 1 , output);
        if (strstr(buffer, "END OF HEADER") != NULL) 
        {
            break;
        }
    }
    //extracting data files 
    int c = 0 ; //counter for wich line we are in in raw data 
    while (fgets(buffer , 100 , input) != NULL)
    {
        //need to change D to e , for scientific numbers reading c can't understand D , the operation is done in each buffer containing the data 
        for (int i = 0 ;i < sizeof(buffer);i++){
            if (buffer[i] == 'D'){
                buffer[i] = 'e'; 
            } 
        } 
        c++;
        fwrite(buffer , strlen(buffer)-2, 1 , output);
        if (c%8 == 0 )
        {
        fputc('\n', output); //new line after determining one epoch 
        }
    }
    fclose(input);
    fclose(output);
    //pasing to csv 
    int number_of_epochs = c / 8; 
    navobs data[number_of_epochs];
    FILE *ninput = fopen("output.txt" , "r");//the converted rinex in our previous state a.k.a : modified rinex navfile , easy to parse to csv 
    FILE *csvoutput = fopen("csvoutput.txt" , "w");  // csv output of the data 
    char buffer1[600]; // 600 was the size of the buffer because the strlen(line_of_data) = 595 
    char headerbuffer[100]; //buffer for the header of the modified rinex 
    while (fgets(headerbuffer, 100, ninput)  != NULL) 
    {
        if (strstr(headerbuffer, "END OF HEADER") != NULL) 
        {
            break;
        }
    }
    // writing the first row of the csv data
    int line = 0 ;
    fprintf(csvoutput, "numsat;day;month;hour;minute;sec;mlsec;a0;a1;a2;iod;crs;delta_n;M0;Cuc;e;cus;sqrtA;toe;cic;OMEGA;cis;I;Crc;omega;OMEGADOT;isec;L2;GPSweek;L2pCODE;precision_sat;state;TGD;IODC;trans_time;time_spare\n");
    while (fgets(buffer1 , 600 , ninput)!= NULL){
        navobs myobs ; 
        //extracting data from the text and parsing it to the struct 
        sscanf(buffer1 ," %i %i %i %i %i %i %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf "
                    , &myobs.numsat, &myobs.day, &myobs.month, &myobs.hour, &myobs.minute, &myobs.sec , &myobs.mlsec , &myobs.a0,&myobs.a1
                    , &myobs.a2, &myobs.iod, &myobs.crs, &myobs.delta_n, &myobs.M0, &myobs.Cuc, &myobs.e, &myobs.cus, &myobs.sqrtA,
               &myobs.toe, &myobs.cic, &myobs.OMEGA, &myobs.cis, &myobs.I, &myobs.Crc, &myobs.omega, &myobs.OMEGADOT, &myobs.isec, &myobs.L2, &myobs.GPSweek,
               &myobs.L2pCODE, &myobs.precision_sat, &myobs.state, &myobs.TGD, &myobs.IODC, &myobs.trans_time, &myobs.time_spare); 
        // allocating the data to the list of struct navops for better control over the algorithm of coordinate determination 
        data[line] = myobs; 
        line ++ ; 
        //parsing the data to csv 
        fprintf(csvoutput ,  "%i;%i;%i;%i;%i;%i;%lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf;%.20lf\n"
               , myobs.numsat, myobs.day, myobs.month, myobs.hour, myobs.minute, myobs.sec , myobs.mlsec , myobs.a0,myobs.a1 , myobs.a2, myobs.iod, myobs.crs, myobs.delta_n, myobs.M0, myobs.Cuc, myobs.e, myobs.cus, myobs.sqrtA,
               myobs.toe, myobs.cic, myobs.OMEGA, myobs.cis, myobs.I, myobs.Crc, myobs.omega, myobs.OMEGADOT, myobs.isec, myobs.L2, myobs.GPSweek,
               myobs.L2pCODE, myobs.precision_sat, myobs.state, myobs.TGD, myobs.IODC, myobs.trans_time, myobs.time_spare);
    }
    fclose(ninput);
    fclose(csvoutput);
    // extracing coordinates using the algorithm  to be continued .....
    print_data(data[132]);//testing if the struct has stored the wanted values 
    return 0; 
}
