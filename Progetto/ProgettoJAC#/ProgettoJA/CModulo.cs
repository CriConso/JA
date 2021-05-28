using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ProgettoJA
{
    public class CModulo
    {
        private bool connesso;
        private string ip;
        private int numm;

        public CModulo()
        {
            ip = "";
            connesso = false;
        }
        public CModulo(int numm)
        {
            ip = "";
            this.numm = numm;
            connesso = false;
        }
        public CModulo(string ip)
        {
            this.ip = ip;
            connesso = true;
        }
        public void setConn()
        {
            connesso = !connesso;
        }

        public bool getConn()
        {
            return connesso;
        }

        public void setIP(string ip)
        {
            this.ip = ip;
            string letto="";
            string scrivi = "";
            letto=File.ReadAllText(Directory.GetCurrentDirectory() + @"\..\..\Resources\IPMODULI.txt");
            string[] righe = letto.Split('\n');
            if (righe[0]=="1") //SE HO GIÀ LAVORATO
            {
                if (numm == 1) //SE STO LAVORANDO SUL PRIMO MODULO
                {
                    string[] riga = righe[2].Split(';'); //SPLITTA LA RIGA DEL SECONDO
                    scrivi += "1\n1;" + ip + "\n"; //SCRIVI AL PRIMO L'IP
                    if (riga[0] == "1") //SE È GIÀ STATO FATTO IL SECONDO ALLORA NON VA CAMBIATO
                    {
                        scrivi += "1;" + riga[1];
                    }
                    else if (riga[0] == "0") //ALTRIMENTI LASCIA ZERO
                    {
                        scrivi += "0;";
                    }
            }
                else if (numm == 2) //SE STO LAVORANDO SUL SECONDO
                {
                    string[] riga = righe[1].Split(';'); //SPLITTA LA RIGA DEL PRIMO

                    if (riga[0] == "1") //CONTROLLA CHE NON SIA GIÀ STATO FATTO IL PRIMO
                    {                        
                        scrivi += "1\n1;" + riga[1] + "\n"; //SE SÌ ALLORA LO RISCRIVO
                    }
                    else if(riga[0]=="0")//ALTRIMENTI LASCIA ZERO
                    {
                        scrivi += "1\n0;\n";
                    }
                    scrivi += "1;" + ip; //SCRIVO IL SECONDO

                }
                
            }
            else if (righe[0] == "0")
            {
                if (numm == 1)
                {
                    scrivi += "1\n1;" + ip + "\n0;";
                }
                else if (numm == 2)
                {
                    scrivi += "1\n0;\n1;" + ip;
                }
                File.WriteAllText(Directory.GetCurrentDirectory() + @"\..\..\Resources\IPMODULI.txt", scrivi);
            }
        }
            
        public string getIP()
        {
            return ip;
        }

    }
}
