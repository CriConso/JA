using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Threading;

namespace ProgettoJA
{
    public partial class Collegamento : Form
    {
        public List<CRete> vettSSID= new List<CRete>();
        string[] porte;
        bool inizioric = true;
        bool fineric = false;
        bool contrconn = false;
        public SerialPort P;
        int cont, i;
        string ip;
        UdpClient receivingUdpClient = new UdpClient(11000);
        IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
        CModulo M;
        Form1 F;
        public Collegamento(Form1 F, int numm)
        {
            InitializeComponent();
            i = 0;
            this.F = F;
            if (numm==1)
            {
                F.M1 = M;
            }
            else if(numm==2)
            {
                F.M2 = M;
            }
            listView1.View = View.Details;
            // Allow the user to edit item text.
            listView1.LabelEdit = true;
            // Allow the user to rearrange columns.
            listView1.AllowColumnReorder = true;
            // Select the item and subitems when selection is made.
            listView1.FullRowSelect = true;
            // Display grid lines.
            listView1.GridLines = true;
            listView1.Columns.Add("Nome rete d'area", -2, HorizontalAlignment.Left);
            porte = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(porte);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                P = new SerialPort(comboBox1.Text, 115200);
                P.Open();
                P.DataReceived += COM_DataReceived;
            }
            catch (Exception)
            {
                MessageBox.Show("Impossibile avviare la comunicazione\ncon la porta selezionata");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            label3.Text += " ricerca di reti d'area in corso...";
            button2.Enabled = false;
            P.Write("a");
        }
        private void COM_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string c = P.ReadExisting();
            
            AppendTextInvoke(textBox1, c);

        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            Inserimento_password I = new Inserimento_password(vettSSID, listView1.FocusedItem.Index, this);        
            I.Show();
        }


        public void MOSTRARISULTATO()
        {
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            bool ricevi = true; ;
            UdpClient udpClient = new UdpClient(M.getIP(), 82);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("b");
            udpClient.Send(sendBytes, sendBytes.Length);
            while (ricevi)
            {
                Byte[] receiveBytes = receivingUdpClient.Receive(ref RemoteIpEndPoint);
                textBox1.Text += string.Join(" ", receiveBytes);
                string s = System.Text.Encoding.UTF8.GetString(receiveBytes);
                //MessageBox.Show(s);
                if (s=="s")
                {
                    label4.Text = "Connesso alla rete";
                    label4.ForeColor = Color.Green;
                    button4.Visible = true;
                    ricevi = false;
                }
                //Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));
            }
            /*inizioric = false;
            fineric = true;
            contrconn = true;
            P.Write("b");*/
        }

        private void Collegamento_Load(object sender, EventArgs e)
        {
            
        }

        private void button4_Click(object sender, EventArgs e)
        {
            P.Write("c");
            button4.Visible = false;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            UdpClient udpClient = new UdpClient(M.getIP(), 82);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("b");
            udpClient.Send(sendBytes, sendBytes.Length);
        }

        private void Send(string remoteIp, int remotePort, string message)
        {
            remoteIp = M.getIP();
            remotePort = 82;
            UdpClient udpClient = new UdpClient(remoteIp, remotePort);
            Byte[] sendBytes = Encoding.ASCII.GetBytes(message);
            udpClient.Send(sendBytes, sendBytes.Length);
        }

        bool lgIP = false;
        private void AppendTextInvoke(TextBox t, String Text)
        {
            if (t.InvokeRequired)
                t.Invoke((MethodInvoker)delegate { AppendTextInvoke(t, Text); });
            else
            {
                if (Text.Contains("f")&&inizioric) 
                {
                    cont = Convert.ToInt32(Text.ElementAt(3))-48;
                    label3.Text = "Stato: ricerca di reti d'area completata. " + cont + " reti trovate";
                    inizioric = false;
                    
                }
                else if(cont>=0&&!inizioric&&!fineric)
                {
                    Text.TrimEnd('\n');
                    vettSSID.Add(new CRete(Text));

                    listView1.Items.Add(new ListViewItem(vettSSID.ElementAt(i).getSSID()));
                    i++;
                    cont--;
                    if(cont<=0)
                    {
                        fineric = true;
                        button2.Enabled = true;
                    }
                }
                if (Text == "s")
                {                    
                    label4.Text = "Connesso alla rete";
                    label4.ForeColor = Color.Green;
                    button4.Visible = true;
                    lgIP = true;
                }
                else if (Text=="n" && !contrconn)
                {
                    MessageBox.Show("Password errata");
                }
                if (Text=="n"&& contrconn)
                {
                    label4.Text = "Non connesso alla rete";
                    label4.ForeColor = Color.Red;
                    contrconn = false;
                    inizioric = true;
                    fineric = false;
                }
                if (lgIP&&Text!="s")
                {
                    lgIP = false;
                    ip = Text;
                    label5.Text += ip;
                    M.setIP(ip);
                }
                t.AppendText(Text + ";");
            }
        }
    }
}
