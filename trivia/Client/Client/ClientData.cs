using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class ClientData
    {
        public string username;
        public string password;
        public string email;

        public ClientData(string name, string pass, string mail)
        {
            username = name;
            password = pass;
            email = mail;
        }

        public string getUsername() { return username; }
        public string getPassword() { return password; }
        public string getEmail() { return email; }
    }
}
