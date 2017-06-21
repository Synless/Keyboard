using System.IO.Ports;

namespace SynlessKeyboardMapper
{
    public class AutoCOM : SerialPort
    {
        public bool found = false;
        int[] _baudrate = { 115200, 9600, 19200, 38400  };

        public AutoCOM(string _querry, string[] _answers, int _maxPort = 20, int waitTime = 20)
        {
            //GET ALL COM PORT CURRENTLY AVAILABLE (LIKE UNDER DEVICE MANAGER)
            string[] _ports = SerialPort.GetPortNames();

            //SCANNING THROUGH BAUDRATE, LIKELY TO BE 9600 OR 19200
            foreach (int b in _baudrate)
            {
                BaudRate = b;
                //SCANNING THROUGH PORTS
                foreach (string p in _ports)
                {
                    PortName = p;
                    try
                    {
                        Open();
                        WriteLine(_querry);
                        System.Threading.Thread.Sleep(waitTime);
                        string received = ReadExisting();
                        Close();

                        //IF THE ANWSER IS CONTAINED IN THE _ANSWER ARRAY, THEN WE CAN'T STOP HERE, JOB'S DONE
                        foreach (string anwser in _answers)
                        {
                            if (received.Contains(anwser))
                            {
                                found = true;
                                return;
                            }
                        }
                    }
                    catch
                    {
                        found = false;                        
                        return;
                    }
                    if (PortName.ToString() == ("COM" + _maxPort.ToString()))
                    {
                        break;
                    }
                }
            }            
        }
        public void _Write(string tmp)
        {
            try {
                if (!IsOpen)
                {
                    Open();
                }
                Write(tmp);
                if (IsOpen)
                {
                    //Close();
                }
            }
            catch
            {

            }
        }
        public string _ReadExisting()
        {
            string tmp = "";
            try
            {
                if (!IsOpen)
                {
                    Open();
                }
                tmp = ReadExisting();
                if (IsOpen)
                {
                    //Close();
                }
            }
            catch
            {

            }
            return tmp;
        }
    }
}