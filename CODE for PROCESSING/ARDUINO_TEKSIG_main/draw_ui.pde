
void draw_image()
{
  background(255);             // Set background to white

  // Draw 'axes' for the ellipse centre
  stroke(50, 50, 50);
  line( ellx-220, elly, ellx+220, elly);
  line( ellx, elly-220, ellx, elly+220);


  /*************************************/
  /*************************************/
  // draw the visualisation.
  // not the most realistic reproduction, but should be fast enough for something
  // This should be improved on par with the resolution and precision of the hardware. :)


  // save display and do translation and rotation magic
  pushMatrix();
  translate(ellx, elly);
  strokeWeight(5.0);

  // black ellipse, no filling
  stroke(0, 0, 0);
  noFill();  


  // Bloatiness and other values used for the creation of the ellipse. 
  // In reality, these are influenced by the textile, the components, the conditions and the load resistor used.
  // As soon as the hardware is better, this should be improved to reflect that.
  // For now, it gives an approximation.
  //
  // Two Most important improvements to focus on 
  //  1) the semiconductive and non-linear behaviour.
  //  2) the hardware to have op-amps
  // Less important improvements:
  // adjust the length of the ellipse to match width the 'input width', as per in real lissajous


  float bloaty = 20.7-delayvalue; // bloaty represents the delay, i.e. how round the ellipse should be. Assumes wrongly that the 20.7 is the 0-value.

  // if amplitude is 0 -> no signal, reset visualisation  
  if ( amplitudevalue==0)bloaty=1;

  // if there is something larger, bloat and rotate
  if ( amplitudevalue>1)
  {
    rotate(PI-abs((float)amplitudevalue/16.4));

    ellipse(0, 0, 400, abs(bloaty/amplitudevalue)*15);
  }//
  else // nothing large, just rotate, no bloat...
  {
    rotate(PI);
    ellipse(0, 0, 400, 3);
  }

  // once everything is drawn, pop all back in place for the display
  popMatrix();


  // Draw 'axes' for the history values
  stroke(0);
  strokeWeight(1.0);
  line(60, height-50, 850, height-50);
  line(60, height-50, 60, height-140);


  // Draw history, note that each value has their own Y-axis -scale. (the multipliers, 2* and 8*)
  // drawind is used as the index for rolling the values, to create a moving history
  for (int g=drawind+1; g<199; g++)
  {
    // delay
    stroke(delcolour);    
    int tempx=g-drawind;
    line(tempx*4+60, height - 50 - ddelay[g-1], tempx*4+62, height - 50 -ddelay[g]);

    // width
    stroke(widcolour);    
    line(tempx*4+60, height - 50 -2*dwida[g-1], tempx*4+62, height - 50 -2*dwida[g]);

    // amplitude
    stroke(ampcolour);    
    line(tempx*4+60, height - 50 -8*damp[g-1], tempx*4+62, height - 50 -8*damp[g]);
  }
  //  
  for (int g=1; g<drawind; g++)
  {

    // delay
    int tempx=198+g-drawind;
    stroke(delcolour);    
    line(tempx*4+60, height - 50 - ddelay[g-1], tempx*4+62, height - 50 -ddelay[g]);

    // width
    stroke(widcolour);    
    line(tempx*4+60, height - 50 -2*dwida[g-1], tempx*4+62, height - 50 -2*dwida[g]);

    // amplitude
    stroke(ampcolour);    
    line(tempx*4+60, height - 50 -8*damp[g-1], tempx*4+62, height - 50 -8*damp[g]);
  }
}
