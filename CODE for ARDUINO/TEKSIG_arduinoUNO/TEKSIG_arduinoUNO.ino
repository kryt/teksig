/* TEKSIG for Arduino UNO.

  Please read the instructions from the pdf before going any further.


  Jussi Mikkonen and Riikka Townsend. 2019. Frequency-Based Design of Smart Textiles. In CHI Conference on Human Factors in Computing Systems Proceedings (CHI 2019), May 4–9, 2019, Glasgow, Scotland UK. ACM, New York, NY, USA, 12 pages. https://doi.org/ 10.1145/3290605.3300524

  Free to use for academic and educational purposes, not for commercial purposes. Use at your own risk, we take no liability of anything.


  We've set up www.teksig.net to collect and show examples of use from everyone!

  .    Please let us know if you make a major stable modification. We'd be happy to hear from you.
  .    Want to improve the hardware? Go ahead, and show us!
  .    Want to let us know what you did? We'll be happy to hear from you!
  .    Send us your improvements, textiles and we can also test them and compare findings.

  And if you make a TEKSIG-PCB-shield that can be ordered, let us know! :)

  Jussi & Riikka  jussi@teksig.net and riikka@teksig.net

*/

/*
  Note for the arduino-code.

  There's probably a lot that can be improved, the code is not pretty. :)

*/



// Arduino pins
int Ain0 = 6;
int Ain1 = 7;
int led0 = 10;
int led1 = 11;

// variables to help
int g = 0;
int j = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(Ain0, INPUT);
  pinMode(Ain1, INPUT);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);

  pinMode(12, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  PORTC = 0;

  ACSR = 0;
}

// vectors for storing comparison values, and also for making better analysis of the sine-distortion.
byte compvalues[48];
byte res[30];
byte point[30];
byte lev[30];
byte btemp;

void loop()
{
  int indx = 0;

  //direct port manipulation to generate sine-wave
  DDRB |= 0x10;

  g = 29;


  noInterrupts();

  //create signal and stabilise sine-wave before measuring (could probably be shorter)

  for ( int d = 0; d < 2763; d++)
  {
    PORTB |= 0x10;
    PORTC = g;

    compvalues[0] = ACSR;
    compvalues[1] = ACSR;
    compvalues[2] = ACSR;
    compvalues[3] = ACSR;
    compvalues[4] = ACSR;
    compvalues[5] = ACSR;
    compvalues[6] = ACSR;
    compvalues[7] = ACSR;
    compvalues[8] = ACSR;
    compvalues[9] = ACSR;
    compvalues[10] = ACSR;
    compvalues[11] = ACSR;
    compvalues[12] = ACSR;
    compvalues[13] = ACSR;
    compvalues[14] = ACSR;
    compvalues[15] = ACSR;
    compvalues[16] = ACSR;
    compvalues[17] = ACSR;
    compvalues[18] = ACSR;
    compvalues[19] = ACSR;
    compvalues[20] = ACSR;
    compvalues[21] = ACSR;
    compvalues[22] = ACSR;

    // padding for 100kHz, compensating the influence of the for-loop
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    PORTB &= ~0x10;
    asm("nop");
    asm("nop");

    compvalues[23] = ACSR;
    compvalues[24] = ACSR;
    compvalues[25] = ACSR;
    compvalues[26] = ACSR;
    compvalues[27] = ACSR;
    compvalues[28] = ACSR;
    compvalues[29] = ACSR;
    compvalues[30] = ACSR;
    compvalues[31] = ACSR;
    compvalues[32] = ACSR;
    compvalues[33] = ACSR;
    compvalues[34] = ACSR;
    compvalues[35] = ACSR;
    compvalues[36] = ACSR;
    compvalues[37] = ACSR;
    compvalues[38] = ACSR;
    compvalues[39] = ACSR;
    compvalues[40] = ACSR;
    compvalues[41] = ACSR;
    compvalues[42] = ACSR;
    compvalues[43] = ACSR;
    compvalues[44] = ACSR;
    compvalues[45] = ACSR;
    compvalues[46] = ACSR;
  }
  g = 0;
  for ( int d = 15; d < 63; d++)
  {
    PORTC = d;

    for ( int e = 0; e < 30; e++)
    {
      PORTB |= 0x10;

      compvalues[0] = ACSR;
      compvalues[1] = ACSR;
      compvalues[2] = ACSR;
      compvalues[3] = ACSR;
      compvalues[4] = ACSR;
      compvalues[5] = ACSR;
      compvalues[6] = ACSR;
      compvalues[7] = ACSR;
      compvalues[8] = ACSR;
      compvalues[9] = ACSR;
      compvalues[10] = ACSR;
      compvalues[11] = ACSR;
      compvalues[12] = ACSR;
      compvalues[13] = ACSR;
      compvalues[14] = ACSR;
      compvalues[15] = ACSR;
      compvalues[16] = ACSR;
      compvalues[17] = ACSR;
      compvalues[18] = ACSR;
      compvalues[19] = ACSR;
      compvalues[20] = ACSR;
      compvalues[21] = ACSR;
      compvalues[22] = ACSR;

      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");

      PORTB &= ~0x10;
      asm("nop");
      asm("nop");

      compvalues[23] = ACSR;
      compvalues[24] = ACSR;
      compvalues[25] = ACSR;
      compvalues[26] = ACSR;
      compvalues[27] = ACSR;
      compvalues[28] = ACSR;
      compvalues[29] = ACSR;
      compvalues[30] = ACSR;
      compvalues[31] = ACSR;
      compvalues[32] = ACSR;
      compvalues[33] = ACSR;
      compvalues[34] = ACSR;
      compvalues[35] = ACSR;
      compvalues[36] = ACSR;
      compvalues[37] = ACSR;
      compvalues[38] = ACSR;
      compvalues[39] = ACSR;
      compvalues[40] = ACSR;
      compvalues[41] = ACSR;
      compvalues[42] = ACSR;
      compvalues[43] = ACSR;
      compvalues[44] = ACSR;
      compvalues[45] = ACSR;
      compvalues[46] = ACSR;
    }

    PORTB |= 0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[0] = ACSR;
    compvalues[1] = ACSR;
    compvalues[2] = ACSR;
    compvalues[3] = ACSR;
    compvalues[4] = ACSR;
    compvalues[5] = ACSR;
    compvalues[6] = ACSR;
    compvalues[7] = ACSR;
    compvalues[8] = ACSR;
    compvalues[9] = ACSR;
    compvalues[10] = ACSR;
    compvalues[11] = ACSR;
    compvalues[12] = ACSR;
    compvalues[13] = ACSR;
    compvalues[14] = ACSR;
    compvalues[15] = ACSR;
    compvalues[16] = ACSR;
    compvalues[17] = ACSR;
    compvalues[18] = ACSR;
    compvalues[19] = ACSR;
    compvalues[20] = ACSR;
    compvalues[21] = ACSR;
    compvalues[22] = ACSR;
    compvalues[23] = ACSR;


    PORTB &= ~0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[24] = ACSR;
    compvalues[25] = ACSR;
    compvalues[26] = ACSR;
    compvalues[27] = ACSR;
    compvalues[28] = ACSR;
    compvalues[29] = ACSR;
    compvalues[30] = ACSR;
    compvalues[31] = ACSR;
    compvalues[32] = ACSR;
    compvalues[33] = ACSR;
    compvalues[34] = ACSR;
    compvalues[35] = ACSR;
    compvalues[36] = ACSR;
    compvalues[37] = ACSR;
    compvalues[38] = ACSR;
    compvalues[39] = ACSR;
    compvalues[40] = ACSR;
    compvalues[41] = ACSR;
    compvalues[42] = ACSR;
    compvalues[43] = ACSR;
    compvalues[44] = ACSR;
    compvalues[45] = ACSR;
    compvalues[46] = ACSR;
    compvalues[47] = ACSR;

    PORTB |= 0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[0] = ACSR;
    compvalues[1] = ACSR;
    compvalues[2] = ACSR;
    compvalues[3] = ACSR;
    compvalues[4] = ACSR;
    compvalues[5] = ACSR;
    compvalues[6] = ACSR;
    compvalues[7] = ACSR;
    compvalues[8] = ACSR;
    compvalues[9] = ACSR;
    compvalues[10] = ACSR;
    compvalues[11] = ACSR;
    compvalues[12] = ACSR;
    compvalues[13] = ACSR;
    compvalues[14] = ACSR;
    compvalues[15] = ACSR;
    compvalues[16] = ACSR;
    compvalues[17] = ACSR;
    compvalues[18] = ACSR;
    compvalues[19] = ACSR;
    compvalues[20] = ACSR;
    compvalues[21] = ACSR;
    compvalues[22] = ACSR;
    compvalues[23] = ACSR;


    PORTB &= ~0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[24] = ACSR;
    compvalues[25] = ACSR;
    compvalues[26] = ACSR;
    compvalues[27] = ACSR;
    compvalues[28] = ACSR;
    compvalues[29] = ACSR;
    compvalues[30] = ACSR;
    compvalues[31] = ACSR;
    compvalues[32] = ACSR;
    compvalues[33] = ACSR;
    compvalues[34] = ACSR;
    compvalues[35] = ACSR;
    compvalues[36] = ACSR;
    compvalues[37] = ACSR;
    compvalues[38] = ACSR;
    compvalues[39] = ACSR;
    compvalues[40] = ACSR;
    compvalues[41] = ACSR;
    compvalues[42] = ACSR;
    compvalues[43] = ACSR;
    compvalues[44] = ACSR;
    compvalues[45] = ACSR;
    compvalues[46] = ACSR;
    compvalues[47] = ACSR;

    PORTB |= 0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[0] = ACSR;
    compvalues[1] = ACSR;
    compvalues[2] = ACSR;
    compvalues[3] = ACSR;
    compvalues[4] = ACSR;
    compvalues[5] = ACSR;
    compvalues[6] = ACSR;
    compvalues[7] = ACSR;
    compvalues[8] = ACSR;
    compvalues[9] = ACSR;
    compvalues[10] = ACSR;
    compvalues[11] = ACSR;
    compvalues[12] = ACSR;
    compvalues[13] = ACSR;
    compvalues[14] = ACSR;
    compvalues[15] = ACSR;
    compvalues[16] = ACSR;
    compvalues[17] = ACSR;
    compvalues[18] = ACSR;
    compvalues[19] = ACSR;
    compvalues[20] = ACSR;
    compvalues[21] = ACSR;
    compvalues[22] = ACSR;
    compvalues[23] = ACSR;


    PORTB &= ~0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[24] = ACSR;
    compvalues[25] = ACSR;
    compvalues[26] = ACSR;
    compvalues[27] = ACSR;
    compvalues[28] = ACSR;
    compvalues[29] = ACSR;
    compvalues[30] = ACSR;
    compvalues[31] = ACSR;
    compvalues[32] = ACSR;
    compvalues[33] = ACSR;
    compvalues[34] = ACSR;
    compvalues[35] = ACSR;
    compvalues[36] = ACSR;
    compvalues[37] = ACSR;
    compvalues[38] = ACSR;
    compvalues[39] = ACSR;
    compvalues[40] = ACSR;
    compvalues[41] = ACSR;
    compvalues[42] = ACSR;
    compvalues[43] = ACSR;
    compvalues[44] = ACSR;
    compvalues[45] = ACSR;
    compvalues[46] = ACSR;
    compvalues[47] = ACSR;

    // calculate slice begins here
    PORTB |= 0x10;

    // slice calculation
    int firstslice = 0;
    for ( int t = 0; t < 48; t++)
    {
      if ( (compvalues[t]  & 0x20) == 0 )
      {
        j++;
      }
      else
      {
        g++;
      }
      // check which value is the first -> it is used for calculating width
      if ( firstslice == 0)
      {
        if ( j > 0)
        {
          firstslice = t;
        }
      }
    }


    if ( g == 0 )
    {
      indx = 0;
    }
    else
    {
      indx++;
    }

    if ( indx != 0 )
    {
      res[indx - 1] = g;
      point[indx - 1] = d;
      lev[indx - 1] = firstslice;
    }

    if ( g == 48)
    {
      break;
    }

    j = 0;
    g = 0;



    PORTB |= 0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[0] = ACSR;
    compvalues[1] = ACSR;
    compvalues[2] = ACSR;
    compvalues[3] = ACSR;
    compvalues[4] = ACSR;
    compvalues[5] = ACSR;
    compvalues[6] = ACSR;
    compvalues[7] = ACSR;
    compvalues[8] = ACSR;
    compvalues[9] = ACSR;
    compvalues[10] = ACSR;
    compvalues[11] = ACSR;
    compvalues[12] = ACSR;
    compvalues[13] = ACSR;
    compvalues[14] = ACSR;
    compvalues[15] = ACSR;
    compvalues[16] = ACSR;
    compvalues[17] = ACSR;
    compvalues[18] = ACSR;
    compvalues[19] = ACSR;
    compvalues[20] = ACSR;
    compvalues[21] = ACSR;
    compvalues[22] = ACSR;
    compvalues[23] = ACSR;


    PORTB &= ~0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[24] = ACSR;
    compvalues[25] = ACSR;
    compvalues[26] = ACSR;
    compvalues[27] = ACSR;
    compvalues[28] = ACSR;
    compvalues[29] = ACSR;
    compvalues[30] = ACSR;
    compvalues[31] = ACSR;
    compvalues[32] = ACSR;
    compvalues[33] = ACSR;
    compvalues[34] = ACSR;
    compvalues[35] = ACSR;
    compvalues[36] = ACSR;
    compvalues[37] = ACSR;
    compvalues[38] = ACSR;
    compvalues[39] = ACSR;
    compvalues[40] = ACSR;
    compvalues[41] = ACSR;
    compvalues[42] = ACSR;
    compvalues[43] = ACSR;
    compvalues[44] = ACSR;
    compvalues[45] = ACSR;
    compvalues[46] = ACSR;
    compvalues[47] = ACSR;
    PORTB |= 0x10;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    compvalues[0] = ACSR;
    compvalues[1] = ACSR;
    compvalues[2] = ACSR;
    compvalues[3] = ACSR;
    compvalues[4] = ACSR;
    compvalues[5] = ACSR;
    compvalues[6] = ACSR;
    compvalues[7] = ACSR;
    compvalues[8] = ACSR;
    compvalues[9] = ACSR;
    compvalues[10] = ACSR;
    compvalues[11] = ACSR;
    compvalues[12] = ACSR;
    compvalues[13] = ACSR;
    compvalues[14] = ACSR;
    compvalues[15] = ACSR;
    compvalues[16] = ACSR;
    compvalues[17] = ACSR;
    compvalues[18] = ACSR;
    compvalues[19] = ACSR;
    compvalues[20] = ACSR;
    compvalues[21] = ACSR;
    compvalues[22] = ACSR;
    compvalues[23] = ACSR;


    PORTB &= ~0x10;
    asm("nop");
    asm("nop");

    compvalues[23] = ACSR;
    compvalues[24] = ACSR;
    compvalues[25] = ACSR;
    compvalues[26] = ACSR;
    compvalues[27] = ACSR;
    compvalues[28] = ACSR;
    compvalues[29] = ACSR;
    compvalues[30] = ACSR;
    compvalues[31] = ACSR;
    compvalues[32] = ACSR;
    compvalues[33] = ACSR;
    compvalues[34] = ACSR;
    compvalues[35] = ACSR;
    compvalues[36] = ACSR;
    compvalues[37] = ACSR;
    compvalues[38] = ACSR;
    compvalues[39] = ACSR;
    compvalues[40] = ACSR;
    compvalues[41] = ACSR;
    compvalues[42] = ACSR;
    compvalues[43] = ACSR;
    compvalues[44] = ACSR;
    compvalues[45] = ACSR;
    compvalues[46] = ACSR;
  }


  DDRB &= ~0x10;


  interrupts();

  // what follows can probably be massively improved, however, it might best be improved on par with the hardware.


  //repurpose g and j for estimating shape
  // this is one point for improvement -> to calc better shape
  g = 0;
  j = 0;


  // Improve trig calc here.

  // calc shape avg and values which are >0
  while ( point[g] > 0)
  {
    j += res[g];

    g++;
  }

  j = j / g;
  // j = avg
  // g = amount


  // fix res error
  btemp = lev[31];
  lev[31] = lev[32];
  lev[32] = btemp;

  btemp = res[31];
  res[31] = res[32];
  res[32] = btemp;

  // calculate values to be sent, zero them first
  float startdelayvalue = 0.0;
  float widthvalue = 0.0;
  int samples = 0;

  for ( int y = 0; y < (g - 1); y++)
  {
    if ( lev[y] > 2)
    {
      startdelayvalue += (float)lev[y]; // lev == level i.e. starting point
      widthvalue += (float)res[y]; // res == leveys

      samples++;
    }
  }

  // calculate averages
  if ( samples > 0)
  {
    startdelayvalue /= (samples );
    widthvalue /= (samples );
  }
  else
  {
    startdelayvalue = 0.0;
    widthvalue = 0.0;
  }

  // send packet
  Serial.print("S");
  Serial.print(samples);
  Serial.print("s");
  Serial.print(startdelayvalue);
  Serial.print("l");
  Serial.print(widthvalue);
  Serial.print("E");

  // clear out stuff
  for ( int fff = 0; fff < 20; fff++)
  {
    point[fff] = 0;
    res[fff] = 0;
  }


}
