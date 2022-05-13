using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;
using Newtonsoft.Json;

namespace Client
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Window
    {
        private const char CODE = 'B';
        private static bool isConnected = false;
        private class ClientLog
        {
            public string username { get; set; }
            public string password { get; set; }
        }

        public Login()
        {
            try
            {
                if (!isConnected)
                {
                    Communicator.Connect();
                    isConnected = true;
                }
                InitializeComponent();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Connection Error", MessageBoxButton.OK, MessageBoxImage.Error);
                this.Close();
            }

        }

        private void LoginRequest(object sender, RoutedEventArgs e)
        {
            int isSuccessful = 0;
            ClientLog client = new ClientLog();

            if(String.IsNullOrEmpty(tbName.Text) || String.IsNullOrEmpty(tbPassword.Password.ToString())) //if user hasn't filled the fields
            {
                MessageBox.Show("ERROR! Fields need to be filled!", "Error In Login.", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                //filling fields
                client.username = tbName.Text;
                client.password = tbPassword.Password.ToString();

                string serReq = JsonConvert.SerializeObject(client);
                Communicator.SendRequest(CODE.ToString(), serReq);


                //recive result in json
                isSuccessful = BasicDeserializer.GetStatus();
                if (isSuccessful == 0)
                {
                    string prob = BasicDeserializer.GetData(Communicator.GetJsonString());
                    string errMsg = BasicDeserializer.GetData(Communicator.GetJsonString());
                    MessageBox.Show(errMsg, "Login Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                else
                {
                    BasicDeserializer.CleanRes();
                    MainWindow win = new MainWindow();
                    win.Show();
                    this.Close();
                }
            }
            tbName.Clear();
            tbPassword.Clear();
        }

        private void SignUpRequest(object sender, RoutedEventArgs e)
        {
            Signup win = new Signup();
            win.Show();
            this.Close();
        }

        public void PasswordBox_GotFocus(object sender, RoutedEventArgs e)
        {
            PasswordBox tb = (PasswordBox)sender;
            tb.Password = string.Empty;
            tb.GotFocus -= PasswordBox_GotFocus;
        }

        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            tb.Text = string.Empty;
            tb.GotFocus -= TextBox_GotFocus;
        }


    }
}
