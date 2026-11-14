int PickStationTrajStates(int state){
  switch(state){
    // PICKING STATION TRAJECTORY
    case 4:{ // drive to picking station

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(180.0f, 5.0f);
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 5;                  // Advance to next robot state
      }
    break;}

    case 5:{ // rotate 90 deg

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startRotate(-PI/2.0f, 1.0f);
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 6;                  // Advance to next robot state
      }
    break;}

      case 6:{ // reverse it a little

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(-22.0f, 1.0f);
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 7;                  // Advance to next robot state
      }
    break;}

    case 7:{ // line follow
      lineFollow.start(100);

      if(distSensorF.getDistCm() <= 32){
        // distance from wall and front sensor achieved, stop line following
        drive.stop();
        state = 8;
      }
    break;}

    

  }

  
  Serial.println(state);


  return state;
}