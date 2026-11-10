int StartStates(int state){
  switch(state){

    // RESETTING CASE
    case 0:{
      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.startHome(); // home
      }

      rackTrajectory.update();

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}

    // LEAVING STARTING AREA
    case 1:{ // line follow and extend rack
      lineFollow.start(200);

      if (rackTrajectory.state == RackTrajectory::IDLE) {
        rackTrajectory.setPosition(10.0, rackSpeed); // 10 cm at 5 cm/s
      }

      rackTrajectory.update();

      // Get distance data from front sensor
      distCm = distSensorF.getDistCm();


      if(distCm <= 32){
        // distance from wall and front sensor achieved, stop line following
        drive.stop();
        state = 2;
      }

    break;}

    case 2:{ // turn 90 degrees
      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        // Start command sets driveTrajecoty state to ACTIVE
        driveTrajectory.startRotate(PI/2.0f, 0.5); // 90 deg in 0.5 seconds
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 3;                  // Advance to next robot state
      }

    break;}

    case 3:{ // wall follow for some time
      // record time once 
      if (!wallFollowTiming) {
        wallFollowStartT = millis();
        wallFollowTiming = true;
      }
      
      //Serial.println(distSensorR.getDistCm());
      //Serial.println(wallFollowStartT);

      wallFollow.start(-200, 26);
      
      if (millis() - wallFollowStartT >= 5000UL){ // stop after 5 s
        wallFollow.stop();
        state = 4;
        wallFollowTiming = false;
      }
    break;}
  }

  return state;
}