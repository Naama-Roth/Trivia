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
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    /// 
    public class RoomData
    {
	    public int id { get; set; }
        public string name { get; set; }
        public int maxPlayers { get; set; }
        public int numOfQuestionsInGame { get; set; }
        public int timePerQuestion { get; set; }
        public int isActive { get; set; }
    }

    public class rooms
    {
        public List<String> Rooms;
    }

    public class RoomState
    {
        public int status { get; set; }
        public bool hasGameBegun { get; set; }
        public List<string> players { get; set; }
        public int questionCount { get; set; }
    }
    public partial class JoinRoom : Window
    {
        private const string IMAGE_AVALIABLE = "http://cdn1.iconfinder.com/data/icons/fatcow/16/bullet_green.png";
        private const string IMAGE_NOT_AVALIABLE =  "https://cdn1.iconfinder.com/data/icons/fatcow/16/bullet_black.png";
        private const string IMAGE_ROOM_FULL = "http://cdn1.iconfinder.com/data/icons/fatcow/16/bullet_red.png";
        private const string CODE = "H";
        private const string ROOM_STATE_CODE = "N";
        private const int WAIT_TIME = 3000;
        private BackgroundWorker background_worker = new BackgroundWorker();

        public JoinRoom()
        {
            background_worker.WorkerSupportsCancellation = true;
            background_worker.WorkerReportsProgress = true;
            background_worker.DoWork += background_worker_DoWork;


            InitializeComponent();
            AllRoomsList();
        }

        private void AllRoomsList()
        {
            

            spRoomData.Children.Clear(); // clear all previus data in stack panel

            Communicator.SendRequest(CODE, "");

            int isSuccessful = BasicDeserializer.GetStatus();
            if (isSuccessful != 0) //if succeed
            {
                rooms roomsList = BasicDeserializer.GetRooms(Communicator.GetJsonString()); //get all rooms
                RoomData[] roomsDataList = AllRoomsData(roomsList);

                //put each room on screen
                for (int i = 0; i < roomsList.Rooms.Count(); i++)
                {
                    RoomData rd = roomsDataList[i];

                    //get room state
                    string serReq = JsonConvert.SerializeObject(rd);
                    Communicator.SendRequest(ROOM_STATE_CODE, serReq);

                    isSuccessful = BasicDeserializer.GetStatus();
                    if (isSuccessful != 0) //if succeed
                    {
                        RoomState rs = BasicDeserializer.GetRoomState(Communicator.GetJsonString());


                        AccessText rplayers = new AccessText();
                        rplayers.Margin = new Thickness(16, 0, 0, 0);
                        rplayers.Text = rs.players.Count().ToString() + "/" + rd.maxPlayers.ToString(); // "<current player count> / <max player count>"
                        rplayers.Foreground = Brushes.DarkSlateBlue;


                        //creating label
                        Label label = new Label();
                        StackPanel s1 = new StackPanel();
                        s1.Orientation = Orientation.Horizontal;
                        Image im = new Image();

                        //choose image
                        if (rd.isActive == 0 || !rs.hasGameBegun) //if is not active
                        {
                            im.Source = new BitmapImage(new Uri(IMAGE_NOT_AVALIABLE));
                        }
                        else if (rd.maxPlayers == rs.players.Count()) //if room is full
                        {
                            im.Source = new BitmapImage(new Uri(IMAGE_ROOM_FULL));
                        }
                        else
                        {
                            im.Source = new BitmapImage(new Uri(IMAGE_AVALIABLE));
                        }

                        s1.Children.Add(im);

                        AccessText name = new AccessText();
                        name.Text = rd.name;
                        name.Foreground = Brushes.DarkSlateBlue;
                        s1.Children.Add(name);


                        //creating "bigger" stack panel that contains both lines
                        StackPanel s = new StackPanel();
                        s.Orientation = Orientation.Vertical;
                        s.Children.Add(s1);
                        s.Children.Add(rplayers);


                        label.HorizontalAlignment = HorizontalAlignment.Left;
                        label.VerticalAlignment = VerticalAlignment.Top;

                        label.Content = s;
                        label.Visibility = Visibility.Visible;

                        spRoomData.Children.Add(label);
                    }
                }
            }
        }

        private void startAsyncButton_Click(object sender, RoutedEventArgs e)
        {
            bRef.Visibility = Visibility.Hidden;
            background_worker.RunWorkerAsync(1000);
        }

        void background_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            while(true)
            {
                MessageBox.Show("here", "bg w", MessageBoxButton.OK, MessageBoxImage.Error);
                AllRoomsList();
                Thread.Sleep(WAIT_TIME); //wait 3 seconds
            }
        }

        private RoomData[] AllRoomsData(rooms roomsList)
        {
            RoomData[] roomsData = new RoomData[roomsList.Rooms.Count()];
            for (int i = 0; i < roomsList.Rooms.Count(); i++)
            {
                string[] items = roomsList.Rooms[i].Split(',');
                RoomData temp = new RoomData();
                temp.id = Int32.Parse((items[0].Split(' '))[1]);
                temp.name = (items[1].Split(' '))[2];
                temp.maxPlayers = Int32.Parse((items[2].Split(' '))[4]);
                temp.numOfQuestionsInGame = Int32.Parse((items[3].Split(' '))[4]);
                temp.timePerQuestion = Int32.Parse((items[4].Split(' '))[5]);
                roomsData[i] = temp;
            }
            return roomsData;
        }

        private void bExit_Click(object sender, RoutedEventArgs e)
        {
            MainWindow m = new MainWindow();
            m.Show();
            this.Close();
        }
    }
}
