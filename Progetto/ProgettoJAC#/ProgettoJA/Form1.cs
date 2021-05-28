using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ProgettoJA
{
    public partial class Form1 : Form
    {
        public CModulo M1, M2;
        public UdpClient receivingUdpClient = new UdpClient(11000);
        UdpClient receivingUdpClient2 = new UdpClient(11001);
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
            //receivingUdpClient.Close();
            Collegamento C = new Collegamento(this, 1);
            C.label1.Text = "Stato sensore uno";
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
            C.label1.Text = "Stato sensore due";
            C.Show();
            this.Hide();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            bool ricevi = true;
            UdpClient udpClient = new UdpClient(M1.getIP(), 82);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("b");
            udpClient.Send(sendBytes, sendBytes.Length);
            while (ricevi)
            {
                Byte[] receiveBytes = receivingUdpClient.Receive(ref RemoteIpEndPoint);
                string s = System.Text.Encoding.UTF8.GetString(receiveBytes);
                //MessageBox.Show(s);
                if (s == "s")
                {
                    button3.Enabled = true;
                    label12.ForeColor = Color.Green;
                    M1.setConn();
                    ricevi = false;
                }
                MessageBox.Show("Primo fatto");
                //Thread.Sleep(4000);
                //UdpClient udpClient2 = new UdpClient(M2.getIP(), 82);
                //udpClient2.Send(sendBytes, sendBytes.Length);
                //while (ricevi)
                //{
                //    Byte[] receiveBytes2 = receivingUdpClient.Receive(ref RemoteIpEndPoint);
                //    s = System.Text.Encoding.UTF8.GetString(receiveBytes2);
                //    //MessageBox.Show(s);
                //    if (s == "s")
                //    {
                //        button3.Enabled = true;
                //        label13.ForeColor = Color.Green;
                //        M2.setConn();
                //        ricevi = false;
                //    }
                //    //Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));
                //}
            }
        } 

        private void Form1_Load(object sender, EventArgs e)
        {
            M1 = new CModulo(1);
            M2 = new CModulo(2);
            string letto = "";
            letto = File.ReadAllText(Directory.GetCurrentDirectory()+ @"\..\..\Resources\IPMODULI.txt");
            if (letto == "0")
            {
                button3.Enabled = false;

            }
            else
            {
                string[] righe = letto.Split('\n');
                string[] riga1 = righe[1].Split(';');
                string[] riga2 = righe[2].Split(';');
                M1.setIP(riga1[1]);
                if (riga2[0]=="1")
                {
                    M2.setIP(riga2[1]);
                }
                //MessageBox.Show(M1.getIP());
            }
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            bool ricevi = true;
            //MessageBox.Show(GetIP());
            UdpClient udpClient = new UdpClient(M1.getIP(), 82);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("d");
            udpClient.Send(sendBytes, sendBytes.Length);
            int k = 50;
            label14.Text = "Ricezione...";
            //while (k>0)
            //{
                //Thread.Sleep(900);
                Byte[] receiveBytes = receivingUdpClient2.Receive(ref RemoteIpEndPoint);
                label14.Text = "Ricevuto, scrivo";
                string s = System.Text.Encoding.UTF8.GetString(receiveBytes);

                string[] teu = s.Split(';');
                label7.Text = teu[0];
                label8.Text = teu[1];
               // k--;
            //}
            //MessageBox.Show(s);
            float temperatura, umidità;
            temperatura = Convert.ToInt32(float.Parse(label7.Text))/100;
            umidità = Convert.ToInt32(float.Parse(label8.Text)) / 100;
           // MessageBox.Show(umidità.ToString());
            if (temperatura < -5)
            {
                pictureBox1.ImageLocation = @"\..\..\..\..\immaginiTemperatura\termometroFreddo.png";
            }
            else if (temperatura < 0)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro-5.png";
                ////MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro-5.png");
            }
            else if (temperatura < 5)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro0.png";
                MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro0.png");
            }
            else if (temperatura < 10)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro5.png";
                //MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro5.png");
            }
            else if (temperatura < 15)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro10.png";
                MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro10.png");
            }
            else if (temperatura < 20)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro15.png";
                //MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro15.png");
            }
            else if (temperatura < 25)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro20.png";
                //MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro20.png");
            }
            else if (temperatura < 30)
            {
                pictureBox1.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro25.png";
                //MessageBox.Show(Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiTemperatura\termometro25.png");
            }

            if (umidità<100 && umidità > 80)
            {
                pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL6.png";
            }
            else if (umidità < 80 && umidità > 64)
            {
                pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL5.png";
            }
            else if (umidità < 64 && umidità > 48)
            {
                pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL4.png";
            }
            else if (umidità < 48 && umidità > 32)
            {
                pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL3.png";
            }
            else if (umidità < 32 && umidità > 16)
            {
                pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL2.png";
            }
            else if (umidità < 16 && umidità > 0)
            {
                pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL1.png";
                //pictureBox3.ImageLocation = "https://cdn.discordapp.com/attachments/699741150256431164/847892762846429194/pila_2.png";
            }
            else if (umidità == 0)
            {
                //pictureBox3.ImageLocation = Directory.GetCurrentDirectory() + @"\..\..\..\..\immaginiPila\pilaLVL1.png";
                
            }
            //Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));
            //}
        }
    }
}
