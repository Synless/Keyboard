using System.ComponentModel;
using System.Windows.Input;

namespace SynlessKeyboardMapper
{
    public class key : INotifyPropertyChanged
    {
        private byte keyInt = 0;
        public byte KeyInt
        {
            get
            {
                return keyInt;
            }
            set
            {
                keyInt = value;
                OnPropertyChanged("KeyInt");
            }
        }
        private string keyChar = "-";
        public string KeyChar
        {
            get
            {
                return keyChar;
            }
            set
            {
                keyChar = value;
                
                OnPropertyChanged("KeyChar");
            }
        }
        private bool focused = false;

        private static bool locked = false;

        public ICommand pushedCommand { get; set; }

        public key(byte n)
        {
            KeyInt = n;
            KeyChar = n.ToString();
            pushedCommand = new Command(pushed);
            Messenger.Default.Register<object>(this, Received_Message);
        }
        private void pushed(object paramter)
        {
            if (!locked)
            {
                locked = true;
                focused = true;                
            }
        }
        private void Received_Message(object _message)
        {
            if(focused)
            {
                //try
               // {
                    KeyChar = _message.ToString().ToUpper();
                    KeyInt = CharToByte.conv[keyChar];
               // }
                //catch
                //{

               // }
                locked = false;
                focused = false;
            }
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
