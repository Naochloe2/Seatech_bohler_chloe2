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
            serialPort1 = new ReliableSerialPort("COM12", 115200, Parity.None, 8, StopBits.One);
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
                textBoxReception.Text += "0x"+c.ToString("X2")+" ";
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
            byte[] byteList = new byte[20];
            for(int i=0; i< byteList.Length; i++)
            {
                byteList[i] = (byte)(i * 2);
            }
            serialPort1.Write(byteList, 0, byteList.Length);
        }

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum;
            int nb = msgPayloadLength / 8;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            while (nb != 0) ;
            checksum ^= msgPayload >> 8 * (nb - 1);
            nb = nb - 1;
            return Checksum
}


        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            //byte[] msgFunction = Encoding.ASCII.GetBytes(msgFunction);
            //byte[] msgPayloadLength = Encoding.ASCII.GetBytes(msgPayloadLength);
            byte message[msgPayloadLength + 6]
                    message[0] = 0xFE;
            message[1] = msgFunction[0];
            message[2] = msgFunction[1];
            message[3] = msgPayloadLength[0];
            message[4] = msgPayloadLength[1];
            for (i = 0; i < msgPayloadLength; i++)
            {
                message[i + 4] = msgPayload[i];
            }
            message[msgPayloadLength + 4] = CalculateChecksum;
            SendMessage(message, int msgPayloadLength + 6);
        }
    }
}
