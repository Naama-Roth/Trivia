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
using System.Threading;
using System.ComponentModel;
using System.Timers;


namespace Client
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    /// 

    public class RoomCreateData
    {
        public string roomName { get; set; }
        public int questionCount { get; set;}
        public int maxUsers { get; set; }
        public int answerTimeOut { get; set; }
    }

    public partial class CreateRoom : Window
    {
        private const string CODE = "E";
        private RoomCreateData rcd;
        private bool comboPlayers;
        private bool comboQues;
        public CreateRoom()
        {
            InitializeComponent();
            rcd = new RoomCreateData();
            comboPlayers = false;
            comboQues = false;
        }

        private void cbQuesNumber_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBoxItem boxItem = (ComboBoxItem)cbQuesNumber.SelectedItem;
            rcd.questionCount = StringToInt(boxItem.Content.ToString());
            comboQues = true;
        }

        private void cbPlayerMaxNumber_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBoxItem boxItem = (ComboBoxItem)cbPlayerMaxNumber.SelectedItem;
            rcd.maxUsers = StringToInt(boxItem.Content.ToString());
            comboPlayers = true;
        }

        private void tbName_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            tb.Text = string.Empty;
            tb.GotFocus -= tbName_GotFocus;
        }

        private void bCreate_Click(object sender, RoutedEventArgs e)
        {
            string name = tbName.Text;

            if (tbName.Text == "Room Name" || String.IsNullOrEmpty(name)  || !comboQues || !comboPlayers) //if missing value
            {
                MessageBox.Show("ERROR!! Values Are Empty!", "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else
            {
                rcd.roomName = name;
                rcd.answerTimeOut = 30;

                //create request in json
                //string serReq = JsonConvert.SerializeObject(clientData, Formatting.Indented);
                string serReq = JsonConvert.SerializeObject(rcd, Formatting.Indented);

                MessageBox.Show("Sending to server create");
                //send to server
                Communicator.SendRequest(CODE.ToString(), serReq);

                //recive result in json
                int isSuccessful = BasicDeserializer.GetStatus();
                if (isSuccessful == 0) //if not successful
                {
                    string prob = BasicDeserializer.GetData(Communicator.GetJsonString());
                    string errMsg = BasicDeserializer.GetData(Communicator.GetJsonString());
                    MessageBox.Show(errMsg, "Creation Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                else
                {
                    //waiting for server
                    if (BasicDeserializer.GetStatus() == 0)
                    {
                        string prob = BasicDeserializer.GetData(Communicator.GetJsonString());
                        string errMsg = BasicDeserializer.GetData(Communicator.GetJsonString());
                        MessageBox.Show(errMsg, "Creation Error", MessageBoxButton.OK, MessageBoxImage.Error);

                        return;
                    }
                    else
                    {
                        AdminRoomScreen ars = new AdminRoomScreen();
                        ars.Show();
                        this.Close();
                    }
                }
            }
        }

        private void bExit_Click(object sender, RoutedEventArgs e)
        {
            MainWindow mw = new MainWindow();
            mw.Show();
            this.Close();
        }


        public static int StringToInt(string s)
        {
            int num = 0;
            for(int i = 0; i < s.Length; i++)
            {
                num = num * 10 + (s[i] - '0');
            }
            return num;
        }
        
    }
}
