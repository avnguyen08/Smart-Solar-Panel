/***********************************************************************
* FILENAME :        servo_move.h            
* DESCRIPTION :     Moves servo to specified point at a slow rate 
*                   Wraps servo.write() function 
* AUTHOR :          Aaron Nguyen       
*
*/

void servo_move(Servo *servo, uint *cur_deg, uint next_deg, int waitTime)
{
  char str[50] = "";
  // if current degree < next degree, execute while statement
  sprintf(str, "Servo starting to rotate to: %d", next_deg);
  Serial.println(str);

  if ((*cur_deg) == next_deg)
  {

    sprintf(str, "Current Degree: 90", (*cur_deg));
    Serial.println(str);
  }
  else if ((*cur_deg) < next_deg)
  {
    // while current degrees < next degrees, keep adding +1 degree every few ms until current degree > next degree
    while ((*cur_deg) < next_deg)
    {
      (*cur_deg)++;
      (*servo).write((*cur_deg));
      sprintf(str, "Current Degree: %d", (*cur_deg));
      Serial.println(str);
      delay(waitTime);
    }
  }
  // if current degree > next degree, execute while statement
  else if ((*cur_deg) > next_deg)
  {
    // While current degree > next degrees, keep subtracting -1 degree from current degree until current degree < next degree.
    while ((*cur_deg) > next_deg)
    {
      (*cur_deg)--;
      (*servo).write((*cur_deg));
      sprintf(str, "Current Degree: %d", *cur_deg);
      Serial.println(str);
      delay(waitTime);
    }
  }
  sprintf(str, "Servo finished rotating to: %d", next_deg);
  Serial.println(str);
}
