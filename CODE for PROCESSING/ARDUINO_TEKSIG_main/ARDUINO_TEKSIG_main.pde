/* TEKSIG - the visualisation program
 
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
 
 
 ///// ///// ///// ///// ///// ///// /////
 
 A small note for the processing-program. This version makes a considerable use of global variables. This is mainly due to the programmers background with assembler.
 In "microprocessor-reality", every variable and memory location is always accessible.
 
 However, feel free to improve it! In fact, we believe that the smart textile community would greatly improve from open tools.
 */

//if you want to see how the visualisation looks with random data, set "enable_fake_data" to 1.
int enable_fake_data=1;
long fakesamplemillis=0;


// imports, serial and decimal format
import processing.serial.*;
import java.text.DecimalFormat;


// set Serial port and data communication
Serial myPort;
int inchar;      // incoming characters from the serial port
float first_received=0.0;
float second_received=0.0;
float third_received=0.0;
int reading=0;


// formatting and display
int decimalpoint=0;
PFont f; // setting font
color ampcolour =color(0, 0, 255);        // amplitude colour
color widcolour =color(255, 0, 255);      // width colour    
color delcolour =color(0, 255, 0);        // delay colour    


// ellipse coordinates
int ellx=350;  //1550
int elly=250;   //700

// text coordinates
int xorig = 890; 
int yorig = 140;  
int resultspacing=-70;
int heightspacing=50;


// for displaying values
int amplitudevalue;
float delayvalue=20.0;
float widthvalue=30.0;
float widthvalueadj=0.0;
int samplespersec=0;


// variables for calculating amount of samples per second
int amount_of_samples=0;
long oldmillis=0;


// index for drawing history values, draws by rolling index, rather than values
int drawind=0;
// data structures for storing old data, to show data history
float [] ddelay= new float[200];
float [] dwida= new float[200];
float [] damp= new float[200];


// for calculating averages (vectors are probably too large for practice, since default is 9)
float[]sdelayvalue=new float[200];
float[]swidthvalue=new float[200];
float[]swidthvalueadj=new float[200];
int[]samplitudevalue=new int[200];
int avg_index=0;  //index, moving average
int MAXAVGS=4;    //MAXIMUM-AVERAGE-SAMPLES! MINIMUM IS ONE 1 (YOU CAN PLAY WITH THIS ONE, TO SEE HOW MUCH NOISE THERE IS)


/*****************************************************************************/
/*****************************************************************************/

void setup() 
{
  // set display size
  size(1040, 720);

  // create font (assumes such font is available in the system)
  // if not available, then processing will automatically select something else
  f = createFont("SansSerif", 30);
  textFont(f);

  // Visualisation program needs to know where your arduino is connected
  // if the program crashes during the first run, change the number in the
  // line:   String portName = Serial.list()[ this number here! ]; 
  // to match your arduino. You should see it on the console after the crash.
  //
  printArray(Serial.list());
  if ( enable_fake_data==0)
  {
    String portName = Serial.list()[0];
    myPort = new Serial(this, portName, 115200);
  }//
  stroke(0);

  // This is just protection, clean exit instead of crashing when dividing by 0.
  if (MAXAVGS==0)
  {
    println("\nNo. Minimum value for averages is 1. Please set MAXAVGS to something other than 0. :)");  
    exit();
  }
}

/*****************************************************************************/
/*****************************************************************************/
// Main drawing loop.
// Checks incoming data, and parses it. Parses data as it arrives.

void draw()
{


  // The serial-receiver routine is written to deal with the data as soon as it arrives, rather than reading full packets.
  // Thus, it looks a bit crude. This is especially evident from the way floating point numbers are handled. 
  // (a leftover from the olden days of writing assembler-code, don't judge us. :) )

  // data format: S[amplitude]s[delay]l[width]E

  // purge serial-buffer as much as there is incoming data
  if ( enable_fake_data==0)
  {
    while ( myPort.available() > 0) 
    {//
      inchar = myPort.read();         // read incoming character, store to inchar

      // S, s and l set the 'reading' to collect incoming numbers to specific variables.
      // E stops this, and begins processing the variables to useable values.
      // reading -variable contains 'state machine' control, so that each character is read correctly.  


      // first part manages the letters, second part manages the reading-states.

      // if the character is 'S' -> start reading
      if ( inchar == 'S')
      {
        // start reading
        reading=1;
      }//
      else if ( inchar == 's')
      {
        reading=2;
      }//
      else if ( inchar == 'l')
      {
        if ( decimalpoint >0)
        {
          second_received = second_received / 100;
        }
        decimalpoint=0;
        reading=3;
      }//
      else if ( inchar == 'E')
      {
        if ( decimalpoint >0)
        {
          third_received = third_received / 100;
        }
        decimalpoint=0;

        reading=0; // reset the 'state machine'

        // set the most recent values from received numbers
        delayvalue=second_received;
        widthvalue=third_received;
        amplitudevalue=(int)first_received;

        // use the values to calculate the width/amplitude, only if amplitude is >0
        widthvalueadj=0;
        if ( amplitudevalue>0) widthvalueadj = widthvalue/amplitudevalue;

        // add the values to the averaging buffers
        sdelayvalue[avg_index]=delayvalue;
        swidthvalue[avg_index]=widthvalue;
        swidthvalueadj[avg_index]=widthvalueadj;
        samplitudevalue[avg_index]=amplitudevalue;
        // increase averaging index
        avg_index++;

        // clear averaging index, if it is equal or over the maximum average values
        if ( avg_index>=MAXAVGS)avg_index=0;

        // calculate the averages (one could probably calculate the width/amp based on these, and not calculate it separately)
        // however, it is for future use, where the shape is analysed in more detail. thus, it's good to know how much time it takes here. 
        float temp_s=0.0;
        float temp_l=0.0;
        float temp_la=0.0;
        float temp_n=0;
        for (int d=0; d<MAXAVGS; d++)
        {
          temp_s+=sdelayvalue[d];
          temp_l+=swidthvalue[d];
          temp_la+=swidthvalueadj[d];
          temp_n+=samplitudevalue[d];
        }

        delayvalue=temp_s/MAXAVGS;
        widthvalue=temp_l/MAXAVGS;
        widthvalueadj=temp_la/MAXAVGS;
        amplitudevalue=(int)temp_n/MAXAVGS;


        // add the values to the history-drawing buffer
        ddelay[drawind]=delayvalue;
        damp[drawind]=temp_n/MAXAVGS;
        dwida[drawind]=widthvalue;
        drawind++;

        // if the historybuffer runs over -> reset to zero
        if ( drawind >199)
        {
          drawind=0;
        }

        // Clear out the inputs for the next time 
        first_received=0;
        second_received=0;
        third_received=0;

        // make note that we added a sample
        amount_of_samples++;
      }//
      //////////////////////////////////////////////////////////////////////////////
      // collect numbers specific to the correct variable
      else if (reading == 1)
      {
        if ( inchar >= '0' && inchar <= '9')
        {
          first_received=first_received*10;
          first_received+=(inchar-0x30);    // prune ASCII
        }
      }//
      else if (reading == 2)
      {
        if ( inchar >= '0' && inchar <= '9')
        {
          second_received=second_received*10;
          second_received+=(inchar-0x30);    // prune ASCII
        }//
        else if ( inchar == '.')
        {
          decimalpoint++;
        }
      }//
      else if (reading == 3)
      {
        if ( inchar >= '0' && inchar <= '9')
        {
          third_received=third_received*10;
          third_received+=(inchar-0x30);    // prune ASCII
        }//
        else if ( inchar == '.')
        {
          decimalpoint++;
        }
      }
    }//
  }

  // Calculate the amount of samples per second, once per second
  if (millis()-oldmillis>1000)
  {
    samplespersec=amount_of_samples;
    amount_of_samples=0;
    oldmillis=millis();
  }


  if ((enable_fake_data==1) && (millis()-fakesamplemillis>32))
  {
    decimalpoint=0;

    first_received = (int)random(1, 11);
    second_received = random(1, 30);
    third_received = random(1, 30.4);


    delayvalue=second_received;
    widthvalue=third_received;

    reading=0;
    amplitudevalue=(int)first_received;
    widthvalueadj=0;
    if ( amplitudevalue>0) widthvalueadj = widthvalue/amplitudevalue;


    sdelayvalue[avg_index]=delayvalue;
    swidthvalue[avg_index]=widthvalue;
    swidthvalueadj[avg_index]=widthvalueadj;
    samplitudevalue[avg_index]=amplitudevalue;
    avg_index++;
    if ( avg_index>=MAXAVGS)avg_index=0;

    float temp_s=0.0;
    float temp_l=0.0;
    float temp_la=0.0;
    float temp_n=0;
    for (int d=0; d<MAXAVGS; d++)
    {
      temp_s+=sdelayvalue[d];
      temp_l+=swidthvalue[d];
      temp_la+=swidthvalueadj[d];
      temp_n+=samplitudevalue[d];
    }
    delayvalue=temp_s/MAXAVGS;
    widthvalue=temp_l/MAXAVGS;
    widthvalueadj=temp_la/MAXAVGS;
    amplitudevalue=(int)temp_n/MAXAVGS;


    // add drawing table
    ddelay[drawind]=delayvalue;
    damp[drawind]=temp_n/MAXAVGS;
    dwida[drawind]=widthvalue;
    drawind++;
    if ( drawind >199)
    {
      drawind=0;
    }

    // zero out values for next round
    first_received=0;
    second_received=0;
    third_received=0;

    // add one to the sample counter
    amount_of_samples++;
    fakesamplemillis=millis();
  }


  // draw the image
  draw_image();


  stroke(0, 0, 0);
  fill(0, 0, 0);

  // use DecimalFormat to limit the displayed amplitudevalue after decimal point.
  DecimalFormat formatter = new DecimalFormat("0.0");


  // Display values at the right-hand side of the figure.

  // Amplitude
  fill(ampcolour);    
  text("A", xorig+0, yorig+0);
  text(amplitudevalue, xorig+resultspacing, yorig+0);

  // Delay
  fill(delcolour);    
  text("Dph", xorig, yorig+heightspacing*1);
  text(formatter.format(delayvalue), xorig+resultspacing, yorig+heightspacing*1);

  // Width
  fill(widcolour);    
  text("W", xorig, yorig+heightspacing*2);
  text(formatter.format(widthvalue), xorig+resultspacing, yorig+heightspacing*2);

  // Amplitude / width
  fill(0, 0, 0);
  text("W/A", xorig, yorig+heightspacing*3);
  text(formatter.format(widthvalueadj), xorig+resultspacing, yorig+heightspacing*3);

  // Samples per second
  text("smpl/sec", xorig, yorig+heightspacing*4+50);
  text(samplespersec, xorig+resultspacing, yorig+heightspacing*4+50);
}

// not really used in this version
void keyPressed()
{
  if ( key=='1')
  {
    println("reserved 1");
  }//
  else if ( key=='2')
  {
    println("reserved 2");
  }//
}
