using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Windows.Input;

namespace SynlessKeyboardMapper
{
    public class keyboard : INotifyPropertyChanged
    {
        private AutoCOM SAMD21;

        private List<key> keys = new List<key>();
        public  List<key> Keys
        {
            get
            {
                return keys;
            }
            set
            {
                keys = value;
                OnPropertyChanged("Keys");
            }
        }

        private string commandToSend;
        private static string receivedLayout;

        public ICommand Apply { get; set; }
        public ICommand Key { get; set; }

        private bool keyboardFound = true;
        public  bool KeyboardFound
        {
            get
            {
                return keyboardFound;
            }
            set
            {
                keyboardFound = value;
                OnPropertyChanged("KeyboardFound");
            }
        }
        private bool lastKeyboardFound = false;

        public keyboard()
        {
            for (byte n = 0; n < 33; n++)
            {
                keys.Add(new key());
            }

            SAMD21 = new AutoCOM("ping", new string[] { "pong" });
            KeyboardFound = SAMD21.found;
            if(KeyboardFound)
            {
                SAMD21.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
                SAMD21._Write("ack");                
            }
            else
            {
                System.Windows.MessageBox.Show("Keyboard not found  :(");
            }
            Apply = new Command(ApplyPushed);
            Key = new Command(keyPushed);
            Messenger.Default.Register<object>(this, Received_Message);
        }
        private static void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            System.Threading.Thread.Sleep(50);
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();
            if (indata.Length > 10 && indata.Contains("|"))
            {
                receivedLayout = indata;
                Messenger.Default.Send("processLayout");
            }
        }

        private void processLayout()
        {
            string tmp = "";
            foreach (char c in receivedLayout)
            {
                if ((c >= '0' && c <= '9') || c == '|')
                {
                    System.Threading.Thread.Sleep(1);
                    tmp += c;
                }
            }
            receivedLayout = tmp.Remove(tmp.Length - 1);

            if (receivedLayout.Length > 10 && receivedLayout.Contains("|"))
            {
                var backward = Key2Key.forward.Reverse();
                string[] splitLayout = receivedLayout.Split('|');
                for (int n = 0; n < splitLayout.Length && n < Keys.Count; n++)
                {
                    try
                    {
                        string tmp2 = backward[splitLayout[n]];
                        Keys[n].KeyChar = tmp2;
                    }
                    catch
                    {

                    }
                }
            }
        }
        private void Received_Message(object _message)
        {
            if(_message.ToString() == "_end")
            {
                for (int n = 0; n < Keys.Count; n++)
                {
                    Keys[n].Enabled = true;
                }
                return;
            }
            else if(_message.ToString() == "processLayout")
            {
                processLayout();
            }
            else
            {
                int go = 0;
                for (int n = 0; n < Keys.Count; n++)
                {
                    if(Keys[n].Enabled)
                    {
                        if (++go > 1)
                            return;
                    }
                }
                foreach (key k in Keys)
                {
                    if(k.Enabled)
                    {
                        k.KeyChar = _message.ToString();

                        if (Key2Key.forward_ext.ContainsKey(k.KeyChar))
                        {
                            k.KeyString = Key2Key.forward_ext[k.KeyChar.ToString()];
                        }
                        else if(k.KeyChar.Length>1)
                        {
                            if (k.KeyChar[0] == 'N' && k.KeyChar[1] == 'u')
                            {
                                string tmp = "";
                                for (int n = 6; n < k.KeyChar.Length; n++)
                                {
                                    tmp += k.KeyChar[n];
                                }
                                k.KeyChar = tmp;
                            }
                        }
                        if (k.KeyChar == "Add")
                        {
                            k.KeyChar = "+";
                        }
                        else if (k.KeyChar == "Subtract")
                        {
                            k.KeyChar = "-";
                        }
                        else if (k.KeyChar == "Divide")
                        {
                            k.KeyChar = "/";
                        }
                        else if (k.KeyChar == "Multiply")
                        {
                            k.KeyChar = "*";
                        }
                        else if (k.KeyChar == "Return")
                        {
                            k.KeyChar = "Enter";
                        }
                    }
                }
                //KeyboardFound = true;
            }
            for (int n = 0; n < Keys.Count; n++)
            {
                Keys[n].Enabled = true;
            }
        }
        private void ApplyPushed(object paramter)
        {
            int n = 0;
            string keytmp = "";
            try {
                commandToSend = "";
                while(n < Keys.Count)
                {
                    keytmp = Keys[n].KeyChar;
                    commandToSend += Key2Key.forward_ext[Keys[n].KeyChar] + "|";
                    n++;
                }
                commandToSend.Remove(commandToSend.Length - 1);
                SAMD21._Write(commandToSend);
            }
            catch
            {
                System.Windows.MessageBox.Show(keytmp + " is not supported yet !");
                SAMD21._Write("ack");
            }
        }
        private void keyPushed(object paramter)
        {
            int intKey = int.Parse(paramter.ToString());
            for(int n = 0; n < Keys.Count; n++)
            {
                Keys[n].Enabled = false;
            }            
            Keys[intKey].Enabled = true;
        }

        #region PropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
    }
}