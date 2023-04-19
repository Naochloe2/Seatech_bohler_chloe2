using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;


namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM4", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();
        }
        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            while (robot.byteListReceived.Count > 0)
            {
                byte c = robot.byteListReceived.Dequeue();
                DecodeMessage(c);
                //textBoxReception.Text += "0x"+c.ToString("X2")+" ";

            }
        }

        bool toggle;
        private void button_Click(object sender, RoutedEventArgs e)
        {
            if (toggle)
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
            toggle = !toggle;
            Send();

        }

        private void Send()
        {
            serialPort1.WriteLine(textBoxEmission.Text);
            textBoxEmission.Text = "";
        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Send();
            }
        }
        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);

            for (int i = 0; i < e.Data.Length; i++)
                robot.byteListReceived.Enqueue(e.Data[i]);
        }

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] payload = Encoding.ASCII.GetBytes("Bonjour");
            UartEncodeAndSendMessage(0x0080, payload.Length, payload);

            payload = new byte[] { 45, 78, 65 };
            UartEncodeAndSendMessage((int)Typemessage.DistanceIR, payload.Length, payload);

            payload = new byte[] { 45, 50 };
            UartEncodeAndSendMessage((int)Typemessage.VitesseConsigne, payload.Length, payload);


            //byte[] byteList = new byte[20];
            //for(int i=0; i< byteList.Length; i++)
            //{
            //    byteList[i] = (byte)(i * 2);
            //}
            //serialPort1.Write(byteList, 0, byteList.Length);
        }

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;
        }


        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            //byte[] msgFunction = Encoding.ASCII.GetBytes(msgFunction);
            //byte[] msgPayloadLength = Encoding.ASCII.GetBytes(msgPayloadLength);
            byte[] message = new byte[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8); //MSB
            message[pos++] = (byte)(msgFunction >> 0); //LSB
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }
            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            serialPort1.Write(message, 0, message.Length);
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        int msgDecodedPayloadIndex = 0;
        byte[] msgDecodedPayload;
        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcvState = StateReception.FunctionMSB;
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;

                    break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;

                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;

                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;
                    if (msgDecodedPayloadLength == 0)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    else
                    {
                        msgDecodedPayloadIndex = 0;
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        rcvState = StateReception.Payload;
                    }

                    break;
                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;
                case StateReception.CheckSum:

                    int calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    int receivedChecksum = c;

                    if (calculatedChecksum == receivedChecksum)
                    {
                        Console.WriteLine("decodage Reussi");
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                        //Success, on a un message valide
                    }
                    rcvState = StateReception.Waiting;

                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        public enum Typemessage
        {
            texte = 0x0080,
            Led = 0x0020,
            DistanceIR = 0x0030,
            VitesseConsigne = 0x0040,
            EtapeDeplacement = 0x0050,
        }

        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15
        }


        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            checkboxLed1.IsChecked = false;
            switch ((Typemessage)msgFunction)
            {
                case Typemessage.texte:

                    textBoxReception.Text = Encoding.ASCII.GetString(msgPayload);

                    break;
                case Typemessage.Led:
                    switch(msgPayload[0])
                    {
                        case 1:
                            if (msgPayload[1] == 1)
                                checkboxLed1.IsChecked = true;
                            else 
                                checkboxLed1.IsChecked = false;
                            break;
                        case 2:
                            if (msgPayload[1] == 1)
                                checkboxLed2.IsChecked = true;
                            else
                                checkboxLed2.IsChecked = false;
                            break;
                        case 3:
                            if (msgPayload[1] == 1)
                                checkboxLed3.IsChecked = true;
                            else
                                checkboxLed3.IsChecked = false;
                            break;
                    }
                    break;
                case Typemessage.DistanceIR:

                    IRgauche.Content = msgPayload[0] + " cm";
                    IRcentre.Content = msgPayload[1] + " cm";
                    IRdroit.Content = msgPayload[2] + " cm";

                    break;
                case Typemessage.VitesseConsigne:

                    vitesseG.Content = "Vitesse Droite: " + msgPayload[0] + "%";
                    vitesseD.Content = "Vitesse Gauche: " + msgPayload[1] + "%";

                    break;

                case Typemessage.EtapeDeplacement:

                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16) + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    textBoxReception.Text += "\nRobot␣State␣:␣" +
                    ((StateRobot)(msgPayload[0])).ToString() +
                    "␣-␣" + instant.ToString() + "␣ms";

                    break;
            }
        }

    }
}
