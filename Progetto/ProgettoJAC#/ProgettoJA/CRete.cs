using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProgettoJA
{
    class CRete
    {
        string SSID;
        string PW;
        string IP;

        public CRete()
        {
            SSID = "default";
            PW = "";
            IP = "0.0.0.0";
        }
        public CRete(string SSID)
        {
            this.SSID = SSID;
        }

        public void setPW(string PW)
        {
            this.PW = PW;
        }

        public void setIP(string IP)
        {
            this.IP = IP;
        }

        public string getIP()
        {
            return IP;
        }
        public string getSSID()
        {
            return SSID;
        }
    }
}
