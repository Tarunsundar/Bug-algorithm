/*
 * File:   assignment.c
 * Author: Tarunsundar
 *
 * Created on 7 March, 2021, 10:55 AM
 */


#include "allcode_api.h" 

int main() {
    FA_RobotInit();         // MUST call this before any other robot functions
    FA_LCDClear();
    FA_LCDBacklight(50);    // Switch on backlight (half brightness)
    int obstacleDetected=0;
    int mlaway=1;
    int mltowards=0;
    bool state;
    bool left=1; // true means turned left, false means turned right 
    float mLDistance=0;
    float aDistance=0;
    char cd ='N'; //current direction robot is moving 
    char turn ='R';
    float eDistance=0;
    FA_DelayMillis(1000); 
    char changecd(char turn){  //function to find the direction robot is travelling to
       if(turn=='R' && cd =='N') 
           cd = 'E';
       else if(turn=='R' && cd =='S') 
           cd = 'W';
       else if(turn=='R' && cd =='W') 
           cd = 'N';
       else if(turn=='R' && cd =='E') 
           cd = 'S';
       else if(turn=='L' && cd =='N') 
           cd = 'W';
       else if(turn=='L' && cd =='S') 
           cd = 'E';
       else if(turn=='L' && cd =='W') 
           cd = 'S';
       else if(turn=='L' && cd =='E') 
           cd = 'N';
       return cd;
    } 
    while(1){
    while(mLDistance<=1000) //changed from 50 meters to 5 meters for testing need to change after testing completed            // Execute this loop as long as robot is running
    { //FA_LCDPrint(cd, 1, 20, 25, FONT_NORMAL,  LCD_OPAQUE); 
    while(FA_ReadIR(2) < 1000){ //while not found an obstacle it would move forward m Line driving  {
        //FA_Forwards(200);
        FA_SetMotors(20,18);
        aDistance = FA_ReadEncoder(0)*0.32;
        mLDistance = FA_ReadEncoder(0)*0.32; //number of ticks to Actual distance
        if(mLDistance>1000)
            break;
    }
    if(mLDistance>1000)
            break;
    FA_ResetEncoders();
    FA_SetMotors(0,0);
    if(FA_ReadIR(2) > 200)  // Check front centre IR
        {   obstacleDetected++;
            FA_PlayNote (2200, 500);
            //FA_Backwards(30);
            FA_DelayMillis(90);
            FA_Right(90); // Skip back and then to its right if obstacle
            FA_PlayNote (2000, 500);
            FA_DelayMillis(100);
            cd = changecd('R'); // changes the current direction 
            FA_LCDPrint(cd, 1, 20, 25, FONT_NORMAL,  LCD_OPAQUE);
        }
      while(mlaway>mltowards){
          //mlaway=0;
          FA_ResetEncoders();
        //if(FA_ReadIR(7)>120) //keep moving forward until the end of object0
           // {
            //FA_DelayMillis(5000);
            //FA_SetMotors(40,35);
            while(FA_ReadIR(1)>25||FA_ReadIR(7)>200){ //while left sensor not greater than back sensor move forward
            //FA_DelayMillis(5000);
            FA_PlayNote (100, 500);
           /*if(FA_ReadIR(0)>1200) //traveling close to the obstacle
            {
               FA_Right(10);           
            }*/  
            if(cd='E'){
            FA_SetMotors(20,17);
            mlaway+= FA_ReadEncoder(0)*0.32; //number of ticks to Actual distanceaDistance = FA_ReadEncoder(0)*0.32; //number of ticks to Actual distance
            aDistance+= FA_ReadEncoder(0)*0.32; //number of ticks to Actual distance
            }
            else if(cd='W'){
            FA_SetMotors(20,17);
            mltowards+=FA_ReadEncoder(0)*0.32;
            aDistance+= FA_ReadEncoder(0)*0.32;;
            }   
            else if(cd='N'){
            FA_SetMotors(20,17);
            mLDistance+=FA_ReadEncoder(0)*0.32;
            aDistance+= FA_ReadEncoder(0)*0.32;;
            //} 
            //}
            }
            else if(cd='S'){
            FA_SetMotors(20,17);
            aDistance+= FA_ReadEncoder(0)*0.32;;
            } 
        }
          FA_SetMotors(0,0);
          FA_DelayMillis(1000);
          if(FA_ReadIR(1)<200){ //turn left and drive around the obstacle
                FA_DelayMillis(100);
                FA_Left(100);
                if(FA_ReadIR(2) > 200)  // Check front centre IR
                {   
                    FA_PlayNote (2200, 500);
                    //FA_Backwards(30);
                    FA_DelayMillis(90);
                    FA_Right(90);// Skip back and then to its right if obstacle
                    while(FA_ReadIR(1)>200){
                    FA_SetMotors(20,17);
                    mlaway+= FA_ReadEncoder(0)*0.32; //number of ticks to Away distanceaDistance
                    aDistance+= FA_ReadEncoder(0)*0.32; //number of ticks to Actual distance
                    FA_PlayNote (2000, 500);
                    FA_DelayMillis(100);
                    cd = changecd('R');
                    cd= changecd('L');
                    }
                }
                else if(FA_ReadIR(1)<200)
                    FA_Left(10);
                    while(FA_ReadIR(1)<600){////while left sensor not lesser than value move towards obstacle
                      FA_SetMotors(20,17); 
                      aDistance+= FA_ReadEncoder(0)*0.32; //number of ticks to Actual distance
                }
                FA_SetMotors(0, 0);
          }
            else if(FA_ReadIR(2)>400){ //if found obstacle corner
                FA_Left(90);  
                cd= changecd('R');
                FA_LCDPrint(cd, 1, 20, 25, FONT_NORMAL, LCD_OPAQUE);
            }
            else if(FA_ReadIR(1)>600) //traveling close to the obstacle
            {
               FA_Left(10);           
            }   
          else if(mltowards=mlaway){// obstacle should be avoided totally and drive straight
            FA_Right(90);
            cd= changecd('R');            FA_SetMotors(0,0);
            mlaway=0;
            mltowards=0; 
            aDistance+=FA_ReadEncoder(0)*0.32;
            mLDistance+=FA_ReadEncoder(0)*0.32;;
        }
            else{ //this is while robot not traveling towards or away from mline 
            while(FA_ReadIR(1)>300&&FA_ReadIR(1)<700){
            FA_SetMotors(20,17);
               aDistance+= FA_ReadEncoder(0)*0.32;;
            }
            }
      }
            FA_PlayNote (2000, 500);
            FA_DelayMillis(100);
     }
      FA_SetMotors(0,0);
      do{
      eDistance = aDistance - mLDistance;
      FA_LCDPrint("gpa!!!", 6, 20, 10, FONT_NORMAL, LCD_OPAQUE);//gpa is goal position acquired
      FA_PlayNote (2000, 500);
      FA_DelayMillis(1000);
      FA_LCDClear();
      FA_LCDPrint("m line dist: ", 13, 0, 23, FONT_NORMAL, LCD_OPAQUE);
      FA_LCDNumber(mLDistance, 72, 23, FONT_NORMAL, LCD_OPAQUE); //prints the m Linr distance travelled
      if(obstacleDetected!=0){
      FA_LCDPrint("n obstacles: ", 13, 0, 10, FONT_NORMAL, LCD_OPAQUE); //n obstacles means number of obstacles    
      FA_LCDNumber(obstacleDetected, 70, 10, FONT_NORMAL, LCD_OPAQUE); //prints the number of obstacles avoided
      FA_PlayNote (2200, 1000);
      FA_DelayMillis(3000);
      FA_LCDClear();
      FA_LCDPrint("actual dist: ", 13, 0, 23, FONT_NORMAL, LCD_OPAQUE);
      FA_LCDNumber(aDistance, 82, 23, FONT_NORMAL, LCD_OPAQUE); //prints the actual distance travelled
      FA_LCDPrint("extra dist: ", 12, 0, 10, FONT_NORMAL, LCD_OPAQUE); //prints the number of obstacles avoided
      FA_LCDNumber(eDistance, 70, 10, FONT_NORMAL, LCD_OPAQUE); //prints the extra distance travelled
      }
      else {
      FA_LCDPrint("n obstacles: ", 13, 0, 10, FONT_NORMAL, LCD_OPAQUE); //n obstacles means number of obstacles    
      FA_LCDNumber( obstacleDetected, 72, 10, FONT_NORMAL, LCD_OPAQUE); //prints the number of obstacles avoided  
      }
      FA_DelayMillis(5000);
      FA_LCDClear();
      //FA_DelayMillis(5000);
    }while(!FA_ReadSwitch(0)||!FA_ReadSwitch(1));
    FA_LCDClear();
    }
    return 0;
}
