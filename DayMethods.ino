/*
 * Checks if the system has been actice for the time specified by
 * the dayInterval and if so transition from day to night or vice versa
 */
void dayOrNight() {

  // Time is transitioning from day to night
  if ((currTime - prevDay > dayInterval) && timeDay) {
      timeDay = false;                    // Make it night
      prevDay = currTime;
      playFile("E109.wav");                   //Wolf
  }

  // Time is transitioning from night to day
  if ((currTime - prevDay > dayInterval) && !timeDay) {
      timeDay = true;                     // Make It day
      prevDay = currTime;
      playFile("E108.wav");                   //Rooster
  }
}