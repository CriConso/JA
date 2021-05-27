using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProgettoJA
{
    public class CModulo
    {
        private bool connesso;
        private string ip;

        public void setIP(string ip)
        {
            this.ip = ip;
        }
        public string getIP()
        {
            return ip;
        }

    }
}
