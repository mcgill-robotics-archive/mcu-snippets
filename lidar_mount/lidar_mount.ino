/*
 * Reading of encoder.
 * 
 * Protocols are based on the datasheet for the CUI-AMT20 which
 * can be found at: http://www.cui.com/product/resource/amt20-v.pdf
 * 
 * Circuit:
 ** CSB : pin 10 (slave select)
 ** MOSI: pin 11 
 ** MISO: pin 12
 ** SCK : pin 13 (spi clock)
 * 
 * Author: Malcolm Watt
 */
 
#include<SPI.h>

// Slave select pin.
const int CHIP_SEL_PIN = 10;

// SPI Parameter.
const long DATA_RATE = 1000000;

// Bytes for different command to interface with the encoder.
const byte NOP_A5 = 0x00;
const byte RD_POS = 0x10;
const byte SET_ZERO = 0x70;
const byte IDLE_CHAR = 0xA5;

void setup() 
{
  Serial.begin(9600);

  // Start the SPI library.
  SPI.begin();

  // Setup the chip select pin.
  pinMode(CHIP_SEL_PIN, OUTPUT);
  
  // Allow time for initialization.
  delay(100);

  // Print welcome message and instructions.
  Serial.println("This module is to test the encoders.");
  Serial.println("Enter commands (without quotes) to perform actions.");
  Serial.println("Availlable Commands:");
  Serial.println("- `init` : Zeros the encoders.");
  Serial.println("- `read` : Read the value from the encoders.");
}


void loop() 
{
  // Handle read-in of command if availlable.
  String command = "default";
  if(Serial.available() > 0)
  {
    command = Serial.readString();
  }

  // Follow command.
  if (command == "init")
  {
    // Zero the encoder.
    Serial.println("Initializing...");
    set_zero_point();
    Serial.println("Encoder initialized.");
  } 
  else if (command == "read")
  {
    // Read the encoder.
    Serial.println("Reading value from encoder...");
    float angle = read_encoder();
    Serial.print("The angle read is: ");
    Serial.println(angle);
  }
  else if (command == "default")
  {
    // No command.
    nop_a5();
  }
}

/*
 * Initialize the SPI transaction.
 */
void initialize_transaction()
{
  SPI.beginTransaction (SPISettings (DATA_RATE, MSBFIRST, SPI_MODE0));
  digitalWrite(CHIP_SEL_PIN, LOW);
}

/*
 * Finalize the SPI transaction.
 */
void end_transaction()
{
  digitalWrite(CHIP_SEL_PIN, HIGH);
  SPI.endTransaction ();
}

/*
 * Thes particular encoder requires that we release the slave select line
 * every time we transmit a byte.
 * 
 * Also from the spec: "It is recommended that the master leave a 20
 * microsecond delay between reads to avoid extending the read time
 * by forcing wait sequences.
 */
void release_ss()
{
  delayMicroseconds(20);
  digitalWrite(CHIP_SEL_PIN, HIGH);
  digitalWrite(CHIP_SEL_PIN, LOW);
}

/*
 * Transfer a `no operation` command.
 */
void nop_a5()
{
  initialize_transaction();
  SPI.transfer(NOP_A5);
  end_transaction();
}

/*
 * Transfer a `zero set` command based on the CUI protocol.
 */
void set_zero_point()
{
  unsigned int count = 0;
  initialize_transaction();

  // Transfer the set_zero_point command.
  byte ack = SPI.transfer(SET_ZERO);
  // Transfer no_operation command untill we receive 0x80 which
  // is an acknowledgement that the encoder has been zeroed.
  while (ack != 0x80)
  {
    release_ss();
    ack = SPI.transfer(NOP_A5);
    count++;
  }

  end_transaction();
}

/*
 * Returns the angle of the encoder.
 * The encoder gives a 12-bit absolute position via SPI (4096 positions).
 */
float read_encoder()
{
  initialize_transaction();

  // To hold transfer values.
  byte read_value;

  // Transfer the original read position command.
  read_value = SPI.transfer(RD_POS);

  /* 
   * Transfer until we receive the RD_POS byte back. Based on the 
   * data sheet, when we receive the RD_POS back, the next two bytes
   * contain the angle of the encoder.
   */
  while (read_value != RD_POS)
  {
    release_ss();
    read_value = SPI.transfer(NOP_A5);
  }

  /* 
   * Based on the datasheet:
   * 1 -  send nop_a5 and receive MSB position (lower 4 bits of this byte are the upper 4 of 
   *      the 12-bit position)
   * 2 -  Send second nop_a5 command and receive LSB position (lower 8 bits of 12-bit positon)
   */
  release_ss();
  read_value = SPI.transfer(NOP_A5);

  unsigned int bit_angle = 0;
  
  bit_angle = (unsigned int)(read_value & 0x0F) * 256;

  release_ss();
  read_value = SPI.transfer(NOP_A5);

  bit_angle = bit_angle + (unsigned int) read_value;

  // Convert to degrees.
  float angle = (float) bit_angle * 360 / 4096;

  end_transaction();

  return angle;
}
