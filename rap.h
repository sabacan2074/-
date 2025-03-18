const int GREEN = 2;
const int BLACK = 0;
const int WHITE = 1;

void setR(int power)
{
	setMotorSpeed(right,power);
}

void setL(int power)
{
	setMotorSpeed(left,power);
}

void setB(int power)
{
	setMotorSpeed(back,power);
}

void drive(int power)
{
	setR(power);
	setL(power);
	setB(power);
}
int getCRLR()
{
	return getColorReflected(RTcolor);
}

int getCRLL()
{
	return getColorReflected(LTcolor);
}

int getCRNR()
{
	return getColorName(RTcolor);
}

int getCRNL()
{
	return getColorName(LTcolor);
}

int getCRNF()
{
	return getColorName(flont);
}

int getCRLF()
{
	return getColorReflected(flont);
}

void turnR(int angle,int powor)
{
	int turnAngle = ((2*WIDTH)/(HEIGHT))*angle;
	resetMotorEncoder(right);
	moveMotorTarget(right, turnAngle, angle > 0 ? power : -power);
	waitUntilMotorStop(right);
}

void turnL(int angle,int powor)
{
	int turnAngle = ((2*WIDTH)/(HEIGHT))*angle;
	resetMotorEncoder(left);
	moveMotorTarget(left, turnAngle, angle > 0 ? power : -power);
	waitUntilMotorStop(left);
}

void turn2(int angle,int powor)
{
	resetmotorEncoder(right);
	resetMotorEncoder(left);
	float waa =((2 * WIDTH)/HEIGHT)*angle*2.7/2;
	moveMotorTarget(right, waa, angle*2.7 > 0 ? powor : -power);
	moveMotorTarget(left, waa, angle*2.7 > 0 ? - powor : powor);
	waitUntilMotorStop(right);
	waitUntilMotorStop(left);
}

void senti(float nann,int b)
{
	float waa;
	resetMotorEncoder(right);
	resetMotorEncoder(left);
	waa = (nann / HEIGHT) * 360;
	moveMotorTarget(right, waa, b);
	moveMotorTarget(left, waa, b);
	moveMotorTarget(back, waa, b);
	waitUntilMotorStop(right);
	waitUntilMotorStop(left);
	waitUntilMotorStop(back);
}

void touch()
{
	int a,b;
		{
	 	  		senti(18,-50);
	 	  		turn2(90,-40);
	 	  		senti(18,50);
	 	  		while(1)
	 	  		{
	 	  			setR(60);
	 	  			setL(28);
	 	  			setB(55);
	 	  			displayTextLine(7,"%d",a=getCRLR());
	 	  			displayTextLine(8,"%d",b=getCRLL());
	 	  			if(a<=20||b<=20)
	 	  			{
	 	  				senti(2,40);
	 	  					turn2(45,-20);
	 	  				while(1){
	 	  					displayTextLine(7,"%d",a=getCRLR());
	 	  					displayTextLine(8,"%d",b=getCRLL());
	 	  					turn2(10,-20);
	 	  					if(b<=20)
	 	  					{
	 	  						turn2(55,60);
	 	  						break;
	 	  					}

	 	  								}
	 	  								break;

	 	  			}
	 	  			}
	 	  	}
}

void boost()
	{
		senti(1,-60);
		drive(200);
		delay(1500);
		drive(0);
	}

/*void senti(float nann,int b)
{
	float waa;
	resetMotorEncoder(right);
	resetMotorEncoder(left);
	waa = 360 / HEIGHT * nann*2.2;
	moveMotorTarget(right, 2.472*waa, b);
	moveMotorTarget(left, 2.472*waa, b);
	waitUntilMotorStop(right);
	waitUntilMotorStop(left);
}

void turn2(int angle,int power)
{
	float waa,f;
	resetMotorEncoder(left):
	resetMotorEncoder(right);
	waa = WIDTH*PI/360;     //itidoatarinnonagasa
	f = 360*waa/WIDTH  //encoder itido
	moveMotorTarget(right, 2.472*f*angle, angle > 0 ? power : -power);
	moveMotorTarget(left, 2.472*f*angle, angle > 0 ? -power : power);
  waitUntilMotorStop(right);
	waitUntilMotorStop(left);
	}*/

/*	Trace trace;
task main(){
	initTrace(trace,6.0,1.9,0.60);
		while(1){
			setMotorSpeed(back,10);
			tracePID(trace,50);
		}
}*/

void checker()
{
	int green,tgreen,green1=0,green2=0,white,white1=0,white2=0,Black,black1=0,black2=0,A,B,n;
	int l, r;
	playTone(500, 1);
	for(int i=0;i<50;i++)
		{
			switch (getCRNR()) {
				case 1:
					black1++;
					break;
				case 6:
					white1++;
					break;
				case 3:
					green1++;
					break;
			}
			switch (getCRNL()) {
				case 1:
					black2++;
					break;
				case 6:
					white2++;
					break;
				case 3:
					green2++;
					break;
			}
			delay(10);
		}
	drive(0);
	displayTextLine(11,"%d",white1);
	displayTextLine(12,"%d",white2);
	displayTextLine(13,"%d",green1);
	displayTextLine(14,"%d",green2);
	delay(200);
	//green 2 white 1 black 0
	if(white1<green1+20&&black1<green1+20)
		A=GREEN;
	else if (white1>black1 || white1==0&&black1==0)
		A=WHITE;
	else if (black1>=25)
  	A=BLACK;
	if(white2<green2+20&&black2<green2+20)
		B=GREEN;
	else if (white2>black2 || white2==0&&black2==0)
		B=WHITE;
	else
		B=BLACK;
	drive(0);
	displayTextLine(6,"%d",A);
	displayTextLine(7,"%d",B);
	if(A==BLACK && B==BLACK)
	{
		displayTextLine(10,"%d",1);
		drive(0);
	}
	else if(A==GREEN && B==GREEN)
	{
	  	displayTextLine(10,"%d",2);
	  	drive(0);
	  	turn2(190,100);
	}
	else if(A==GREEN && B!=GREEN)
	{
	  	displayTextLine(10,"%d",3);
	  	drive(0);
	  	delay(100);
	  	senti(2,100);
	  	turn2(-90,100);
	  	senti(3,60);
	}
		else if(A!=GREEN && B==GREEN)
	{
	  	displayTextLine(10,"%d",3);
	  	drive(0);
	  	senti(2,100);
	  	turn2(90,100);
	  	senti(3,60);
	}
	else if(A==BLACK && B==WHITE || A==WHITE && B==BLACK)
		{
			displayTextLine(10,"%d",5);
			drive(0);
			if(getCRLF()<=10)
		  {
		  	displayTextLine(10,"%d",500000000);
				senti(3,60);
			}
			else
			{
				displayTextLine(10,"%d",200000000);
				senti(6,-60);
			}
		}
	else
		{
				displayTextLine(10,"%d",808000000000);
		  	senti(5,-60);
		}

	// white < green && black < green => GREEN else white > black => WHITE else black > white => BLACK
		//O|O    O|X    X|O   X|X
		/* while(1)
		{
			if(abs (n=black1-black2)<= 4||black1&&black2!=0)
			{
				playTone(250, 10);
			    senti(5,100);
			    break;
			}
			if(abs (n=green1-green2)<= 4)
			{
				playTone(500, 10);
				turn2(180,100);
				break;
			}
			if(abs (n=green1-white1)>0)
				A=green;
			else
				A=white;
			if(abs (n=green2-white2)>0)
				B=green;
			else
				B=white;
	  		if(A==green||B==green)
			{
			  	playTone(1000, 10);
			  	turn2(180,100);
				break;
			}
			if(A==green||B==white)
			{
				playTone(1000, 10);
				turn2(90,100);
				break;
			}
			if(A==white||B==green)
			{
				playTone(1000, 10);
				turn2(-90,100);
				break;
			}
			if(A==white||B==white)
				break;
			else
				break;
		} */
}
