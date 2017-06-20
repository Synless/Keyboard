using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Input;

namespace SynlessKeyboardMapper
{
    public class keyboard : INotifyPropertyChanged
    {
        private List<key> keys = new List<key>();
        public List<key> Keys
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
        public ICommand Apply { get; set; }
        public ICommand Key { get; set; }
        private bool keyboardFound = true;
        public bool KeyboardFound
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
            Apply = new Command(ApplyPushed);
            Key = new Command(keyPushed);
            Messenger.Default.Register<object>(this, Received_Message);

            for (byte n=0;n<33;n++)
            {
                keys.Add(new key(n));
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

                        if (KeyToString.forward.ContainsKey(k.KeyChar))
                        {
                            k.KeyString = KeyToString.forward[k.KeyChar.ToString()];
                        }
                        if (k.KeyChar[0] == 'D' && k.KeyChar.Length == 2)
                        {
                            k.KeyChar = k.KeyChar[1].ToString();
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
                KeyboardFound = true;
            }
            for (int n = 0; n < Keys.Count; n++)
            {
                Keys[n].Enabled = true;
            }
        }
        private void ApplyPushed(object paramter)
        {
            commandToSend = "";
            for (int n = 0; n < Keys.Count; n++)
            {
                commandToSend += Keys[n].KeyString + "|";
            }
        }
        private void keyPushed(object paramter)
        {
            /*lastKeyboardFound = KeyboardFound;
            KeyboardFound = false;*/
            int intKey = int.Parse(paramter.ToString());
            for(int n = 0; n < Keys.Count; n++)
            {
                Keys[n].Enabled = false;
            }            
            Keys[intKey].Enabled = true;
            Keys = Keys;
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
