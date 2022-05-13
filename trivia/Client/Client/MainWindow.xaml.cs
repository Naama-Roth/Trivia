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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void bJoin_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom win = new JoinRoom();
            win.Show();
            this.Close();
        }

        private void bCreate_Click(object sender, RoutedEventArgs e)
        {
            CreateRoom win = new CreateRoom();
            win.Show();
            this.Close();
        }

        private void bStatistics_Click(object sender, RoutedEventArgs e)
        {
            Statistics win = new Statistics();
            win.Show();
            this.Close();
        }

        private void bExit_Click(object sender, RoutedEventArgs e)
        {
            Communicator.SendRequest("D", "");

            int isSuccessful = BasicDeserializer.GetStatus();
            if (isSuccessful == 0)
            {
                string prob = BasicDeserializer.GetData(Communicator.GetJsonString());
                string errMsg = BasicDeserializer.GetData(Communicator.GetJsonString());
                MessageBox.Show(errMsg, "Login Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                MessageBox.Show("Bye Bye!", "Exit Window", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                this.Close();
            }
        }

        private void bPrompt_Click(object sender, RoutedEventArgs e)
        {
            Communicator.SendRequest("P", "");
        }
    }
}
