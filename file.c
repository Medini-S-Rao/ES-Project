#include<LPC17xx.h>

int irIP, lane1, lane2, lane3, lane4;
int red = 4, yellow = 2, green = 1;
int light1 = 4, light2 = 4, light3 = 4, light4 = 4;
int lights[4] = {1, 1, 4, 4};
int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
int x[4], y[4];
int i;
int k;

void Timer(int pr) 
{
    // Set Timer0 clock
	  //LPC_TIM0->TCR = 0x02;  // Reset Timer
  //  LPC_TIM0->PR = pr; // PCLK = 299 MHz
    //LPC_TIM0->MR0 = 999999;
    //LPC_TIM0->EMR =0x80; // Enable toggle on match 0
   // LPC_TIM0->MCR = 0x20;
	//	LPC_TIM0->CTCR=0;
   // LPC_TIM0->TCR = 0x01; // Enable Timer
   // while ((LPC_TIM0->EMR && 0X2) ==0); 
   // LPC_TIM0->TCR = 0x00; // Disable Timer
		for(k=0; k<pr*300000; k++);
}

void readlanes(void)
{
	// Read input from sensors into variables x0-x7
    irIP = ((LPC_GPIO1->FIOPIN >> 23) & 0XF) | ((LPC_GPIO2->FIOPIN >> 6) & 0XF0);
    x[0] = irIP & 0x3;
    x[1] = (irIP >> 2) & 0x3;
    x[2] = (irIP >> 4) & 0x3;
    x[3] = (irIP >> 6) & 0x3;

    // Delay for 8 seconds
    Timer(8);

    // Read input from sensors into variables y0-y7
    irIP = ((LPC_GPIO1->FIOPIN >> 23) & 0XF) | ((LPC_GPIO2->FIOPIN >> 6) & 0XF0);
    y[0] = irIP & 0x3;
    y[1] = (irIP >> 2) & 0x3;
    y[2] = (irIP >>4) & 0x3;
    y[3] = (irIP >> 6) & 0x3;

    // Check if both x and y variables are 1 and store in traffic_stat array
    // Determine the traffic status for the specified lane
    lane1 = x[0] & y[0];
    lane2 = x[1] & y[1];
    lane3 = x[2] & y[2];
    lane4 = x[3] & y[3];
	
}

void setlight(int lane, int light) {
    switch(lane) {
        case 1 :
            LPC_GPIO0->FIOCLR = 1 << 4 | 1 << 5 | 1 << 6;
												Timer(8);
            LPC_GPIO0->FIOSET = light << 4;
												Timer(8);
            light1 = light;
            lights[0] = light;
            break;
        case 2:
            LPC_GPIO0->FIOCLR = 1 << 7 | 1 << 8 | 1 << 9;
													Timer(8);
            LPC_GPIO0->FIOSET = light << 7;
													Timer(8);
            light2 = light;
            lights[1] = light;
            break;
        case 3:
            LPC_GPIO0->FIOCLR = 1 << 15 | 1 << 16 | 1 << 17;
													Timer(8);
            LPC_GPIO0->FIOSET = light << 23;
													Timer(8);
            light3 = light;
            lights[2] = light;
            break;
        case 4:
            LPC_GPIO0->FIOCLR = 1 << 18 | 1 << 19 | 1 << 20;
													Timer(8);
            LPC_GPIO0->FIOSET = light << 18;
													Timer(8);
            light4 = light;
            lights[3] = light;
            break;
    }
}

void switchlights() {
		if(light1==green && light2==green)
		{
			setlight(1, yellow);
			setlight(2, yellow);
			Timer(8);
			
			setlight(1, red);
			setlight(2, red);
			Timer(8);
			
			setlight(3, green);
			setlight(4,green);
			Timer(8);
		}
		else if(light3==green && light4==green)
		{
			setlight(3, yellow);
			setlight(4, yellow);
			Timer(8);
			
			setlight(3, red);
			setlight(4, red);
			Timer(8);
			
			setlight(1, green);
			setlight(2,green);
			Timer(8);
		}
		
}

void readlaness() {
    irIP = ((LPC_GPIO1->FIOPIN >> 23) & 0XF) | ((LPC_GPIO2->FIOPIN >> 6) & 0XF0);
    lane1 = irIP & 0x3;
    lane2 = (irIP >> 2) & 0x3;
    lane3 = (irIP >> 4) & 0x3;
    lane4 = (irIP >> 6) & 0x3;
}

int main() {
				int j;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL3 = 0; //P1.23 - P1.26 SENSOR I/P GPIO
    LPC_PINCON->PINSEL4 = 0; //P2.10 - P2.13 SENSOR I/P GPIO
    LPC_PINCON->PINSEL0 = 0; //P0.4 - P0.9, P0.15 LED O/P GPIO
    LPC_PINCON->PINSEL1 = 0; //P0.16-P0.20 LED O/P GPIO

    LPC_GPIO1->FIODIR = 0; //P1.23 - P1.26 I/P SENSOR
    LPC_GPIO2->FIODIR = 0; //P2.10 - P2.13 I/P SENSOR
    LPC_GPIO0->FIODIR = 0X3F << 4 | 0X1F8 << 15 | 0x7 <<23; // P0.4 - P0.9, P0.15-P0.20 O/P LED

	
				for(j=1; j<5; j++)
				{
					 setlight(j, red);
				}
				Timer(8);
				
    while(1) {
       // readlane1();
        //readlane2();
        //readlane3();
        //readlane4();
				readlanes();
        if (lane1 == lane2 && lane2 == lane3 && lane3 == lane4) {
            // All lanes have the same value, implementing round robin
            switchlights();
					//	Timer(8);
					
            switchlights();
						//Timer(8);
        }
        else if (lane1 == 3 || lane2 == 3) {
            // If lane 1 or lane 2 has high traffic
            // Set lane 3 and lane 4 to red, lane 1 and lane 2 to green
            if(light3 == green && light4 == green) {
                setlight(3, yellow);
                setlight(4, yellow);
								Timer(8);
            }
            setlight(3, red);
            setlight(4, red);
            setlight(1, green);
            setlight(2, green);
						Timer(8);
        }
        else if (lane3 == 3 || lane4 == 3) {
            // If lane 3 or lane 4 has high traffic
            // Set lane 1 and lane 2 to red, lane 3 and lane 4 to green
            if(light1 == green && light2 == green) {
                setlight(1, yellow);
                setlight(2, yellow);
								Timer(8);
            }
            setlight(1, red);
            setlight(2, red);
            setlight(3, green);
            setlight(4, green);
						Timer(8);
        }
        else if(lane1 == 1 || lane2 == 1) {
            // If lane 1 or lane 2 has low traffic
            // Set lane 3 and lane 4 to red, lane 1 and lane 2 to green
            if(light3 == green && light4 == green) {
                setlight(3, yellow);
                setlight(4, yellow);
								Timer(8);
            }
            
                setlight(3, red);
                setlight(4, red);
            
            setlight(1, green);
            setlight(2, green);
						Timer(8);
        }
        else if(lane3 == 1 || lane4 == 1) {
            // If lane 3 or lane 4 has low traffic
            // Set lane 1 and lane 2 to red, lane 3 and lane 4 to green
            if(light1 == green && light2 == green) {
                setlight(1, yellow);
                setlight(2, yellow);
																Timer(8);
            }
           
                setlight(2, red);
                setlight(1, red);
            
            setlight(3, green);
            setlight(4, green);
						Timer(8);
        }
    }
}
