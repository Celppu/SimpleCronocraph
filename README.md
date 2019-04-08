# SimpleCronocraph
Simple arduino chronograph for air guns


Code is trash but should be easy to modify

I'm running adc in "free runnging" mode. Code is build on example from arduino forum.
link -


My code has big problems with stability and calibration. raja (limit) variables are value for to trigger port. 
I have done for "secondtry" some interesting automatic calibrations, but it still isn't good enough.
I recommend to build op-amp amplifier for signal, because fast projectile wont make huge change in
voltage...
