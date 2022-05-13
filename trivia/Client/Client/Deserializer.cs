using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Client
{
    class BasicDeserializer
    {
        private const string STATUS = "'status': ";

        public static int GetStatus()
        {
            return Convert.ToInt32(Communicator.GetResponseType());
        }

        public static string GetData(byte[] data)
        {
            string strData = System.Text.Encoding.Default.GetString(data);
            return JsonConvert.DeserializeObject<string>(strData);
        }

        public static PersonalStats GetPersonalStats(byte[] data)
        {
            string strData = System.Text.Encoding.Default.GetString(data);
            return JsonConvert.DeserializeObject<PersonalStats>(strData);
        }

        public static Leaderboards GetLeaderboards(byte[] data)
        {
            string strData = System.Text.Encoding.Default.GetString(data);
            return JsonConvert.DeserializeObject<Leaderboards>(strData);
        }

        public static rooms GetRooms(byte[] data)
        {
            string strData = System.Text.Encoding.Default.GetString(data);
            return JsonConvert.DeserializeObject<rooms>(strData);
        }

        public static RoomState GetRoomState(byte[] data)
        {
            string strData = System.Text.Encoding.Default.GetString(data);
            return JsonConvert.DeserializeObject<RoomState>(strData);
        }

        //"cleanning" the messages the client got and doesnt need
        public static void CleanRes()
        {
            Communicator.GetJsonString();
        }
    }
}
