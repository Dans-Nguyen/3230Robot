int LoopCases(int state){

  // LOOPING CASES
  switch(state){
    case 50:{ // set rack to init position and hit picking station button
      // record time once
      if (!servoTiming){
        servoStartT = millis();
        servoTiming = true;
      }

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(15, rackSpeed);
      }

      rackTrajectory.update();

      pickButtonServo.setAngle(150);
      //Serial.println(servoStartT);

      if (rackTrajectory.state == RackTrajectory::IDLE 
          && (millis() - servoStartT) >= 1000UL){ 
        pickButtonServo.setAngle(pickButtonInit);
        servoTiming = false;
        state = 51;
      }

    break;}

    case 51:{ // wait for block detection
      detectBlock.update();
      bool blockDetected = detectBlock.blockDetected();

      if(blockDetected){
        state = 52;
      }
    break;}

    case 52:{ // process block
      if (!blockProcessTiming){
        blockProcessT = millis();
        blockProcessTiming = true;
      }

      detectBlock.update();
      char blockColor = detectBlock.getColor();
      bool isMagnet = detectBlock.isMagnet();

    if (millis() - blockProcessT > 500UL){ // wait .5 s
      if(blockColor == 'B' || isMagnet){
        // discard block
        state = 53;
      } else{
        // keep block
        state = 54;
      }
    }

    break;}

    case 53:{ // push block
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(20.0, rackSpeed);
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 54;                  // Advance to next robot state
      }
    break;}


    case 54:{ // wait for old block to leave detector
      detectBlock.update();

      if (!detectBlock.blockDetected()) {
        blockProcessTiming = false;
        state = 50;
      }
    break;}
    

  }

  return state;
}