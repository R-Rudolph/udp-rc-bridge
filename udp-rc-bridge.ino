#include <EtherCard.h>
#include <IPAddress.h>
#include <RCSwitch.h>


#define STATICIP

uint16_t listen_port = 17000;
uint16_t broadcast_port = 17001;
#ifdef STATICIP
// Ethernet interface IP address
static byte myip[] = { 192,168,178,200 };
static uint8_t netmask[] = { 255,255,255,0 };
// Gateway IP address
static byte gwip[] = { 192,168,178,1 };
#else
static byte netmask[IP_LEN];
#endif


// Ethernet MAC address - must be unique on your network
static byte macAddr[] = { 0x04,0x20,0x69,0x04,0x20,0x69 };

byte Ethernet::buffer[500]; // TCP/IP send and receive buffer
uint8_t broadcastAddress[IP_LEN];

RCSwitch rcSender = RCSwitch();
RCSwitch rcReceiver = RCSwitch();

// Callback that prints received packets to the serial port
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len)
{
  long code = atol(data);
  Serial.print("Received: ");
  Serial.println(data);
  Serial.print("Sending: ");
  Serial.println(code);
  rcSender.send(code, 24);
}

void setup()
{
  //set up serial connection, for debugging
  Serial.begin(9600);

  // register IP stuff
  // Change 'SS' to your Slave Select pin if you aren't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, macAddr, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
#ifdef STATICIP
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
  memcpy(netmask,ether.netmask,IP_LEN);
#endif
  // determine broadcast address
  for(int i=0;i<IP_LEN;++i)
  {
    broadcastAddress[i] = ether.myip[i] | (netmask[i]^0xFF);
  }

  // debug output
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("Gateway:  ", ether.gwip);
  ether.printIp("Netmask:  ", netmask);
  ether.printIp("Broadcast:  ", broadcastAddress);

  // Register udpSerialPrint()
  ether.udpServerListenOnPort(&udpReceive, listen_port);

  rcSender.enableTransmit(10);  // register sender to pin 10
  rcReceiver.enableReceive(0);  // register receiver to interrupt pin 0 (pin 2 on Mega)
}

void rcReceiveLoop()
{
  if (rcReceiver.available())
  {
    unsigned long receivedData      = rcReceiver.getReceivedValue();
    unsigned int receivedDatalength = rcReceiver.getReceivedBitlength();
    
    if(receivedData != 0)
    {
      char udpString[500];
      sprintf(udpString, "%lu,%u", receivedData,receivedDatalength);
      Serial.print("received: ");
      Serial.println(udpString);
      ether.sendUdp(udpString, strlen(udpString), broadcast_port, broadcastAddress, broadcast_port );
    }
    rcReceiver.resetAvailable();
  }
}

void loop()
{
  //loop ethernet receive
  ether.packetLoop(ether.packetReceive());
  //loop 433MHz receive
  rcReceiveLoop();
}
