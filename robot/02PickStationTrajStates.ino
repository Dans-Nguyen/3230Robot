int PickStationTrajStates(int state){
  // S-curve variables
  float sCurveRadius = 18.786; // cm
  float sCurveAngle = 0.616; // rad

  switch(state){
    // PICKING STATION TRAJECTORY

    case 4:{ // drive to picking station

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(170.0f, 5.0f);
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

    case 6:{ // approach button

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(23.0f, 3.0f);
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 50;                  // Advance to next robot state
      }
    break;}

    // case 6:{ // reverse it a little

    //   if (driveTrajectory.state == DriveTrajectory::IDLE) {
    //     driveTrajectory.reset();
    //     driveTrajectory.startStraight(-22.0f, 1.0f);
    //   }

    //   driveTrajectory.update();

    //   if (driveTrajectory.state == DriveTrajectory::IDLE) {
    //     state = 7;                  // Advance to next robot state
    //   }
    // break;}

    // case 7:{ // line follow
    //   lineFollow.start(50);

    //   if(distSensorF.getDistCm() <= 22){
    //     // distance from wall and front sensor achieved, stop line following
    //     drive.stop();
    //     state = 8;
    //   }
    // break;}

    // case 8:{ // S-curve 1

    //   if (driveTrajectory.state == DriveTrajectory::IDLE) {
    //     driveTrajectory.reset();
    //     driveTrajectory.startCurve(sCurveAngle, sCurveRadius, 4.0f, false);
    //   }

    //   driveTrajectory.update();

    //   if (driveTrajectory.state == DriveTrajectory::IDLE) {
    //     state = 9;                  // Advance to next robot state
    //   }
    // break;}

    // case 9:{ // S-curve 2

    //   if (driveTrajectory.state == DriveTrajectory::IDLE) {
    //     driveTrajectory.reset();
    //     driveTrajectory.startCurve(sCurveAngle, sCurveRadius, 4.0f, true);
    //   }

    //   driveTrajectory.update();

    //   if (driveTrajectory.state == DriveTrajectory::IDLE) {
    //     state = 10;                  // Advance to next robot state
    //   }
    // break;}

    

  }

  return state;
}