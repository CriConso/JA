using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ProgettoJA
{
    public partial class Inserimento_password : Form
    {
        int index;
        List<CRete> vett;
        Collegamento C;
        public Inserimento_password(List<CRete> vett, int index, Collegamento C)
        {
            InitializeComponent();
            this.index = index;
            this.vett = vett;
            this.C = C;
            label1.Text += index + " " + vett.ElementAt(index).getSSID();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            vett.ElementAt(index).setPW(textBox1.Text);
            C.vettSSID = vett;
            C.P.Write((index+1).ToString());
            C.P.Write(textBox1.Text+";");
            C.MOSTRARISULTATO();
            this.Close();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                textBox1.PasswordChar = '\0';
            }
            else
            {
                textBox1.PasswordChar = '*';
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void Inserimento_password_Load(object sender, EventArgs e)
        {

        }
    }
}
