//------------------------------
//file name:   interrupts_timers.c
//description: defines the interrupt timers for the project
//author:      henry lindbo 10/12/18
//------------------------------

#include "macros.h"
#include "init.h"
#include "msp430.h"
#include "functions.h"
#include "ports.h"
#include "timers.h"

unsigned int count;
unsigned int count1;
unsigned int count2;
volatile unsigned int state_mchn_state;
volatile unsigned int new_state;
volatile unsigned int done;
volatile unsigned int total_time_running;//time running in ms
volatile unsigned int max_state_mchn_state;

#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_A0_ISR(void){
  //timer A2 interrupt handler
  
  //implementation here:
  
  count2++;
  if(count2 >= 10){//triggers every half second
    count2 = 0;
    //P1OUT ^= GRN_LED;
    new_state = 1;
    state_mchn_state++;
    if(state_mchn_state >= max_state_mchn_state){//2 seconds
      state_mchn_state = 0;
    }
  }
  
  
  //TA0CCTL0 |= CCIE;
  
  
  TA2CCR0 += TA2CCR0_INTERVAL; //add offset to TACCR0
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void){
  //timer A1 interrupt handler
  
  //implementation here:
  //update_display = 0;
  //display_changed = 0;
  P5IFG &= ~BUTTON1;
  P5IFG &= ~BUTTON2;
  
  TA1CCTL0 &= ~CCIE; 
  P5IE |= BUTTON1;
  P5IE |= BUTTON2;
  P1OUT |= RED_LED;
  
  button_debounce = RESET_STATE;
  //TA0CCTL0 |= CCIE;
  
  
  TA1CCR0 += TA1CCR0_INTERVAL; //add offset to TACCR0
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void){
  //timer A0 interrupt handler
  
  //implementation here:
  //update_display = 0;
  //display_changed = 0;
  count++;
  
  if(count >= TENTIMES){
    /*
    if(button_pressed == 2){
    P5OUT &= ~LCD_BACKLIGHT;
  }else{
    P5OUT ^= LCD_BACKLIGHT;
  }
    */
    
    P1OUT ^= GRN_LED;
    update_display = ALWAYS;
    display_changed = ALWAYS;
    total_time_running += 2;//add 200 ms every time the display updates
    count = RESET_STATE;
    //button_pressed = RESET_STATE;
  }
  
  
  TA0CCR0 += TA0CCR0_INTERVAL; //add offset to TACCR0
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
  //timer A0 1-2, overflow interrupt vector (TAIV) handler
  
  switch(__even_in_range(TA0IV,14)){
  case 0: break;
  
  case 2:
    //implementation
    TA0CCR1 += TA0CCR1_INTERVAL;
    break;
    
  case 4:
    //implementation
    TA0CCR1 += TA0CCR1_INTERVAL;
    break;
    
  case 14:
    //implementation
    TA0CCR1 += TA0CCR1_INTERVAL;
    break;
    
  default:
    break;   
    
  }
}