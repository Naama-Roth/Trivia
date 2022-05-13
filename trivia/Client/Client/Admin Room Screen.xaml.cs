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
    /// Interaction logic for Admin_Room_Screen.xaml
    /// </summary>
    public partial class AdminRoomScreen : Window
    {
        private const string LEAVE_CODE = "11";
        private const string START_CODE = "12";
        private const string CODE = "13";
        private const int WAIT_TIME = 3000;
        private const string IMAGE_ADMIN = "https://cdn2.iconfinder.com/data/icons/funtime-objects-part-2/60/005_037_crown_corona_king_power_best-512.png";
        private const string IMAGE_MEMBER = "https://cdn1.iconfinder.com/data/icons/fatcow/16/bullet_black.png";
        private int roomID;
        private BackgroundWorker background_worker = new BackgroundWorker();
        public AdminRoomScreen()
        {
            InitializeComponent();
        }
        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            Communicator.SendRequest(LEAVE_CODE, null);

            int isSuccessful = BasicDeserializer.GetStatus();
            if (isSuccessful == 0)
            {
                JoinRoom jr = new JoinRoom();
                jr.Show();
                this.Close();
            }
            else //if cant close room
            {
                MessageBox.Show("ERROR! Cannot Close Room!", "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            Communicator.SendRequest(LEAVE_CODE, null);

            int isSuccessful = BasicDeserializer.GetStatus();
            if (isSuccessful == 0)
            {
                MessageBox.Show("=>~ Start Game ~<=", "Started Game", MessageBoxButton.OK, MessageBoxImage.Hand);
                this.Close();
            }
            else //if cant close room
            {
                MessageBox.Show("ERROR! Cannot Start Game!", "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }


        private void startAsyncButton_Click(object sender, RoutedEventArgs e)
        {
            bRef.Visibility = Visibility.Hidden;
            background_worker.RunWorkerAsync(1000);
        }

        void background_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            //Empty TextBlock
            TextBlock empty = new TextBlock();
            empty.Width = 50;

            Thread.Sleep(WAIT_TIME);
            Communicator.SendRequest(CODE, null);

            int isSuccessful = BasicDeserializer.GetStatus();
            if (isSuccessful != 0) //if succeed
            {
                //get room state
                RoomState rs = BasicDeserializer.GetRoomState(Communicator.GetJsonString());
                StackPanel players = new StackPanel();
                players.Orientation = Orientation.Vertical;

                //checking if the room is still active
                if (rs.status != 0)
                {
                    //adding if the game started or not
                    TextBlock started = new TextBlock();
                    started.Text = "Has Started: " + (rs.hasGameBegun ? "True" : "False");
                    started.Foreground = Brushes.LimeGreen;

                    TextBlock count = new TextBlock();
                    count.Text = "Players:" + rs.players.Count().ToString();
                    count.Foreground = Brushes.LimeGreen;


                    //put the room member count and if active or not
                    StackPanel sMenu = new StackPanel();
                    sMenu.Orientation = Orientation.Horizontal;
                    sMenu.Children.Add(count);
                    sMenu.Children.Add(empty);
                    sMenu.Children.Add(started);

                    spRoomPlayers.Children.Add(sMenu);
                    //adding empty lines
                    spRoomPlayers.Children.Add(empty);
                    spRoomPlayers.Children.Add(empty);
                    spRoomPlayers.Children.Add(empty);

                    //adding players
                    for (int i = 0; i < rs.players.Count(); i++)
                    {
                        //creating label
                        Label label = new Label();
                        StackPanel s1 = new StackPanel();
                        s1.Orientation = Orientation.Horizontal;

                        AccessText name = new AccessText();
                        name.Text = rs.players[i];
                        name.Foreground = Brushes.LimeGreen;

                        Image im = new Image();

                        if (i == 0) //if admin
                        {
                            im.Source = new BitmapImage(new Uri(IMAGE_ADMIN));
                        }
                        else
                        {
                            im.Source = new BitmapImage(new Uri(IMAGE_MEMBER));
                        }
                        im.Height = 20;

                        s1.Children.Add(im);
                        s1.Children.Add(name);


                        //adding the player
                        players.Children.Add(s1);
                    }
                }
            }
        }
    }
}
