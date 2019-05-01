package botpackage;

import java.awt.Color;

import robocode.*;

public class Claptrap extends Robot {
	public void run(){
		setAllColors(Color.pink);
		
	while(true){
		int a = getOthers();
		if(a>3){
			ahead(Math.random()*500);
			turnRight(33);
			}
		else{
			ahead(100);
			turnGunLeft(360);
			}
		}
	}
	//Når roboten ser en annen robot: 
	public void onScannedRobot(ScannedRobotEvent e) {
		//Hvis distansen er mer en 200 bruker den bare fire(1.9) er ikke så kritisk med tanke på energy fordi roboten kjører bare rundt
		//som en idiot på starten uansett
		if(e.getDistance() > 200){
			fire(1.9);
		}
		else if(e.getDistance() > 50){
			fire(2);
		}
		else{
			fire(3);
		}
		
	}
	//Når roboten treffer en vegg
	public void onHitWall(HitWallEvent e) {
		int a = getOthers();
		//Hvis det er det er mindre enn 3 på banen rygg
		//Når det er flere klarer den å komme seg bort fra veggen på egenhånd ved hjelp av move when shot
		if(a<3){
		back(200);
		turnRight(33);
		}
		else{
			
		}
	}
	//Når roboten kræsjer med annen robot
	public void onHitRobot(HitRobotEvent e){
		//Hvis det er mindre enn 3 på banen så rygger den, eller hvis det er jeg som kræsjer inn i en annen..
		int a = getOthers();
		if(a<3 || e.isMyFault() == true){
		back(200);
		turnLeft(33);
		}
		else{
			//Hadde tenkt å ha isMyFault() her isteden med en fire 3 men siden jeg har det i onScannedRobot hvis den er nærme så er ikke det behov
		}
	}
	
	//Når roboten blir truffet
	public void onHitByBullet(HitByBulletEvent e){
		int a = getOthers();
		//Hvis den blir truffet når det er mange på banen så fortsetter den bare å kjøre i random pattern i håp om å overleve...
		if(a>3){
			return;
		}
		else{
			if(e.getBearing() > -90 && e.getBearing() < 90){
				turnLeft(33);
			}
			else{
				
			}
		}
		
	}
	public void onBulletHit(BulletHitEvent e){
		double a = e.getEnergy();
		int b = getOthers();
		if(10>a){
			setAllColors(Color.BLACK);
		}
		else if(a>10 && a<20){
			setAllColors(Color.cyan);
		}
		else if(a>20 && a<50){
			setAllColors(Color.green);
		}
		else{
			setAllColors(Color.orange);
			setBulletColor(Color.green);
		}
		//går litt til høyre hver gang den treffer noen når det er mindre enn 3 på banen for litt mer movement
		if(b<3){
			turnRight(33);
			ahead(20);
		}
		else{	
			
			}

		}
}