int PickStationTrajStates(int state){
  switch(state){
    // PICKING STATION TRAJECTORY
    case 4:{ // reverse

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        driveTrajectory.reset();
        driveTrajectory.startStraight(20.0, 1.0); // 20 cm in 1 seconds
      }

      driveTrajectory.update();

      if (driveTrajectory.state == DriveTrajectory::IDLE) {
        state = 999;                  // Advance to next robot state
      }
    break;}
  }

  return state;
}