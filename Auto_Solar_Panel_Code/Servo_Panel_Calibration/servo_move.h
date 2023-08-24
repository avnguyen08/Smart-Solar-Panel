//function that will move servo to specified point at a slow rate (As  opposed to "Servo.write()" function which goes as fast as possible)
void servo_move(Servo *servo, uint *cur_deg, uint next_deg, int waitTime) {
  char str[50] = "";
  //if current degree < next degree, execute while statement
  sprintf(str, "Servo starting to rotate to: %d", next_deg);
  Serial.println(str);
  if ((*cur_deg) < next_deg) {
    //while current degrees < next degrees, keep adding +1 degree every few ms until current degree > next degree
    while ((*cur_deg) < next_deg) {
      delay(waitTime);
      (*cur_deg)++;
      sprintf(str, "Current Degree: %d", (*cur_deg));
      Serial.println(str);
      (*servo).write((*cur_deg));
    }
  }
  // if current degree > next degree, execute while statement
  if ((*cur_deg) > next_deg) {
    //While current degree > next degrees, keep subtracting -1 degree from current degree until current degree < next degree.
    while ((*cur_deg) > next_deg) {
      delay(waitTime);
      (*cur_deg)--;
      sprintf(str, "Current Degree: %d", *cur_deg);
      Serial.println(str);
      (*servo).write((*cur_deg));
    }
  }
  sprintf(str, "Servo finished rotating to: %d", next_deg);
  Serial.println(str);
}
