﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ProgettoJA
{
    public partial class Collegamento : Form
    {
        public List<CRete> vettSSID= new List<CRete>();
        string[] porte;
        bool inizioric = true;
        bool fineric = false;
        public SerialPort P;
        int cont, i;
        public Collegamento()
        {
            InitializeComponent();
            i = 0;
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
            inizioric = false;
            fineric = true;
            P.Write("b");
            label4.Text = "Connesso alla rete " + listView1.FocusedItem;
            label4.ForeColor = Color.Green;
        }

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
                    label4.Text = "Connesso alla rete " + listView1.FocusedItem;
                    label4.ForeColor = Color.Green;
                }
                else if (Text=="n")
                {
                    MessageBox.Show("Password errata");
                }
                t.AppendText(Text + ";");
            }
        }
    }
}