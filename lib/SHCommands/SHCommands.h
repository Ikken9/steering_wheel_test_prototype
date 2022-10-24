#define MESSAGE_HEADER 0x03

void Command_Hello() {
	FlowSerialTimedRead();
	delay(10);
	FlowSerialPrint(VERSION);
	FlowSerialFlush();
}

void Command_SetBaudrate() {
	SetBaudrate();
}

void Command_DeviceName() {
    FlowSerialPrint("SimHub Dash");
    FlowSerialPrint("\n");
    FlowSerialFlush();
}

void Command_UniqueId() {
    FlowSerialPrint("80460611-5bc1-45b1-ab25-075da36e6045"); // default = "80460611-5bc1-45b1-ab25-075da36e6045"
    FlowSerialPrint("\n");
    FlowSerialFlush();
}

void Command_MCUType() {
    FlowSerialPrint(SIGNATURE_0);
    FlowSerialPrint(SIGNATURE_1);
    FlowSerialPrint(SIGNATURE_2);
    FlowSerialFlush();
}

void Command_ButtonsCount() {
	FlowSerialWrite((byte)(12)); // ENABLED_BUTTONMATRIX * (BMATRIX_COLS * BMATRIX_ROWS) = 12
	FlowSerialFlush();
}

void Command_ModulesCount() {
    FlowSerialWrite((byte)(0));
    FlowSerialFlush();
}

void Command_Acq() {
	FlowSerialWrite(0x03);
	FlowSerialFlush();
}

void Command_EncodersCount() {
    FlowSerialWrite((byte)4);
    FlowSerialFlush();
}

void Command_SimpleModulesCount() {
    FlowSerialWrite((byte)(0));
    FlowSerialFlush();
}

void Command_RGBLEDSCount() {
    FlowSerialWrite((byte)(16)); // WS2812B_RGBLEDCOUNT = 16
    FlowSerialFlush();
}

void Command_ExpandedCommandsList() {
	FlowSerialPrintLn("encoders"); // 4 ENABLED ENCODERS
	FlowSerialPrintLn("mcutype");
	FlowSerialPrintLn("keepalive");
	FlowSerialPrintLn();
	FlowSerialFlush();
}

void Command_Features() {
	delay(10);

	// Name
	FlowSerialPrint("N");

	// UniqueID
	FlowSerialPrint("I");

	// Additional buttons
	FlowSerialPrint("J");

	// Custom Protocol Support
	FlowSerialPrint("P");

	// Xpanded support
	FlowSerialPrint("X");

	FlowSerialPrint("\n");
	FlowSerialFlush();
}
