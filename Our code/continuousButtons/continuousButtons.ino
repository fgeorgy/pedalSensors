// Licensed under a Creative Commons Attribution 3.0 Unported License.
// Based on rcarduino.blogspot.com previous work.
// www.electrosmash.com/pedalshield

// Code pour tester les neauveaux potentiometres
 
int in_ADC0, in_ADC1;  //variables for 2 ADCs values (ADC0, ADC1)
int POT0, POT1, POT2, out_DAC0, out_DAC1; //variables for 3 pots (ADC8, ADC9, ADC10)
int LED = 3;
int FOOTSWITCH = 7; 
int TOGGLE = 2; 

int MAXPOT = 5;

// Ici on teste avec un pot.

int MEMORY = 0; // Nombre des tours qu'on a fait depuis le debut.
int MEMORYPOTMOD = 0; // Valeur precedent de POTMOD
int POTMOD = 0; //Valeur de la sortieddd

int Pot2Limit = 10;

void setup()
{
  //ADC Configuration
  ADC->ADC_MR |= 0x80;   // DAC in free running mode.
  ADC->ADC_CR=2;         // Starts ADC conversion.
  ADC->ADC_CHER=0x1CC0;  // Enable ADC channels 0 and 1.  
 
  //DAC Configuration
  analogWrite(DAC0,0);  // Enables DAC0
  analogWrite(DAC1,0);  // Enables DAC0
}
 
void loop()
{
  //Read the ADCs
  while((ADC->ADC_ISR & 0x1CC0)!=0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
  in_ADC0=ADC->ADC_CDR[7];               // read data from ADC0
  in_ADC1=ADC->ADC_CDR[6];               // read data from ADC1
  
  POT0=ADC->ADC_CDR[10];                 // read data from ADC10
  POT1=ADC->ADC_CDR[11];                 // read data from ADC11  
  POTMOD=ADC->ADC_CDR[12];                 // read data from ADC12 POTMOD c'est pour la lecture, POT2 va controler le son.
  

  
  
  
  if ((POTMOD – MEMORYPOTMOD < -(9/10)*MAXPOT) && (POT2 < Pot2Limit)) {
    MEMORY=MEMORY+1;
   }
  else if ((POTMOD – MEMORYPOTMOD > (9/10)*MAXPOT) && (POT2 > 0)) {
   MEMORY = MEMORY -1 ;
  }

   MEMORYPOTMOD = POTMOD
   
   POT2 = MAXPOT*MEMORY + POTMOD //+PotSensor

   if (POT2 > Pot2Limit) {
   POT2 = Pot2Limit;
  }
  
  if (POT2 < 0) {
    POT2 = 0;
  }
   //TODO : Add a second pot with the sensors
     
  //Add volume feature with POT2
  out_DAC0=map(in_ADC0,0,4095,1,POT2);
  out_DAC1=map(in_ADC1,0,4095,1,POT2);
   
  //Write the DACs
  dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
  dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
  dacc_set_channel_selection(DACC_INTERFACE, 1);       //select DAC channel 1
  dacc_write_conversion_data(DACC_INTERFACE, out_DAC1);//write on DAC
}


//a=ADC->ADC_CDR[12];

//memory = 0

//if (pot_modulo(t)-pot_modulo(t-1)) > ?
//	then memory = memory + 1
//elif (pot_modulo(t)-pot_modulo(t-1)) < ?
//	then memory = memory -1
//POT2 = memory * ? + pot_modulo(t)

