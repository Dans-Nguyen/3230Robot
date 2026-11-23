int TestCasesUno(int state){
  // TEST CASES FOR UNO
  switch (state) {

    case 121 : { // Detect what color and if theres a magnet
      detectBlock.update();
      uint8_t blockColor = detectBlock.getColor();
      bool isMagnet = detectBlock.isMagnet();
      bool blockDetected = detectBlock.blockDetected();

      Serial.print(blockColor);
      Serial.print(isMagnet);
      Serial.println(blockDetected);

    break;}

    case 122: {
      SerialUno.sendState((int)300);
      state = 0;
      break;
    }
    
  
  }
  return state;
}