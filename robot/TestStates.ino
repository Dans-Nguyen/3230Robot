int TestStates(int state){
  switch(state){
    // 120 state for testing order reception
    case 120:{
      float frequency = IR.getFrequency();
      Serial.println(frequency);
      if (order.update()) {
        activeOrderCount = order.count();
        
        // A complete start → colors → end message was received.
        for (uint8_t i = 0; i < order.count(); i++) {
          activeOrder[i] = order.block(i); // store block color to activeOrder
          //Serial.println(activeOrder[i]);
          if(activeOrder[i] == 1){
            Serial.println("RED");
          } else if(activeOrder[i] == 2){
            Serial.println("YELLOW");
          } else if(activeOrder[i] == 3){
            Serial.println("BLUE");
          } else if(activeOrder[i] == 4){
            Serial.println("BROWN");
          } 


        }

        state = 999;
      }
      
    break;}

    // 121 state for testing block detection
    case 121:{
      detectBlock.update();
      char blockColor = detectBlock.getColor();
      bool isMagnet = detectBlock.isMagnet();
      Serial.print(blockColor);
      Serial.println(isMagnet);
      
    break;}

    // 122 state for testing trajectory
    case 122:{

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(50.0, 1.0); // 50 cm in 6 seconds
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}

    // 123 state for testing rack trajectory
    case 123:{
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(20.0, 15.0); // 10 cm at 5 cm/s
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}


    // STATES FOR TESTING ACTUATION
    case 400:{
      // record time once
      if (!servoTiming){
        servoStartT = millis();
        servoTiming = true;
      }

      pickButtonServo.setAngle(150);

      if (millis() - servoStartT >= 2000UL){ // stop after 2 s 
        pickButtonServo.setAngle(pickButtonInit);
        servoTiming = false;
        state = 999;
      }
    break;}

    case 401:{
      // record time once
      if (!servoTiming){
        servoStartT = millis();
        servoTiming = true;
      }

      paddleServo.setAngle(80);

      if (millis() - servoStartT >= 2000UL){ // stop after 2 s  
        paddleServo.setAngle(paddleInit);
        servoTiming = false;
        state = 999;
      }
    break;}

    case 402:{
      // record time once
      if (!servoTiming){
        servoStartT = millis();
        servoTiming = true;
      }

      shipButtonServo.setAngle(180);

      if (millis() - servoStartT >= 2000UL){ // stop after 2 s  
        shipButtonServo.setAngle(shipButtonInit);
        servoTiming = false;
        state = 999;
      }
    break;}

    case 403:{
      // record time once
      if (!servoTiming){
        servoStartT = millis();
        servoTiming = true;
      }

      leverServo.setAngle(150);

      if (millis() - servoStartT >= 2000UL){ // stop after 2 s  
        leverServo.setAngle(leverInit);
        servoTiming = false;
        state = 999;
      }
    break;}
  }

  return state;
}