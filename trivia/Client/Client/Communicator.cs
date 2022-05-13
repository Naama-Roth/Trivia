using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace Client
{
    public static class Communicator
    {
        private static TcpClient _client;
        private static NetworkStream _clientStream;
        public static void Connect()
        {
            _client = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8826);

            _client.Connect(serverEndPoint);
            _clientStream = _client.GetStream();
        }

        public static void SendRequest(string code, string jsonString)
        {
            // Add the code as byte
            byte[] codeBytes = new ASCIIEncoding().GetBytes(code);

            //Add length as 4 bytes
            int length = jsonString.Length;
            byte[] lengthBytes = { (byte)length, (byte)0, (byte)0, (byte)0 };

            // Add string as bytes
            byte[] jsonStringBytes = new ASCIIEncoding().GetBytes(jsonString);

            // append all to one arry
            byte[] requestBuffer = new byte[codeBytes.Length + lengthBytes.Length + jsonStringBytes.Length];
            System.Buffer.BlockCopy(codeBytes, 0, requestBuffer, 0, codeBytes.Length);
            if (length != 0)
            {
                System.Buffer.BlockCopy(lengthBytes, 0, requestBuffer, codeBytes.Length, lengthBytes.Length);
                System.Buffer.BlockCopy(jsonStringBytes, 0, requestBuffer, codeBytes.Length + lengthBytes.Length, jsonStringBytes.Length);
            }
            // send Request Buffer to server
            _clientStream.Write(requestBuffer, 0, requestBuffer.Length);
            _clientStream.Flush();
        }

        public static byte GetResponseType()
        {
            byte[] type = new byte[1];
            //waiting for server to send result
            while(!_clientStream.DataAvailable)
            {
                ;
            }
            _clientStream.Read(type, 0, 1);
            return type[0];
        }

        private static int GetSizeOfJson()
        {
            byte[] size = new byte[4];
            //waiting for server to send result
            while (!_clientStream.DataAvailable)
            {
                ;
            }

            _clientStream.Read(size, 0, 4);
            return ByteToInt(size);
        }

        public static byte[] GetJsonString()
        {
            int size = GetSizeOfJson();
            byte[] jsonString = new byte[size];
            ////waiting for server to send result
            //while (!_clientStream.DataAvailable)
            //{
            //    ;
            //}
            _clientStream.Read(jsonString, 0, size);
            return jsonString;
        }

        //convert byte to int to get the size
        private static int ByteToInt(byte[] arr)
        {
            int size = 0;
            for (int i = 0; i < 4; i++)
            {
                int asciiVal = Convert.ToInt32(arr[i]);
                string number = (Convert.ToChar(asciiVal)).ToString();
                size *= 10;
                size += Int32.Parse(number);
            }
            return size;
        }
    }
}
