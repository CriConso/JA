using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ProgettoJA
{
    public partial class Form1 : Form
    {
        public CModulo M1, M2;
        UdpClient receivingUdpClient = new UdpClient(11001);
        IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
        public Form1()
        {
            InitializeComponent();
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            Collegamento C = new Collegamento(this, 1);
            C.Show();
            this.Hide();
        }

        private string GetIP()
        {
            string IPAddress = string.Empty;
            IPHostEntry Host = default(IPHostEntry);
            string Hostname = null;
            Hostname = System.Environment.MachineName;
            Host = Dns.GetHostEntry(Hostname);
            foreach (IPAddress IP in Host.AddressList)
            {
                if (IP.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    IPAddress = Convert.ToString(IP);
                }
            }
            return IPAddress;

        }

        private void button5_Click(object sender, EventArgs e)
        {
            Collegamento C = new Collegamento(this, 2);
            C.Show();
            this.Hide();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            bool ricevi = true;
            //MessageBox.Show(GetIP());
            UdpClient udpClient = new UdpClient("192.168.1.100", 82);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("d");
            udpClient.Send(sendBytes, sendBytes.Length);
            //while (ricevi)
            //{
            Byte[] receiveBytes = receivingUdpClient.Receive(ref RemoteIpEndPoint);

            string s = System.Text.Encoding.UTF8.GetString(receiveBytes);

            string[] teu = s.Split(';');
            label7.Text = teu[0];
            label8.Text = teu[1];
            //MessageBox.Show(s);

            //Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));
            //}
        }
    }
}
